#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSharedMemory>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QBuffer>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool ReadImage(); //读取图片文件
    bool saveToSharedMemory();//保存到共享内存
    void loadFromSharedMemory();//从共享内存读出
    void clearFields(); //清空共享内存

private slots:
    //写入按下的槽函数
    void on_write_action_triggered(bool checked);
    //读取按下的槽函数
    void on_read_action_triggered(bool checked);
    //清除按下的槽函数
    void on_clear_action_triggered();

private:
    Ui::MainWindow *ui;
    QImage srcImage,dstImage; //操作对象
    QSharedMemory sharedMemory; //共享内存对象
};
#endif // MAINWINDOW_H
