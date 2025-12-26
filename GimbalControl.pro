QT       += core gui
QT       += core gui network
QT       += core gui network multimedia multimediawidgets
QT       += widgets

CONFIG += c++17

INCLUDEPATH += C:/gstreamer/1.0/msvc_x86_64/include/gstreamer-1.0
INCLUDEPATH += C:/gstreamer/1.0/msvc_x86_64/include/glib-2.0
INCLUDEPATH += C:/gstreamer/1.0/msvc_x86_64/lib/glib-2.0/include

LIBS += -LC:/gstreamer/1.0/msvc_x86_64/lib
LIBS += -lgstreamer-1.0
LIBS += -lgstvideo-1.0
LIBS += -lgobject-2.0
LIBS += -lglib-2.0

LIBS += -LC:/gstreamer/1.0/msvc_x86_64/bin
LIBS += -lgstreamer-1.0
LIBS += -lgstvideo-1.0
LIBS += -lgobject-2.0
LIBS += -lglib-2.0

SOURCES += \
    gimbalremote.cpp \
    gstvideowidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    gimbalremote.h \
    gstvideowidget.h \
    mainwindow.h

FORMS += \
    gimbalremote.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
