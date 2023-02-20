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


    connect(&diceThread,SIGNAL(started()),this,SLOT(on_threadAStarted()));
    connect(&diceThread,SIGNAL(finished()),this,SLOT(on_threadAFinished()));

    connect(&diceThread,SIGNAL(newValued(int,int)),SLOT(on_threadNewValue(int,int)), Qt::BlockingQueuedConnection);

//    connect(&mTimer, SIGNAL(timeout()), SLOT(onTimeOut()));
    QPixmap pic; //图片显示
    QString filename=QString::asprintf(":/images/d%d.jpg",0);
    pic.load(filename);
    ui->LabPic->setPixmap(pic);

    mSeq = 0;
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
    qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ <<" -- ";
    qDebug() << " Dialog::on_threadNewValue ";
    //QDiceThread的newValue()信号的响应槽函数，显示骰子次数和点数
    QString  str = QString::asprintf("第 %d 次掷骰子，点数为：%d",seq,diceValue);
    ui->plainTextEdit->appendPlainText(str);
    QPixmap pic; //图片显示
    QString filename = QString::asprintf(":/images/d%d.jpg",diceValue);
    pic.load(filename);
    ui->LabPic->setPixmap(pic);
}

void Dialog::on_threadAStarted()
{
    qDebug() << " Dialog::on_threadAStarted";
    ui->LabA->setText(tr("thread状态: 启动"));
    ui->btnStartThread->setEnabled(false);
    ui->btnDiceBegin->setEnabled(true);
    ui->btnStopThread->setEnabled(true);
    ui->btnDiceEnd->setEnabled(true);
    ui->btnDiceBegin->setEnabled(true);
}

void Dialog::on_threadAFinished()
{
    qDebug() << " Dialog::on_threadAFinished";
    ui->LabA->setText(tr("thread状态: 停止"));

}

void Dialog::onTimeOut()
{
    qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ <<" -- ";
    int tmpSeq = 0, tmpValue = 0;
    bool valid = diceThread.readValue(&tmpSeq, &tmpValue);
    if(valid && tmpSeq != mSeq)
    {
        on_threadNewValue(tmpSeq, tmpValue);
        tmpSeq = mSeq;
    }
}


void Dialog::on_btnStartThread_clicked()
{
    qDebug() << " Dialog::on_btnStartThread_clicked ";
    diceThread.start();

}


void Dialog::on_btnStopThread_clicked()
{
    diceThread.stopThread();
    diceThread.wait();
    ui->btnStartThread->setEnabled(true);
    ui->btnDiceBegin->setEnabled(false);
    ui->btnStopThread->setEnabled(false);
    ui->btnDiceEnd->setEnabled(false);
    ui->btnDiceBegin->setEnabled(false);
}


void Dialog::on_btnDiceBegin_clicked()
{
    diceThread.diceBegin();
    ui->LabA->setText(tr("thread状态: 运行"));
    ui->btnDiceBegin->setEnabled(false);
    mTimer.start(100);
}


void Dialog::on_btnDiceEnd_clicked()
{
     diceThread.dicePause();
     ui->LabA->setText(tr("thread状态: 暂停"));
     ui->btnDiceBegin->setEnabled(true);
     mTimer.stop();
}


void Dialog::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}

