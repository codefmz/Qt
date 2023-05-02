#include "MainWindow.h"
#include "ui_MainWindow.h"

static int num = 0;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new MdiArea;
    mdiArea->installEventFilter(mdiArea);
    setCentralWidget(mdiArea);
    setWindowState(Qt::WindowMaximized);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabsClosable(true);
//    QList<QTabBar*> tabList = mdiArea->findChildren<QTabBar*>();
//    qDebug() << " tabList size() = " << tabList.size();
//    QTabBar * tabBar = tabList.at(0);

//    tabBar->setExpanding(false);
//    tabBar->setTabsClosable(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actDoc_New_triggered()
{
    QFormDoc * formDoc = new QFormDoc(this);
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(formDoc);
    subWindow->setWindowIcon(QIcon(":/images/images/3.ico"));
    subWindow->setWindowTitle(QString::number(num++));
    formDoc->show();

}


void MainWindow::on_actDoc_Open_triggered()
{
    //打开文件
    //必须先获取当前MDI子窗口，再使用打开文件对话框，否则无法获得活动的子窗口
    bool needNew=false;// 是否需要新建子窗口

    QFormDoc    *formDoc;
    if (mdiArea->subWindowList().count()>0) //如果有打开的主窗口，获取活动窗口
    {
        QMdiSubWindow * mdiSubWindow = mdiArea->activeSubWindow();
        QWidget * widget = mdiSubWindow->widget();
        formDoc = static_cast<QFormDoc*>(widget);
        needNew=formDoc->isFileOpened();//文件已经打开，需要新建窗口
    }
    else
    {
        needNew=true;
    }
    QString curPath = QDir::currentPath();
    QString aFileName = QFileDialog::getOpenFileName(this,tr("打开一个文件"),curPath,QString::fromLocal8Bit("C程序文件(*.h *cpp);;文本文件(*.txt);;所有文件(*.*)"));
    if (aFileName.isEmpty())
        return; //如果未选择文件，退出

    if (needNew) //需要新建子窗口
    {
        formDoc = new QFormDoc(this);//指定父窗口，必须在父窗口为Widget窗口提供一个显示区域
        QMdiSubWindow *subWindow = mdiArea->addSubWindow(formDoc);
        subWindow->setWindowIcon(QIcon(":/images/images/3.ico"));
        subWindow->setWindowTitle(QString::number(num++));
    }

    formDoc->loadFromFile(aFileName); //打开文件
    formDoc->show();
    ui->actCut->setEnabled(true);
    ui->actCopy->setEnabled(true);
    ui->actPaste->setEnabled(true);
    ui->actFont->setEnabled(true);
}


void MainWindow::on_actViewMode_triggered(bool checked)
{
    if(checked)
    {
        mdiArea->setViewMode(QMdiArea::TabbedView);//Tab 多页显示模式
        mdiArea->setTabsClosable(true);
        ui->actCascade->setEnabled(false);
        ui->actTile->setEnabled(false);
    }else
    {
        mdiArea->setViewMode(QMdiArea::SubWindowView); //子窗口模式
        ui->actCascade->setEnabled(true);
        ui->actTile->setEnabled(true);
    }
}


void MainWindow::on_actCascade_triggered()
{
    mdiArea->cascadeSubWindows();
}


void MainWindow::on_actTile_triggered()
{
    mdiArea->tileSubWindows();
}


void MainWindow::on_actCloseALL_triggered()
{
    mdiArea->closeActiveSubWindow();
}






//bool Object::eventFilter(QObject *watched, QEvent *event)
//{

//    if(watched == this->findChildren<QTabBar*>().at(0))
//    {

//            if(event->type() == QEvent::MouseButtonPress)
//            {
//                qDebug() << "MdiArea::eventFilter";
//                event->accept();
//                return true;
//            }
//    }

//     return QObject::eventFilter(watched,event);
//}
