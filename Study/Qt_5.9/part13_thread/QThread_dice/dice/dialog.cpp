#include "dialog.h"
#include "ui_dialog.h"

#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    DiceThread diceThread;


    connect(&diceThread, SIGNAL(started()), this, SLOT(on_threadAStarted()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::closeEvent(QCloseEvent *event)
{
    qDebug() << " Dialog::closeEvent ";
    if(diceThread.isRunning())
    {
        diceThread.stopThread();
        diceThread.wait();
    }
    QDialog::closeEvent(event);

}

void Dialog::on_threadNewValue(int seq, int diceValue)
{

}

void Dialog::on_threadAStarted()
{
    qDebug() << " Dialog::on_threadAStarted";
    ui->LabA->setText(tr("thread状态: 启动"));
    ui->btnStartThread->setEnabled(false);
    ui->btnDiceBegin->setEnabled(true);
    ui->btnDiceEnd->setEnabled(true);
    ui->btnDiceBegin->setEnabled(true);
}

void Dialog::on_threadAFinished()
{
    qDebug() << " Dialog::on_threadAFinished";
    ui->LabA->setText(tr("thread状态: 停止"));
    ui->btnStartThread->setEnabled(false);
    ui->btnDiceBegin->setEnabled(true);
    ui->btnDiceEnd->setEnabled(true);
    ui->btnDiceBegin->setEnabled(true);
}


void Dialog::on_btnStartThread_clicked()
{
    qDebug() << " Dialog::on_btnStartThread_clicked ";
    diceThread.start();
}

