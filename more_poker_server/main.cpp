#include "pokerserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PokerServer w;
    w.show();

    return a.exec();
}
