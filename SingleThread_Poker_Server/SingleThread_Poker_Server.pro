#-------------------------------------------------
#
# Project created by QtCreator 2021-02-10T10:48:06
#
#-------------------------------------------------

QT       += core gui network

QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SingleThread_Poker_Server
TEMPLATE = app


SOURCES += main.cpp\
        pokerserver.cpp \
    server.cpp \
    client.cpp \
    player.cpp

HEADERS  += pokerserver.h \
    server.h \
    client.h \
    player.h

FORMS    += pokerserver.ui
