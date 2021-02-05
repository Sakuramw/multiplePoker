#include "server.h"
#include <QHostAddress>


Server::Server(QObject *parent,int port) : QTcpServer(parent)
{
    listen(QHostAddress::Any, port); //监听
}

Server::~Server()
{

}

void Server::incomingConnection(int socket)
{
    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this);//只要有新的连接就生成一个新的通信套接字
    //将新创建的通信套接字描述符指定为参数socketdescriptor
    tcpclientsocket->setSocketDescriptor(socketDescriptor);

}

