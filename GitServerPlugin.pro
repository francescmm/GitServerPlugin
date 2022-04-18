TEMPLATE = lib

CONFIG += plugin

VERSION = 0.1.0

DEFINES += GITSERVERPLUGIN_LIBRARY QT_DEPRECATED_WARNINGS

TARGET = GitServerPlugin

QT += network widgets webenginewidgets webchannel

CONFIG += qt warn_on c++17 c++1z

DESTDIR = lib

include(src/GitServerPlugin.pri)
include(src/git/Git.pri)
include(src/git_server/GitServer.pri)
include(src/git_server/GitServerWidgets.pri)
include(src/AuxiliarCustomWidgets/AuxiliarCustomWidgets.pri)
include(src/QLogger/QLogger.pri)

win32 {
    QMAKE_TARGET_PRODUCT = "$$TARGET"
    QMAKE_TARGET_COMPANY = "Cesc Software"
    QMAKE_TARGET_COPYRIGHT = "Francesc M."
} else:mac {
    QMAKE_TARGET_BUNDLE_PREFIX = "com.francescmm."
}

CONFIG(debug, debug|release) {
    mac: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,d)
    linux: TARGET = $$join(TARGET,,,d_$$VERSION)
}

win32 {
   copydata.commands = $(COPY_DIR) $$shell_quote($$shell_path($$PWD/src/interface/*.h)) $${DESTDIR}
} else {
   copydata.commands = $(COPY_DIR) $$PWD/src/interface/IGitServerWidget.h $$PWD/src/interface/GitServerTypes.h $$PWD/src/interface/ConfigData.h $$PWD/src/interface/IGitServerCache.h $$PWD/src/interface/gitserverplugin_global.h $${DESTDIR}
}

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
