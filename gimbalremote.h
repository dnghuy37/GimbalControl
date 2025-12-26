#ifndef GIMBALREMOTE_H
#define GIMBALREMOTE_H

#include <cstdint>
#include <QWidget>
#include <QUdpSocket>
#include <QTimer>
#include <QByteArray>
#include <QVariantAnimation>

namespace Ui {
class GimbalRemote;
}

// [cite: 154] Define Command IDs globally or inside the class
enum GimbalCmd : uint8_t {
    CMD_NULL        = 0x00,
    CMD_CALIBRATE   = 0x01,
    CMD_NEUTRAL     = 0x03,
    CMD_ANGLE_CTRL  = 0x10, // Control Euler Angles
    CMD_SPEED_CTRL  = 0x11, // Head Lock (Speed)
    CMD_TRACK       = 0x17,
    CMD_CLICK_AIM   = 0x1A,
    CMD_ZOOM_IN     = 0x22,
    CMD_ZOOM_OUT    = 0x23,
    CMD_ZOOM_STOP   = 0x24,
    CMD_ZOOM_SPEC   = 0x25,
    CMD_FOCUS       = 0x26,
    CMD_SHUTTER     = 0x20,
    CMD_RECORD      = 0x21
};

class GimbalRemote : public QWidget
{
    Q_OBJECT

public:
    explicit GimbalRemote(QWidget *parent = nullptr);
    ~GimbalRemote();
    bool isCustomAimMode() const { return m_useCustomAim; }

    /**
     * @brief Sends a command to the gimbal.
     * @param roll  Angle (deg) or Speed (deg/s)
     * @param pitch Angle (deg) or Speed (deg/s)
     * @param yaw   Angle (deg) or Speed (deg/s)
     * @param cmd   Command ID (GimbalCmd enum)
     * @param payload Optional extra bytes (Zoom ID, Coordinates, etc.)
     */
    void sendGimbalCommand(float roll, float pitch, float yaw,
                           GimbalCmd cmd = CMD_NULL,
                           const QByteArray& payload = QByteArray());

    void updateClickDisplay(int x, int y);
    void moveToCoordinate(int targetX, int targetY);

private slots:
    void updateMovementStep();
    void processIncomingData();

    // Button Slots
    void on_pitchUp_clicked();
    void on_pitchDown_clicked();
    void on_rollUp_clicked();
    void on_rollDown_clicked();
    void on_yawUp_clicked();
    void on_yawDown_clicked();

    void on_sliderRoll_valueChanged(int value);
    void on_sliderPitch_valueChanged(int value);
    void on_sliderYaw_valueChanged(int value);

    void on_zoomIn_pressed();
    void on_zoomIn_released();
    void on_zoomOut_pressed();
    void on_zoomOut_released();

    void on_btnTrack_clicked();
    void on_btnTrackOff_clicked();
    void on_goTo_clicked();
    void on_btnNeutral_clicked();
    void on_btnFocus_clicked();
    void on_goToB_clicked();

    void on_ctaProtocol_clicked();
    void on_ctaCustom_clicked();

private:
    Ui::GimbalRemote *ui;
    QUdpSocket *commandSocket;
    QTimer *m_heartbeatTimer = nullptr;
    QTimer *m_moveLoopTimer = nullptr;
    bool m_useCustomAim = false;

    // Helper to calculate CRC16
    uint16_t calculateCRC16(const uint8_t* data, int length);

    // Member variables
    int m_lastX = 0;
    int m_lastY = 0;

    // PID / Movement Variables
    float m_targetYaw = 0;
    float m_feedbackYaw = 0;
    float m_targetPitch = 0;
    float m_feedbackPitch = 0;
    float m_integralP = 0.0f;
    float m_integralY = 0.0f;
    float m_prevErrorP = 0.0f;
    float m_prevErrorY = 0.0f;
    const float m_dt = 0.020f;
};

#endif // GIMBALREMOTE_H
