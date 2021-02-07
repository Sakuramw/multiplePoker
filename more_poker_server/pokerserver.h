#ifndef POKERSERVER_H
#define POKERSERVER_H

#include <QWidget>
#include "server.h"
#include "clientthread.h"

namespace Ui {
class PokerServer;
}

class PokerServer : public QWidget
{
    Q_OBJECT

public:
    explicit PokerServer(QWidget *parent = 0);
    ~PokerServer();

private slots:
    void on_pushButton_listen_clicked();
    void slot_newConnect();

private:
    Ui::PokerServer *ui;
    Server server;
    int port;
};

#endif // POKERSERVER_H
