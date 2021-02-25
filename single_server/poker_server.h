#ifndef POKER_SERVER_H
#define POKER_SERVER_H

#include <QWidget>
#include "client.h"
#include "server.h"

namespace Ui {
class Poker_Server;
}

class Poker_Server : public QWidget
{
    Q_OBJECT

public:
    explicit Poker_Server(QWidget *parent = 0);
    ~Poker_Server();
public slots:
    void slot_updateChat(QString str);
    void slot_updateLog(QString str);
    void slot_gameBegin();

signals:
    void sig_defaultSet(int money,int id);

private slots:
    void on_pushButton_listen_clicked();


    void on_pushButton_listen_net_clicked();

    void on_pushButton_clicked();

private:
    Ui::Poker_Server *ui;
    Server server;
};

#endif // POKER_SERVER_H
