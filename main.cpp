#include "fifpuzzle.h"
#include "board.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fifpuzzle w;
    w.show();

    return a.exec();
}
