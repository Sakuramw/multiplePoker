#ifndef POKER_H
#define POKER_H

#include <QObject>

class Poker : public QObject
{
    Q_OBJECT
public:
    explicit Poker(quint8 su,quint8 nu,QObject *parent = 0);
    Poker();
    ~Poker();
    quint8 getsuit();
    quint8 getnum();
    QString toString();
    Poker& operator=(const Poker &pk);
signals:

public slots:
private:
    quint8 suit;
    quint8 num;
};

#endif // POKER_H
