#include "mainwindow.h"
#include "ui_mainwindow.h"


//构造函数
//设置了主窗口的标题和图标
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromLocal8Bit("QT共享内存Demo"));
    setWindowIcon(QIcon(":/zhouxuan.ico"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//将图像读取到QImage::srcImage中并显示在src_label窗口
bool MainWindow::ReadImage()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
                                              QString::fromLocal8Bit("选择图像"),
                                              "",
                                              tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(filename.isEmpty())
    {
         statusBar()->showMessage(QString::fromLocal8Bit("读取图片失败."));
         return false;
    }
    else
    {
        if(! (srcImage.load(filename) ) ) //加载图像
        {
            statusBar()->showMessage(QString::fromLocal8Bit("读取图片失败."));
            return false;
        }
    }

    return true;
}

//将读取到的图片文件写入到共享内存
bool MainWindow::saveToSharedMemory()
{

    // 创建共享内存段
//    QString qstrKey = "zhouxuan";
//    sharedMemory.setKey(qstrKey);
//    sharedMemory.setNativeKey(qstrKey);
    QString qstrKey = ui->key_lineEdit->text();
    if(qstrKey.isEmpty())
    {
        statusBar()->showMessage(QString::fromLocal8Bit("KEY值不能为空."));
        return false;
    }
    else
    {
        sharedMemory.setKey(qstrKey);
    }

    //如果sharedMemory对象已经链接该内存，则解除,后续create()将会自动attach
    if (sharedMemory.isAttached())
       if (!sharedMemory.detach())
       {
           statusBar()->showMessage(QString::fromLocal8Bit("共享内存被关联锁定.")+sharedMemory.errorString());
           return false;
       }

    //将图片缓存到QBuffer中
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out( &buffer );
    out<<srcImage;
    int size = buffer.size();

    if (!sharedMemory.create(size))
    {
        statusBar()->showMessage(tr("Unable to create shared memory segment.")+sharedMemory.errorString());
        return false;
    }


    sharedMemory.lock();
    char *to = (char *) sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory.size(), size));
    sharedMemory.unlock();

    statusBar()->showMessage(QString::fromLocal8Bit("图片成功写入共享内存."));

    return true;
}

//从共享内存读出文件并显示
void MainWindow::loadFromSharedMemory()
{

//    QString qstrKey = QString("zhouxuan");
//    sharedMemory.setKey(qstrKey);
//    sharedMemory.setNativeKey(qstrKey);
    QString qstrKey = ui->key_lineEdit->text();
    if(qstrKey.isEmpty())
    {
        statusBar()->showMessage(QString::fromLocal8Bit("KEY不能为空"));
        return;
    }
    else
    {
        sharedMemory.setKey(qstrKey);
    }

//    if(!sharedMemory.isAttached())
//    {
//        qDebug()<<"2:is not Attached";
//    }

    //先让sharedMemory对象attch()到共享内存
    if(sharedMemory.isAttached()==false)//该行可保证单个程序自身可写可读
    {
        if (sharedMemory.attach(QSharedMemory::ReadOnly)==false)
         {
            statusBar()->showMessage(tr("Unable to attach the SharedMemory.  ")+sharedMemory.errorString());
            return;
         }
    }

    QBuffer buffer;
    QDataStream in(&buffer);

    sharedMemory.lock();
    buffer.setData((char *)sharedMemory.constData(), sharedMemory.size());
    buffer.open( QBuffer::ReadOnly);
    in >> dstImage;
    sharedMemory.unlock();

    sharedMemory.detach(); //该行断掉了本进程与共享内存的联系。
    //显示读取图
    ui->dst_label->setPixmap(QPixmap::fromImage(dstImage).scaledToHeight(ui->src_frame->height()));

//    statusBar()->showMessage(QString::fromLocal8Bit("读取共享内存成功."));
}

//清除操作
void MainWindow::clearFields()
{
    ui->src_label->clear();
    ui->dst_label->clear();
    statusBar()->showMessage(QString::fromLocal8Bit("清除."));
}

//槽函数
void MainWindow::on_write_action_triggered(bool checked)
{
    if(ReadImage())
        if(saveToSharedMemory())
            //显示读取图
            ui->src_label->setPixmap(QPixmap::fromImage(srcImage).scaledToHeight(ui->src_frame->height()));
}

void MainWindow::on_read_action_triggered(bool checked)
{
    loadFromSharedMemory();
}

void MainWindow::on_clear_action_triggered()
{
    clearFields();
}
