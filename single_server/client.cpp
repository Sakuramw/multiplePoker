#include "client.h"
#define CHAT 100
#define PLAYER_NAME 11
#define READY_FLAG 12
#define PLAYDATA 31
#define WINNER  43

Client::Client(QObject *parent) : QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),
            this,SLOT(slot_readClient()));
    connect(this,SIGNAL(disconnected()),
            this,SLOT(slot_disconnected()));

    nextBlockSize = 0,allMoney = defaultBet,addMoney = 0;
    score = 0;
}

Client::~Client()
{

}


void Client::slot_readClient()
{
    QDataStream in(this);
    in.setVersion((QDataStream::Qt_4_8));
    if(nextBlockSize == 0){
        if(bytesAvailable()<sizeof(quint16)) return;

        in >> nextBlockSize;
    }

    if(bytesAvailable()<nextBlockSize) return;
    quint8 requestType;
    bool areYouOk = false;
    QByteArray btaChat ;
    QByteArray tempName;
    QString str;
    in >> requestType;
    switch(requestType){
    case PLAYER_NAME:
        in >> tempName;
        playerName = QString(tempName);
        emit sig_newPlayer();
        break;
    case READY_FLAG:
        in >> areYouOk;
        if(areYouOk){
            emit sig_isReady(playerName);
        }
        break;
    case PLAYDATA:
        in >> seatId;
        in >> addMoney >>isPass >>isGiveup;
        if(isPass){
            str = playerName+"跳过";
            emit sig_radioLogText(str.toUtf8());
        }
        if(isGiveup){
            str = playerName+"弃牌";
            emit sig_radioLogText(str.toUtf8());
        }
        if(addMoney != 0){
            allMoney += addMoney;
            score = score - addMoney;
            str = playerName+"加注："+QString::number(addMoney);
            emit sig_radioLogText(str.toUtf8());

        }
        if(addMoney == 0&&!isPass&&!isGiveup){
            str = playerName+"跟注";
            emit sig_radioLogText(str.toUtf8());
        }
        emit sig_playData(seatId,addMoney,isPass,isGiveup);
        break;
    case WINNER:
        in >> winnerId;
        emit sig_winner(winnerId);
        break;

    case CHAT:
        in >> btaChat;
        emit sig_radioChatText(btaChat);
        break;


    }
#if 0
    QByteArray btaChat /*= readAll()*/;
    in >> btaChat;
    emit sig_radioChatText(btaChat);
#endif
    nextBlockSize = 0;
}

void Client::slot_disconnected()
{
    emit sig_disconnected(this->socketDescriptor());
}

void Client::slot_defaultSet(int money)
{
    defaultBet = money;
}


