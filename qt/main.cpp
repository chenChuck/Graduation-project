#include "windowmain.h"
#include <QApplication>

char PICbuff[1024*20];
//ord date;
bool admin = false;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    windowmain w;
    w.show();

    return a.exec();
}
