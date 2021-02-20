#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
public slots:
    void slot_readClient();
    void slot_disconnected();

signals:
    void sig_radioChatText(QByteArray bta);
    void sig_isReady(QString name);
    void sig_newPlayer();
    void sig_disconnected(int desc);

public slots:
public:
    quint16 nextBlockSize;
    QString playerName;
};

#endif // CLIENT_H
