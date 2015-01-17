#-------------------------------------------------
#
# Project created by QtCreator 2014-12-23T01:32:36
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = omd
TEMPLATE = app


SOURCES += main.cpp\
        viewer.cpp

HEADERS  += viewer.h

win32:      CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libqt-omd/release/ -llibqt-omd
else:win32: CONFIG(debug,   debug|release): LIBS += -L$$OUT_PWD/../libqt-omd/debug/ -llibqt-omd
else:unix:                                  LIBS += -L$$OUT_PWD/../libqt-omd/ -llibqt-omd

INCLUDEPATH += $$PWD/../libqt-omd
DEPENDPATH += $$PWD/../libqt-omd

CONFIG += c++11
