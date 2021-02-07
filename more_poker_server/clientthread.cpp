#include "clientthread.h"

ClientThread::ClientThread(int socketDescriptor,QObject *parent) : QThread(parent),socketDescriptor(socketDescriptor)
{

}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{
    QTcpSocket tcpSocket;
    if(!tcpSocket.setSocketDescriptor(socketDescriptor)){
        emit sig_error(tcpSocket.error());
        return;
    }
    //功能实现



}

