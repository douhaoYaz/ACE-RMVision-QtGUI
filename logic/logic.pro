TEMPLATE = app
TARGET = ACEVision

#更改这里以改变ui
DEFINES += QTGUI

CONFIG += console c++11
LIBS += \
        -pthread \
        -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_video \
        -lopencv_calib3d \
        -lopencv_videoio \
        -lopencv_dnn \
        /usr/lib/libgxiapi.so \
        /usr/lib/libdximageproc.so \
#        -L../../build-ACERMVision/bin/ \
        -L../../build-ACERMVision/bin/ \
        -lutils \
        -lalgorithm \
        -lglobal \
        -lsettings

INCLUDEPATH += \
        ../utils/include \
        ../global/include \
        ../settings/include \
        ../algorithm/include \
        /usr/local/include/opencv4

INCLUDEPATH += \
        /home/douhao/ACE/dhcam_install_20181107/dh_camera/daheng-sdk-x64/sdk/genicam/library/CPP/include \
        /home/douhao/ACE/dhcam_install_20181107/dh_camera/daheng-sdk-x64/sdk/include

HEADERS += \
    armor_detector.h \
    buff_detector.h \
    port.h \
    solve_angle.h \
    thread_task.h \
    top_detector.h \
    ui.h

SOURCES += \
    buff_detector.cpp \
    port.cpp \
    thread_task.cpp \
    armor_detector.cpp \
    solve_angle.cpp \
    top_detector.cpp \
    ui.cpp

contains(DEFINES, QTGUI){
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    QT += core gui

    LIBS += \
        -L../../build-ACERMVision/bin/ \
        -lui-component \
        -lqtgui

    INCLUDEPATH += \
            ../ui/qtgui/include \
            ../ui-component/include

    SOURCES += \
            main_qtgui.cpp \
            qtgui.cpp

    HEADERS += \
            qtgui.h
}

contains(DEFINES, CUI) {
    CONFIG -= app_bundle
#    CONFIG -= qt
    LIBS += \
        -lopencv_highgui
    HEADERS += cui.h
    SOURCES += \
        main_cui.cpp \
        cui.cpp
}

contains(DEFINES, NOUI) {
    CONFIG -= app_bundle
    CONFIG -= qt
    SOURCES += main_noui.cpp
}
