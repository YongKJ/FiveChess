#include "indexwindow.h"
#include "gamewindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w(0,0);
    w.show();

    return a.exec();
}
