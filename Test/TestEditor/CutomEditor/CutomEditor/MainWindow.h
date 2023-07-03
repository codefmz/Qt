#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QToolBar>
#include "Editor.h"
#include <QAction>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void createActions();
protected:
//    void paintEvent(QPaintEvent* event);
private:
    PlainTextEdit * mplainTextEdit;
    QToolBar * mpToolbar;
    QAction* mpUndoAction;
    QAction* mpRedoAction;
    QAction* mpResetZoomAction;
    QAction* mpZoomInAction;
    QAction* mpZoomOutAction;
};
#endif // WIDGET_H
