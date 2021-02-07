#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>

#include "clientthread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

signals:
protected:
    void incomingConnection(int socketDescriptor);

public slots:
private:
    QList<ClientThread*> clientThreadList;
};

#endif // SERVER_H
