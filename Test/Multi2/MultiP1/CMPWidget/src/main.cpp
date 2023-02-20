#include "../include/MainWindow.h"

#include <QApplication>
#include <QDebug>
#include "CMP1.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    const char * hh = CMP1::getName();

    w.setWindowTitle(hh);
    w.show();
    return a.exec();
}
