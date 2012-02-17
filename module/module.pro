# module.pro
# 7/31/2011

include(aiml/aiml.pri)
include(annotcloud/annotcloud.pri)
include(annotcodec/annotcodec.pri)
include(blockiodevice/blockiodevice.pri)
include(clientagent/clientagent.pri)
include(compress/compress.pri)
include(crypt/crypt.pri)
include(datastream/datastream.pri)
include(download/download.pri)
include(debug/debug.pri)
include(doll/doll.pri)
include(gsoap/gsoap.pri)
include(luacpp/luacpp.pri)
include(luaresolver/luaresolver.pri)
include(mp4box/mp4box.pri)
include(mrlresolver/mrlresolver.pri)
include(player/player.pri)
include(qtext/qtext.pri)
include(serveragent/serveragent.pri)
include(streamservice/streamservice.pri)
include(translator/translator.pri)

TEMPLATE = subdirs

!unix: SUBDIRS = webbrowser
unix:include(webbrowser/webbrowser_static.pri)

# EOF
