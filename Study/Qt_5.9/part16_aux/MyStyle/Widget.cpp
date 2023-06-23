#include "Widget.h"
#include "ui_Widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btnOK,&QPushButton::clicked, ui->lineEdit,[&](){
        bool isReadOnly = ui->lineEdit->isReadOnly();
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- " << isReadOnly;
        ui->lineEdit->setReadOnly(!isReadOnly);
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

//    int h = this->height(), w = this->width();
    painter.setViewport(this->rect());
    painter.setWindow(-100,-100,200,200);

    painter.drawText(-100,0, tr("大老"));

}

