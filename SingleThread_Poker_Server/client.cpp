#include "client.h"

Client::Client(QObject *parent) : QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),
            this,SLOT(slot_readClient()));
    connect(this,SIGNAL(disconnected()),
            this,SLOT(deleteLater()));
    connect(this,SIGNAL(disconnected()),
            this,SLOT(slot_emitOnlineId()));
}

Client::~Client()
{

}

void Client::setId(int id)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << 21 <<id ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    write(block);
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
    in >>requestType;
    switch(requestType){

    case 11:
        in>>player.playerName;
        emit sig_joinPlayer();
        break;
    case 3:
        in>>player.playerID>>player.money>>player.isPass>>player.isGiveup;
        emit sig_iDo(player.playerID,player.money,player.isPass,player.isGiveup);
        break;

    }
}

void Client::slot_beginGame(int first)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << 22 <<first ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    write(block);
}

void Client::slot_gameOver(int winnerId)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << 41 <<winnerId ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    write(block);
}

void Client::slot_updatePlayNo(int num)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << 11 <<num ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    write(block);
}

void Client::slot_getOnlineId(int id)
{
    onlineId = id;
}

void Client::slot_emitOnlineId()
{
    emit sig_emitOnlineId(onlineId);
}

