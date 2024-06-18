#include <QApplication>
#include "boost_example.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    testBoostLib();

    w.show();
    return a.exec();
}
