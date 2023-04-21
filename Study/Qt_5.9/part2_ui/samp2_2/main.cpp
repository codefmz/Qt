#include "Dialog.h"
#include <QApplication>
#include "QScopedPointer"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    QString s;
    w.show();
    return a.exec();
}
