#-------------------------------------------------
#
# Project created by QtCreator 2014-12-15T23:18:39
#
#-------------------------------------------------

QT       += core network xml gui

TARGET = libqt-omd
TEMPLATE = lib

SOURCES += camera.cpp \
    liveview.cpp \
    properties.cpp

HEADERS  += camera.h \
    liveview.h \
    properties.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG += c++11
