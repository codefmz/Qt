#include "mainwindow.h"

#include <QApplication>
#include <QProcess>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QString exeFilePath = R"(E:\Code\Qt\Test\Multi2\exe\SharedMemory_boxed.exe)";
    QProcess process;
    process.start(exeFilePath);
    return a.exec();
}
