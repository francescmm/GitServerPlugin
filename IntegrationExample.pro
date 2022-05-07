CONFIG += qt warn_on c++17 c++1z

DEFINES += QT_DEPRECATED_WARNINGS \
   SOURCE_PATH=\\\"$$PWD\\\"

TARGET = IntegrationExample

QT += network widgets webenginewidgets webchannel

include(src/GitServerPlugin.pri)
include(src/git/Git.pri)
include(src/git_server/GitServer.pri)
include(src/git_server/GitServerWidgets.pri)
include(src/AuxiliarCustomWidgets/AuxiliarCustomWidgets.pri)
include(src/QLogger/QLogger.pri)

SOURCES += $$PWD/main.cpp
