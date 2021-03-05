#ifndef POKER_CLIENT_H
#define POKER_CLIENT_H

#include <QTcpSocket>
#include <QWidget>
#include <QStringList>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QDialog>
//#define CHAT 100
//#define LOG 101
//#define SEAT 21
//#define WHOPLAY 22
//#define MYCARDS 23
//#define PUBLICCARD 33
//#define RADIOCARD 34
//#define NEWROUND 35
//#define OVERFLAG 41
//#define JUDGE 36
//#define OVERTIME 102
//#define SCORELIST 14
//#define PLAYERLIST 13
//#define PAUSE 37
//#define CONTINUE 38

namespace Ui {
class Poker_Client;
}

class Poker_Client : public QWidget
{
    Q_OBJECT

public:
    explicit Poker_Client(QWidget *parent = 0);
    ~Poker_Client();
//    void keyPressEvent(QKeyEvent *event);
    enum C2S_comProt {
        PLAYERLIST = 13,
        SCORELIST,
        SEAT = 21,
        WHOPLAY,
        MYCARDS,
        PUBLICCARD = 33,
        RADIOCARD,
        NEWROUND,
        JUDGE,
        PAUSE,
        CONTINUE,
        OVERFLAG = 41,
        CHAT = 100,
        LOG,
        OVERTIME
    };
public slots:
    void slot_readServer();
    void slot_connected();
    void slot_disconnected();
    void slot_quicklyMessage(QString str);
//    void slot_loseConnect();
//    void slot_turnMy();
//    void slot_gameOver();

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

    void on_pushButton_winner_clicked();

private:
    Ui::Poker_Client *ui;
    QTcpSocket *tcpsocket;
    quint16 nextBlockSize;
    QString m_name,serverIp;
//    Player player;
    int addMoney,seatId,turnWho,myCardFlag,puCardFlag;
    bool isPass,isPause,isGiveup,isTurnMy,isAdd,isCall,isNewRound,isFirstRun,isSending;
    int score,judgeId,port;
    QStringList nameList,playerScore;
    QList<QLabel *> cardLabel;
    QList<QLabel *> pCard;
    QDialog *pauseDialog;
};

#endif // POKER_CLIENT_H
