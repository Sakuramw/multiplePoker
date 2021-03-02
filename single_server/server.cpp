#include "server.h"

#include <QTime>
#include <ctime>
#include <cstdlib>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    whoDealer = 0,round = 1;
    whoCall = 0;
    allMoney = 0;
    addMoney = 0;
    winnerId.clear();
    isANC = false,isFP = true , isFirstRun = true,isSomeOneLose =false;
    isPlaying = false;
    connect(this,SIGNAL(sig_gameBegin()),
            this,SLOT(slot_playGame()));
    //    connect(this,SIGNAL(sig_next()),
    //            this,SLOT(slot_next()));
    connect(this,SIGNAL(sig_roundNum(int)),
            this,SLOT(slot_roundNum(int)));
    connect(this,SIGNAL(sig_gameOver(int)),
            this,SLOT(slot_gameOver(int)));
    qsrand(time(0));
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"<font color = red>♥A</font>";
        }else if(i == 11){
            poker<<"<font color = red>♥J</font>";
        }else if(i == 12){
            poker<<"<font color = red>♥Q</font>";
        }else if(i == 13){
            poker<<"<font color = red>♥K</font>";
        }else{
            poker<<QString("<font color = red>♥%1</font>").arg(QString::number(i));
        }
    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"<font color = red>♦A</font>";
        }else if(i == 11){
            poker<<"<font color = red>♦J</font>";
        }else if(i == 12){
            poker<<"<font color = red>♦Q</font>";
        }else if(i == 13){
            poker<<"<font color = red>♦K</font>";
        }else{
            poker<<QString("<font color = red>♦%1</font>").arg(QString::number(i));

        }
    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"♠A";
        }else if(i == 11){
            poker<<"♠J";
        }else if(i == 12){
            poker<<"♠Q";
        }else if(i == 13){
            poker<<"♠K";
        }else{
            poker<<QString("♠%1").arg(QString::number(i));
        }

    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"♣A";
        }else if(i == 11){
            poker<<"♣J";
        }else if(i == 12){
            poker<<"♣Q";
        }else if(i == 13){
            poker<<"♣K";
        }else{
            poker<<QString("♣%1").arg(QString::number(i));
        }

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
        ClientList[i]->waitForBytesWritten();
    }
    m_sleep(50);

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
    m_sleep(17);
}

void Server::m_sleep(int msec)
{
    QTime t1 = QTime::currentTime();
    QTime now;
    do{
        now=QTime::currentTime();
    }while(t1.msecsTo(now)<=msec);
}

void Server::newRound()
{

    isNewRound = true;
    isFP = true;
    isANC = false;
    whoNext = whoCall;
    addMoney = 0;
    for(int i =0;i<ClientList.count();i++){
        ClientList[i]->thisRoundAdd = 0;
    }
    while(inDesk[whoNext] == 0){
        whoNext++;
        if(whoNext >= ClientList.count()){
            whoNext = 0;
        }
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(35) <<bool(true);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    for(int j = 0;j<ClientList.count();j++){
        ClientList[j]->write(block);
        ClientList[j]->waitForBytesWritten();
    }
    m_sleep(50);
}


void Server::incomingConnection(int socketId)
{
    if(isPlaying){
        //发送停止信号给客户端，等待重连

        //有人重连
        Client *socket = new Client(this);
        socket->setSocketDescriptor(socketId);
        reconnectedClient.append(socket);
        connect(socket,SIGNAL(sig_newPlayer()),
                this,SLOT(slot_reconnected()));
    }else{
        Client *socket = new Client(this);
        readyId.append(0);
        inDesk.append(1);
        socket->setSocketDescriptor(socketId);
        ClientList.append(socket);
        isFirstRun = true;
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
        connect(socket,SIGNAL(sig_winner(QString)),
                this,SLOT(slot_winner(QString)));
    }
}

void Server::slot_emitChatText(QByteArray bta)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(100) <<bta ;
    out.device() ->seek(0);
    //    qDebug()<<quint16(block.size()-sizeof(quint16));
    out<<quint16(block.size()-sizeof(quint16));
    //    qDebug()<<block.size();
    for(int i = 0;i<ClientList.count();i++){
        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
    }
    emit sig_updateChat(QString(bta));
    m_sleep(50);
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
    m_sleep(50);
}

void Server::slot_newPlayer()
{
    m_sleep(50);
    QString temp = ClientList[ClientList.count()-1]->playerName + "加入房间";
    slot_emitLogText(temp.toUtf8());
    playerList.append(ClientList[ClientList.count()-1]->playerName);
    playerScore.append(QString::number(ClientList[ClientList.count()-1]->score));
    m_sleep(50);
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(13) <<playerList;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0 ;i<ClientList.count();i++){
            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
            m_sleep(50);
        }
    }
    m_sleep(50);
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(14) <<playerScore;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0 ;i<ClientList.count();i++){
            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
            m_sleep(50);
        }
    }


}

void Server::slot_newReady(QString name)
{
    if(isPlaying){
        m_sleep(50);
        for(int i = 0;i<ClientList.count();i++){
            if(ClientList[i]->playerName == name){
                QString str = ClientList[i]->playerName + "重新连接";
                slot_emitLogText(str.toUtf8());
                m_sleep(50);

                {
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_8);
                    out << quint16(0) << quint8(13) <<playerList;
                    out.device() ->seek(0);
                    out<<quint16(block.size()-sizeof(quint16));
                    ClientList[i]->write(block);
                    ClientList[i]->waitForBytesWritten();
                    m_sleep(50);
                }
                {
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_8);
                    out << quint16(0) << quint8(14) <<playerScore;
                    out.device() ->seek(0);
                    out<<quint16(block.size()-sizeof(quint16));
                    ClientList[i]->write(block);
                    ClientList[i]->waitForBytesWritten();
                    m_sleep(50);
                }

                //座位号
                {
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_8);
                    out << quint16(0) << quint8(21) <<i;
                    out.device() ->seek(0);
                    out<<quint16(block.size()-sizeof(quint16));
                    ClientList[i]->write(block);
                    ClientList[i]->waitForBytesWritten();
                    ClientList[i]->seatId = i;
                    m_sleep(50);
                }
                //发牌
                if((i-whoDealer) >= 0){
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(23) <<poker[i - whoDealer];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                    }
                    m_sleep(50);
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(23) <<poker[i - whoDealer + ClientList.count()];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                }else{
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(23) <<poker[i - whoDealer + ClientList.count()];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                    }
                    m_sleep(50);
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(23) <<poker[i - whoDealer + 2 * ClientList.count()];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                }
                //回合
                if(round == 1){
                    QByteArray block;
                    QDataStream out(&block,QIODevice::WriteOnly);
                    out << quint16(0) << quint8(22) <<whoNext <<bool(false)<<bool(false);
                    out.device() ->seek(0);
                    out<<quint16(block.size()-sizeof(quint16));
                    ClientList[i]->write(block);
                    ClientList[i]->waitForBytesWritten();
                    m_sleep(50);
                }else if(round == 2){
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(22) <<whoNext <<bool(false)<<bool(false);
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    for(int j = 2 * ClientList.count();j<2 * ClientList.count() + 3;j++){
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[j];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                }else if(round == 3){
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(22) <<whoNext <<bool(false)<<bool(false);
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    for(int j = 2 * ClientList.count();j<2 * ClientList.count() + 3;j++){
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[j];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 3];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                }else if(round == 4){
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(22) <<whoNext <<bool(false)<<bool(false);
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    for(int j = 2 * ClientList.count();j<2 * ClientList.count() + 3;j++){
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[j];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 3];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 4];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                }else if(round == 5){
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(22) <<whoNext <<bool(false)<<bool(false);
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    for(int j = 2 * ClientList.count();j<2 * ClientList.count() + 3;j++){
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[j];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 3];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 4];
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                    }
                    for(int j = 0;j<inDesk.count();j++){
                        if(inDesk[j] == 1){
                            QByteArray block;
                            QDataStream out(&block,QIODevice::WriteOnly);
                            out.setVersion(QDataStream::Qt_4_8);
                            out << quint16(0) << quint8(34)<<ClientList[j]->seatId<<ClientList[j]->playerName
                                << poker[j] <<poker[j + ClientList.count()];
                            out.device() ->seek(0);
                            out<<quint16(block.size()-sizeof(quint16));
                            ClientList[i]->write(block);
                            ClientList[i]->waitForBytesWritten();
                            m_sleep(50);
                        }

                    }
                    {
                        QByteArray block;
                        QDataStream out(&block,QIODevice::WriteOnly);
                        out.setVersion(QDataStream::Qt_4_8);
                        out << quint16(0) << quint8(36)<<defaultJudge;
                        out.device() ->seek(0);
                        out<<quint16(block.size()-sizeof(quint16));
                        ClientList[i]->write(block);
                        ClientList[i]->waitForBytesWritten();
                        m_sleep(50);
                        slot_emitLogText("游戏结束");
                    }
                }

            }
        }
    }else{
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
            if(ClientList.count() == 1){
                QString str = "一个人不能玩游戏，快去叫人吧";
                slot_emitLogText(str.toUtf8());
            }else{
                isPlaying = true;
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
                }
                m_sleep(50);


                if(isFirstRun){
                    whoDealer = 0;
                    whoCall = whoDealer + 1;
                }else{
                    whoDealer++;
                    if(whoDealer >= ClientList.count()) whoDealer = 0;
                    whoCall = whoDealer + 1;
                    if(whoCall >= ClientList.count()) whoCall = 0;
                }

                round = 1;
                allMoney = defaultMoney * ClientList.count();
                whoNext = whoCall;
                addMoney = 0;
                QString str = "游戏开始，这把" + ClientList[whoDealer]->playerName
                        + "坐庄，" + ClientList[whoCall]->playerName + "先叫";
                slot_emitLogText(str.toUtf8());
                m_sleep(50);
                emit sig_gameBegin();
            }

        }
    }
}

void Server::slot_disconnected(int desc)
{
    if(isPlaying){
        isSomeOneLose =true;
        for(int i = 0;i<ClientList.count();i++){
            if(ClientList[i]->socketDescriptor() == desc){
                QString str = ClientList[i]->playerName + "中途掉线,请等待";
                slot_emitLogText(str.toUtf8());
                m_sleep(50);
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_8);
                out << quint16(0) << quint8(37);
                out.device() ->seek(0);
                out<<quint16(block.size()-sizeof(quint16));
                for(int j = 0 ;j<ClientList.count();j++){
                    ClientList[j]->write(block);
                    ClientList[j]->waitForBytesWritten();
                    m_sleep(50);
                }
                ClientList[i]->close();
            }
        }

    }else{
        for(int i = 0;i<ClientList.count();i++){
            if(ClientList[i]->socketDescriptor() == desc){
                QString str = ClientList[i]->playerName + "退出房间，ta的积分为" + QString::number(ClientList[i]->score);
                slot_emitLogText(str.toUtf8());
                playerList.removeAt(i);
                playerScore.removeAt(i);
                //                ClientList[i]->timer1.stop();
                //                ClientList[i]->disconTime.stop();
                ClientList[i]->close();
                //                delete ClientList[i];
                ClientList.remove(i);
                readyId.remove(i);
                inDesk.remove(i);
            }
        }

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(13) <<playerList;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0 ;i<ClientList.count();i++){
            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
            m_sleep(50);
        }

        isFirstRun = true;
    }

}

void Server::slot_playGame()
{
    isFirstRun = false;
    for(int i = 0;i<ClientList.count();i++){
        ClientList[i]->score = ClientList[i]->score - defaultMoney;
    }

    //洗牌
    shuffleCards();
    //发牌
    for(int i = 0;i<2;++i){
        int k = whoDealer;
        for(int j = 0;j<ClientList.count();j++){
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_8);
            out << quint16(0) << quint8(23) <<poker[j+i*ClientList.count()];
            out.device() ->seek(0);
            out<<quint16(block.size()-sizeof(quint16));
            ClientList[k]->write(block);
            ClientList[k]->waitForBytesWritten();
            k++;
            if(k >= ClientList.count()) k = 0;
        }
        m_sleep(50);
    }

    emit sig_roundNum(round);


}

void Server::slot_reconnected()
{
    m_sleep(50);
    //遍历clientlist找到名字
    for(int j = 0;j<reconnectedClient.count();j++){
        isRecon = false;
        for(int i = 0;i<ClientList.count();i++){
            if(reconnectedClient[j]->playerName == ClientList[i]->playerName){
                *reconnectedClient[j] = *ClientList[i];
                ClientList[i] = reconnectedClient[j];
                connect(ClientList[i],SIGNAL(sig_radioChatText(QByteArray)),
                        this,SLOT(slot_emitChatText(QByteArray)));
                connect(ClientList[i],SIGNAL(sig_radioLogText(QByteArray)),
                        this,SLOT(slot_emitLogText(QByteArray)));
                //    connect(socket,SIGNAL(connected()),
                //            this,SLOT(slot_emitChatText(QByteArray)));
                connect(ClientList[i],SIGNAL(sig_newPlayer()),
                        this,SLOT(slot_newPlayer()));
                connect(ClientList[i],SIGNAL(sig_isReady(QString)),
                        this,SLOT(slot_newReady(QString)));
                connect(ClientList[i],SIGNAL(sig_disconnected(int)),
                        this,SLOT(slot_disconnected(int)));
                connect(ClientList[i],SIGNAL(sig_playData(int,int,bool,bool)),
                        this,SLOT(slot_playData(int,int,bool,bool)));
                connect(ClientList[i],SIGNAL(sig_winner(QString)),
                        this,SLOT(slot_winner(QString)));
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_8);
                out << quint16(0) << quint8(38);
                out.device() ->seek(0);
                out<<quint16(block.size()-sizeof(quint16));
                for(int i = 0 ;i<ClientList.count();i++){
                    ClientList[i]->write(block);
                    ClientList[i]->waitForBytesWritten();
                    m_sleep(50);
                }
                isRecon = true;
            }
        }
        if(!isRecon){
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_8);
            QString str = "游戏进行中，请等待游戏结束后重启客户端进入";
            out << quint16(0) << quint8(101)<<str.toUtf8();
            out.device() ->seek(0);
            out<<quint16(block.size()-sizeof(quint16));
            reconnectedClient[j]->write(block);
            reconnectedClient[j]->waitForBytesWritten();
            reconnectedClient[j]->close();

        }
        reconnectedClient.remove(j);
    }
    isSomeOneLose = false;

}


void Server::slot_roundNum(int round)
{
    QString str;
    if(round == 1){
        str = "第一回合";
        slot_emitLogText(str.toUtf8());
        m_sleep(50);
        newRound();
        //发送开始叫分的座位号
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(22) <<whoNext <<bool(false)<<bool(false);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
        m_sleep(50);
    }else if(round == 2){
        //        emit sig_updateLog("第二回合");
        str = "第二回合";
        slot_emitLogText(str.toUtf8());
        m_sleep(50);
        newRound();
        //发公共牌
        for(int i = 2 * ClientList.count();i<2 * ClientList.count() + 3;i++){
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_8);
            out << quint16(0) << quint8(33) <<poker[i];
            out.device() ->seek(0);
            out<<quint16(block.size()-sizeof(quint16));
            for(int j = 0;j<ClientList.count();j++){
                ClientList[j]->write(block);
                ClientList[j]->waitForBytesWritten();
            }
            m_sleep(50);
        }
    }else if(round == 3){
        //        emit sig_updateLog("第三回合");
        str = "第三回合";
        slot_emitLogText(str.toUtf8());
        m_sleep(50);
        newRound();
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 3];
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int j = 0;j<ClientList.count();j++){
            ClientList[j]->write(block);
            ClientList[j]->waitForBytesWritten();
        }
        m_sleep(50);
    }else if(round == 4){
        //        emit sig_updateLog("第四回合");
        str = "第四回合";
        slot_emitLogText(str.toUtf8());
        m_sleep(50);
        newRound();
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(33) <<poker[2 * ClientList.count() + 4];
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int j = 0;j<ClientList.count();j++){
            ClientList[j]->write(block);
            ClientList[j]->waitForBytesWritten();
        }
        m_sleep(50);
    }else if(round == 5){
        emit sig_updateLog("开牌");
        str = "开牌";
        slot_emitLogText(str.toUtf8());
        m_sleep(50);
        //拼到最后，至少有两个人没放弃，开牌
        for(int i = 0;i<inDesk.count();i++){
            if(inDesk[i] == 1){
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_8);
                if((i-whoDealer) >= 0){
                    out << quint16(0) << quint8(34)<<ClientList[i]->seatId<<ClientList[i]->playerName
                        << poker[i - whoDealer] <<poker[i - whoDealer + ClientList.count()];
                }else{
                    out << quint16(0) << quint8(34)<<ClientList[i]->seatId<<ClientList[i]->playerName
                        << poker[i - whoDealer + ClientList.count()] <<poker[i - whoDealer + 2 * ClientList.count()];
                }
                out.device() ->seek(0);
                out<<quint16(block.size()-sizeof(quint16));
                for(int j = 0;j<ClientList.count();j++){
                    ClientList[j]->write(block);
                    ClientList[j]->waitForBytesWritten();
                }
                m_sleep(50);
            }
        }

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(36)<<defaultJudge;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int j = 0;j<ClientList.count();j++){
            ClientList[j]->write(block);
            ClientList[j]->waitForBytesWritten();
        }
        m_sleep(50);
        slot_emitLogText("游戏结束");
        //        m_sleep(50);
        //        emit sig_gameOver(1);
    }
}

void Server::slot_playData(int id, int money, bool pass, bool giveup)
{
    m_sleep(50);
    if(giveup){
        //放弃了，只在whoDealer的时候需要判断，其他无区别
        inDesk[id] = 0;
        int inDesknum = 0;
        for(int i = 0;i<inDesk.count();i++){
            if(inDesk[i] == 1){
                inDesknum++;
            }
        }
        if(inDesknum == 1){
            //游戏结束，放弃结束，不开牌

            isPlaying = false;
            slot_emitLogText("游戏结束");
            //发射结束信号，处理相关数据
            emit sig_gameOver(2);
            return;
        }
    }
    if(money == 0&&!giveup){
        if(pass){
            //过牌
            if(isFP){
                firstPassId = id;
                isFP = false;
            }
        }else{
            //跟牌
            ClientList[id]->score = ClientList[id]->score-(addMoney-ClientList[id]->thisRoundAdd);
            allMoney += (addMoney-ClientList[id]->thisRoundAdd);
            ClientList[id]->thisRoundAdd = addMoney;
        }
    }
    if(money){
        if(money>addMoney){
            beatId = id;
            isANC = true,isFP = false;
            addMoney = money;
            //            allMoney += money;
            allMoney += (money-ClientList[id]->thisRoundAdd);
            ClientList[id]->score = ClientList[id]->score-(money-ClientList[id]->thisRoundAdd);
            ClientList[id]->thisRoundAdd = money;
            QString str = ClientList[id]->playerName + "加注："+QString::number(money);
            slot_emitLogText(str.toUtf8());
            m_sleep(50);
            str = "本回合最高注：" + QString::number(money);
            slot_emitLogText(str.toUtf8());
            m_sleep(50);
        }else{
            QString str = "金额不能小于或等于上位投注玩家的注，请重新输入，跟注请按-跟-！";
            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_8);
                out << quint16(0) << quint8(101) <<str.toUtf8();
                out.device() ->seek(0);
                out<<quint16(block.size()-sizeof(quint16));
                ClientList[id]->write(block);
                ClientList[id]->waitForBytesWritten();
                m_sleep(50);
            }

            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_8);
                out << quint16(0) << quint8(22) <<id<<bool(true) <<bool(false);
                out.device() ->seek(0);
                out<<quint16(block.size()-sizeof(quint16));
                ClientList[id]->write(block);
                ClientList[id]->waitForBytesWritten();
                m_sleep(50);
                return;
            }
        }
    }
    isNewRound = false;
    do{
        whoNext++;
        if(whoNext > ClientList.count()-1) whoNext = 0;
    }while(inDesk[whoNext] == 0);
    if(isANC){
        if(whoNext == beatId) emit sig_roundNum(++round);
    }else{
        if(whoNext == firstPassId) emit sig_roundNum(++round);
    }
    m_sleep(300);
    if(isNewRound){
        whoNext = whoCall;
        while(inDesk[whoNext] == 0){
            whoNext++;
            if(whoNext >= inDesk.count()){
                whoNext = 0;
            }
        }
    }
    if(round == 5) return;
    if(giveup){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        if(!isANC){
            out << quint16(0) << quint8(22) <<whoNext<<bool(false)<<bool(false);
        }else{
            out << quint16(0) << quint8(22) <<whoNext<<bool(false)<<bool(true);
        }
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
        m_sleep(50);
    }
    if(money == 0&&!giveup){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        if(pass){
            out << quint16(0) << quint8(22) <<whoNext<<bool(false)<<bool(false);
        }else{
            out << quint16(0) << quint8(22) <<whoNext<<bool(false)<<bool(true);
        }
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
        m_sleep(50);
    }
    if(money){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(22) <<whoNext<<bool(true) <<bool(false);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        for(int i = 0;i<ClientList.count();i++){

            ClientList[i]->write(block);
            ClientList[i]->waitForBytesWritten();
        }
        m_sleep(50);
    }
}

void Server::slot_gameOver(int status)
{
    if(status == 1){
        for(int i = 0;i<winnerId.count();i++){
            if(winnerId[i]<0 || winnerId[i] >= ClientList.count() || winnerId.count() > inDesk.count()||inDesk[winnerId[i]] == 0){
                QString str = "获胜者ID有误，请重新输入";
                slot_emitLogText(str.toUtf8());
                m_sleep(50);

                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_8);
                out << quint16(0) << quint8(36)<<defaultJudge;
                out.device() ->seek(0);
                out<<quint16(block.size()-sizeof(quint16));
                for(int j = 0;j<ClientList.count();j++){
                    ClientList[j]->write(block);
                    ClientList[j]->waitForBytesWritten();
                }
                winnerId.clear();
                m_sleep(50);

                return;
            }
        }
        divideMoney = allMoney / winnerId.count();
        //开牌结束，收到获胜者id
        if(winnerId.count() == 1){
            ClientList[winnerId[0]]->score += allMoney;
            QString str = "获胜者是：" + ClientList[winnerId[0]]->playerName
                    +"，"+ "本局共有筹码：" + QString::number(allMoney);
            slot_emitLogText(str.toUtf8());
            m_sleep(50);
        }else{
            QString str = "本局共有" + QString::number(winnerId.count()) + "位赢家：," ;
            for(int i = 0;i<winnerId.count();i++){
                ClientList[winnerId[i]]->score += divideMoney;
                str = str + ClientList[winnerId[i]]->playerName;
                if(i != winnerId.count()-1){
                    str = str + "、";
                }
            }
            str = str + "，分别获得：" + QString::number(divideMoney);
            slot_emitLogText(str.toUtf8());
            m_sleep(50);
        }


    }
    if(status == 2){
        //弃牌结束
        for(int i = 0;i<inDesk.count();i++){
            if(inDesk[i] == 1){
                ClientList[i]->score += allMoney;
                QString str = "获胜者是：" + ClientList[i]->playerName
                        +"，"+ "本局共有筹码：" + QString::number(allMoney);
                slot_emitLogText(str.toUtf8());
            }
        }
    }
    m_sleep(200);
    for(int i = 0;i<ClientList.count();i++){
        //发送积分告知结束
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(41) <<ClientList[i]->score ;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
    }
    m_sleep(312);
    for(int i = 0;i<ClientList.count();i++){
        playerScore[i] = QString::number(ClientList[i]->score);
    }
    for(int i = 0;i<ClientList.count();i++){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(14) <<playerScore ;
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        ClientList[i]->write(block);
        ClientList[i]->waitForBytesWritten();
    }
    m_sleep(200);
    isPlaying = false;
    //重置数据
    if(reconnectedClient.count()>0){
        for(int i = 0;i<reconnectedClient.count();i++){
            reconnectedClient.remove(i);
        }
    }
    round = 1;
    //    allMoney = defaultMoney*ClientList.count();
    //    roundMoney = 0;
    //    whoDealer++;
    //    if(whoDealer >= ClientList.count()) whoDealer = 0;
    //    whoCall = whoDealer + 1;
    //    if(whoCall >= ClientList.count()) whoCall = 0;
    for(int i = 0;i<ClientList.count();i++){
        readyId[i] = 0;
        inDesk[i] = 1;
        ClientList[i]->thisRoundAdd = 0;
    }
    //    isFP = true,isANC = false;
    winnerId.clear();
    m_sleep(300);
    return;
}

void Server::slot_defaultSet(int money, int id)
{
    defaultMoney = money;
    defaultJudge = id;
}

void Server::slot_winner(QString id)
{
    //    for(int i = 0;i<id.count();i++){
    //        if()
    //        if(id[i+1]>='0'&&id[i+1]<='9'){
    //            winnerId.append((id[i]+'0')*10+(id[i+1]+'0'));
    //        }else{
    //            winnerId.append(id[i]+'0');
    //        }
    //    }
    QStringList idList = id.split(",",QString::SkipEmptyParts);
    for(int i = 0;i<idList.count();i++){
        winnerId.append(idList[i].toInt()-1);
    }
    emit sig_gameOver(1);
}
