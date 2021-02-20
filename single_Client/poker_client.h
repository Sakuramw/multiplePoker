#ifndef POKER_CLIENT_H
#define POKER_CLIENT_H

#include <QTcpSocket>
#include <QWidget>
#define CHAT 100
#define LOG 101
#define SEAT 21
#define WHOPLAY 22

namespace Ui {
class Poker_Client;
}

class Poker_Client : public QWidget
{
    Q_OBJECT

public:
    explicit Poker_Client(QWidget *parent = 0);
    ~Poker_Client();
public slots:
    void slot_readServer();
    void slot_connected();

signals:
//    void sig_updateToLog(QString str);
//    void sig_updateToChat(QString str);


private slots:
    void on_pushButton_send_clicked();

    void on_pushButton_join_clicked();

    void on_pushButton_ready_clicked();


private:
    Ui::Poker_Client *ui;
    QTcpSocket *tcpsocket;
    quint16 nextBlockSize;
    QString m_name;
//    Player player;
    int money,seatId;
    bool isPass,isGiveup,isTurnMy;
};

#endif // POKER_CLIENT_H
