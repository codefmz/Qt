#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTextEdit"
#include "QDebug"
#include "QDockWidget"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DockWindows");
    QTextEdit * te = new QTextEdit(this);
    te->setText(tr("Main Window"));
    te->setAlignment(Qt::AlignCenter);
    setCentralWidget(te);
    //停靠窗口1
    QDockWidget * dock = new QDockWidget(tr("DockWind1"),this);
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QTextEdit * te1 = new QTextEdit();
    te1->setText(tr("Window1, the dock widget can be moved between docks by the user, left and right"));
    dock->setWidget(te1);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    //停靠窗口2
    QDockWidget * dock2 = new QDockWidget(tr("DockWind2"),this);
    dock2->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    QTextEdit * te2 = new QTextEdit();
    te2->setText(tr("Window2, the dock widget can be close and float,  init in the right widgetarea"));
    dock2->setWidget(te2);
    addDockWidget(Qt::RightDockWidgetArea, dock2);


    //停靠窗口3
    QDockWidget * dock3 = new QDockWidget(tr("DockWind3"),this);
    dock3->setFeatures(QDockWidget::AllDockWidgetFeatures);
    QTextEdit * te3 = new QTextEdit();
    te3->setText(tr("Window2, the dock widget can be close and float,  init in the right widgetarea"));
    dock3->setWidget(te3);
    addDockWidget(Qt::RightDockWidgetArea, dock3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

