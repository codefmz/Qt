#include "Widget.h"
#include "QHeaderView"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

//        tableModel();
    varTreeModel();
    this->close();
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
//    qDebug() << " evetn";
//    QPainter painter(this);  //this 指的是绘图设备
//    QPolygonF starPolygon;
//    QPolygonF diamondPolygon;
//    starPolygon << 100 * QPointF(1.0, 0.5);
//    for (int i = 1; i < 5; ++i)
//    {
//        starPolygon <<100 * QPointF(0.5 + 0.5 * std::cos(0.8 * i * 3.14),
//                                    0.5 + 0.5 * std::sin(0.8 * i * 3.14));
//    }
//    diamondPolygon << 100*QPointF(0.4, 0.5) << 100* QPointF(0.5, 0.4)
//                   << 100*QPointF(0.6, 0.5) << 100 * QPointF(0.5, 0.6)
//                   << 100 * QPointF(0.4, 0.5);

//    painter.save();

//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setPen(Qt::SolidLine);
//    painter.translate(100, 100);

//    for (int i = 0; i < 5; ++i) {
//        if (i < 3){
//            painter.drawPolygon(starPolygon, Qt::WindingFill);
//        }
//        else{
//            painter.drawPolygon(diamondPolygon, Qt::WindingFill);
//        }
//        painter.translate(100.0, 0);
//    }
    //    painter.restore();
}

void Widget::tableModel()
{

    model.setColumnCount(4);
    model.setRowCount(4);
    tableView = new QTableView;
    //表头qss设置
    QString strHeaderQss;
    strHeaderQss = "QHeaderView::section { background:green; color:white;min-height:3em;}";
    tableView->setStyleSheet(strHeaderQss);

      //表头设置最后一列数据填充整个控件
    tableView->horizontalHeader()->setStretchLastSection(true);

      //表头内容均分填充整个控件
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setItemDelegateForColumn(1, &dateDelegate);
    tableView->setItemDelegateForColumn(2, &comboDelegate);
    tableView->setItemDelegateForColumn(3, &spinBoxDelegate);

    model.setHeaderData(0,Qt::Horizontal,QObject::tr("Name"));
    model.setHeaderData(1,Qt::Horizontal,QObject::tr("Birthday"));
    model.setHeaderData(2,Qt::Horizontal,QObject::tr("Job"));
    model.setHeaderData(3,Qt::Horizontal,QObject::tr("Income"));

    QFile file(":/test.txt");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " open file ";
        QTextStream stream(&file);
        QString line;
        model.removeRows(0, model.rowCount(QModelIndex()), QModelIndex());
        int row = 0;
        do{
            line = stream.readLine();
            if(!line.isEmpty())
            {
                qDebug() << " line = " << line;
                model.insertRows(row,1, QModelIndex());
                QStringList pieces = line.split(",", QString::SkipEmptyParts);
                model.setData(model.index(row, 0, QModelIndex()), pieces.value(0));
                model.setData(model.index(row, 1, QModelIndex()), pieces.value(1));
                model.setData(model.index(row, 2, QModelIndex()), pieces.value(2));
                model.setData(model.index(row, 3, QModelIndex()), pieces.value(3));
                row++;
            }else
            {
                break;
            }
        }while(true);
        file.close();
    }else
    {
        qDebug() << " cann't open file ";
    }

//    tableView->setWindowTitle(QObject::tr("Delegate"));
    tableView->setModel(&model);
//    tableView->show();
    QGridLayout * gridLayout = new QGridLayout;
    gridLayout->addWidget(tableView, 1,0);
    this->setLayout(gridLayout);
}

void Widget::varTreeModel()
{
    qDebug().nospace()<<__FILE__<<"("<<__LINE__<<")"<<__FUNCTION__<<"hhhhh";
    varTreeWidget = new VarTreeWidget;

    QVector<QVariant>  varDataVct;
    varDataVct << "T"<<"t"<<"t"<<"t"<<"t"<<"t"<<"Test";

    varTreeWidget->varTreeModel->insertVarTreeItems(varDataVct);

    varTreeWidget->show();
}

