CONFIG -= qt

TEMPLATE = lib
DEFINES += UTILS_LIBRARY
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv4 \
                ../algorithm/include

INCLUDEPATH += \
        /home/douhao/ACE/dhcam_install_20181107/dh_camera/daheng-sdk-x64/sdk/genicam/library/CPP/include \
        /home/douhao/ACE/dhcam_install_20181107/dh_camera/daheng-sdk-x64/sdk/include

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_video \
        -L/usr/lib \
        -lgxiapi \
        -ldximageproc


SOURCES += \
    capture_video.cpp \
    kalman.cpp \
    serial.cpp \
    utils.cpp

HEADERS += \
    include/capture_video.h \
    include/kalman.hpp \
    include/serial.h \
    include/utils.h

DESTDIR = ../bin
