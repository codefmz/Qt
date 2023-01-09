#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QLayout"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget * widget = new QWidget;
    setWindowTitle(tr("StackedWidget"));
    list = new QListWidget(this);
    list->insertItem(0,tr("window1"));
    list->insertItem(1,tr("window2"));
    list->insertItem(2,tr("window3"));

//    label1 = new QLabel(tr("windowsTest1"));
//    label2 = new QLabel(tr("windowsTest2"));
//    label3 = new QLabel(tr("windowsTest3"));

    QTextEdit * edit1 = new QTextEdit;
    edit1->setText(tr("windowsTest1"));

    QTextEdit * edit2 = new QTextEdit;
    edit2->setText(tr("windowsTest2"));

    QTextEdit * edit3 = new QTextEdit;
    edit3->setText(tr("windowsTest3"));


    stack = new QStackedWidget(this);
    stack->addWidget(edit1);
    stack->addWidget(edit2);
    stack->addWidget(edit3);

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(list);
    mainLayout->addWidget(stack,0);
    mainLayout->setStretchFactor(list,1);
    mainLayout->setStretchFactor(stack,3);
    connect(list, &QListWidget::currentRowChanged, stack, &QStackedWidget::setCurrentIndex);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

