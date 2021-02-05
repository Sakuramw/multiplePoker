#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>

namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = 0);
    ~TcpServer();

private:
    Ui::TcpServer *ui;
    int port;
//    Server *server;
public slots:
    void slot_update(QString);//收到信号更新页面

private slots:
    void on_pushButton_begin_clicked();
};

#endif // TCPSERVER_H
