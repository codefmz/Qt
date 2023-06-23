#include "Widget.h"
#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QStyleFactory>
#include <QFile>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QFile file(":/css/stylesheet.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }else
    {
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- cant open file";
    }
    //    QStringList keys = QStyleFactory::keys();
    //    foreach (auto key, keys)
    //    {
    //        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- key = " << key;
    //    }
    QStyle* style = QStyleFactory::create("Windows");
    QApplication::setStyle(style);
    Widget w;
    w.show();

    QString styleName = QApplication::style()->metaObject()->className();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << styleName;

    return a.exec();
}
