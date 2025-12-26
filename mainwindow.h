#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QtNetwork/qudpsocket.h"
#include "gimbalremote.h" // Include the header
#include <QMainWindow>
#include <QUdpSocket>
#include <QMouseEvent>
#include <QUrl>
#include "gstvideowidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    void setRemote(GimbalRemote *remote) { m_remote = remote; }

private:
    GimbalRemote *m_remote = nullptr;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    // This catches mouse clicks on the window
    void mousePressEvent(QMouseEvent *event) override;
private:
    Ui::MainWindow *ui;
    GstVideoWidget *m_videoWidget;
    QUdpSocket *commandSocket; // Separate from the video player
};
#endif // MAINWINDOW_H;
