#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QTableView>
#include <QListView>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QFileSystemModel>
#include <QSplitter>
#include <QHeaderView>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTreeView * treeView;
    QTableView * tableView;
    QListView * listView;

    QLabel * nameLabel;
    QLabel * sizeLabel;
    QLabel * fileFolderLabel;
    QCheckBox * dirCkBox;
    QLabel * pathLabel;

    QFileSystemModel * systemModel;

public slots:
    void on_treeView_clicked(QModelIndex modelIndex);
};
#endif // WIDGET_H
