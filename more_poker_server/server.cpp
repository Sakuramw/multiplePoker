#include "server.h"



Server::Server(QObject *parent) : QTcpServer(parent)
{

}

Server::~Server()
{

}

void Server::incomingConnection(int socketDescriptor)
{
    ClientThread *thread = new ClientThread(socketDescriptor,this);
    clientThreadList.append(thread);
    connect(thread,SIGNAL(finished()),
            thread,SLOT(deleteLater()));
    thread->start();
}

