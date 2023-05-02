#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->graphicsView);
    itemInfo = new QLabel("ItemInfo");
    itemInfo->setMinimumWidth(150);
    scene = new QGraphicsScene(-300, -150, 600, 300);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setCursor(Qt::CrossCursor);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->statusbar->addWidget(itemInfo);
    //    ui->graphicsView->setD

    connect(ui->graphicsView, &GraphicsView::keyPress, this, &MainWindow::on_keyPress);
    connect(ui->graphicsView, &GraphicsView::mouseClicked, this, &MainWindow::on_mousePress);
    connect(ui->graphicsView, &GraphicsView::mouseDoubleClicked, this, &MainWindow::on_mouseDoublePress);

    qsrand(QTime::currentTime().second());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_keyPress(QKeyEvent* event)
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        switch (event->key())
        {
        case Qt::Key_Delete:
        {
            for (int i = 0; i < cnt; i++)
            {
                QGraphicsItem* item = scene->selectedItems().at(0);
                scene->removeItem(item);
            }
        }
        }
    }
}

void MainWindow::on_mousePress(QPoint point)
{
    QPointF        pointScene = ui->graphicsView->mapToScene(point);
    QGraphicsItem* item = NULL;
    item = scene->itemAt(pointScene, ui->graphicsView->transform());
    if (item != NULL)
    {
        itemInfo->setText(item->data(ItemId).toString() + " , " + item->data(ItemDescription).toString());
    }
}

void MainWindow::on_mouseDoublePress(QPoint point)
{
    QPointF        pointScene = ui->graphicsView->mapToScene(point);
    QGraphicsItem* item = NULL;
    item = scene->itemAt(pointScene, ui->graphicsView->transform());
    if (item == NULL)
    {
        return;
    }

    switch (item->type())
    {
    case QGraphicsRectItem::Type:
    {

        QGraphicsRectItem* theItem = NULL;
        theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
        QColor color = theItem->brush().color();
        color = QColorDialog::getColor(color, NULL, "选择颜色");
        theItem->setBrush(QBrush(color));
        break;
    }
    case QGraphicsEllipseItem::Type:
    {

        QGraphicsEllipseItem* theItem = NULL;
        theItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        QColor color = theItem->brush().color();
        color = QColorDialog::getColor(color, NULL, "选择颜色");
        theItem->setBrush(QBrush(color));
        break;
    }
    case QGraphicsPolygonItem::Type:
    {

        QGraphicsPolygonItem* theItem = NULL;
        theItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
        QColor color = theItem->brush().color();
        color = QColorDialog::getColor(color, NULL, "选择颜色");
        theItem->setBrush(QBrush(color));
        break;
    }
    case QGraphicsLineItem::Type:
    {
        QGraphicsLineItem* theItem = NULL;
        theItem = qgraphicsitem_cast<QGraphicsLineItem*>(item);
        QColor color = theItem->pen().color();
        color = QColorDialog::getColor(color, NULL, "选择颜色");
        if (color.isValid())
        {
            theItem->setPen(QPen(color));
        }
        break;
    }
    case QGraphicsTextItem::Type:
    {
        QGraphicsTextItem* theItem = NULL;
        theItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
        QColor color = theItem->defaultTextColor();
        color = QColorDialog::getColor(color, NULL, "选择颜色");
        if (color.isValid())
        {
            theItem->setDefaultTextColor(color);
        }
        break;
    }

    default:
        break;
    }
}

void MainWindow::on_actItem_Rect_triggered()
{
    QGraphicsRectItem* rectItem = new QGraphicsRectItem(-30, -40, 80, 40);
    rectItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    rectItem->setBrush(QBrush(Qt::blue));
    rectItem->setPos(-100 + qrand() % 400, -100 + qrand() % 400);
    rectItem->setData(ItemId, ++seqNum);
    rectItem->setData(ItemDescription, "Rectangle");
    scene->addItem(rectItem);
    scene->clearSelection();
    rectItem->setSelected(true);
}

void MainWindow::on_actItem_Ellipse_triggered()
{
    QGraphicsEllipseItem* ellipseItem = new QGraphicsEllipseItem(-30, -40, 80, 40);
    ellipseItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    ellipseItem->setBrush(QBrush(Qt::yellow));
    ellipseItem->setPos(-100 + qrand() % 400, -100 + qrand() % 400);

    ellipseItem->setData(ItemId, ++seqNum);
    ellipseItem->setData(ItemDescription, "椭圆");
    scene->addItem(ellipseItem);
    scene->clearSelection();
    ellipseItem->setSelected(true);
}

void MainWindow::on_actItem_Circle_triggered()
{
    QGraphicsEllipseItem* circleItem = new QGraphicsEllipseItem(-30, -40, 80, 80);
    circleItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    circleItem->setBrush(QBrush(Qt::green));
    circleItem->setPos(-100 + qrand() % 400, -100 + qrand() % 400);

    circleItem->setData(ItemId, ++seqNum);
    circleItem->setData(ItemDescription, "圆");
    scene->addItem(circleItem);
    scene->clearSelection();
    circleItem->setSelected(true);
}

void MainWindow::on_actItem_Line_triggered()
{
    QGraphicsLineItem* lineItem = new QGraphicsLineItem(-30, -40, 30, 40);
    lineItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    QPen pen(Qt::black);
    pen.setWidth(3);
    lineItem->setPen(pen);
    lineItem->setPos(-100 + qrand() % 400, -100 + qrand() % 400);
    lineItem->setData(ItemId, ++seqNum);
    lineItem->setData(ItemDescription, "直线");
    scene->addItem(lineItem);
    scene->clearSelection();
    lineItem->setSelected(true);
}

void MainWindow::on_actItem_Polygon_triggered()
{
    QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem;
    QPolygonF             points;
    points.append(QPointF(-40, -40));
    points.append(QPointF(40, -40));
    points.append(QPointF(60, 40));
    points.append(QPointF(-60, 40));
    polygonItem->setPolygon(points);
    polygonItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    polygonItem->setBrush(QBrush(Qt::cyan));
    polygonItem->setPos(-100 + qrand() % 400, -100 + qrand() % 400);
    polygonItem->setData(ItemId, ++seqNum);
    polygonItem->setData(ItemDescription, "梯形");
    scene->addItem(polygonItem);
    scene->clearSelection();
    polygonItem->setSelected(true);
}

void MainWindow::on_actItem_Triangle_triggered()
{
    QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem;
    QPolygonF             points;
    points.append(QPointF(0, -40));
    points.append(QPointF(60, 40));
    points.append(QPointF(-60, 40));
    polygonItem->setPolygon(points);
    polygonItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    polygonItem->setBrush(QBrush(Qt::cyan));
    polygonItem->setPos(-100 + qrand() % 400, -100 + qrand() % 400);
    polygonItem->setData(ItemId, ++seqNum);
    polygonItem->setData(ItemDescription, "三角形");
    scene->addItem(polygonItem);
    scene->clearSelection();
    polygonItem->setSelected(true);
}

void MainWindow::on_actItem_Text_triggered()
{
    QString            str = QInputDialog::getText(this, "输入对话框", "文本");
    QGraphicsTextItem* item = new QGraphicsTextItem(str);
    QFont              font;
    font.setPointSize(20);
    item->setFont(font);
    item->setDefaultTextColor(Qt::darkRed);
    item->setPos(-100 + qrand() % 400, -100 + qrand() % 400);
    item->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    item->setData(ItemId, ++seqNum);
    item->setData(ItemDescription, "文本");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actZoomIn_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        ui->graphicsView->scale(1.1, 1.1);
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        item->setScale(0.1 + item->scale());
    }
}

void MainWindow::on_actZoomOut_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        ui->graphicsView->scale(0.9, 0.9);
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        item->setScale(-0.1 + item->scale());
    }
}

void MainWindow::on_actRestore_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        ui->graphicsView->resetTransform();
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        //        item->resetTransform();
        item->setScale(1);
    }
}

void MainWindow::on_actRotateLeft_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        ui->graphicsView->rotate(-45);
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        //        item->resetTransform();
        item->setRotation(item->rotation() - 45);
    }
}

void MainWindow::on_actRotateRight_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        ui->graphicsView->rotate(45);
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        //        item->resetTransform();
        item->setRotation(item->rotation() + 45);
    }
}

void MainWindow::on_actEdit_Front_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {

        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        //        item->resetTransform();
        item->setZValue(item->zValue() + 1);
    }
}

void MainWindow::on_actEdit_Back_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {

        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        //        item->resetTransform();
        item->setZValue(item->zValue() - 1);
    }
}

void MainWindow::on_actGroupBreak_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 0)
    {
        return;
    }
    QGraphicsItemGroup*        group = NULL;
    QList<QGraphicsItemGroup*> list;
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(i);
        if (item->type() == QGraphicsItemGroup ::Type)
        {
            group = qgraphicsitem_cast<QGraphicsItemGroup*>(item);
            list.append(group);
        }
    }
    foreach (auto item, list)
    {
        scene->destroyItemGroup(item);
    }
}

void MainWindow::on_actGroup_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt <= 1)
    {
        return;
    }
    QGraphicsItemGroup* group = new QGraphicsItemGroup;
    for (int i = 0; i < cnt; i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setSelected(false);
        group->addToGroup(item);
    }
    group->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    group->setSelected(true);
    scene->addItem(group);
}
