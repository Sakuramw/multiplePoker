#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QStringList>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    ~Player();
//    int getId();
//    int getMoney();
//    bool getIsPass();
//    bool getIsGiveup();
    QString playerName;
    int playerID;
    QStringList allPlayerName;
    bool isOK;
    int beginInWho;
    int money;
    bool isPass;
    bool isGiveup;
    int score;
    int winnerNo;

signals:

public slots:
private:



};

#endif // PLAYER_H
