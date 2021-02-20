#include "client.h"
#define CHAT 100
#define PLAYER_NAME 11
#define READY_FLAG 12

Client::Client(QObject *parent) : QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),
            this,SLOT(slot_readClient()));
    connect(this,SIGNAL(disconnected()),
            this,SLOT(slot_disconnected()));

    nextBlockSize = 0;
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


