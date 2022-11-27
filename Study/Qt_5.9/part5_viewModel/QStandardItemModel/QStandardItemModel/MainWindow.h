#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QStandardItemModel"
#include "QTableView"
#include "QPlainTextEdit"
#include "QLabel"

#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
namespace Ui {
class MainWindow;
}
#define FixedColumnCount 6   //文件固定行6列
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel * LabCurFile;  //当前文件
    QLabel * LabCellPos;  //当前单元格行列号
    QLabel * LabCellText;  //当前单元格内容
    QStandardItemModel * theModel;  //数据模型
    QItemSelectionModel * theSelectModel; //选择模型
    void initModelFromStringList(QStringList &); //初始化数据模型
private slots:
    //当前选择单元格发生变化
    void on_currentChanged(const QModelIndex & current, const QModelIndex &previous);
};

#endif // MAINWINDOW_H
