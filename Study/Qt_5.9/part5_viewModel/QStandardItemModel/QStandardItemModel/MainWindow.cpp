#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter);
    theModel = new QStandardItemModel(2, FixedColumnCount, this);
    theSelectModel = new QItemSelectionModel(theModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initModelFromStringList(QStringList &)
{

}

void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{

}
