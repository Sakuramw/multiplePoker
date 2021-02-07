#include "pokerclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PokerClient w;
    w.show();

    return a.exec();
}
