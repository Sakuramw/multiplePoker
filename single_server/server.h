﻿#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QTcpServer>
#include "client.h"
#define MAXNUM 100

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void serverRadioLog(QByteArray bta);
    void shuffleCards();
    void m_sleep(int msec);

signals:
    void sig_updateLog(QString str);
    void sig_updateChat(QString str);
    void sig_gameBegin();
    void sig_next();
    void sig_roundNum(int round);

public slots:
    void slot_emitChatText(QByteArray bta);
    void slot_emitLogText(QByteArray bta);
    void slot_newPlayer();
    void slot_newReady(QString name);
    void slot_disconnected(int desc);
    void slot_playGame();
    void slot_next();
    void slot_roundNum(int round);
    void slot_playData(int id,int money,bool pass,bool giveup);

private:
    void incomingConnection(int socketId);
    QVector<Client* > ClientList;
    QVector<int > readyId,inDesk;
    int turnWho,round,beatId,inquiryNum,firstPassId;
    QStringList poker;
    bool isANC,isFP;
};

#endif // SERVER_H
