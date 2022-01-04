TEMPLATE = lib
DEFINES += QTGUI_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT += core gui

LIBS += \
        -lopencv_core                   \
        -L../../../build-ACERMVision/bin/ \
        -lglobal        \
        -lui-component  \
        -lsettings

SOURCES += \
        image_widget.cpp \
        main_window.cpp \
        start_winodw.cpp

HEADERS += \
        include/image_widget.h \
        include/main_window.h \
        include/start_window.h

INCLUDEPATH += \
        /usr/local/include/opencv4  \
        ../../global/include        \
        ../../settings/include      \
        ../../ui-component/include

DESTDIR = ../../bin
