#include "pokerserver.h"
#include "ui_pokerserver.h"
#include <QMessageBox>

PokerServer::PokerServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PokerServer)
{
    ui->setupUi(this);
    port = 0;
    connect(&server,SIGNAL(newConnection()),
            this,SLOT(slot_newConnect()));


}

PokerServer::~PokerServer()
{
    delete ui;
}

void PokerServer::on_pushButton_listen_clicked()
{
    if(!server.listen(QHostAddress::Any, port)){
        QMessageBox::warning(this,"error","开启失败");
    }
}

void PokerServer::slot_newConnect()
{
    ui->textBrowser_log->append(tr("新的用户加入了"));
}
