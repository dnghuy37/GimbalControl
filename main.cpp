#include "mainwindow.h"
#include "gimbalremote.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    // Window 1: Camera Feed
    MainWindow w;
    w.setWindowTitle("Gimbal View");
    GimbalRemote r;
    r.setWindowTitle("Gimbal Control");

    w.setRemote(&r); // Give MainWindow access to the Remote logic
    w.show();
    r.show();
    return a.exec();
}
