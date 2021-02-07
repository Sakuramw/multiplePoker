#ifndef POKERCLIENT_H
#define POKERCLIENT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class PokerClient;
}

class PokerClient : public QWidget
{
    Q_OBJECT

public:
    explicit PokerClient(QWidget *parent = 0);
    ~PokerClient();

private:
    Ui::PokerClient *ui;
    QTcpSocket *tcpConnect;
};

#endif // POKERCLIENT_H
