#include "waitingspinnerwidget.h"
#include <QApplication>
#include <QGridLayout>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget * widget = new QWidget;

    WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget;
    QGridLayout * gridLayout = new QGridLayout;
    gridLayout->addWidget(spinner,0, Qt::AlignHCenter);
    widget->setLayout(gridLayout);

    spinner->setRoundness(10.0);
    spinner->setMinimumTrailOpacity(5.0);
    spinner->setTrailFadePercentage(40.0);
    spinner->setNumberOfLines(10);
    spinner->setLineLength(20);
    spinner->setLineWidth(2);
    spinner->setInnerRadius(10);
    spinner->setRevolutionsPerSecond(1);
    spinner->setColor(QColor(0, 0, 0));

    spinner->start(); // gets the show on the road!



    widget->show();
    return a.exec();
}

