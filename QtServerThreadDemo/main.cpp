#include "qtserverthreaddemo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtserverthreaddemo w;
    w.show();
    return a.exec();
}
