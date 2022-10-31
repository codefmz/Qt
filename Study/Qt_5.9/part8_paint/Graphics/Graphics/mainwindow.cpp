#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    initGraphics();
    //initWidget
   QGridLayout * gridLayout = new QGridLayout;
   gridLayout->setSpacing(6);
   gridLayout->setContentsMargins(11,11,11,11);
   gridLayout->addWidget(ui->LabSceneRect);
   gridLayout->addWidget(ui->labViewSize);
   ui->groupBox->setLayout(gridLayout);



   QGridLayout * layOut = new QGridLayout;
   layOut->setSpacing(6);
   layOut->setContentsMargins(11,11,11,11);
   layOut->addWidget(ui->groupBox,0,0,1,1);
   layOut->addWidget(ui->graphicsView,1,0,1,1);

   QWidget * centralwidget = new QWidget;
   centralwidget->setLayout(layOut);
   setCentralWidget(centralwidget);


   labViewCord = new QLabel("View 坐标");
   labViewCord->setMinimumWidth(150);


   labItemCord = new QLabel("Item 坐标");
   labItemCord->setMinimumWidth(150);


   labSceneCord = new QLabel("Scene 坐标");
   labSceneCord->setMinimumWidth(150);


   ui->statusbar->addWidget(labViewCord, 0);

   ui->statusbar->addWidget(labSceneCord,5);

   ui->graphicsView->setCursor(Qt::CrossCursor);
   ui->graphicsView->setMouseTracking(true);

   connect(ui->graphicsView, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_mouseMove(QPoint)));
   connect(ui->graphicsView, SIGNAL(mouseClicked(QPoint)), this, SLOT(on_mouseClicked(QPoint)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGraphics()
{
    QRectF rect(-200, -100, 400, 200);
    graphScene = new QGraphicsScene(rect);
    //scene 与 graphicsview 关联
    ui->graphicsView->setScene(graphScene);
    QGraphicsRectItem  * item = new QGraphicsRectItem(rect);
    item->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    QPen pen;
    pen.setWidth(2);
    item->setPen(pen);
    graphScene->addItem(item);

    QGraphicsEllipseItem * ellipseItem = new QGraphicsEllipseItem(-100, -50, 200, 100);
//    ellipseItem->setPos(0,0);

    ellipseItem->setBrush(QBrush(Qt::green));
    graphScene->addItem(ellipseItem);
    ellipseItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsMovable);

    QGraphicsEllipseItem * circularItem = new QGraphicsEllipseItem(-100, 50, 100, 100);
    circularItem->setBrush(QBrush(Qt::cyan));
    graphScene->addItem(circularItem);
    circularItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable
                           | QGraphicsItem::ItemIsMovable);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->labViewSize->setText(QString::asprintf("Graphics View 坐标 宽= %d  高=%d",
                ui->graphicsView->width(), ui->graphicsView->height()));

    QRectF rect = graphScene->sceneRect();
    ui->LabSceneRect->setText(QString::asprintf("Graphics sceneRect(x, y, w, h) = "
                              "%0.f %0.f %0.f %0.f", rect.left(), rect.top(), rect.width(), rect.height()));

}

void MainWindow::on_mouseMove(QPoint point)
{
    labViewCord->setText(QString::asprintf("view 坐标 ： %d, %d",
                             point.x(), point.y()));
    //获取该节点在QGraphicsScene中的位置
    QPointF pointScene = ui->graphicsView->mapToScene(point);

    labSceneCord->setText(QString::asprintf("Item 坐标 ： %.0f, %.0f",
                             pointScene.x(), pointScene.y()));
}

void MainWindow::on_mouseClicked(QPoint point)
{
    QPointF pointScene = ui->graphicsView->mapToScene(point);
    QGraphicsItem * item = NULL;
    item = graphScene->itemAt(pointScene, ui->graphicsView->transform());
    if(item != NULL)
    {
        QPointF pointItem = item->mapFromScene(pointScene);
        labItemCord->setText(QString::asprintf("item 坐标  %0.f , %0.f ",
                                      pointItem.x(), pointItem.y()));

    }
}



