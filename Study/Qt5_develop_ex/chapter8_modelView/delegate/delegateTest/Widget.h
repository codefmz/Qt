#ifndef WIDGET_H
#define WIDGET_H
#include <QTableView>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "DateDelegate.h"
#include "ComboDelegate.h"
#include "SpinboxDelegate.h"
#include <QWidget>
#include <QStandardItemModel>
#include "QGridLayout"
#include "TreeView.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual void paintEvent(QPaintEvent *event);

    void tableModel();

    void varTreeModel();
private:
    QTableView * tableView;

    VarTreeWidget * varTreeWidget;


    QStandardItemModel model;
    DateDelegate dateDelegate{this};
    ComboDelegate comboDelegate{this};
    SpinboxDelegate spinBoxDelegate{this};
};
#endif // WIDGET_H
