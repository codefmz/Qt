#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
#include "QStringListModel"

#include "QDebug"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnIniList_clicked();

    void on_btnListAppend_clicked();

    void on_btnListDelete_clicked();

    void on_btnListInsert_clicked();

    void on_btnListClear_clicked();

    void on_btnTextClear_clicked();

    void on_btnTextImport_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStringListModel * theModel;
    QStringList * strList;
};
#endif // MAINWINDOW_H
