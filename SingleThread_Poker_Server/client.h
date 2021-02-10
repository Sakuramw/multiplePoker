#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "player.h"

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    void setId(int id);
    Player player;

    int onlineId;

signals:
    void sig_joinPlayer();
    void sig_iDo(int id,int money,bool pass,bool giveup);
    void sig_emitOnlineId(int id);


public slots:
    void slot_readClient();
    void slot_beginGame(int first);
    void slot_gameOver(int winnerId);
    void slot_updatePlayNo(int num);
    void slot_getOnlineId(int id);
    void slot_emitOnlineId();
private:
    quint16 nextBlockSize;

};

#endif // CLIENT_H
