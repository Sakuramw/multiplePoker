#include "poker_server.h"
#include "ui_poker_server.h"

Poker_Server::Poker_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poker_Server)
{
    ui->setupUi(this);
    connect(&server,SIGNAL(sig_updateChat(QString)),
            this,SLOT(slot_updateChat(QString)));
    connect(&server,SIGNAL(sig_updateLog(QString)),
            this,SLOT(slot_updateLog(QString)));

}

Poker_Server::~Poker_Server()
{
    delete ui;
}

void Poker_Server::slot_updateChat(QString str)
{
    ui->textBrowser_lelaoji->append(str);
}

void Poker_Server::slot_updateLog(QString str)
{
    ui->textBrowser_log->append(str);
}

void Poker_Server::on_pushButton_listen_clicked()
{
    ui->textBrowser_log->append(tr("开始监听"));
    if(!server.listen(QHostAddress::LocalHost,10005)){
        ui->textBrowser_log->append("监听错误");
    }
}
