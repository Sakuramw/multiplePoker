#-------------------------------------------------
#
# Project created by QtCreator 2021-02-05T17:12:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MulPokerServer
TEMPLATE = app


SOURCES += main.cpp\
        tcpserver.cpp \
    server.cpp

HEADERS  += tcpserver.h \
    server.h

FORMS    += tcpserver.ui
