#include "poker.h"

Poker::Poker(quint8 su, quint8 nu, QObject *parent) : suit(su),num(nu),QObject(parent)
{

}

Poker::Poker()
{

}



Poker::~Poker()
{

}

quint8 Poker::getsuit()
{
    return suit;
}

quint8 Poker::getnum()
{
    return num;
}

QString Poker::toString()
{
    QString str;
    switch (suit) {
    case 'd':
        str = "<font color = red>♦";
        break;
    case 'h':
        str = "<font color = red>♥";
        break;
    case 's':
        str = "♠";
        break;
    case 'c':
        str = "♣";
        break;
    }

    if(num>=2&&num<=10){
        str += QString::number(num);
    }else if(num == 11){
        str += "J";
    }else if(num == 12){
        str += "Q";
    }else if(num == 13){
        str += "K";
    }else if(num == 14){
        str += "A";
    }
    switch (suit) {
    case 'd':
        str += "</font>";
        break;
    case 'h':
        str += "</font>";
        break;
    }
    return str;
}

Poker &Poker::operator=(const Poker &pk)
{
    suit = pk.suit;
    num = pk.num;
    return *this;
}

