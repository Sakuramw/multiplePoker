#include "pokerclient.h"
#include "ui_pokerclient.h"
#include <QHostAddress>
#include <QMessageBox>

PokerClient::PokerClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PokerClient)
{
    ui->setupUi(this);

    tcpConnect = new QTcpSocket();
    nextBlockSize =0;
    tcpConnect->connectToHost("127.0.01",4944);
    connect(tcpConnect,SIGNAL(connected()),
            this,SLOT(slot_connected()));
    connect(tcpConnect,SIGNAL(disconnected()),
            this,SLOT(slot_serverClose()));
    connect(tcpConnect,SIGNAL(readyRead()),
            this,SLOT(slot_readServer()));

}

PokerClient::~PokerClient()
{
    delete ui;
}

void PokerClient::slot_connected()
{
    ui->textBrowser_log->append("已连接服务器");

}

void PokerClient::slot_serverClose()
{
    ui->textBrowser_log->append("服务器关闭了连接");
}

void PokerClient::slot_readServer()
{
    QDataStream in(tcpConnect);
    in.setVersion((QDataStream::Qt_4_8));
    if(nextBlockSize == 0){
        if(tcpConnect->bytesAvailable()<sizeof(quint16)) return;

        in >> nextBlockSize;
    }

    if(tcpConnect->bytesAvailable()<nextBlockSize) return;
    quint8 requestType;
    in >>requestType;
    switch(requestType){
    case 21:


         break;
    }
}

void PokerClient::on_pushButton_connect_clicked()
{
    QString m_Name = ui->lineEdi_name->text();
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << 11 << m_Name;
    out.device() ->seek(0);
    out<<quint16(block.size()-sizeof(quint16));
    tcpConnect->write(block);
}

void PokerClient::on_pushButton_500_clicked()
{

}
