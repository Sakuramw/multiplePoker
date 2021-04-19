#include "client.h"
//#define CHAT 100
//#define PLAYER_NAME 11
//#define READY_FLAG 12
//#define PLAYDATA 31
//#define WINNER  43
//#define OVERTIME 102



Client::Client(QObject *parent) : QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),
            this,SLOT(slot_readClient()));
    connect(this,SIGNAL(disconnected()),
            this,SLOT(slot_disconnected()));
    //    connect(this,SIGNAL(connected()),
    //            this,SLOT(slot_connected()));

    nextBlockSize = 0,allMoney = defaultBet,addMoney = 0;
    score = 0,thisRoundAdd = 0;
    //    connect(&timer1,SIGNAL(timeout()),
    //            this,SLOT(slot_overTime()));
    //    connect(&disconTime,SIGNAL(timeout()),
    //            this,SLOT(slot_gameDiscon()));


}

Client::~Client()
{
}

Client &Client::operator=(const Client &other)
{
    nextBlockSize = other.nextBlockSize;
    playerName = other.playerName;
    winnerId = other.winnerId;
    allMoney = other.allMoney;
    thisRoundAdd = other.thisRoundAdd;
    isPass = other.isPass;
    isGiveup = other.isGiveup;
    seatId = other.seatId;
    defaultBet = other.defaultBet;
    score = other.score;
    return *this;
}



void Client::slot_readClient()
{
    QDataStream in(this);
    in.setVersion((QDataStream::Qt_4_8));
    while(1){
        if(nextBlockSize == 0){
            if(bytesAvailable()<sizeof(quint16)) return;

            in >> nextBlockSize;
        }

        if(bytesAvailable()<nextBlockSize) return;
        quint8 requestType;
        bool areYouOk = false/*,isConfirm*/;
        QByteArray btaChat ;
        QByteArray tempName;
        QString str,tempWinnerId;
        in >> requestType;
        switch(requestType){
        case PLAYER_NAME:
            in >> tempName;
            //        timer1.start(60550);
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

            }
            if(addMoney == 0&&!isPass&&!isGiveup){
                str = playerName+"跟注";
                emit sig_radioLogText(str.toUtf8());
            }
            emit sig_playData(seatId,addMoney,isPass,isGiveup);
            break;
        case WINNER:
            in >> tempWinnerId;
            emit sig_winner(tempWinnerId);
            break;

        case CHAT:
            in >> btaChat;
            emit sig_radioChatText(btaChat);
            break;
            //    case OVERTIME:
            //        in >> isConfirm;
            //        disconTime.stop();
            //        disconTime.start(70000);
            //        break;


        }
        nextBlockSize = 0;
    }
}

void Client::slot_disconnected()
{
    emit sig_disconnected(this->socketDescriptor());
}

void Client::slot_defaultSet(int money)
{
    defaultBet = money;
}

//void Client::slot_overTime()
//{
//    QByteArray block;
//    QDataStream out(&block,QIODevice::ReadWrite);
//    out.setVersion(QDataStream::Qt_4_8);
//    out << quint16(0) << quint8(102) <<bool(true);
//    out.device() ->seek(0);
//    out<<quint16(block.size()-sizeof(quint16));
//    this->write(block);
//}

//void Client::slot_gameDiscon()
//{
//    QString str = playerName + "已掉线";
//    emit sig_radioLogText(str.toUtf8());
//}

//void Client::slot_connected()
//{
//    timer1.start(3000);
//    emit  sig_radioLogText(tr("sdsa").toUtf8());
//}


