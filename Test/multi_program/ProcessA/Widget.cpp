#include "Widget.h"
#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QGridLayout>
#include <QIntValidator>
#include <QMessageBox>
#include <QSystemSemaphore>
#include <QtConcurrent/QtConcurrent>
struct CustomData
{
    QString processName;
    int     command;

    // 添加其他自定义数据成员
};
Widget::Widget(QWidget* parent)
    : QWidget { parent }
{
    setUpWindow();
    init();
}

void Widget::setUpWindow()
{
    commandLabel = new QLabel("指令(1 - 6): ");
    commandLineEdit = new QLineEdit;
    commandLineEdit->setValidator(new QIntValidator(1, 6, commandLineEdit));

    QObject::connect(commandLineEdit, &QLineEdit::textChanged, [=](const QString& text) {
        // 在文本发生变化时，根据验证结果设置文本颜色
        if (commandLineEdit->hasAcceptableInput())
        {
            commandLineEdit->setStyleSheet("color: black;");
        }
        else
        {
            commandLineEdit->setStyleSheet("color: red;");
        }
    });

    sendBtn = new QPushButton("发送");
    connect(sendBtn, SIGNAL(clicked()), SLOT(slotSend()));

    picLabel = new QLabel;
    picLabel->setStyleSheet("QLabel { border: 1px solid black; padding: 5px; }");
    QPixmap pixmap(":/default.png"); // 替换为你的图片路径
    picLabel->setPixmap(pixmap.scaled(picLabel->width(), picLabel->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(commandLabel, 0, 0, 1, 1);
    gridLayout->addWidget(commandLineEdit, 0, 1, 1, 3);
    gridLayout->addWidget(sendBtn, 0, 4, 1, 1);
    gridLayout->addWidget(picLabel, 1, 0, -1, -1);

    gridLayout->setRowStretch(0, 0);
    gridLayout->setRowStretch(1, 1);

    setLayout(gridLayout);

    resize(400, 400);
}

void Widget::init()
{
    keyA2B = "SharedMemory_A_B";
    sharedMemoryA.setKey(keyA2B);
    keyB2A = "SharedMemory_B_A";
    sharedMemoryB.setKey(keyB2A);

    process = new QProcess(this);

    exeFilePath = QApplication::applicationDirPath() + "/ProcessB_boxed.exe";

    isProcessRunning = false;
}

int Widget::startProcess(const QString& exeFilePath)
{
    if(!process)
    {
        process = new QProcess;
    }
    process->start(exeFilePath, QStringList());
    process->waitForStarted();
    return 1;
}

void Widget::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void Widget::slotSend()
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- process->state() = " << process->state();
    QFuture<int> future;
    bool         isStart = false;
    if (!isProcessRunning)
    {
        future = QtConcurrent::run(this, &Widget::startProcess, exeFilePath);
        isStart = true;
        isProcessRunning = true;
    }
    if (!commandLineEdit->hasAcceptableInput() || commandLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "thie command is empty!");
        return;
    }
    sharedMemoryA.detach();
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    CustomData  customData;
    customData.processName = "processWrite";
    customData.command = commandLineEdit->text().toInt();
    out << customData.processName;
    out << customData.command;
    int size = buffer.size();
    if (!sharedMemoryA.create(size))
    {
        QMessageBox::critical(this, "Error", "Failed to create  shared memory！");
        return;
    }
    // 将自定义数据复制到共享内存
    char*       to = (char*)sharedMemoryA.data();
    const char* from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemoryA.size(), size));
    buffer.close();
    if (isStart)
    {
        future.result();
    }
    // 释放一个信号量，通知进程2
    QSystemSemaphore semaphore("MySemaphore_A_B", 0, QSystemSemaphore::Open);
    semaphore.release();

    if (customData.command == 0)
    {
        isProcessRunning = false;
        process->deleteLater();
        process = nullptr;
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- process";
        return;
    }
    sharedMemoryB.detach();
    QSystemSemaphore semaphore2("MySemaphore_B_A", 0, QSystemSemaphore::Open);
    semaphore2.acquire();
    sharedMemoryA.detach();
    //读取进程返回的结果
    if (!sharedMemoryB.attach())
    {
        QMessageBox::critical(this, "Error", sharedMemoryB.errorString());
        return;
    }
    QDataStream in(&buffer);
    buffer.setData((char*)sharedMemoryB.constData(), sharedMemoryB.size());
    buffer.open(QBuffer::ReadOnly);
    QPixmap pixmap;
    in >> pixmap;
    buffer.close();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << pixmap;
    picLabel->setPixmap(pixmap.scaled(picLabel->width(), picLabel->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    sharedMemoryB.detach();
}

void Widget::slotProcessStarted()
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- Widget::slotProcessStarted";
}

void Widget::readProcessStandardOutput()
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- StandardOutput = " << QString(process->readAllStandardOutput());
}

void Widget::readProcessStandardError()
{
//    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- StandardError = " << QString(process->readAllStandardError());
}

void Widget::processError(QProcess::ProcessError error)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- StandardError = " << QString(process->readAllStandardError());
}

void Widget::processFinished(int num, QProcess::ExitStatus status)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- num = " << num << "  status = " << status;
}
