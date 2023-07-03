#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    //initial action and menu
    hideAction = new QAction;
    hideAction->setText("隐藏");


    showAction = new QAction;
    showAction->setText("显示");
    stopAction = new QAction;
    stopAction->setText("暂停");
    continueAction = new QAction;
    continueAction->setText("继续");

    connect(stopAction, SIGNAL(triggered()), this, SLOT(slotStop()));
    connect(continueAction, SIGNAL(triggered()), this, SLOT(slotContinue()));
    connect(hideAction, SIGNAL(triggered()), this, SLOT(slotHide()));
    connect(showAction, SIGNAL(triggered()), this, SLOT(slotShow()));
    menu.addAction(hideAction);
    menu.addAction(showAction);
    menu.addAction(stopAction);
    menu.addAction(continueAction);




    customPlot = new QCustomPlot;
    connect(customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

    statusBar = new QStatusBar;

    setStatusBar(statusBar);
    setCentralWidget(customPlot);
    //    setDemo1();
    //    setDemo2();
//    setDemo3();
//    test();
//    setDemo4();
//    setupScatterPixmapDemo();
//    setupLineStyleDemo();
//    setupDateDemo();
//    setupTextureBrushDemo();
//    setupMultiAxisDemo();
    realTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDemo1()
{

    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    QCPGraph * graph = customPlot->addGraph();
    graph->setData(x, y);




    foreach(auto e, x)
    {
        qDebug() << e;
    }

    QCPGraph * graph3 = customPlot->addGraph(customPlot->yAxis,customPlot->xAxis);
    graph3->setData(x,y);
        for(int i = 0; i < 101; i++)
        {
            x[i] = i / 50.0 - 1;
            y[i] = x[i] * x[i] * x[i];
        }
    QCPGraph * graph2 = customPlot->addGraph();
    graph2->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
//    customPlot->xAxis->setRange(-1, 1);
//    customPlot->yAxis->setRange(-1, 1);
    customPlot->rescaleAxes(true);
//    customPlot->savePng("customPlot.png", 400, 300);
}

void MainWindow::setDemo2()
{
    // add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), y0(251), y1(251);
    for (int i=0; i<251; ++i)
    {
      x[i] = i;
      y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
      y1[i] = qExp(-i/150.0);              // exponential envelope
    }
    customPlot->xAxis->setLabel("x");
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
//    customPlot->xAxis2->setVisible(true);
//    customPlot->xAxis->setTicks(false);
//    customPlot->xAxis2->setTickLabels(false);
//    customPlot->xAxis->setTickLabels(true);
//    customPlot->yAxis2->setVisible(true);
//    customPlot->yAxis2->setTickLabels(true);
    // make left and bottom axes always transfer their ranges to right and top axes:
    // 使上下两个X轴的范围总是相等，使左右两个Y轴的范围总是相等
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:
    customPlot->graph(0)->setData(x, y0);
    customPlot->graph(1)->setData(x, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
//    customPlot->graph(0)->resca leAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
//    customPlot->graph(1)->rescaleAxes(false);
    customPlot->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void MainWindow::setDemo3()
{
    qDebug() << "MainWindow::setDemo3 itemCount -- " << customPlot->legend->itemCount();
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica",16));
    // set locale to english, so we get english decimal separator:
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    // add confidence band graphs:
    customPlot->addGraph();
    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    pen.setColor(QColor(180,180,180));
    customPlot->graph(0)->setName("Confidence Band 68%");
    customPlot->graph(0)->setPen(pen);
    customPlot->graph(0)->setBrush(QBrush(QColor(255,50,30,20)));
    customPlot->addGraph();
    customPlot->legend->removeItem(customPlot->legend->itemCount()-1); // don't show two confidence band graphs in legend
    customPlot->graph(1)->setPen(pen);
    customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

    // add theory curve graph:
    customPlot->addGraph();
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    customPlot->graph(2)->setPen(pen);
    customPlot->graph(2)->setName("Theory Curve");

    // add data point graph:
    customPlot->addGraph();
    customPlot->graph(3)->setPen(QPen(Qt::blue));
    customPlot->graph(3)->setName("Measurement");
    customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));

    // add error bars:
    QCPErrorBars *errorBars = new QCPErrorBars(customPlot->xAxis, customPlot->yAxis);
    errorBars->removeFromLegend();
    errorBars->setAntialiased(false);
    errorBars->setDataPlottable(customPlot->graph(3));
    errorBars->setPen(QPen(QColor(180,180,180)));

    // generate ideal sinc curve data and some randomly perturbed data for scatter plot:
    QVector<double> x0(250), y0(250);
    QVector<double> yConfUpper(250), yConfLower(250);
    for (int i=0; i<250; ++i)
    {
      x0[i] = (i/249.0-0.5)*30+0.01; // by adding a small offset we make sure not do divide by zero in next code line
      y0[i] = qSin(x0[i])/x0[i]; // sinc function
      yConfUpper[i] = y0[i]+0.15;
      yConfLower[i] = y0[i]-0.15;
      x0[i] *= 1000;
    }
    QVector<double> x1(50), y1(50), y1err(50);
    for (int i=0; i<50; ++i)
    {
      // generate a gaussian distributed random number:
      double tmp1 = rand()/(double)RAND_MAX;
      double tmp2 = rand()/(double)RAND_MAX;
      double r = qSqrt(-2*qLn(tmp1))*qCos(2*M_PI*tmp2); // box-muller transform for gaussian distribution
      // set y1 to value of y0 plus a random gaussian pertubation:
      x1[i] = (i/50.0-0.5)*30+0.25;
      y1[i] = qSin(x1[i])/x1[i]+r*0.15;
      x1[i] *= 1000;
      y1err[i] = 0.15;
    }
    // pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
    customPlot->graph(0)->setData(x0, yConfUpper);
    customPlot->graph(1)->setData(x0, yConfLower);
    customPlot->graph(2)->setData(x0, y0);
    customPlot->graph(3)->setData(x1, y1);
    errorBars->setData(y1err);
    customPlot->graph(2)->rescaleAxes();
    customPlot->graph(3)->rescaleAxes(true);

    // setup look of bottom tick labels:
    customPlot->xAxis->setTickLabelRotation(30);
    customPlot->xAxis->ticker()->setTickCount(9);
    customPlot->xAxis->setNumberFormat("ebc");
    customPlot->xAxis->setNumberPrecision(1);
    customPlot->xAxis->moveRange(-10);
    // make top right axes clones of bottom left axes. Looks prettier:
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::setDemo4()
{
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setRowSpacing(-3);
    QVector<QCPScatterStyle::ScatterShape> shapes;
    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
    shapes << QCPScatterStyle::ssStar;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;
    shapes << QCPScatterStyle::ssCustom;

    QPen pen;
    // add graphs with different scatter styles:
    for (int i=0; i<shapes.size(); ++i)
    {
      customPlot->addGraph();
      pen.setColor(QColor(qSin(i*0.3)*100+100, qSin(i*0.6+0.7)*100+100, qSin(i*0.4+0.6)*100+100));
      // generate data:
      QVector<double> x(10), y(10);
      for (int k=0; k<10; ++k)
      {
        x[k] = k/10.0 * 4*3.14 + 0.01;
        y[k] = 7*qSin(x[k])/x[k] + (shapes.size()-i)*5;
      }
      customPlot->graph()->setData(x, y);
      customPlot->graph()->rescaleAxes(true);
      customPlot->graph()->setPen(pen);
      customPlot->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
      customPlot->graph()->setLineStyle(QCPGraph::lsLine);
      // set scatter style:
      if (shapes.at(i) != QCPScatterStyle::ssCustom)
      {
        customPlot->graph()->setScatterStyle(QCPScatterStyle(shapes.at(i), 10));
      }
      else
      {
        QPainterPath customScatterPath;
        for (int i = 0; i < 4; ++i)
        {
          customScatterPath.cubicTo(qCos(M_PI /2.0 * i)*9, qSin(M_PI/2.0 * i)*9, qCos(M_PI*(1/2.0 * i + 1/3.0))*9, qSin(M_PI*(1/2.0 * i + 1/3.0))*9, 0, 0);
        }
        customPlot->graph()->setScatterStyle(QCPScatterStyle(customScatterPath, QPen(Qt::black, 0), QColor(40, 70, 255, 50), 10));
      }
    }
    // set blank axis lines:
    customPlot->rescaleAxes();
    customPlot->xAxis->setTicks(false);
    customPlot->yAxis->setTicks(false);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->rescaleAxes(true);
    customPlot->replot();
}

void MainWindow::test()
{
//     QLinearGradient myGradient;
//     QPen myPen;
//     QPainterPath myPath;
//     QPoint c1(0,0);
//     QPoint c2(4,4);
//     QPoint endPoint(8, 0);
//     myPath.cubicTo(c1, c2, endPoint);
//     QPainter painter(this);
//     painter.setBrush(myGradient);
//     painter.setPen(myPen);
//     painter.drawPath(myPath);


//    customPlot->rescaleAxes(true);
//    customPlot->replot();

}

void MainWindow::paintEvent(QPaintEvent *)
{
//    static QList<QPointF> points = QList<QPointF>() << QPointF(0, 0) << QPointF(100, 100) << QPointF(200, -100)
//                                                        << QPointF(300, 100) << QPointF(330, -80) << QPointF(350, -70);
//        QPainterPath path(points[0]);
//        //计算
//        for (int i = 0; i < points.size() - 1; ++i) {
//            // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
//            // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
//            // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
//            QPointF sp = points[i];
//            QPointF ep = points[i+1];
//            QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
//            QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
//            path.cubicTo(c1, c2, ep);
//        }
//        QPainter painter(this);
//        //设置渲染提示为消除锯齿
//        painter.setRenderHint(QPainter::Antialiasing, true);
//        //设置画笔颜色和宽度
//        painter.setPen(QPen(Qt::black, 2));
//        //将坐标系转换为矢量
//        painter.translate(40, 130);
//        //绘制path
//        painter.drawPath(path);
//        // 绘制曲线上的点
//        painter.setBrush(Qt::gray);
//        //绘制曲线上的点
//        for (int i = 0; i < points.size(); ++i) {
//            painter.drawEllipse(points[i], 4, 4);
//        }


//         QLinearGradient myGradient;
//         QPen myPen;
//         QPainterPath myPath;
//         QPoint c1(200,200);
//         QPoint c2(150,150);
//         QPoint endPoint(200, 200);
//         myPath.cubicTo(c1, c2, endPoint);
//         QPainter painter(this);
//         painter.setBrush(myGradient);
//         painter.setPen(myPen);
//         painter.drawPath(myPath);
}


void MainWindow::setupScatterPixmapDemo()
{
    customPlot->axisRect()->setBackground(QPixmap(":/solarpanels.jpg"));
    customPlot->addGraph();
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    QPen pen;
    pen.setColor(QColor(255, 200, 20, 200));
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(2.5);
    customPlot->graph()->setPen(pen);
    customPlot->graph()->setBrush(QBrush(QColor(255,200,20,70)));
    customPlot->graph()->setScatterStyle(QCPScatterStyle(QPixmap(":/sun.png")));
    // set graph name, will show up in legend next to icon:
    customPlot->graph()->setName("Data from Photovoltaic\nenergy barometer 2011");
    // set data:
    QVector<double> year, value;
    year  << 2005 << 2006 << 2007 << 2008  << 2009  << 2010 << 2011;
    value << 2.17 << 3.42 << 4.94 << 10.38 << 15.86 << 29.33 << 52.1;
    customPlot->graph()->setData(year, value);

    // set title of plot:
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Regenerative Energies", QFont("sans", 12, QFont::Bold)));
    // axis configurations:
    customPlot->xAxis->setLabel("Year");
    customPlot->yAxis->setLabel("Installed Gigawatts of\nphotovoltaic in the European Union");
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
  //  customPlot->xAxis2->setTickLabels(false);
  //  customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
  //  customPlot->xAxis2->setSubTicks(true);
  //  customPlot->yAxis2->setSubTicks(false);
    customPlot->xAxis->setRange(2004.5, 2011.5);
    customPlot->yAxis->setRange(0, 52);
    // setup legend:
    customPlot->legend->setFont(QFont(font().family(), 7));
    customPlot->legend->setIconSize(50, 20);
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
}
void MainWindow::setupLineStyleDemo()
{
  customPlot->legend->setVisible(true);
  customPlot->legend->setFont(QFont("Helvetica", 9));
  QPen pen;
  QStringList lineNames;
  lineNames << "lsNone" << "lsLine" << "lsStepLeft" << "lsStepRight" << "lsStepCenter" << "lsImpulse";
  // add graphs with different line styles:
  for (int i=QCPGraph::lsNone; i<=QCPGraph::lsImpulse; ++i)
  {
    customPlot->addGraph();
    pen.setColor(QColor(qSin(i*1+1.2)*80+80, qSin(i*0.3+0)*80+80, qSin(i*0.3+1.5)*80+80));
    customPlot->graph()->setPen(pen);
    customPlot->graph()->setName(lineNames.at(i-QCPGraph::lsNone));
    customPlot->graph()->setLineStyle((QCPGraph::LineStyle)i);
    customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    // generate data:
    QVector<double> x(15), y(15);
    for (int j=0; j<15; ++j)
    {
      x[j] = j/15.0 * 5*3.14 + 0.01;
      y[j] = 7*qSin(x[j])/x[j] - (i-QCPGraph::lsNone)*5 + (QCPGraph::lsImpulse)*5 + 2;
    }
    customPlot->graph()->setData(x, y);
    customPlot->graph()->rescaleAxes(true);
  }
  // zoom out a bit:
  customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
  customPlot->xAxis->scaleRange(1.1, customPlot->xAxis->range().center());
  // set blank axis lines:
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(true);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(true);
  // make top right axes clones of bottom left axes:
//  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();
}


void MainWindow::setupDateDemo()
{
  // set locale to english, so we get english month names:
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  // seconds of current time, we'll use it as starting point in time for data:
  double now = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
  srand(8); // set the random seed, so we always get the same random data
  // create multiple graphs:
  for (int gi=0; gi<5; ++gi)
  {
    customPlot->addGraph();
    QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setPen(QPen(color.lighter(200)));
    customPlot->graph()->setBrush(QBrush(color));
    // generate random walk data:
    QVector<QCPGraphData> timeData(250);
    for (int i=0; i<250; ++i)
    {
      timeData[i].key = now + 24*3600*i;
      if (i == 0){
        double randD = rand();
        timeData[i].value = (i/50.0+1)*(randD/(double)RAND_MAX-0.5);
        qDebug() <<" randD = " << randD << " key = " << timeData[i].key << " value = " << timeData[i].value;
      }else{
        timeData[i].value = qFabs(timeData[i-1].value)*(1+0.02/4.0*(4-gi)) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
      }
    }
    customPlot->graph()->data()->set(timeData);
  }
  // configure bottom axis to show date instead of number:
  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
  customPlot->xAxis->setTicker(dateTicker);
  // configure left axis text labels:
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  textTicker->addTick(10, "a bit\nlow");
  textTicker->addTick(50, "quite\nhigh");
  customPlot->yAxis->setTicker(textTicker);
  // set a more compact font size for bottom and left axis tick labels:
  customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  // set axis labels:
  customPlot->xAxis->setLabel("Date");
  customPlot->yAxis->setLabel("Random wobbly lines value");
  // make top and right axes visible but without ticks and labels:
  customPlot->xAxis2->setVisible(true);
  customPlot->yAxis2->setVisible(true);
  customPlot->xAxis2->setTicks(false);
  customPlot->yAxis2->setTicks(false);
//  customPlot->xAxis2->setTickLabels(false);
//  customPlot->yAxis2->setTickLabels(false);
  // set axis ranges to show all data:
  customPlot->xAxis->setRange(now, now+24*3600*249);
  customPlot->yAxis->setRange(0, 60);
  // show legend with slightly transparent background brush:
  customPlot->legend->setVisible(true);
  customPlot->legend->setBrush(QColor(255, 255, 255, 150));
}

void MainWindow::setupTextureBrushDemo()
{
    // add two graphs with a textured fill:
    customPlot->addGraph();
    QPen redDotPen;
    redDotPen.setStyle(Qt::DotLine);
    redDotPen.setColor(QColor(170, 100, 100, 180));
    redDotPen.setWidthF(2);
    customPlot->graph(0)->setPen(redDotPen);
    customPlot->graph(0)->setBrush(QBrush(QPixmap(":/balboa.jpg"))); // fill with texture of specified image

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));

    // activate channel fill for graph 0 towards graph 1:
    customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

    // generate data:
    QVector<double> x(250);
    QVector<double> y0(250), y1(250);
    for (int i=0; i<250; ++i)
    {
      // just playing with numbers, not much to learn here
      x[i] = 3*i/250.0;
      y0[i] = 1+qExp(-x[i]*x[i]*0.8)*(x[i]*x[i]+x[i]);
      y1[i] = 1-qExp(-x[i]*x[i]*0.4)*(x[i]*x[i])*0.1;
    }

    // pass data points to graphs:
    customPlot->graph(0)->setData(x, y0);
    customPlot->graph(1)->setData(x, y1);
    // activate top and right axes, which are invisible by default:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    // make tick labels invisible on top and right axis:
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // set ranges:
    customPlot->xAxis->setRange(0, 2.5);
    customPlot->yAxis->setRange(0.9, 1.6);
    // assign top/right axes same properties as bottom/left:
    customPlot->axisRect()->setupFullAxesBox();
}

void MainWindow::setupMultiAxisDemo()
{
    customPlot->setInteractions(QCP::iSelectPlottables | QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    customPlot->legend->setFont(legendFont);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    // setup for graph 0: key axis left, value axis bottom
    // will contain left maxwell-like function
    customPlot->addGraph(customPlot->yAxis, customPlot->xAxis);
    customPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    customPlot->graph(0)->setBrush(QBrush(QPixmap(":/balboa.jpg"))); // fill with texture of specified image
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    customPlot->graph(0)->setName("Left maxwell function");

    // setup for graph 1: key axis bottom, value axis left (those are the default axes)
    // will contain bottom maxwell-like function with error bars
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(1)->setBrush(QBrush(QPixmap(":/balboa.jpg"))); // same fill as we used for graph 0
    customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    customPlot->graph(1)->setName("Bottom maxwell function");
    QCPErrorBars *errorBars = new QCPErrorBars(customPlot->xAxis, customPlot->yAxis);
    errorBars->removeFromLegend();
    errorBars->setDataPlottable(customPlot->graph(1));

    // setup for graph 2: key axis top, value axis right
    // will contain high frequency sine with low frequency beating:
    customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    customPlot->graph(2)->setPen(QPen(Qt::blue));
    customPlot->graph(2)->setName("High frequency sine");

    // setup for graph 3: same axes as graph 2
    // will contain low frequency beating envelope of graph 2
    customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidthF(4);
    customPlot->graph(3)->setPen(blueDotPen);
    customPlot->graph(3)->setName("Sine envelope");

    // setup for graph 4: key axis right, value axis top
    // will contain parabolically distributed data points with some random perturbance
    customPlot->addGraph(customPlot->yAxis2, customPlot->xAxis2);
    customPlot->graph(4)->setPen(QColor(50, 50, 50, 255));
    customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(4)->setName("Some random data around\na quadratic function");

    // generate data, just playing with numbers, not much to learn here:
    QVector<double> x0(25), y0(25);
    QVector<double> x1(15), y1(15), y1err(15);
    QVector<double> x2(250), y2(250);
    QVector<double> x3(250), y3(250);
    QVector<double> x4(250), y4(250);
    for (int i=0; i<25; ++i) // data for graph 0
    {
      x0[i] = 3*i/25.0;
      y0[i] = qExp(-x0[i]*x0[i]*0.8)*(x0[i]*x0[i]+x0[i]);
    }
    for (int i=0; i<15; ++i) // data for graph 1
    {
      x1[i] = 3*i/15.0;;
      y1[i] = qExp(-x1[i]*x1[i])*(x1[i]*x1[i])*2.6;
      y1err[i] = y1[i]*0.25;
    }
    for (int i=0; i<250; ++i) // data for graphs 2, 3 and 4
    {
      x2[i] = i/250.0*3*M_PI;
      x3[i] = x2[i];
      x4[i] = i/250.0*100-50;
      y2[i] = qSin(x2[i]*12)*qCos(x2[i])*10;
      y3[i] = qCos(x3[i])*10;
      y4[i] = 0.01*x4[i]*x4[i] + 1.5*(rand()/(double)RAND_MAX-0.5) + 1.5*M_PI;
    }

    // pass data points to graphs:
    customPlot->graph(0)->setData(x0, y0);
    customPlot->graph(1)->setData(x1, y1);
    errorBars->setData(y1err);
    customPlot->graph(2)->setData(x2, y2);
    customPlot->graph(3)->setData(x3, y3);
    customPlot->graph(4)->setData(x4, y4);
    // activate top and right axes, which are invisible by default:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    // set ranges appropriate to show data:
    customPlot->xAxis->setRange(0, 2.7);
    customPlot->yAxis->setRange(0, 2.6);
    customPlot->xAxis2->setRange(0, 3.0*M_PI);
    customPlot->yAxis2->setRange(-70, 35);
    // set pi ticks on top axis:
    customPlot->xAxis2->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));
    // add title layout element:
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Way too many graphs in one plot", QFont("sans", 12, QFont::Bold)));
    // set labels:
    customPlot->xAxis->setLabel("Bottom axis with outward ticks");
    customPlot->yAxis->setLabel("Left axis label");
    customPlot->xAxis2->setLabel("Top axis label");
    customPlot->yAxis2->setLabel("Right axis label");
    // make ticks on bottom axis go outward:
    customPlot->xAxis->setTickLength(0, 5);
    customPlot->xAxis->setSubTickLength(0, 3);
    // make ticks on right axis go inward and outward:
    customPlot->yAxis2->setTickLength(3, 3);
    customPlot->yAxis2->setSubTickLength(1, 1);
}

void MainWindow::realTimer()
{
    //设置基本坐标轴（左侧Y轴和下方X轴）可拖动、可缩放、曲线可选、legend可选、设置伸缩比例，使所有图例可见
    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    QCPGraph * graph1 = customPlot->addGraph();
    graph1->setPen(QPen(Qt::blue));
    graph1->setName("blue");
    customPlot->legend->setVisible(true);
    QCPGraph * graph2 = customPlot->addGraph();
    graph2->setName("red");
    graph2->setPen(QPen(Qt::red));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    timer.start(5); // Interval 0 means to refresh as fast as possible
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressSlot(QMouseEvent *)));


}

void MainWindow::mousePressSlot(QMouseEvent *event)
{
//    qDebug() << "MainWindow::mouseWheelSlot";
    if(event->button() == Qt::RightButton)
    {
//        QPoint point = event->pos();
        menu.exec(QCursor::pos());


    }



}

void MainWindow::selectionChanged()
{
    qDebug() << "MainWindow::selectionChanged";
    // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    // 将图形的选择与相应图例项的选择同步
    for (int i=0; i<customPlot->graphCount(); ++i)
    {
        QCPGraph *graph = customPlot->graph(i);
        QCPPlottableLegendItem *item = customPlot->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
            item->setSelected(true);
            //注意：这句需要Qcustomplot2.0系列版本
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
            //这句1.0系列版本即可
            //graph->setSelected(true);
        }
    }
}

void MainWindow::slotStop()
{
    timer.stop();
}

void MainWindow::slotContinue()
{
    timer.start(50);
}

void MainWindow::slotShow()
{
    QList<QCPAbstractPlottable*>  listPlots = customPlot->selectedPlottables();
    foreach(auto e, listPlots)
    {
        if(e->selected())
        {
            e->setVisible(true);
        }
    }
}

void MainWindow::slotHide()
{
    QList<QCPAbstractPlottable*>  listPlots = customPlot->selectedPlottables();
    foreach(auto e, listPlots)
    {
        if(e->selected())
        {
            e->setVisible(false);
        }
    }

}

void MainWindow::realtimeDataSlot()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      customPlot->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
      customPlot->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(customPlot->graph(0)->data()->size()+customPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}
