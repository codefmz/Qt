#include "widget.h"
#include "QLayout"
#include "QDebug"
// 若MSVC 编译版本错误，修改 msvc-version.conf 文件
// 解决MSVC编译时，界面汉字乱码的问题
//#if _MSC_VER >= 1600     //MSVC2015>1899,    MSVC_VER= 14.0
//#pragma execution_character_set("utf-8")
//#endif


Widget::Widget(QWidget *parent) :
   QWidget(parent)
{

   LabInfo = new QLabel;
   battery = new QwBattery;
   QSlider * slider = new QSlider(Qt::Horizontal);
   slider->setTickInterval(1);
   slider->setRange(0,100);

   QVBoxLayout * layout = new QVBoxLayout(this);
   layout->addWidget(battery);
   layout->addWidget(slider);
   layout->addWidget(LabInfo);

   this->resize(300,200);
   connect(slider, SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider(int)));
   connect(battery, SIGNAL(powerLevelChanged(int)),this,SLOT(on_battery(int)));

}

Widget::~Widget()
{

}

void Widget::on_horizontalSlider(int value)
{
   qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ <<" -- ";
   battery->setPowerLevel(value);
}

void Widget::on_battery(int arg1)
{
    qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ <<" -- ";
//   QString  str=QString::asprintf("当前电量：%d %%",arg1);
//   QString  str="当前电量："+QString::asprintf("%d %%",arg1);
   QString  str=QStringLiteral("当前电量：")+QString::asprintf("%d %%",arg1);
   LabInfo->setText(str);
}
