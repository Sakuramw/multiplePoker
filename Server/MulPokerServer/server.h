#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>



class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, int port = 0);
    ~Server();
     QList<TcpClientSocket*> tcpclientsocketlist;
protected:
     void incomingConnection(int socket);//对虚函数进行重写
public slots:
     slot_clientData(QString );
     slot_clientConnect(int );

signals:
     void sig_update(QString );

public slots:
};

#endif // SERVER_H
