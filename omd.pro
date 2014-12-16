#-------------------------------------------------
#
# Project created by QtCreator 2014-12-15T23:18:39
#
#-------------------------------------------------

QT       += core gui network xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = omd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    liveview.cpp \
    viewer.cpp \
    cameraproperties.cpp

HEADERS  += mainwindow.h \
    camera.h \
    liveview.h \
    viewer.h \
    cameraproperties.h

FORMS    += mainwindow.ui
