#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include<QTcpSocket>

class Socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = 0);
    ~Socket();

signals:

public slots:
};

#endif // SOCKET_H
