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

}

PokerClient::~PokerClient()
{
    delete ui;
}

void PokerClient::on_pushButton_connect_clicked()
{
    tcpConnect->connectToHost(QHostAddress::LocalHost,10000);
}
