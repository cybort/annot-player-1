# annotcloud.pri
# 6/28/2011

include(../../config.pri)

DEFINES += WITH_MODULE_ANNOTCLOUD

HEADERS += \
    $$PWD/alias.h\
    $$PWD/annotation.h \
    $$PWD/annothtml.h \
    $$PWD/annotpaint.h \
    $$PWD/annottag.h \
    $$PWD/annotxml.h \
    $$PWD/traits.h \
    $$PWD/token.h \
    $$PWD/user.h

SOURCES += \
    $$PWD/alias.cc \
    $$PWD/annotation.cc \
    $$PWD/annothtml.cc \
    $$PWD/annothtmlparse.cc \
    $$PWD/annothtmlthread.cc \
    $$PWD/annothtmlunparse.cc \
    $$PWD/annotpaint.cc \
    $$PWD/annotxml.cc \
    $$PWD/token.cc \
    $$PWD/user.cc

JSF_FILES += \
    $$PWD/jsf/a.xhtml \
    $$PWD/jsf/i.xhtml \
    $$PWD/jsf/t.xhtml

QT +=   core xml webkit

OTHER_FILES += $$JSF_FILES

# EOF


