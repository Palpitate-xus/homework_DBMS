#include "mainwindow.h"
#include "dialog_login.h"
#include <QApplication>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Dialog_login login;
    if (login.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    else return 0;
}
