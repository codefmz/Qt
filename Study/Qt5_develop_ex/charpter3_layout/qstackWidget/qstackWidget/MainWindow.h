#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTextEdit"
#include "QDebug"
#include "QDockWidget"
#include "QListWidget"
#include "QLabel"
#include "QStackedWidget"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QListWidget * list;
    QStackedWidget * stack;
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
};
#endif // MAINWINDOW_H
