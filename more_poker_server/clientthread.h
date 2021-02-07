#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(int socketDescriptor,QObject *parent = 0);
    ~ClientThread();
    void run();

signals:
    void sig_error(QTcpSocket::SocketError socketError);

public slots:
private:
    int socketDescriptor;
};

#endif // CLIENTTHREAD_H
