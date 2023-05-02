#include "Widget.h"
#include <QApplication>
#include <QDebug>


#define QT_NO_DEBUG_OUTPUT
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    const QMetaObject * meta = w.metaObject();
    //测试元对象系统
//    Test::testMetaObject(w, meta);
//    //测试属性系统
//    Test::testProperty(w,meta);
//    //测试版本宏
//    Test::testMacro();

    //测试容器
    Test::testContainer();
    return a.exec();
}
