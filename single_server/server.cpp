#include "server.h"

#include <QTime>
#include <ctime>
#include <cstdlib>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    turnWho = 0,round = 1;
    isANC = false,isFP = true;
    connect(this,SIGNAL(sig_gameBegin()),
            this,SLOT(slot_playGame()));
    connect(this,SIGNAL(sig_next()),
            this,SLOT(slot_next()));
    connect(this,SIGNAL(sig_roundNum(int)),
            this,SLOT(slot_roundNum(int)));
    qsrand(time(0));
    for(int i=1;i<=13;i++){
        poker<<QString("♥%1").arg(QString::number(i));
    }
    for(int i=1;i<=13;i++){
        poker<<QString("♦%1").arg(QString::number(i));
    }
    for(int i=1;i<=13;i++){
        poker<<QString("♠%1").arg(QString::number(i));
    }
    for(int i=1;i<=13;i++){
        poker<<QString("♣%1").arg(QString::number(i));
    }
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

void Server::shuffleCards()
{
    int x,y;
    QString temp;
    for(int i=52;i>=1;i--){
        x = qrand()%i;
        y=i-1;
        temp = poker[y];
        poker[y]=poker[x];
        poker[x]=temp;
    }
}

void Server::m_sleep(int msec)
{
    QTime t1 = QTime::currentTime();
    QTime now;
    do{
        now=QTime::currentTime();
    }while(t1.msecsTo(now)<=msec);
}

void Server::incomingConnection(int socketId)
{

    Client *socket = new Client(this);
    readyId.append(0);
    socket->setSocketDescriptor(socketId);
    ClientList.append(socket);
    connect(socket,SIGNAL(sig_radioChatText(QByteArray)),
            this,SLOT(slot_emitChatText(QByteArray)));
    connect(socket,SIGNAL(sig_radioLogText(QByteArray)),
            this,SLOT(slot_emitLogText(QByteArray)));
    //    connect(socket,SIGNAL(connected()),
    //            this,SLOT(slot_emitChatText(QByteArray)));
    connect(socket,SIGNAL(sig_newPlayer()),
            this,SLOT(slot_newPlayer()));
    connect(socket,SIGNAL(sig_isReady(QString)),
            this,SLOT(slot_newReady(QString)));
    connect(socket,SIGNAL(sig_disconnected(int)),
            this,SLOT(slot_disconnected(int)));
    connect(socket,SIGNAL(sig_playData(int,int,bool,bool)),
            this,SLOT(slot_playData(int,int,bool,bool)));
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
        m_sleep(200);
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
    //分配座位号
    for(int i = 0;i<ClientList.count();i++){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(21) <<i;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
        ClientList[i]->seatId = i;
        inDesk.append(1);
    }
    m_sleep(200);
    shuffleCards();


    //发牌
    for(int i = 0;i<2;++i){
        for(int j = 0;j<ClientList.count();j++){
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_8);
            out << quint16(0) << quint8(23) <<poker[j+i*ClientList.count()];
            out.device() ->seek(0);
            out<<quint16(block.size()-sizeof(quint16));
            ClientList[j]->write(block);
            ClientList[j]->waitForBytesWritten();
        }
        m_sleep(200);
    }

    emit sig_roundNum(round);


}

void Server::slot_next()
{
    do{
        turnWho++;
        if(turnWho > ClientList.count()-1) turnWho = 0;
    }while(inDesk[turnWho] == 0);
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(22) <<turnWho;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    for(int i = 0;i<ClientList.count();i++){

        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
    }
}

void Server::slot_roundNum(int round)
{
    if(round == 1){
        //发送开始的座位号
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        qDebug()<<ClientList.count();
        out << quint16(0) << quint8(22) <<turnWho <<bool(false);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
        emit sig_updateLog("第一回合");
        return;
    }else if(round == 2){
        //发公共牌
        emit sig_updateLog("第二回合");
    }
}

void Server::slot_playData(int id, int money, bool pass, bool giveup)
{
    if(giveup){
        //放弃了，只在turnwho的时候需要判断，其他无区别
        inDesk[id] = 0;
        int inDesknum;
        for(int i;i<inDesk.count();i++){
            if(inDesk[i] == 1){
                inDesknum++;
            }
            if(inDesknum == 1){
                //游戏结束
                slot_emitLogText("游戏结束");
                //发射结束信号，处理相关数据

                return;
            }
        }

    }
    do{
        turnWho++;
        if(turnWho > ClientList.count()-1) turnWho = 0;
    }while(inDesk[turnWho] == 0);
    emit sig_updateLog(QString::number(turnWho));
    if(money == 0){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(22) <<turnWho<<bool(false);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
        if(pass){
            if(isFP){
                firstPassId = id;
                isFP = false;
            }
        }else{

        }
    }
    if(money){
        beatId = id;
//        inquiryNum = beatId;
        isANC = true,isFP = false;
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(22) <<turnWho<<bool(true);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
    }
//    if(inquiryNum == inDesk.count()) inquiryNum = 0;
//    if(inquiryNum == firstPassId) emit sig_roundNum(++round);
//    if(inquiryNum == beatId) emit sig_roundNum(++round);
    if(isANC){

        if(turnWho == beatId) {
            emit sig_updateLog("进入了");
            emit sig_roundNum(++round);
        }
    }else{
        if(turnWho == firstPassId) emit sig_roundNum(++round);
    }
}
