#include "server.h"

#include <QTime>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    turnWho = 0;
    connect(this,SIGNAL(sig_gameBegin()),
            this,SLOT(slot_playGame()));
}

Server::~Server()
{

}

void Server::serverRadioLog(QByteArray bta)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(101) <<bta ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    for(int i = 0;i<ClientList.count();i++){
        ClientList[i]->write(block);
    }

}

void Server::incomingConnection(int socketId)
{

    Client *socket = new Client(this);
    readyId.append(0);
    socket->setSocketDescriptor(socketId);
    ClientList.append(socket);
    connect(socket,SIGNAL(sig_radioChatText(QByteArray)),
            this,SLOT(slot_emitChatText(QByteArray)));
    //    connect(socket,SIGNAL(connected()),
    //            this,SLOT(slot_emitChatText(QByteArray)));
    connect(socket,SIGNAL(sig_newPlayer()),
            this,SLOT(slot_newPlayer()));
    connect(socket,SIGNAL(sig_isReady(QString)),
            this,SLOT(slot_newReady(QString)));
    connect(socket,SIGNAL(sig_disconnected(int)),
            this,SLOT(slot_disconnected(int)));
}

void Server::slot_emitChatText(QByteArray bta)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(100) <<bta ;
    out.device() ->seek(0);
    qDebug()<<quint16(block.size()-sizeof(quint16));
    out<<quint16(block.size()-sizeof(quint16));
    qDebug()<<block.size();
    for(int i = 0;i<ClientList.count();i++){
        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
    }
    emit sig_updateChat(QString(bta));

}

void Server::slot_emitLogText(QByteArray bta)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(101) <<bta;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));

    for(int i = 0;i<ClientList.count();i++){

        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
    }
    emit sig_updateLog(QString(bta));
}

void Server::slot_newPlayer()
{
    QString temp = ClientList[ClientList.count()-1]->playerName + "加入房间";
    slot_emitLogText(temp.toUtf8());
}

void Server::slot_newReady(QString name)
{
    QString str;
    str = name + "已准备";
    slot_emitLogText(str.toUtf8());
    for(int i = 0;i<ClientList.count();i++){
        if(ClientList[i]->playerName == name){
            readyId[i] = 1;
        }
    }
    int readyNum = 0;
    for(int i = 0;i<readyId.count();i++){
        if(readyId[i] == 1) readyNum++;
    }
    if(readyNum == readyId.count()){  //游戏开始
        QString str = "游戏开始";
        slot_emitLogText(str.toUtf8());
//        QTime t1 = QTime::currentTime();
//        QTime now;
//        do{
//            now=QTime::currentTime();
//        }while(t1.msecsTo(now)<=500);
        emit sig_gameBegin();
    }
}

void Server::slot_disconnected(int desc)
{
    for(int i = 0;i<ClientList.count();i++){
        if(ClientList[i]->socketDescriptor() == desc){
            QString str = ClientList[i]->playerName + "退出房间";
            slot_emitLogText(str.toUtf8());
            ClientList.remove(i);
            readyId.remove(i);
        }
    }
}

void Server::slot_playGame()
{

    for(int i = 0;i<ClientList.count();i++){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        qDebug()<<ClientList.count();
        out << quint16(0) << quint8(21) <<i;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        ClientList[i]->write(block);
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    qDebug()<<ClientList.count();
    out << quint16(0) << quint8(22) <<bool(true);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    ClientList[turnWho]->write(block);
}
