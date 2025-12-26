#include "gimbalremote.h"
#include "ui_gimbalremote.h"
#include <QDebug>
#include <QHostAddress>

// Protocol Constants
const uint8_t HEADER_LOW  = 0xA8; // [cite: 154]
const uint8_t HEADER_HIGH = 0xE5; // [cite: 154]
const uint8_t PROTOCOL_VER = 0x01; // [cite: 246]

GimbalRemote::GimbalRemote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GimbalRemote)
{
    ui->setupUi(this);
    commandSocket = new QUdpSocket(this);
    commandSocket->bind(QHostAddress("192.168.144.99"), 2338);

    connect(commandSocket, &QUdpSocket::readyRead, this, &GimbalRemote::processIncomingData);

    // Heartbeat Timer
    m_heartbeatTimer = new QTimer(this);
    connect(m_heartbeatTimer, &QTimer::timeout, this, [this]() {
        if (!m_moveLoopTimer->isActive()) {
            // Heartbeat: Send NULL command with 0 speed
            sendGimbalCommand(0.0f, 0.0f, 0.0f, CMD_NULL);
        }
    });
    m_heartbeatTimer->start(50); // 20Hz

    // Movement Loop Timer
    m_moveLoopTimer = new QTimer(this);
    connect(m_moveLoopTimer, &QTimer::timeout, this, &GimbalRemote::updateMovementStep);
    m_moveLoopTimer->setTimerType(Qt::PreciseTimer);

    m_useCustomAim = false;
}

GimbalRemote::~GimbalRemote()
{
    delete ui;
}

uint16_t GimbalRemote::calculateCRC16(const uint8_t* data, int length) {
    uint16_t crc = 0x0000;
    for (int i = 0; i < length; i++) {
        crc ^= (static_cast<uint16_t>(data[i]) << 8);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
            else crc <<= 1;
        }
    }
    return crc;
}

// --- MAIN COMMAND FUNCTION ---
void GimbalRemote::sendGimbalCommand(float roll, float pitch, float yaw,
                                     GimbalCmd cmd, const QByteArray& payload)
{
    // 1. Prepare Buffer
    QByteArray packet;
    packet.reserve(128);

    // 2. Header (Bytes 0-4)
    packet.append((char)HEADER_LOW);
    packet.append((char)HEADER_HIGH);
    packet.append((char)0x00); // Length Placeholder (Low)
    packet.append((char)0x00); // Length Placeholder (High)
    packet.append((char)PROTOCOL_VER);

    // 3. Main Data Frame (Bytes 5-36) - Attitude Control
    int16_t sRoll, sPitch, sYaw;

    // Scaling Logic:
    if (cmd == CMD_ANGLE_CTRL || cmd == 0x14) {
        // Angle Control: x100 (0.01 deg resolution)
        sRoll  = static_cast<int16_t>(roll * 100.0f);
        sPitch = static_cast<int16_t>(pitch * 100.0f);
        sYaw   = static_cast<int16_t>(yaw * 100.0f);
    } else {
        // Speed Control: x10 (0.1 deg/s resolution)
        sRoll  = static_cast<int16_t>(roll * 10.0f);
        sPitch = static_cast<int16_t>(pitch * 10.0f);
        sYaw   = static_cast<int16_t>(yaw * 10.0f);
    }

    // Append Little-Endian
    packet.append((char)(sRoll & 0xFF));
    packet.append((char)((sRoll >> 8) & 0xFF));
    packet.append((char)(sPitch & 0xFF));
    packet.append((char)((sPitch >> 8) & 0xFF));
    packet.append((char)(sYaw & 0xFF));
    packet.append((char)((sYaw >> 8) & 0xFF));

    packet.append((char)0x05); // Byte 11: Status Valid [cite: 173]

    // Bytes 12-29: Padding (Carrier Info)
    packet.append(18, (char)0x00);

    packet.append((char)0x01); // Byte 30: Request Sub Frame
    packet.append(6, (char)0x00);  // Bytes 31-36: Reserved

    // 4. Sub Data Frame (Bytes 37-68) - FIXED TO MATCH IMAGE DATA
    packet.append((char)0x01); // Byte 37: Header

    // Bytes 38-41: Longitude (170.9175332 -> 24 F2 DF 65) [cite: 246]
    packet.append((char)0x24); packet.append((char)0xF2); packet.append((char)0xDF); packet.append((char)0x65);

    // Bytes 42-45: Latitude (38.0300822 -> 16 EE AA 16)
    packet.append((char)0x16); packet.append((char)0xEE); packet.append((char)0xAA); packet.append((char)0x16);

    // Bytes 46-49: Altitude (41.123m -> A3 A0 00 00)
    packet.append((char)0xA3); packet.append((char)0xA0); packet.append((char)0x00); packet.append((char)0x00);

    // Byte 50: Available Satellites (15 -> 0F)
    packet.append((char)0x0F);

    // Bytes 51-54: GNSS Microsecond (352718000 -> B0 0C 06 15)
    packet.append((char)0xB0); packet.append((char)0x0C); packet.append((char)0x06); packet.append((char)0x15);

    // Bytes 55-56: GNSS Week (2278 -> E6 08)
    packet.append((char)0xE6); packet.append((char)0x08);

    // Bytes 57-60: Relative Height (10m -> 10 27 00 00)
    packet.append((char)0x10); packet.append((char)0x27); packet.append((char)0x00); packet.append((char)0x00);

    // Bytes 61-68: Reserved (00...)
    packet.append(8, (char)0x00);

    packet.append((char)cmd); // Byte 69: Order [cite: 154]
    if (!payload.isEmpty()) {
        packet.append(payload); // Byte 70+: Parameters
    }

    // 6. Finalize Length
    // Total size including CRC
    uint16_t totalLen = packet.size() + 2;
    packet[2] = (char)(totalLen & 0xFF);
    packet[3] = (char)((totalLen >> 8) & 0xFF);

    // 7. CRC
    uint16_t crc = calculateCRC16(reinterpret_cast<const uint8_t*>(packet.data()), packet.size());
    packet.append((char)((crc >> 8) & 0xFF)); // CRC High [cite: 154]
        packet.append((char)(crc & 0xFF));        // CRC Low

    // 8. Send
    commandSocket->writeDatagram(packet, QHostAddress("192.168.144.108"), 2337);
    //qDebug() << "Sent:" << packet.toHex(' ').toUpper();
}

// --- PID Loop ---
void GimbalRemote::updateMovementStep() {
    // 1. PID Gains - Tuning required for XF Z-1Pro
    const float Kp = 2.0f;   // Proportional: Main drive
    const float Ki = 0.2f;   // Integral: Fixes steady-state offset
    const float Kd = 0.01f;  // Derivative: Dampens oscillations/overshoot

    // 2. Calculate Current Error
    float errorP = m_targetPitch - m_feedbackPitch;
    float errorY = m_targetYaw - m_feedbackYaw;

    // 3. Integral Term (Sum of errors over time)
    m_integralP += errorP * m_dt;
    m_integralY += errorY * m_dt;

    // Optional: Integral Clamping (prevents "windup" where the gimbal goes crazy)
    m_integralP = std::fmax(-5.0f, std::fmin(5.0f, m_integralP));
    m_integralY = std::fmax(-5.0f, std::fmin(5.0f, m_integralY));

    // 4. Derivative Term (Change in error)
    float derivativeP = (errorP - m_prevErrorP) / m_dt;
    float derivativeY = (errorY - m_prevErrorY) / m_dt;

    // 5. PID Formula
    float outP = (errorP * Kp) + (m_integralP * Ki) + (derivativeP * Kd);
    float outY = (errorY * Kp) + (m_integralY * Ki) + (derivativeY * Kd);


    // Update state for next step
    m_prevErrorP = errorP;
    m_prevErrorY = errorY;

    // 7. Convert and Send
    int16_t sendP = static_cast<int16_t>(outP);
    int16_t sendY = static_cast<int16_t>(outY);
    sendGimbalCommand(0, sendP, sendY, CMD_SPEED_CTRL, 0);

    // Logging
    static int logCounter = 0;
    if (++logCounter % 5 == 0) {
        qDebug().noquote() << QString("PID Active | Err: [%1, %2] | Out: [%3, %4]")
        .arg(errorP, 0, 'f', 2).arg(errorY, 0, 'f', 2)
            .arg(sendP).arg(sendY);
    }

    // Stop when error is very small
    if (std::abs(errorP) < 0.1f && std::abs(errorY) < 0.1f) {
        m_moveLoopTimer->stop();
        qDebug() << "GIMBAL: Target reached and loop stopped.";
    }
}

// --- Incoming Data Processing (Unchanged logic) ---
void GimbalRemote::processIncomingData() {
    while (commandSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(commandSocket->pendingDatagramSize());
        commandSocket->readDatagram(datagram.data(), datagram.size());

        // 1. Verify Header 0x8A 0x5E and Length 72
        if (datagram.size() >= 72 &&
            (unsigned char)datagram[0] == 0x8A &&
            (unsigned char)datagram[1] == 0x5E) {

            // 2. Unpack Absolute Angles (Little Endian)
            int16_t rawRelaRoll  = (uint8_t)datagram[12] | ((uint8_t)datagram[13] << 8);
            int16_t rawRelaPitch = (uint8_t)datagram[14] | ((uint8_t)datagram[15] << 8);
            int16_t rawRelaYaw   = (uint8_t)datagram[16] | ((uint8_t)datagram[17] << 8);

            // 2. Unpack Absolute Angles (Little Endian)
            int16_t rawAbsoRoll  = (uint8_t)datagram[18] | ((uint8_t)datagram[19] << 8);
            int16_t rawAbsoPitch = (uint8_t)datagram[20] | ((uint8_t)datagram[21] << 8);
            int16_t rawAbsoYaw   = (uint8_t)datagram[22] | ((uint8_t)datagram[23] << 8);

            int16_t rawAbsoVelRoll  = (uint8_t)datagram[24] | ((uint8_t)datagram[25] << 8);
            int16_t rawAbsoVelPitch = (uint8_t)datagram[26] | ((uint8_t)datagram[27] << 8);
            int16_t rawAbsoVelYaw   = (uint8_t)datagram[28] | ((uint8_t)datagram[29] << 8);

            // 4. Update your Line Edits
            ui->relaRoll->setText(QString::number((double)rawRelaRoll/100.0, 'f', 2));
            ui->relaPitch->setText(QString::number((double)rawRelaPitch/100.0, 'f', 2));
            ui->relaYaw->setText(QString::number((double)rawRelaYaw/100.0, 'f', 2));

            ui->absoRoll->setText(QString::number((double)rawAbsoRoll/100.0, 'f', 2));
            ui->absoPitch->setText(QString::number((double)rawAbsoPitch/100.0, 'f', 2));
            ui->absoYaw->setText(QString::number((double)rawAbsoYaw/100.0, 'f', 2));

            m_feedbackPitch = (double)rawAbsoPitch/100.0;
            m_feedbackYaw = (double)rawAbsoYaw/100.0;


            ui->absoVelRoll->setText(QString::number((double)rawAbsoVelRoll/100.0, 'f', 2));
            ui->absoVelPitch->setText(QString::number((double)rawAbsoVelPitch/100.0, 'f', 2));
            ui->absoVelYaw->setText(QString::number((double)rawAbsoVelYaw/100.0, 'f', 2));
        }
    }
}

void GimbalRemote::updateClickDisplay(int x, int y) {
    m_lastX = x;
    m_lastY = y;
    ui->editLastX->setText(QString::number(x));
    ui->editLastY->setText(QString::number(y));
}

void GimbalRemote::moveToCoordinate(int targetX, int targetY) {
    // 1. Define FOV (Example for SIYI A8 mini: ~66.1° HFOV)
    float HFOV = 57.1f;
    float VFOV = 30.4f; // VFOV depends on aspect ratio

    // 2. Calculate Angular Offset from current position
    float deltaYaw = ((targetX - 5000) / 10000.0f) * HFOV;
    float deltaPitch = ((5000 - targetY) / 10000.0f) * VFOV;

    m_integralP = 0.0f;
    m_integralY = 0.0f;
    m_prevErrorP = 0.0f;
    m_prevErrorY = 0.0f;

    // 3. Set Absolute Targets using current feedback
    m_targetYaw = m_feedbackYaw + deltaYaw;
    m_targetPitch = m_feedbackPitch + deltaPitch;

    // --- DEBUG LOG ---
    qDebug().noquote() << QString("GIMBAL [MoveTo]: Target Pixels(%1, %2) | Current Pos: P%3 Y%4 | Delta: P%5 Y%6 | New Target: P%7 Y%8")
                              .arg(targetX).arg(targetY)
                              .arg(m_feedbackPitch, 0, 'f', 2).arg(m_feedbackYaw, 0, 'f', 2)
                              .arg(deltaPitch, 0, 'f', 2).arg(deltaYaw, 0, 'f', 2)
                              .arg(m_targetPitch, 0, 'f', 2).arg(m_targetYaw, 0, 'f', 2);
    if (!m_moveLoopTimer->isActive()) {
        m_moveLoopTimer->start(20); // 50Hz update rate for smoothness
    }
}


// --- BUTTON SLOTS (Updated to use new function) ---

void GimbalRemote::on_pitchUp_clicked() {
    sendGimbalCommand(0.0f, 50.0f, 0.0f, CMD_SPEED_CTRL);
}

void GimbalRemote::on_pitchDown_clicked() {
    sendGimbalCommand(0.0f, -50.0f, 0.0f, CMD_SPEED_CTRL);
}

void GimbalRemote::on_rollUp_clicked() {
    sendGimbalCommand(50.0f, 0.0f, 0.0f, CMD_SPEED_CTRL);
}

void GimbalRemote::on_rollDown_clicked() {
    sendGimbalCommand(-50.0f, 0.0f, 0.0f, CMD_SPEED_CTRL);
}

void GimbalRemote::on_yawUp_clicked() {
    sendGimbalCommand(0.0f, 0.0f, 50.0f, CMD_SPEED_CTRL);
}

void GimbalRemote::on_yawDown_clicked() {
    sendGimbalCommand(0.0f, 0.0f, -50.0f, CMD_SPEED_CTRL);
}

void GimbalRemote::on_sliderRoll_valueChanged(int value) {
    // Assuming slider value is scaled x10 (e.g., 50 = 5.0 deg/s)
    sendGimbalCommand((float)value / 100.0f, 0.0f, 0.0f, CMD_ANGLE_CTRL);
}

void GimbalRemote::on_sliderPitch_valueChanged(int value) {
    sendGimbalCommand(0.0f, (float)value / 100.0f, 0.0f, CMD_ANGLE_CTRL);
}

void GimbalRemote::on_sliderYaw_valueChanged(int value) {
    sendGimbalCommand(0.0f, 0.0f, (float)value / 100.0f, CMD_ANGLE_CTRL);
}

// --- ZOOM (Requires Payload) ---
void GimbalRemote::on_zoomIn_pressed() {
    QByteArray payload;
    payload.append((char)0x01); // Camera 1
    sendGimbalCommand(0, 0, 0, CMD_ZOOM_IN, payload);
}

void GimbalRemote::on_zoomIn_released() {
    QByteArray payload;
    payload.append((char)0x01);
    sendGimbalCommand(0, 0, 0, CMD_ZOOM_STOP, payload);
}

void GimbalRemote::on_zoomOut_pressed() {
    QByteArray payload;
    payload.append((char)0x01);
    sendGimbalCommand(0, 0, 0, CMD_ZOOM_OUT, payload);
}

void GimbalRemote::on_zoomOut_released() {
    QByteArray payload;
    payload.append((char)0x01);
    sendGimbalCommand(0, 0, 0, CMD_ZOOM_STOP, payload);
}

// --- TRACKING (Requires Complex Payload) ---
void GimbalRemote::on_btnTrack_clicked() {
    QByteArray payload;
    payload.append((char)0x01); // Start Tracking [cite: 199]

    // Append Coordinates (Little Endian)
    // Box Top-Left (Using m_lastX/Y for both corners creates a point track)
    payload.append((char)(m_lastX & 0xFF));
    payload.append((char)((m_lastX >> 8) & 0xFF));
    payload.append((char)(m_lastY & 0xFF));
    payload.append((char)((m_lastY >> 8) & 0xFF));

    // Box Bottom-Right
    payload.append((char)(m_lastX & 0xFF));
    payload.append((char)((m_lastX >> 8) & 0xFF));
    payload.append((char)(m_lastY & 0xFF));
    payload.append((char)((m_lastY >> 8) & 0xFF));

    sendGimbalCommand(0, 0, 0, CMD_TRACK, payload);
    qDebug() << "Tracking Cmd Sent:" << m_lastX << m_lastY;
}

void GimbalRemote::on_btnTrackOff_clicked() {
    // Usually sending 0x00 as first payload byte stops tracking
    QByteArray payload;
    payload.append((char)0x00); // Stop Tracking
    // Padding coordinates might be needed to maintain packet structure integrity
    payload.append(8, (char)0x00);

    sendGimbalCommand(0, 0, 0, CMD_TRACK, payload);
}

void GimbalRemote::on_btnNeutral_clicked() {
    sendGimbalCommand(0, 0, 0, CMD_NEUTRAL);
}

void GimbalRemote::on_btnFocus_clicked() {
    QByteArray payload;
    payload.append((char)0x01); // Focus Param [cite: 204]
        // Note: Command ID for Focus is 0x26 in PDF
        sendGimbalCommand(0, 0, 0, CMD_FOCUS, payload);
}

void GimbalRemote::on_goTo_clicked() {
    moveToCoordinate(5273, 886);
}

void GimbalRemote::on_goToB_clicked() {
    moveToCoordinate(3593, 4835);
}

void GimbalRemote::on_ctaProtocol_clicked()
{
    if (m_moveLoopTimer->isActive()) {
        m_moveLoopTimer->stop();
    }

    m_useCustomAim = false;
    qDebug() << "Aim Mode switched to: PROTOCOL (0x1A)";
}

void GimbalRemote::on_ctaCustom_clicked()
{
    if (m_moveLoopTimer->isActive()) {
        m_moveLoopTimer->stop();
    }

    m_useCustomAim = true;
    qDebug() << "Aim Mode switched to: CUSTOM (Software PID)";
}
