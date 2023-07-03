#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Plot/qcustomplot.h"
#include "QAction"
#include "QMenu"

#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDemo1(); //x^2  x^3
    void setDemo2();
    void setDemo3();
    void setDemo4();
    void setupScatterPixmapDemo();
    void test();
    void paintEvent(QPaintEvent *) override;
    void setupLineStyleDemo();
    void setupDateDemo();
    void setupTextureBrushDemo();
    void setupMultiAxisDemo();
    void realTimer();

    Q_SLOT void mousePressSlot(QMouseEvent *event);
    Q_SLOT void selectionChanged();
    Q_SLOT void slotStop();
    Q_SLOT void slotContinue();
    Q_SLOT void slotShow();
    Q_SLOT void slotHide();
private:
    Ui::MainWindow *ui;

    QCustomPlot * customPlot;
    QStatusBar * statusBar;
    QTimer  timer;
    QAction * hideAction;
    QAction * showAction;
    QAction * stopAction;
    QAction * continueAction;
    QMenu menu;
    Q_SLOT void realtimeDataSlot();

};
#endif // MAINWINDOW_H
