#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "QComboBox"
#include "QTextEdit"
#include "QLineEdit"
#include "QLayout"
#include "QPushButton"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //左侧
    QLabel * UserNameLabel;
    QLabel * NameLabel;
    QLabel * SexLabel;
    QLabel * DepartmentLabel;
    QLabel * AgeLabel;
    QLabel * OtherLabel;
    QLineEdit * UserNameLineEdit;
    QLineEdit * NameLineEdit;
    QComboBox * SexComboBox;
    QTextEdit * DepartmentTextEdit;
    QLineEdit * AgeLineEdit;
    QGridLayout * LeftLayout;
    //右侧
    QLabel * HeadLabel;
    QLabel * HeadIconLabel;
    QPushButton * UpdateHeadBtn;
    QHBoxLayout * TopRightLayout;

    QLabel * IntroductionLabel;
    QTextEdit * IntroductionTextEdit;
    QVBoxLayout * RightLayout;

    //底部
    QPushButton * OkBtn;
    QPushButton * CancelBtn;
    QHBoxLayout * ButtomLayout;

};
#endif // MAINWINDOW_H
