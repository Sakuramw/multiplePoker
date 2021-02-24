#include "poker_client.h"
#include "ui_poker_client.h"

Poker_Client::Poker_Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poker_Client)
{
    ui->setupUi(this);
    tcpsocket = new QTcpSocket();
    nextBlockSize =0;
    allMoney = 0,addMoney = 0,myCardFlag = 0,puCardFlag = 0;
    isAdd = false,isCall = false ,isNewRound = true;
    score = 0;
    m_name = ui->lineEdit_name->text();
    ui->pushButton_add->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    ui->pushButton_pass->setEnabled(false);
    cardLabel = ui->widget_myCard->findChildren<QLabel *>();
    pCard = ui->widget_pCard->findChildren<QLabel *>();
    //    ui->textBrowser_lelaoji->setText(QString::number(cardLabel.count()));
    connect(tcpsocket,SIGNAL(readyRead()),
            this,SLOT(slot_readServer()));
    connect(tcpsocket,SIGNAL(connected()),
            this,SLOT(slot_connected()));
    connect(tcpsocket,SIGNAL(disconnected()),
            this,SLOT(slot_connected()));

    //    connect(this,SIGNAL(sig_iDo()),
    //            this,SLOT(slot_turnMy()));

    QIntValidator* moneyValidator = new QIntValidator;
    moneyValidator->setRange(1, 100);
    ui->lineEdit_money->setValidator(moneyValidator);
    QIntValidator* winnerValidator = new QIntValidator;
    winnerValidator->setRange(0, 100);
    ui->lineEdit_winner->setValidator(winnerValidator);
    ui->lineEdit_winner->hide();
    ui->pushButton_winner->hide();


}

Poker_Client::~Poker_Client()
{
    delete ui;
}

void Poker_Client::slot_readServer()
{
#if 1
    QDataStream in(tcpsocket);
    in.setVersion((QDataStream::Qt_4_8));
    if(nextBlockSize == 0){
        if(tcpsocket->bytesAvailable()<sizeof(quint16)) return;
        in >> nextBlockSize;
    }

    if(tcpsocket->bytesAvailable()<nextBlockSize) return;
    quint8 requestType;
    int tempId;
    QByteArray tempChat,tempLog;
    QString str,tempCard,tempName,otherCard1,otherCard2;
    in >> requestType;
    switch(requestType){
    case SEAT:
        in >> seatId ;
        ui->textBrowser_log->append("座位号："+QString::number(seatId));
        break;
    case WHOPLAY:
        in>>turnWho >> isAdd >>isCall;
        if(turnWho == seatId){
            if(!isNewRound){
                if(isAdd){
                    ui->pushButton_add->setEnabled(true);
                    ui->pushButton_giveup->setEnabled(true);
                    ui->pushButton_call->setEnabled(true);
                }else if(isCall){
                    ui->pushButton_add->setEnabled(true);
                    ui->pushButton_giveup->setEnabled(true);
                    ui->pushButton_call->setEnabled(true);
                }
                else{
                    ui->pushButton_add->setEnabled(true);
                    ui->pushButton_pass->setEnabled(true);
                    ui->pushButton_giveup->setEnabled(true);
                }
            }else{
                ui->pushButton_add->setEnabled(true);
                ui->pushButton_pass->setEnabled(true);
                ui->pushButton_giveup->setEnabled(true);
            }
        }else{

        }
        //        for(int i =0;i<myCards.count();++i){
        //            cardLabel[i]->setText(myCards[i]);
        //        }
        isAdd = false;
        isNewRound = false;
        break;
    case MYCARDS:
        in >> tempCard;
        //        myCards.append(tempCard);
        cardLabel[myCardFlag++]->setText(tempCard);
        break;
    case PUBLICCARD:
        in >> tempCard;
        switch (++puCardFlag) {
        case 1:
            ui->label_pcard1->setText(tempCard);

            break;
        case 2:
            ui->label_pcard2->setText(tempCard);
            break;
        case 3:
            ui->label_pcard3->setText(tempCard);
            break;
        case 4:
            ui->label_pcard4->setText(tempCard);
            break;
        case 5:
            ui->label_pcard5->setText(tempCard);
            break;
        }
        //        pCard[puCardFlag++]->setText(tempCard);
        break;
    case RADIOCARD:
        in >>tempId >>tempName >> otherCard1 >> otherCard2>> judgeId;
        str = QString::number(tempId) +"号"+ tempName + "的手牌是：" + otherCard1
                + "和" + otherCard2;
        ui->textBrowser_log->append(str);
        if(judgeId == seatId){
            ui->lineEdit_winner->show();
            ui->pushButton_winner->show();
        }
        ui->pushButton_add->setEnabled(false);
        ui->pushButton_giveup->setEnabled(false);
        ui->pushButton_call->setEnabled(false);
        ui->pushButton_pass->setEnabled(false);
        break;
    case NEWROUND:
        in >>isNewRound;
        break;
    case OVERFLAG:
        in >> score;
        allMoney = 0,addMoney = 0;
        myCardFlag = 0,puCardFlag = 0;
        ui->pushButton_ready->show();
        break;
    case CHAT:
        in >> tempChat;
        ui->textBrowser_lelaoji->append(QString(tempChat));
        break;
    case LOG:
        in >> tempLog;
        ui->textBrowser_log->append(QString(tempLog));
        break;
    }

#endif
#if 0
    QByteArray block = tcpsocket->readAll();
    ui->textBrowser_lelaoji->append(QString(block)+"flag");
#endif

    nextBlockSize = 0;
}

void Poker_Client::slot_connected()
{
    m_name = ui->lineEdit_name->text();
    QByteArray block;
    QDataStream out(&block,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(11) <<m_name.toUtf8();
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    ui->lineEdit_name->hide();
    ui->pushButton_join->hide();
}

void Poker_Client::slot_disconnected()
{
    ui->textBrowser_log->append("因为网络或者服务器原因，你已断开连接，请点击加入对局重连"
                                "（如果正在对局中，请勿修改名字！！！）");
    ui->lineEdit_name->show();
    ui->pushButton_join->show();
}

//void Poker_Client::slot_turnMy()
//{

//}


void Poker_Client::on_pushButton_send_clicked()
{
    m_name = ui->lineEdit_name->text();
    QString strLelaoji = ui->lineEdit_lelaoji->text();
    strLelaoji = m_name + "：" + strLelaoji;
    ui->lineEdit_lelaoji->clear();
    QByteArray btaChatText = strLelaoji.toUtf8();

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(100) <<btaChatText ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();

}

void Poker_Client::on_pushButton_join_clicked()
{
    if(ui->lineEdit_name->text().isEmpty()){
        ui->textBrowser_log->append("名字不能为空！");
    }else{
        m_name = ui->lineEdit_name->text();
//        tcpsocket->connectToHost("127.0.0.1",10005);
        tcpsocket->connectToHost("3q77570w05.zicp.vip",40128);
    }
}

void Poker_Client::on_pushButton_ready_clicked()
{
    for(int i = 0;i<pCard.count();i++){
        pCard[i]->clear();
    }
    for(int i = 0;i<cardLabel.count();i++){
        cardLabel[i]->clear();
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    bool isOk = true;
    out << quint16(0) << quint8(12) <<isOk ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    ui->pushButton_ready->hide();
}



void Poker_Client::on_pushButton_add_clicked()
{
    if(ui->lineEdit_money->text().isEmpty()){
        ui->textBrowser_log->append("加注只能在此基础上继续增加，如果你想跟，请按下方的 跟 按钮！");
    }else{
        addMoney = ui->lineEdit_money->text().toInt();
        allMoney += addMoney;
        ui->lineEdit_money->clear();
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(31) <<seatId
            <<addMoney<<bool(false)<<bool(false);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        tcpsocket->write(block);
        ui->pushButton_add->setEnabled(false);
        ui->pushButton_pass->setEnabled(false);
        ui->pushButton_giveup->setEnabled(false);
        ui->pushButton_call->setEnabled(false);
        isNewRound = false;
    }
}

void Poker_Client::on_pushButton_pass_clicked()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(31) <<seatId
        <<int(0)<<bool(true)<<bool(false);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    ui->pushButton_add->setEnabled(false);
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    isNewRound = false;
}

void Poker_Client::on_pushButton_giveup_clicked()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(31) <<seatId
        <<int(0)<<bool(false)<<bool(true);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    ui->pushButton_add->setEnabled(false);
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    isNewRound = false;
}

void Poker_Client::on_pushButton_call_clicked()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(31) <<seatId
        <<int(0)<<bool(false)<<bool(false);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    ui->pushButton_add->setEnabled(false);
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    isNewRound = false;
}

void Poker_Client::on_pushButton_winner_clicked()
{
    if(ui->lineEdit_winner->text().isEmpty()){
        ui->textBrowser_log->append("获胜者不能为空！");
    }else{
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(43) <<ui->lineEdit_winner->text().toInt();
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        tcpsocket->write(block);
        ui->lineEdit_winner->hide();
        ui->pushButton_winner->hide();
    }
}

void Poker_Client::on_pushButton_local_clicked()
{
    if(ui->lineEdit_name->text().isEmpty()){
        ui->textBrowser_log->append("名字不能为空！");
    }else{
        m_name = ui->lineEdit_name->text();
        tcpsocket->connectToHost("127.0.0.1",10005);
    }
}
