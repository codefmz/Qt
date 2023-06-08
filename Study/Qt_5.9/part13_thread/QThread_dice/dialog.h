#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <DiceThread.h>
#include <QDebug>
#include <QTimer>
QT_BEGIN_NAMESPACE

#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void closeEvent(QCloseEvent * event);
private:
    Ui::Dialog *ui;
    DiceThread diceThread;
    QTimer mTimer;
    int mSeq;
private slots:
    void on_threadNewValue(int seq, int  diceValue);
    void on_threadAStarted();
    void on_threadAFinished();

    void onTimeOut(); //定时其处理槽函数

    void on_btnStartThread_clicked();
    void on_btnStopThread_clicked();
    void on_btnDiceBegin_clicked();
    void on_btnDiceEnd_clicked();
    void on_btnClear_clicked();
};
#endif // DIALOG_H
