#include "MainWindow.h"
#include <qwt_series_data.h>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    myPlot = new QwtPlot(tr("Two Curves"), this);
    setCentralWidget(myPlot);

    setupQuadraticDemo();

}

MainWindow::~MainWindow() {}

void MainWindow::setupQuadraticDemo()
{
    QString demoName = "Quadratic Demo";
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 - 1; // x goes from -1 to 1
        y[i] = x[i]*x[i];  // let's plot a quadratic function
    }

    QwtPlotCurve * curve1 = new QwtPlotCurve("Curve 1");
    QwtPlotCurve * curve2 = new QwtPlotCurve("Curve 2");
    curve1->setSamples(x,y);

    curve1->attach(myPlot);

    myPlot->replot();
}
