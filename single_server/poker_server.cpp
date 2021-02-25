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
    connect(this,SIGNAL(sig_defaultSet(int,int)),
            &server,SLOT(slot_defaultSet(int,int)));
    connect(&server,SIGNAL(sig_gameBegin()),
            this,SLOT(slot_gameBegin()));

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

void Poker_Server::slot_gameBegin()
{
    for(int i = 0;i<server.ClientList.count();i++){
        connect(this,SIGNAL(sig_defaultSet(int,int)),
                server.ClientList[i],SLOT(slot_defaultSet(int)));
    }
}

void Poker_Server::on_pushButton_listen_clicked()
{
    ui->textBrowser_log->append(tr("开始监听"));
    if(!server.listen(QHostAddress::LocalHost,10005)){
        ui->textBrowser_log->append("监听错误");
    }
    emit sig_defaultSet(ui->spinBox_money->text().toInt(),ui->spinBox_id->text().toInt());
}

void Poker_Server::on_pushButton_listen_net_clicked()
{
    ui->textBrowser_log->append(tr("开始网络监听"));
    if(!server.listen(QHostAddress::Any,10005)){
        ui->textBrowser_log->append("监听错误");
    }
    emit sig_defaultSet(ui->spinBox_money->text().toInt(),ui->spinBox_id->text().toInt());
}

void Poker_Server::on_pushButton_clicked()
{
    if(server.ClientList.count() != 0){
        for(int i =0;i<server.ClientList.count();i++){
            ui->textBrowser_log->append(QString::number(server.ClientList[i]->score));
        }
    }else{
        ui->textBrowser_log->append("暂时无人加入");
    }

}
