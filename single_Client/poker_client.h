#ifndef POKER_CLIENT_H
#define POKER_CLIENT_H

#include <QTcpSocket>
#include <QWidget>
#include <QStringList>
#include <QLabel>
#define CHAT 100
#define LOG 101
#define SEAT 21
#define WHOPLAY 22
#define MYCARDS 23

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
//    void slot_turnMy();

signals:
//    void sig_updateToLog(QString str);
//    void sig_updateToChat(QString str);
//    void sig_iDo();


private slots:
    void on_pushButton_send_clicked();

    void on_pushButton_join_clicked();

    void on_pushButton_ready_clicked();


    void on_pushButton_add_clicked();

    void on_pushButton_pass_clicked();

    void on_pushButton_giveup_clicked();

    void on_pushButton_call_clicked();

private:
    Ui::Poker_Client *ui;
    QTcpSocket *tcpsocket;
    quint16 nextBlockSize;
    QString m_name;
//    Player player;
    int allMoney,addMoney,seatId,turnWho;
    bool isPass,isGiveup,isTurnMy,isAdd;
    QStringList myCards;
    QList<QLabel *> cardLabel;
};

#endif // POKER_CLIENT_H
