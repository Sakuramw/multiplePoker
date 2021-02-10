#ifndef POKERCLIENT_H
#define POKERCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QDateTime>
#include "player.h"
enum Suit{
    spade,
    heart,
    club,
    diamond
};


namespace Ui {
class PokerClient;
}

class PokerClient : public QWidget
{
    Q_OBJECT

public:
    explicit PokerClient(QWidget *parent = 0);
    ~PokerClient();

private slots:
    void on_pushButton_connect_clicked();

    void slot_connected();
    void slot_serverClose();
    void slot_readServer();

    void on_pushButton_500_clicked();

private:

    Ui::PokerClient *ui;
    QTcpSocket *tcpConnect;
//    QDateTime dateTime;//直接使用服务器的时间
//    int requestType;//0为点击连接服务器阶段，发送单独的值询问玩家编号；1为准备阶段，发送单独的准备信号；2为玩的时候，发送信息；3为了牢记的信息。-1为结束标志.
//    QString m_Name;
    int allPlayerNo;
    int suit;
    int cardNo;
    int money;
    QString LeLaoJi;
    int nextBlockSize;
    QDateTime recDateTime;
    Player player;
};

#endif // POKERCLIENT_H
