#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    theModel = new QStringListModel(this);
    strList = new QStringList;
    *strList << "北京" << "上海" << "广州" << "深圳" << "曹县";
    theModel->setStringList(*strList);
    ui->listView->setModel(theModel);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView ::SelectedClicked);
    //设置不可编辑状态
//    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnIniList_clicked()
{
    theModel->setStringList(*strList);
}


void MainWindow::on_btnListAppend_clicked()
{
    //插入列之前
//    theModel->insertRow(theModel->rowCount());
    theModel->insertRow(0); //只是在插入一个空行，没有任何文字
    QModelIndex index = theModel->index(0,0);
    theModel->setData(index, "new Item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}


void MainWindow::on_btnListDelete_clicked()
{
    theModel->removeRow(ui->listView->currentIndex().row());
}


void MainWindow::on_btnListInsert_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
//    qDebug() << " row " <<index.row();
    theModel->insertRow(index.row());
    theModel->setData(index, "insert Item ", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
//     qDebug() << " row " <<index.row();
}


void MainWindow::on_btnListClear_clicked()
{
    theModel->removeRows(0, theModel->rowCount());
}


void MainWindow::on_btnTextClear_clicked()
{
    ui->plainTextEdit->clear();

}


void MainWindow::on_btnTextImport_clicked()
{
    ui->plainTextEdit->clear();
    QStringList strlist = theModel->stringList();
    foreach(auto str, strlist)
    {
        ui->plainTextEdit->appendPlainText(str);
    }
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    ui->LabInfo->adjustSize();   //自适应大小
    ui->LabInfo->setText(QString::asprintf("当前条目: row = %d, cloumn = %d", index.row(), index.column()));
}

