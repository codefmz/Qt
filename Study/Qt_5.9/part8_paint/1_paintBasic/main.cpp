#include "FixedCheckBox.h"
#include "widget.h"
#include <QApplication>
#include <QGridLayout>
int main(int argc, char* argv[])
{
    QApplication   a(argc, argv);
    QWidget*       widget = new QWidget;
    FixedCheckBox* fix1 = new FixedCheckBox(widget);
    FixedCheckBox* fix2 = new FixedCheckBox(widget);
    FixedCheckBox* fix3 = new FixedCheckBox(widget);
    FixedCheckBox* fix4 = new FixedCheckBox(widget);
    fix4->setTickState(false, true);
    QCheckBox* fix5 = new QCheckBox("hhahh",widget);
    QGridLayout * layout = new QGridLayout(widget);
    layout->addWidget(fix1,0,0);
    layout->addWidget(fix2,1,0);
    layout->addWidget(fix3,2,0);
    layout->addWidget(fix4,3,0);
    layout->addWidget(fix5,4,0);
    widget->show();
    //    Widget w;
    //    w.show();

    return a.exec();
}
