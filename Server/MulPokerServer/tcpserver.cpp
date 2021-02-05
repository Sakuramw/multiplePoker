#include "tcpserver.h"
#include "ui_tcpserver.h"

TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::slot_update(QString msg)
{
    if(1){
        ui->textBrowser_gameLog->append(msg);
    }else{
        ui->textBrowser_lelaoji->append(msg);
    }
}

void TcpServer::on_pushButton_begin_clicked()
{
    server = new Server(this ,port);
    connect(server , SIGNAL()
            this,SLOT(slot_))
}
