#include "poker_client.h"
#include <QApplication>
#include <QTextCodec>
#include "login.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    QApplication a(argc, argv);
//    Login l;
    Poker_Client w;
//    l.show();
//    if(l.exec() == QDialog::Accepted)
        w.show();
    return a.exec();
}
