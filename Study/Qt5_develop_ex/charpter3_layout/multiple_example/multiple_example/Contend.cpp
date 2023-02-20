#include "Contend.h"

Contend::Contend(QWidget * parent):QFrame(parent)
{
     stack = new QStackedWidget(this);
     stack->setFrameStyle(QFrame::Panel | QFrame::Raised);

     AmendBtn = new QPushButton(tr("修改"));
     CloseBtn = new QPushButton(tr("关闭"));



}

BaseInfo::BaseInfo(QWidget *parent)
{

}
