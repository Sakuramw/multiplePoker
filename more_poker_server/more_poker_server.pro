#-------------------------------------------------
#
# Project created by QtCreator 2021-02-07T14:52:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = more_poker_server
TEMPLATE = app


SOURCES += main.cpp\
        pokerserver.cpp \
    server.cpp \
    thread.cpp

HEADERS  += pokerserver.h \
    server.h \
    thread.h

FORMS    += pokerserver.ui
