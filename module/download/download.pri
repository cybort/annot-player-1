# download.pri
# 2/4/2012

include(../../config.pri)
include($$ROOTDIR/module/compress/compress.pri)

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/download.h \
    $$PWD/downloader.h \
    $$PWD/downloadmanager.h \
    $$PWD/downloadtask.h \
    $$PWD/mrldownloadtask.h

SOURCES += \
    $$PWD/download.cc \
    $$PWD/downloader.cc \
    $$PWD/downloadmanager.cc \
    $$PWD/downloadtask.cc \
    $$PWD/mrldownloadtask.cc

QT      += core network

# EOF