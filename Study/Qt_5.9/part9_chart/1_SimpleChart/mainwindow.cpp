#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRandomGenerator"
#include "QDateTime"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::createChart()
{   //创建图表
    QChartView *chartView=new QChartView(this); //创建 ChartView
    QChart *chart = new QChart(); //创建 Chart
    chart->setTitle("简单函数曲线");
    chartView->setChart(chart); //Chart添加到ChartView
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    this->setCentralWidget(chartView);
    //创建曲线序列
    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();
    series0->setName("Sin曲线");
    series1->setName("Cos曲线");
    chart->addSeries(series0); //序列添加到图表
    chart->addSeries(series1);

//    QPen    pen;
//    pen.setStyle(Qt::DotLine);//Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine
//    pen.setWidth(2);
//    pen.setColor(Qt::red);
//    series0->setPen(pen);

//    pen.setStyle(Qt::SolidLine);//Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine
//    pen.setColor(Qt::blue);
//    series1->setPen(pen);


    //序列添加数值
    qreal   t=0,y1,y2,intv=0.1;
    int cnt=100;
    QRandomGenerator * randGen = new QRandomGenerator();
    randGen->bounded(100);
//    randGen->generate(0,100);

    QList<QPointF> pl1;
    for(int i=0;i<= cnt;i++)
    {
        qint32 value =   randGen->bounded(10);
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- value = " << value;
        y1=qSin( t);
//        series0->append(t,y1);
        pl1<<QPointF(t,y1);
        y2=qSin(t+20);
        series1->append(t,y2);
        t+=intv;
    }

    series0->append(pl1);
    //    chart->createDefaultAxes();
    //    chart->axisX()->setTitleText("time(secs)");
    //    chart->axisY()->setTitleText("value");

    //创建坐标轴
    QValueAxis *axisX = new QValueAxis; //X 轴
    axisX->setRange(0, 10); //设置坐标轴范围
    //    axisX->setLabelFormat("%.1f"); //标签格式
    //    axisX->setTickCount(11); //主分隔个数
    //    axisX->setMinorTickCount(4);
    axisX->setTitleText("time(secs)"); //标题
    //    axisX->setGridLineVisible(false);

    QValueAxis *axisY = new QValueAxis; //Y 轴
    axisY->setRange(-2, 2);
    axisY->setTitleText("value");
//    axisY->setTickCount(5);
//    axisY->setMinorTickCount(4);
//    axisY->setLabelFormat("%.2f"); //标签格式
//    axisY->setGridLineVisible(false);

    chart->setAxisX(axisX, series0); //为序列设置坐标轴
    chart->setAxisY(axisY, series0); //

    chart->setAxisX(axisX, series1); //为序列设置坐标轴
    chart->setAxisY(axisY, series1); //
}


