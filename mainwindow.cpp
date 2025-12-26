#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray> // Make sure this is included

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_videoWidget = new GstVideoWidget(this);
    setCentralWidget(m_videoWidget);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    double W = m_videoWidget->width();
    double H = m_videoWidget->height();
    QPoint localPos = m_videoWidget->mapFromParent(event->pos());

    // Calculate 0-10000 coordinates
    int x10k = qBound(0, static_cast<int>((localPos.x() / W) * 10000.0), 10000);
    int y10k = qBound(0, static_cast<int>((localPos.y() / H) * 10000.0), 10000);

    if (m_remote) {
        m_remote->updateClickDisplay(x10k, y10k);

        // --- BRANCH LOGIC BASED ON SELECTED MODE ---
        if (m_remote->isCustomAimMode()) {
            // MODE 1: Custom Software PID
            qDebug() << "Click: Executing Custom MoveTo";
            m_remote->moveToCoordinate(x10k, y10k);
        }
        else {
            // MODE 2: Built-in Protocol (0x1A)
            qDebug() << "Click: Executing Protocol 0x1A";

            // Construct Payload: 0x01 + X (U16) + Y (U16)
            QByteArray payload;
            payload.append((char)0x01);

            // Append X (Little Endian)
            payload.append((char)(x10k & 0xFF));
            payload.append((char)((x10k >> 8) & 0xFF));

            // Append Y (Little Endian)
            payload.append((char)(y10k & 0xFF));
            payload.append((char)((y10k >> 8) & 0xFF));

            m_remote->sendGimbalCommand(0.0f, 0.0f, 0.0f, CMD_CLICK_AIM, payload);
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
