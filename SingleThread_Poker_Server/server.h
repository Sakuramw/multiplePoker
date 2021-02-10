#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "client.h"
#define MAX_PLAYER 100

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void beginGame();

signals:
    void sig_beginGame(int firstId);
    void sig_playingData(int id,int money,
                         bool pass,bool giveup,int nextpalyer);
    void sig_gameover(int winnerId);
    void sig_updatePlayNo(int num);
    void sig_setOnlineId(int id);

public slots:
    void slot_joinPlayer();
    void slot_playerDo(int id,int money,bool pass,bool giveup);
    void slot_gameOver(int winnerId);
    void slot_playerQuit(int id);
private:
    void incomingConnection(int socketId);
    QVector<Client* > ClientList;
    int onlineNum;
    int wantPlayNum;
    int firstPlayer;
    int turnWho;
    int nextPlayer;
    int playing[MAX_PLAYER];

//    int inlineId;
};

#endif // SERVER_H
