#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T16:21:51
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += network
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myplayer
TEMPLATE = app


SOURCES += main.cpp\
        myplayer.cpp \
    lrc.cpp \
    httpwindow.cpp

HEADERS  += myplayer.h \
    lrc.h \
    httpwindow.h
RC_FILE = Icon.rc
