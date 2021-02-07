#include "thread.h"

Thread::Thread(int socketDescriptor,QObject *parent) : QObject(parent),socketDescriptor(socketDescriptor)
{
    tcpSocket = new QTcpSocket();
    tcpSocket.setSocketDescriptor(socketDescriptor);
    connect(tcpSocket,SIGNAL(readyRead()),
            this,SLOT(slot_dealData()));
}

Thread::~Thread()
{

}

void Thread::slot_dealData()
{


}

