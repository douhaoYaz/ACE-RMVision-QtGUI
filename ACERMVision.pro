TEMPLATE = subdirs

DEFINES += \
        QTGUI

SUBDIRS +=       \
    global       \
    ui-component \
    utils        \
    settings     \
    algorithm    \
#    ui-component \
    ui           \
    logic

CONFIG += ordered
