#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QTcpServer>
#include <QCoreApplication>
#include "client.h"
#define MAXNUM 100
#define PLAYERLIST 13

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void serverRadioLog(QByteArray bta);
    void shuffleCards();
    void m_sleep(int msec);
    void newRound();
    QVector<Client* > playClientList;
    QVector<Client* > reconnectedClient;
    QVector<Client* > watchClient;
    QVector<Client* > tempClient;
    quint64 maxCardLevel(QStringList sevCard);
    void winRateTest(int playernum);
    void sendAllList();

signals:
    void sig_updateLog(QString str);
    void sig_updateChat(QString str);
    void sig_gameBegin();
//    void sig_next();
    void sig_roundNum(int round);
    void sig_gameOver(int status);

public slots:
    void slot_emitChatText(QByteArray bta);
    void slot_emitLogText(QByteArray bta);
    void slot_newPlayer();
    void slot_newReady(QString name);
    void slot_disconnected(int desc);
    void slot_playGame();
    void slot_reconnected();
//    void slot_next();
    void slot_roundNum(int round);
    void slot_playData(int id,int money,bool pass,bool giveup);
    void slot_gameOver(int status);
    void slot_defaultSet(int money,int id);
    void slot_winner(QString id);
private:
    void incomingConnection(int socketId);
    QVector<int > readyId,inDesk,winnerId,watchPlayerId;
    int whoDealer,round,beatId,firstPassId,whoCall;
    int allMoney,defaultMoney,defaultJudge,whoNext;
//    int callMoney;
    int addMoney,divideMoney;
    QStringList poker,playerList,playerScore,watchList;
    bool isANC,isFP,isFirstRun,isNewRound,isPlaying,isRecon;
    bool isSomeOneLose;
};

#endif // SERVER_H
