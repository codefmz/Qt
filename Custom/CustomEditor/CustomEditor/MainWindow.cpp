#include "MainWindow.h"
#include <QDebug>
#include <QGridLayout>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    mplainTextEdit = new PlainTextEdit(this);
    new TextHighlighter(mplainTextEdit);
    createActions();
    setCentralWidget(mplainTextEdit);
    resize(1000, 600);

}

MainWindow::~MainWindow() {}

MainWindow* MainWindow::instance()
{
    static MainWindow* pMain = new MainWindow;
    return pMain;
}

void MainWindow::createActions()
{
    mpUndoAction = new QAction(QIcon(":/Resources/icons/undo.svg"), tr("Undo"), this);
    mpUndoAction->setShortcut(QKeySequence::Undo);
    mpUndoAction->setEnabled(false);
    connect(mpUndoAction, SIGNAL(triggered()), mplainTextEdit, SLOT(undo()));
    // redo action
    mpRedoAction = new QAction(QIcon(":/Resources/icons/redo.svg"), tr("重做"), this);
    mpRedoAction->setShortcut(QKeySequence::Redo);
    mpRedoAction->setEnabled(false);
    connect(mpRedoAction, SIGNAL(triggered()), mplainTextEdit, SLOT(redo()));

    // reset zoom
    mpResetZoomAction = new QAction(QIcon(":/Resources/icons/zoomReset.svg"), tr("重制缩放"), this);
    mpResetZoomAction->setStatusTip(tr("重制缩放"));
    mpResetZoomAction->setShortcut(QKeySequence("Ctrl+0"));
    connect(mpResetZoomAction, SIGNAL(triggered()), mplainTextEdit, SLOT(resetZoom()));
    // zoom in
    mpZoomInAction = new QAction(QIcon(":/Resources/icons/zoomIn.svg"), tr("缩小"));
    mpZoomInAction->setStatusTip(tr("缩小"));
    mpZoomInAction->setShortcut(QKeySequence("Ctrl++"));
    connect(mpZoomInAction, SIGNAL(triggered()), mplainTextEdit, SLOT(zoomIn()));
    // zoom out
    mpZoomOutAction = new QAction(QIcon(":/Resources/icons/zoomOut.svg"), tr("放大"), this);
    mpZoomOutAction->setStatusTip(tr("放大"));
    mpZoomOutAction->setShortcut(QKeySequence("Ctrl+-"));
    connect(mpZoomOutAction, SIGNAL(triggered()), mplainTextEdit, SLOT(zoomOut()));

    mpToolbar = addToolBar(tr("文本编辑"));
    mpToolbar->addAction(mpUndoAction);
    mpToolbar->addAction(mpRedoAction);
    mpToolbar->addSeparator();
    mpToolbar->addAction(mpResetZoomAction);
    mpToolbar->addAction(mpZoomInAction);
    mpToolbar->addAction(mpZoomOutAction);

    connect(mplainTextEdit->document(), SIGNAL(redoAvailable(bool)),this, SLOT(handleCanUndoChanged(bool)));
    connect(mplainTextEdit->document(), SIGNAL(undoAvailable(bool)), this, SLOT(handleCanUndoChanged(bool)));
}

void MainWindow::handleCanUndoChanged(bool enable)
{
    Q_UNUSED(enable);
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    mpUndoAction->setEnabled(mplainTextEdit->document()->isUndoAvailable());
    mpRedoAction->setEnabled(mplainTextEdit->document()->isRedoAvailable());
}
