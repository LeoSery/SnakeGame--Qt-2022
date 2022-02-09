#include "SnakeGameQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnakeGameQt w;
    w.show();
    return a.exec();
}
