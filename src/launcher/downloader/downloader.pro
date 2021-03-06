# downloader.pro
# 9/3/2011

VERSION = 0.1.0.0

CONFIG += noqt nocrt
include(../../../config.pri)
include($$ROOTDIR/src/launcher/launcher.pri)

## Libraries

#QT              += core gui

## Sources

TEMPLATE    = app
win32: CONFIG += windows
TARGET      = "[ Download ]"

HEADERS += config.h
SOURCES += main.cc

OTHER_FILES += \
    downloader.rc \
    downloader.ico

win32 {
    !wince*: LIBS += -lshell32
    RC_FILE += downloader.rc
}

#mac {
#    ICON = app.icns
#    QMAKE_INFO_PLIST = Info.plist
#}


# EOF

