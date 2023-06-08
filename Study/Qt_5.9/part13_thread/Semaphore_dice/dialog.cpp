#include "dialog.h"
#include "ui_dialog.h"
QList<int> buffer1;
QList<int> buffer2;
QSemaphore semBuf(2);
QSemaphore fullBuf;
int        curNo = 1;
Dialog::Dialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->btnStartThread, SIGNAL(clicked()), SLOT(startThread()));
    connect(ui->btnStopThread, SIGNAL(clicked()), SLOT(endThread()));
    connect(&threadShow, SIGNAL(newValued(int)), SLOT(slotNewValue(int)), Qt::QueuedConnection);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::closeEvent(QCloseEvent* event)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    if (threadDaq.isRunning())
    {
        threadDaq.stopThread();
        //        threadDaq.terminate();
        threadDaq.wait();
    }
    if (threadSaveFile.isRunning())
    {
        threadSaveFile.stopThread();
        threadSaveFile.wait();
        //         threadSaveFile.terminate();
    }
    if (threadShow.isRunning())
    {
        threadShow.stopThread();
        threadShow.wait();
        //         threadShow.terminate();
    }
    event->accept();
}

void Dialog::startThread()
{
    threadDaq.start();
    //    threadSaveFile.start();
    threadShow.start();
    ui->btnStartThread->setEnabled(false);
    ui->btnStopThread->setEnabled(true);
}

void Dialog::endThread()
{
    threadDaq.stopThread();
    //    threadSaveFile.stopThread();
    threadShow.stopThread();
    ui->btnStartThread->setEnabled(true);
    ui->btnStopThread->setEnabled(false);
}

void Dialog::slotNewValue(int diceValue)
{
    static qint32 seq = 0;
    qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ << " -- ";
    // QDiceThread的newValue()信号的响应槽函数，显示骰子次数和点数
    QString str = QString::asprintf("第 %d 次掷骰子，点数为：%d", ++seq, diceValue);
    ui->plainTextEdit->appendPlainText(str);
    QPixmap pic; //图片显示
    QString filename = QString::asprintf(":/images/d%d.jpg", diceValue);
    pic.load(filename);
    ui->LabPic->setPixmap(pic);
}
