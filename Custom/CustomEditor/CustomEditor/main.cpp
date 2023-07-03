#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
//    MainWindow::instance()->setWindowIcon(QIcon(":/Resources/icons/edit.ico"));
    MainWindow::instance()->show();
    return a.exec();
}
