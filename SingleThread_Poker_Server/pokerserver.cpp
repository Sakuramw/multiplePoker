#include "pokerserver.h"
#include "ui_pokerserver.h"

PokerServer::PokerServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PokerServer)
{
    ui->setupUi(this);

    connect(&pokerServer,SIGNAL(sig_gameover(int)),
            this,SLOT(slot_gameOver(int)));


}

PokerServer::~PokerServer()
{
    delete ui;
}

void PokerServer::slot_gameOver(int winner)
{
    ui->textBrowser_log->append(tr("获胜者是：%1").arg(QString::number(winner)));
}

void PokerServer::on_pushButton_listen_clicked()
{
    if(!pokerServer.listen(QHostAddress::Any)){
        ui->textBrowser_log->append("监听错误");
    }
}
