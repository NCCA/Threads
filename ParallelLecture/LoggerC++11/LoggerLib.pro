# -------------------------------------------------
# Project created by QtCreator 2009-11-05T12:31:57
# -------------------------------------------------
# qt 5 wants this may cause errors with 4
isEqual(QT_MAJOR_VERSION, 5) {cache() }
QMAKE_CXXFLAGS+=-Wthread-safety
# We will use the built in Qt lib template
TEMPLATE = lib
CONFIG+=staticlib
CONFIG+=c++11
TARGET = NCCALogger
# this is where we want to put the intermediate build files ( ../obj)
OBJECTS_DIR = obj
QMAKE_LFLAGS-= -headerpad_max_install_names
QMAKE_LFLAGS_SHLIB -= -single_module
QMAKE_LFLAGS_VERSION=
QMAKE_LFLAGS_COMPAT_VERSION=
QMAKE_LFLAGS_SONAME=
INCLUDEPATH+=include
INCLUDEPATH+=/usr/local/include/
HEADERS+=$$PWD/Logger.h
macx:QMAKE_MAC_SDK = macosx10.13




SOURCES+= $$PWD/Logger.cpp
CONFIG-=core qt
