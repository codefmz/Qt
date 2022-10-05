#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setPalette(Qt::white);
    setAutoFillBackground(true);
    ui->setupUi(this);
    widget = new MyWidget;
    widget->setPalette(Qt::white);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

