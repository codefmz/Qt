#include "MainWindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mplainTextEdit = new PlainTextEdit(this);

    mpToolbar = new QToolBar;
    mpToolbar->setToolButtonStyle(Qt::ToolButtonTextOnly);

    addToolBar(mpToolbar);
    setCentralWidget(mplainTextEdit);
    resize(1000,600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    mpUndoAction = new QAction(QIcon(":/Resources/icons/undo.svg"), tr("Undo"), this);
    mpUndoAction->setShortcut(QKeySequence::Undo);
    mpUndoAction->setEnabled(false);
    connect(mpUndoAction, SIGNAL(triggered()), SLOT(undo()));
    // redo action
    mpRedoAction = new QAction(QIcon(":/Resources/icons/redo.svg"), tr("重做"), this);
    mpRedoAction->setShortcut(QKeySequence::Redo);
    mpRedoAction->setEnabled(false);
    connect(mpRedoAction, SIGNAL(triggered()), SLOT(redo()));
    // reset zoom
    mpResetZoomAction = new QAction(QIcon(":/Resources/icons/zoomReset.svg"), tr("重制缩放"), this);
    mpResetZoomAction->setStatusTip(tr("重制缩放"));
    mpResetZoomAction->setShortcut(QKeySequence("Ctrl+0"));
    mpResetZoomAction->setEnabled(false);
    connect(mpResetZoomAction, SIGNAL(triggered()), SLOT(resetZoom()));
    // zoom in
    mpZoomInAction = new QAction(QIcon(":/Resources/icons/zoomIn.svg"), tr("缩小"));
    mpZoomInAction->setStatusTip(tr("缩小"));
    mpZoomInAction->setShortcut(QKeySequence("Ctrl++"));
    mpZoomInAction->setEnabled(false);
    connect(mpZoomInAction, SIGNAL(triggered()), SLOT(zoomIn()));
    // zoom out
    mpZoomOutAction = new QAction(QIcon(":/Resources/icons/zoomOut.svg"), tr("放大"), this);
    mpZoomOutAction->setStatusTip(tr("放大"));
    mpZoomOutAction->setShortcut(QKeySequence("Ctrl+-"));
    mpZoomOutAction->setEnabled(false);
    connect(mpZoomOutAction, SIGNAL(triggered()), SLOT(zoomOut()));

    mpToolbar->addAction(mpRedoAction);
    mpToolbar->addAction(mpUndoAction);
    mpToolbar->addSeparator();
    mpToolbar->addAction(mpResetZoomAction);
    mpToolbar->addAction(mpZoomInAction);
    mpToolbar->addAction(mpZoomOutAction);
}

//void Widget::paintEvent(QPaintEvent *event)
//{
//    qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- ";
//    QWidget::paintEvent(event);
//}

