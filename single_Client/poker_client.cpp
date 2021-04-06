#include "poker_client.h"
#include "ui_poker_client.h"

#include <QDialog>
#include <QMessageBox>

Poker_Client::Poker_Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poker_Client)
{
    ui->setupUi(this);
    tcpsocket = new QTcpSocket();
    nextBlockSize =0;
    addMoney = 0,myCardFlag = 0,puCardFlag = 0,countTime = 14;
    isAdd = false,isCall = false ,isNewRound = true,isFirstRun = true;
    isSending = false,isPause = false;
    watchId = -2,isWatchEnable = false;
    score = 0;
    m_name = ui->lineEdit_name->text();
    ui->pushButton_add->setEnabled(false);
    isAddEnable = false;
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
            this,SLOT(slot_disconnected()));
    connect(ui->lineEdit_lelaoji,SIGNAL(returnPressed()),
            this,SLOT(on_pushButton_send_clicked()));
    connect(ui->lineEdit_name,SIGNAL(returnPressed()),
            this,SLOT(on_pushButton_join_clicked()));
    connect(ui->lineEdit_money,SIGNAL(returnPressed()),
            this,SLOT(slot_addMoney()));
    //    connect(ui->lineEdit_winner,SIGNAL(returnPressed()),
    //            this,SLOT(on_pushButton_winner_clicked()));
    connect(ui->comboBox_quicklyMessage,SIGNAL(activated(QString)),
            this,SLOT(slot_quicklyMessage(QString)));
    connect(&countDown,SIGNAL(timeout()),
            this,SLOT(slot_countDown()));

    //    connect(&disconTimer,SIGNAL(timeout()),
    //            this,SLOT(slot_loseConnect()));
    //        QString test = "♦A";
    //        if(test.contains("♦")){
    //            ui->label_card1->setText("<font color = red>" + test + "</font>");
    //        }
    //    connect(this,SIGNAL(sig_iDo()),
    //            this,SLOT(slot_turnMy()));
    ui->pushButton_ready->setEnabled(false);
    QIntValidator* moneyValidator = new QIntValidator;
    moneyValidator->setRange(1, 100);
    ui->lineEdit_money->setValidator(moneyValidator);
    QRegExp regx("[0-9]+$");
    QValidator *watchValidator = new QRegExpValidator(regx);
    ui->lineEdit_watchId->setValidator(watchValidator);
    //    ui->lineEdit_winner->hide();
    //    ui->pushButton_winner->hide();
    //    ui->label_winner->hide();
    ui->lcdNumber_countDown->hide();
    ui->label_score->setText(QString::number(score));
    //    ui->checkBox_watchEnable->setEnabled(true);
    //    ui->checkBox_watchEnable->setCheckable(true);

}

Poker_Client::~Poker_Client()
{
    delete ui;
}

void Poker_Client::keyPressEvent(QKeyEvent *event)
{
    //当按下ctrl+r则跳出提示框.
    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_R)
            QMessageBox::information(this,"规则", "<font size = 5>同花顺</font><font color = red size = 5>(A2345)</font><br>五张同花色的连续牌<br>"
                                                "<font size = 5>四条/炸弹(6666)</font><br>其中四张是相同点数但不同花的扑克牌，第五张是随意的一张牌<br>"
                                                "<font size = 5>葫芦(33388)</font><br>三张相同点数及任何两张其他相同点数的扑克牌<br>"
                                                "<font size = 5>同花</font><font color = red size = 5>(3569A)</font><br>此牌由五张不按顺序但相同花的扑克牌组成<br>"
                                                "<font size = 5>顺子(23456)</font><br>此牌由五张顺序扑克牌组成<br>"
                                                "<font size = 5>三条(3335A)</font><br>由三张相同点数和两张不同点数的扑克组成<br>"
                                                "<font size = 5>两对(6699A)</font><br>两对点数相同但两两不同的扑克和随意的一张牌组成<br>"
                                                "<font size = 5>一对(2268A)</font><br>由两张相同点数的扑克牌和另三张随意的牌组成高牌既不是同一花色也不是同一点数的五张牌组成。<br>");
    }
}

void Poker_Client::slot_readServer()
{
#if 1
    QDataStream in(tcpsocket);
    in.setVersion((QDataStream::Qt_4_8));
    while(1){
        if(nextBlockSize == 0){
            if(tcpsocket->bytesAvailable()<sizeof(quint16)) return;
            in >> nextBlockSize;
        }

        if(tcpsocket->bytesAvailable()<nextBlockSize) return;
        quint8 requestType;
        int tempId,whoGiveup;
        //    bool isConfirm;
        QByteArray tempChat,tempLog;
        QString str,tempCard,tempName,otherCard1,otherCard2;
        QVector<int > readyId;
        in >> requestType;
        switch(requestType){
        case PLAYERLIST:
            in >> nameList;
            ui->tableWidget_playerName->setRowCount(nameList.count());
            for(int i = 0;i<nameList.count();i++){
                ui->tableWidget_playerName->setItem(i,0,new QTableWidgetItem(nameList[i]));
            }
            break;
        case SCORELIST:
            in >> playerScore;
            ui->tableWidget_playerName->setRowCount(playerScore.count());
            for(int i = 0;i<playerScore.count();i++){
                ui->tableWidget_playerName->setItem(i,1,new QTableWidgetItem(playerScore[i]));
            }
            break;
        case NEWREADY:
            in >>readyId;
            for(int i = 0;i<readyId.count();i++){
                if(readyId[i] == 1){
                    ui->tableWidget_playerName->setItem(i,2,new QTableWidgetItem("已准备"));
                }
            }
            break;
        case WATCHLIST:
            in >> watchList >>watchPlayerId;
            ui->tableWidget_watchName->setRowCount(watchList.count());
            for(int i = 0;i<watchList.count();i++){
                ui->tableWidget_watchName->setItem(i,0,new QTableWidgetItem(watchList[i]));
                ui->tableWidget_watchName->setItem(i,1,new QTableWidgetItem(QString::number(watchPlayerId[i]+1)));
            }
            break;
        case SEAT:
            in >> seatId ;
            ui->textBrowser_log->append("座位号："+QString::number(seatId+1));
            break;
        case WHOPLAY:
        {
            in>>turnWho >> isAdd >>isCall;
            if(turnWho == seatId){
                countTime = 14;
                ui->lcdNumber_countDown->display(15);
                ui->lcdNumber_countDown->show();
                countDown.start(1000);
                if(!isNewRound){
                    if(isAdd){
                        ui->pushButton_add->setEnabled(true);
                        isAddEnable = true;
                        ui->pushButton_giveup->setEnabled(true);
                        ui->pushButton_call->setEnabled(true);
                    }else if(isCall){
                        ui->pushButton_add->setEnabled(true);
                        isAddEnable = true;
                        ui->pushButton_giveup->setEnabled(true);
                        ui->pushButton_call->setEnabled(true);
                    }
                    else{
                        ui->pushButton_add->setEnabled(true);
                        isAddEnable = true;
                        ui->pushButton_pass->setEnabled(true);
                        ui->pushButton_giveup->setEnabled(true);
                    }
                }else{
                    ui->pushButton_add->setEnabled(true);
                    isAddEnable = true;
                    ui->pushButton_pass->setEnabled(true);
                    ui->pushButton_giveup->setEnabled(true);
                }
            }else{

            }
            for(int i = 0;i<playerScore.count();i++){
                if(ui->tableWidget_playerName->item(i,2)->text() == "←"){
                    ui->tableWidget_playerName->setItem(i,2,new QTableWidgetItem(" "));
                }
            }
            ui->tableWidget_playerName->setItem(turnWho,2,new QTableWidgetItem("←"));
            //        for(int i =0;i<myCards.count();++i){
            //            cardLabel[i]->setText(myCards[i]);
            //        }
            isAdd = false;
            isNewRound = false;
            break;
        }
        case MYCARDS:
            in >> tempCard;
            if(tempCard.contains("♥") || tempCard.contains("♦")){
                tempCard = "<font color = red>" + tempCard +"</font>";
            }
            cardLabel[myCardFlag++]->setText(tempCard);
            for(int i = 0;i<playerScore.count();i++){
                ui->tableWidget_playerName->setItem(i,2,new QTableWidgetItem(" "));

            }
            break;
        case GIVEUP:
            in >> whoGiveup;
            ui->tableWidget_playerName->setItem(whoGiveup,2,new QTableWidgetItem("已弃牌"));
            break;
        case PUBLICCARD:
            in >> tempCard;
            if(tempCard.contains("♥") || tempCard.contains("♦")){
                tempCard = "<font color = red>" + tempCard +"</font>";
            }
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
            in >>tempId >>tempName >> otherCard1 >> otherCard2;
            if(otherCard1.contains("♥") || otherCard1.contains("♦")){
                otherCard1 = "<font color = red>" + otherCard1 +"</font>";
            }
            if(otherCard2.contains("♥") || otherCard2.contains("♦")){
                otherCard2 = "<font color = red>" + otherCard2 +"</font>";
            }
            str = QString::number(tempId+1) +"号"+ tempName + "的手牌是：" + otherCard1
                    + "和" + otherCard2;
            ui->textBrowser_log->append(str);

            ui->pushButton_add->setEnabled(false);
            isAddEnable = false;
            ui->pushButton_giveup->setEnabled(false);
            ui->pushButton_call->setEnabled(false);
            ui->pushButton_pass->setEnabled(false);
            break;
        case NEWROUND:
            in >>isNewRound;
            break;
        case JUDGE:
            in>> judgeId;
            if(judgeId == seatId){
                //                ui->lineEdit_winner->show();
                //                ui->pushButton_winner->show();
                //                ui->label_winner->show();
            }
            break;
        case PAUSE:
            //            ui->pushButton_winner->setEnabled(false);
        {
            pauseDialog = new QDialog(this);
            //            pauseDialog->setWindowTitle("请勿操作");
            pauseDialog->setFixedSize(310,205);
            //            pauseDialog->setWindowFlags(Qt::FramelessWindowHint);
            QLabel *label = new QLabel(pauseDialog);
            label->setText("有人掉线，等待中，<font color=red >请勿进行下注等影响游戏进程的操作</font>（可以聊天），"
                           "否则会造成严重的bug，等所有人重连完方可进行下一步操作，所有人重连后将自动关闭本提示窗口！");
            label->setWordWrap(true);
            isPause = true;
            pauseDialog->show();
            break;
        }
        case CONTINUE:
            if(isPause){
                //                ui->pushButton_winner->setEnabled(true);
                pauseDialog->accept();
                delete pauseDialog;
                isPause = false;
            }
            break;
        case OVERFLAG:
            if(!ui->checkBox_watchMod->isChecked()){
                in >> score;
                QString str = "你的积分变为：" + QString::number(score);
                ui->textBrowser_log->append(str);
                ui->label_score->setText(QString::number(score));
                addMoney = 0;
                myCardFlag = 0,puCardFlag = 0;
                ui->pushButton_ready->show();
                for(int i = 0;i<playerScore.count();i++){
                    QTableWidgetItem *item = new QTableWidgetItem(" ");
                    ui->tableWidget_playerName->setItem(i,2,item);
                }
            }else{
                in >> score;
                for(int i = 0;i<pCard.count();i++){
                    pCard[i]->clear();
                }
                for(int i = 0;i<cardLabel.count();i++){
                    cardLabel[i]->clear();
                }
                myCardFlag = 0,puCardFlag = 0;
            }
            break;
        case CHAT:
            in >> tempChat;
            ui->textBrowser_lelaoji->append(QString(tempChat));
            break;
        case LOG:
            in >> tempLog;
            ui->textBrowser_log->append(QString(tempLog));
            break;
            //    case OVERTIME:
            //        in >> isConfirm;
            //        disconTimer.stop();
            //        disconTimer.start(75050);
            //    {
            //        while(isSending){

            //        }
            //        QByteArray block;
            //        QDataStream out(&block,QIODevice::ReadWrite);
            //        out.setVersion(QDataStream::Qt_4_8);
            //        out << quint16(0) << quint8(102) <<bool(true);
            //        out.device() ->seek(0);
            //        out<<quint16(block.size()-sizeof(quint16));
            //        tcpsocket->write(block);
            //        tcpsocket->waitForBytesWritten();
            //    }
            //        break;
        }

#endif
#if 0
        QByteArray block = tcpsocket->readAll();
        ui->textBrowser_lelaoji->append(QString(block)+"flag");
#endif

        nextBlockSize = 0;
    }
}

void Poker_Client::slot_connected()
{
    m_name = ui->lineEdit_name->text();
    isSending = true;
    if(ui->checkBox_watchMod->isChecked()){
        watchId = ui->lineEdit_watchId->text().toInt()-1;
    }else{
        watchId = -2;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(11) <<m_name.toUtf8()<<watchId;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    //    ui->lineEdit_name->hide();
    isSending = false;
    ui->lineEdit_name->setEnabled(false);
    //    ui->label->hide();
    ui->pushButton_join->hide();
    if(!ui->checkBox_watchMod->isChecked()){
        ui->pushButton_ready->setEnabled(true);
    }else{
        ui->pushButton_ready->hide();
    }
    ui->checkBox_watchEnable->setCheckable(true);
    ui->checkBox_watchMod->setEnabled(false);
    seatId = -1;
}

void Poker_Client::slot_disconnected()
{
    ui->textBrowser_log->append("因为网络或者服务器原因，你已断开连接");
    //    ui->lineEdit_name->show();
    //    ui->pushButton_join->show();
}

void Poker_Client::slot_quicklyMessage(QString str)
{
    QString strLelaoji = str;
    strLelaoji = m_name + "：" + strLelaoji;
    isSending = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(100) <<strLelaoji.toUtf8() ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    isSending = false;
}

void Poker_Client::slot_countDown()
{
    ui->lcdNumber_countDown->display(countTime);
    if(countTime) countTime--;
}

void Poker_Client::slot_addMoney()
{
    if(isAddEnable){
        on_pushButton_add_clicked();
    }
}

//void Poker_Client::slot_loseConnect()
//{
//    ui->textBrowser_log->append("你已失去连接，请重启程序重新连接");
//}

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
    isSending = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(100) <<btaChatText ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    isSending = false;
}

void Poker_Client::on_pushButton_join_clicked()
{
    serverIp = ui->lineEdit_ip->text();
    port = ui->lineEdit_port->text().toInt();
    if(ui->checkBox_watchMod->isChecked()){
        if(ui->lineEdit_watchId->text().toInt() < 0 || ui->lineEdit_watchId->text().toInt() > nameList.count()){
            ui->textBrowser_log->append("观战序号不合法，全局观战为0，其余观战序号为选手序号");
            return;
        }
    }
    if(ui->lineEdit_name->text().isEmpty()){
        ui->textBrowser_log->append("名字不能为空！");
    }else{
        m_name = ui->lineEdit_name->text();
        //        tcpsocket->connectToHost("127.0.0.1",10005);
        tcpsocket->connectToHost(serverIp,port);
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
    isSending = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    bool isOk = true;
    out << quint16(0) << quint8(12) <<isOk ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    isSending = false;
    ui->pushButton_ready->hide();
    if(isFirstRun){
        //        disconTimer.start(5501);
        isFirstRun = false;
    }

}



void Poker_Client::on_pushButton_add_clicked()
{
    if(ui->lineEdit_money->text().isEmpty()){
        ui->textBrowser_log->append("加注只能在此基础上继续增加，如果你想跟，请按下方的 跟 按钮！");
    }else{
        addMoney = ui->lineEdit_money->text().toInt();
        ui->lineEdit_money->clear();
        isSending = true;
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << quint8(31) <<seatId
            <<addMoney<<bool(false)<<bool(false);
        out.device() ->seek(0);
        out<<quint16(block.size()-sizeof(quint16));
        tcpsocket->write(block);
        tcpsocket->waitForBytesWritten();
        isSending = false;
        ui->pushButton_add->setEnabled(false);
        isAddEnable = false;
        ui->pushButton_pass->setEnabled(false);
        ui->pushButton_giveup->setEnabled(false);
        ui->pushButton_call->setEnabled(false);
        ui->lcdNumber_countDown->hide();
        countDown.stop();
        isNewRound = false;
    }

}

void Poker_Client::on_pushButton_pass_clicked()
{
    isSending = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(31) <<seatId
        <<int(0)<<bool(true)<<bool(false);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    isSending = false;
    ui->pushButton_add->setEnabled(false);
    isAddEnable = false;
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    ui->lcdNumber_countDown->hide();
    countDown.stop();
    isNewRound = false;

}

void Poker_Client::on_pushButton_giveup_clicked()
{
    isSending = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(31) <<seatId
        <<int(0)<<bool(false)<<bool(true);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    isSending = false;
    ui->pushButton_add->setEnabled(false);
    isAddEnable = false;
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    ui->lcdNumber_countDown->hide();
    countDown.stop();
    isNewRound = false;
}

void Poker_Client::on_pushButton_call_clicked()
{
    isSending = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(31) <<seatId
        <<int(0)<<bool(false)<<bool(false);
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
    isSending = false;
    ui->pushButton_add->setEnabled(false);
    isAddEnable = false;
    ui->pushButton_pass->setEnabled(false);
    ui->pushButton_giveup->setEnabled(false);
    ui->pushButton_call->setEnabled(false);
    ui->lcdNumber_countDown->hide();
    countDown.stop();
    isNewRound = false;
}

//void Poker_Client::on_pushButton_winner_clicked()
//{
//    if(ui->lineEdit_winner->text().isEmpty()){
//        ui->textBrowser_log->append("获胜者不能为空！");
//    }else{
//        isSending = true;
//        QByteArray block;
//        QDataStream out(&block,QIODevice::WriteOnly);
//        out.setVersion(QDataStream::Qt_4_8);
//        out << quint16(0) << quint8(43) <<ui->lineEdit_winner->text();
//        out.device() ->seek(0);
//        out<<quint16(block.size()-sizeof(quint16));
//        tcpsocket->write(block);
//        tcpsocket->waitForBytesWritten();
//        isSending = false;
//        ui->lineEdit_winner->hide();
//        ui->lineEdit_winner->clear();
//        ui->pushButton_winner->hide();
//        ui->label_winner->hide();
//    }
//}


void Poker_Client::on_checkBox_watchMod_stateChanged(int arg1)
{
    if(arg1){
        ui->lineEdit_watchId->setEnabled(true);
    }else{
        watchId = -2;
        ui->lineEdit_watchId->setEnabled(false);
    }
}

void Poker_Client::on_checkBox_watchEnable_stateChanged(int arg1)
{
    if(arg1){
        isWatchEnable =true;
    }else{
        isWatchEnable = false;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << quint8(103) <<isWatchEnable;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
    tcpsocket->waitForBytesWritten();
}
