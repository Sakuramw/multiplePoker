#include "poker_client.h"
#include "ui_poker_client.h"

Poker_Client::Poker_Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poker_Client)
{
    ui->setupUi(this);
    tcpsocket = new QTcpSocket();
    nextBlockSize =0;
    m_name = ui->lineEdit_name->text();

    connect(tcpsocket,SIGNAL(readyRead()),
            this,SLOT(slot_readServer()));
    connect(tcpsocket,SIGNAL(connected()),
            this,SLOT(slot_connected()));



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
    QByteArray tempChat,tempLog;
    in >> requestType;
    switch(requestType){
    case SEAT:
        in >> seatId;
        ui->textBrowser_log->append("座位号："+QString::number(seatId));
        break;
    case WHOPLAY:
        in>>isTurnMy;
        if(isTurnMy){
             ui->pushButton_500->setEnabled(true);
             ui->pushButton_pass->setEnabled(true);
             ui->pushButton_giveup->setEnabled(true);
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
}


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
    m_name = ui->lineEdit_name->text();
    tcpsocket->connectToHost("127.0.0.1",10005);
}

void Poker_Client::on_pushButton_ready_clicked()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    bool isOk = true;
    out << quint16(0) << quint8(12) <<isOk ;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpsocket->write(block);
}


