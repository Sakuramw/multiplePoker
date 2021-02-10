#-------------------------------------------------
#
# Project created by QtCreator 2021-02-07T15:33:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = more_poker_client
TEMPLATE = app


SOURCES += main.cpp\
        pokerclient.cpp \
    player.cpp

HEADERS  += pokerclient.h \
    player.h

FORMS    += pokerclient.ui
