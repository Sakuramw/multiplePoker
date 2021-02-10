#ifndef POKERSERVER_H
#define POKERSERVER_H

#include "server.h"

#include <QWidget>

namespace Ui {
class PokerServer;
}

class PokerServer : public QWidget
{
    Q_OBJECT

public:
    explicit PokerServer(QWidget *parent = 0);
    ~PokerServer();
public slots:
    void slot_gameOver(int winner);

private slots:
    void on_pushButton_listen_clicked();


private:
    Ui::PokerServer *ui;

    Server pokerServer;
};

#endif // POKERSERVER_H
