#include "ui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ui w;
    w.show();

    return a.exec();
}