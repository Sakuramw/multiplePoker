#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
public slots:
    void slot_readClient();
    void slot_disconnected();
    void slot_defaultSet(int money);

signals:
    void sig_radioChatText(QByteArray bta);
    void sig_radioLogText(QByteArray bta);
    void sig_isReady(QString name);
    void sig_newPlayer();
    void sig_disconnected(int desc);
    void sig_playData(int id,int money,bool pass,bool giveup);
    void sig_winner(int id);

public slots:
public:
    quint16 nextBlockSize;
    QString playerName;
    int allMoney,addMoney;
    int thisRoundAdd;
    bool isPass,isGiveup;
    int seatId,defaultBet;
    int score,winnerId;
};

#endif // CLIENT_H
