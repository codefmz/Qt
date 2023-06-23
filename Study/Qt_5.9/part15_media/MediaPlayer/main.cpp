#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
QTranslator * trans = nullptr;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    trans = new QTranslator;
    trans->load(R"(E:\Code\Qt\Study\Qt_5.9\part15_media\MediaPlayer\MediaPlayer_en.qm)");
    a.installTranslator(trans);
    MainWindow w;
    w.show();

    return a.exec();
}
