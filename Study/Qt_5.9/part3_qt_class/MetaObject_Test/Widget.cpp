#include "Widget.h"
#include "TestO.h"
#include "ui_Widget.h"
#include <QDebug>
#include <QList>
#include <QMetaClassInfo>
TestO o;
Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(testSlot()));
    connect(ui->mybtn, SIGNAL(clicked()), this, SLOT(testSlot()));
    connect(ui->mybtn,SIGNAL(clicked()), &o, SLOT(test()));
}

Widget::~Widget()
{
    delete ui;
}

const QString& Widget::name() const
{
    return myname;
}

void Widget::testSlot()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " --  " << button->text();
    }
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- widget slot";
}

void Test::testMetaObject(Widget& w, const QMetaObject* meta)
{
    qWarning().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << w.metaObject()->className();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << w.inherits("QWidget");
    w.setProperty("pro1", QVariant(QString("你好")));
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << w.property("pro1").toString();
}

void Test::testProperty(Widget& w, const QMetaObject* meta)
{
    //测试属性系统
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << w.property("name").toString();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- meta propertyCount = " << meta->propertyCount();
    int count = meta->classInfoCount();
    for (int var = 0; var < count; ++var)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " --  key = " << meta->classInfo(var).name()
                           << " value = " << meta->classInfo(var).value();
    }
}

void Test::testMacro()
{
    if (Q_BYTE_ORDER == Q_LITTLE_ENDIAN)
    {
        qCritical().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- LITTLE_ENDIAN";
    }
    else
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- BIG_ENDIAN";
    }
}

void Test::testContainer()
{
    QList<int> alist;
    alist.append(3);
    alist.append(4);
    alist.append(5);
    alist.append(1);
    alist.append(2);

    //测试java版本迭代器
    QListIterator<int> javaIter(alist);
    while (javaIter.hasNext())
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << javaIter.next();
    }
    qDebug().nospace() << "======================== STL";
    // STL版本迭代器
    for (auto var = alist.begin(); var != alist.end(); ++var)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << *var;
    }
    qDebug().nospace() << "======================== foreach";
    foreach (auto& i, alist)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << i;
    }
    qDebug().nospace() << "======================== for";
    for (auto& i : alist)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << i;
    }

    QMap<int, QString> amap;
    amap.insert(2, "100");
    amap.insert(3, "2");
    amap.insert(1, "4");
    foreach (auto& i, amap) //直接遍历就是 value
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << i;
    }
    foreach (auto& i, amap.keys())
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << i;
    }
    // QMultiMap
    QMultiMap<int, QString> amulmap;
    amulmap.insert(1, "aaa");
    amulmap.insert(1, "bbbb");
    amulmap.insert(1, "cccc");
    amulmap.insert(1, "zzz");
    amulmap.insert(2, "cccc");
    amulmap.insert(2, "cccg");
    amulmap.insert(2, "cccg");
    amulmap.insert(2, "ccgc");
    foreach (const auto& i, amulmap.uniqueKeys())
    {
        foreach (auto j, amulmap.values(i))
        {
            qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- key = " << i << " value = " << j;
        }
    }
}
