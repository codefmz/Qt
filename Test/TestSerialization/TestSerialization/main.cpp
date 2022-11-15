#include "MainWindow.h"

#include <QApplication>
#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;


//    w.show();
    QFile file("F://Code//Qt//Test//TestSerialization//TestSerialization");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QDataStream out(&file);
    out << QString("王老击");
    file.flush();
    file.close();


    return a.exec();
}
