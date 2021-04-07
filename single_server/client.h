#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    Client &operator= (const Client & other);
public slots:
    void slot_readClient();
    void slot_disconnected();
    void slot_defaultSet(int money);
//    void slot_overTime();
//    void slot_gameDiscon();
//    void slot_connected();

signals:
    void sig_radioChatText(QByteArray bta);
    void sig_radioLogText(QByteArray bta);
    void sig_isReady(QString name);
    void sig_newPlayer();
    void sig_disconnected(int desc);
    void sig_playData(int id,int money,bool pass,bool giveup);
    void sig_winner(QString id);

public slots:
public:
    enum S2C_comPro {
        PLAYER_NAME = 11,
        READY_FLAG,
        PLAYDATA = 31,
        WINNER = 43,
        CHAT = 100,
        OVERTIME = 102,
        WATCHENABLE = 103
    };
    quint16 nextBlockSize;
    QString playerName,winnerId;
    int allMoney,addMoney;
    int thisRoundAdd;
    bool isPass,isGiveup,isEnableWatch;
    int seatId,defaultBet,watchId;
    int score;
//    QTimer disconTime,timer1;

};

#endif // CLIENT_H
