#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include "QSlider"
#include "include/qwbattery.h"
class Widget : public QWidget
{
   Q_OBJECT

public:
   explicit Widget(QWidget *parent = 0);
   ~Widget();

public slots:
   void on_horizontalSlider(int value);
   void on_battery(int arg1);

private:
   QwBattery * battery;
   QLabel * LabInfo;
   QSlider * slider;

};

#endif // WIDGET_H
