# dwm.pri
# 7/10/2011

INCLUDEPATH += $$PWD

DEFINES += WITH_WIN_DWM

HEADERS += \
    $$PWD/dwm.h
SOURCES += \
    $$PWD/dwm.cc

DEFINES += WITH_DWM_NOTIFIER

CONFIG(dwmapi_static) {
    DEFINES += WITH_DWM_STATIC
    LIBS += -Ldwmapi
}

QT      += core gui
LIBS    += -luser32

# EOF
