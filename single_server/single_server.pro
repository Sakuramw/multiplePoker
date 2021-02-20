#-------------------------------------------------
#
# Project created by QtCreator 2021-02-19T16:07:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = single_server
TEMPLATE = app


SOURCES += main.cpp\
        poker_server.cpp \
    client.cpp \
    server.cpp

HEADERS  += poker_server.h \
    client.h \
    server.h \
    transportdata.h

FORMS    += poker_server.ui
