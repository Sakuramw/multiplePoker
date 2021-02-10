#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    onlineNum = 0;
    wantPlayNum = 0;
    firstPlayer = 0;
    memset(playing,-1,sizeof(playing));
}

Server::~Server()
{

}

void Server::beginGame()
{
    for(int i = 0;i<wantPlayNum;i++){
        ClientList[i]->setId(i);
        playing[i] = 1;
    }
    emit sig_beginGame(firstPlayer);
}

void Server::slot_joinPlayer()
{
   ++wantPlayNum;
    if(wantPlayNum == onlineNum){
        beginGame();
    }
}

void Server::slot_playerDo(int id, int money, bool pass, bool giveup)
{
    if(giveup){
        playing[id] = 0;
    }
    turnWho = id;
    nextPlayer = turnWho;
    while(1){
        ++nextPlayer;
        if(nextPlayer>wantPlayNum) nextPlayer = 0;
        if(playing[nextPlayer] == 1 && nextPlayer != turnWho) break;
        else if(playing[nextPlayer] == 1 && nextPlayer == turnWho){
            emit sig_gameover(turnWho);
            return;
        }
    }
    ClientList[turnWho]->player.money += money;
    ClientList[turnWho]->player.isPass = pass;
    ClientList[turnWho]->player.isGiveup = giveup;
    emit sig_playingData(turnWho,money,pass,giveup,nextPlayer);
}

void Server::slot_gameOver(int winnerId)
{

}

void Server::slot_playerQuit(int id)
{
    --onlineNum;
    for(int i = 0;i<ClientList.size();++i){
        if(ClientList[i]->onlineId == id){
            ClientList.remove(i);
        }
    }
}

void Server::incomingConnection(int socketId)
{

    Client *socket = new Client(this);
    socket->setSocketDescriptor(socketId);
    ClientList.append(socket);
    ++onlineNum;
    connect(socket,SIGNAL(sig_joinPlayer()),
            this,SLOT(slot_joinPlayer()));
    connect(this,SIGNAL(sig_beginGame(int)),
            socket,SLOT(slot_beginGame(int)));
    connect(socket,SIGNAL(sig_iDo(int,int,bool,bool)),
            this,SLOT(slot_playerDo(int,int,bool,bool)));
    connect(this,SIGNAL(sig_gameover(int)),
            socket,SLOT(slot_gameOver(int)));
    connect(this,SIGNAL(sig_updatePlayNo(int)),
            socket,SLOT(slot_updatePlayNo(int)));
    connect(this,SIGNAL(sig_setOnlineId(int)),
            socket,SLOT(slot_getOnlineId(int)));
    connect(socket,SIGNAL(disconnected()),
            this,SLOT(slot_playerQuit()));
    connect(socket,SIGNAL(sig_emitOnlineId(int)),
            this,SLOT(slot_playerQuit(int)));

    emit sig_setOnlineId(onlineNum);
    emit sig_updatePlayNo(onlineNum);
}

