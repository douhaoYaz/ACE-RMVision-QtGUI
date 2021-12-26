TEMPLATE = subdirs

DEFINES += \
        QTGUI

SUBDIRS +=       \
    global       \
    utils        \
    settings     \
    algorithm    \
    ui-component \
    ui           \
    logic

CONFIG += ordered
