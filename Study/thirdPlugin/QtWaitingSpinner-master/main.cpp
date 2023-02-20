#include "waitingspinnerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget * widget = new QWidget;
    WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(widget);

    spinner->setRoundness(70.0);
    spinner->setMinimumTrailOpacity(15.0);
    spinner->setTrailFadePercentage(70.0);
    spinner->setNumberOfLines(15);
    spinner->setLineLength(30);
    spinner->setLineWidth(7);
    spinner->setInnerRadius(20);
    spinner->setRevolutionsPerSecond(1);
    spinner->setColor(QColor(0, 0, 0));

    spinner->start(); // gets the show on the road!
    widget->show();
    return a.exec();
}

