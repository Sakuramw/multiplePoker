#ifndef SERVER_H
#define SERVER_H

#include <QObject>
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

signals:
    void sig_updateLog(QString str);
    void sig_updateChat(QString str);
    void sig_gameBegin();

public slots:
    void slot_emitChatText(QByteArray bta);
    void slot_emitLogText(QByteArray bta);
    void slot_newPlayer();
    void slot_newReady(QString name);
    void slot_disconnected(int desc);
    void slot_playGame();

private:
    void incomingConnection(int socketId);
    QVector<Client* > ClientList;
    QVector<int > readyId;
    int turnWho;
};

#endif // SERVER_H
