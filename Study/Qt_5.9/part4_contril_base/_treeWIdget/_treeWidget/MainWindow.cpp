#include "MainWindow.h"
#include "ImageLoader.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QThread>
#include <QtConcurrent>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , traverseIndex(0)
{
    ui->setupUi(this);
    setCentralWidget(ui->scrollArea);
    initTree();

    fileNameLabel = new QLabel;
    ui->statusbar->addWidget(fileNameLabel);
    QPixmapCache::setCacheLimit(1024000);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTree()
{
    ui->treeWidget->clear();
    QIcon            icon(":/icons/15.ico");
    QTreeWidgetItem* item = new QTreeWidgetItem(TOP);
    item->setIcon(COLITEM, icon);
    item->setText(COLITEM, "图片文件");
    item->setText(COLITEMTYPE, "Top");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(COLITEM, Qt::Checked);
    ui->treeWidget->addTopLevelItem(item);
}

void MainWindow::addImageItem(QTreeWidgetItem* parItem, const QString& fileName)
{
    QIcon            icon(":/icons/31.ico");
    QTreeWidgetItem* item = new QTreeWidgetItem(parItem, IMAGE);
    QFileInfo        fileInfo(fileName);
    QString          baseName = fileInfo.fileName();
    item->setIcon(COLITEM, icon);
    item->setText(COLITEM, baseName);
    item->setText(COLITEMTYPE, "IMAGE");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(COLITEM, Qt::Checked);
    item->setData(COLITEM, Qt::UserRole, QVariant(fileName));
    imageItemList << item;
}

void MainWindow::displayImage(const QString& fileName)
{

    if (QPixmapCache::find(fileName, &curPixmap))
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    }
    else
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- QPixmapCache not find key - " << fileName;
        curPixmap.load(fileName);
    }
    on_actHeight_triggered();
}

void MainWindow::changeActStatus(bool isEnable)
{
    ui->actBig->setEnabled(isEnable);
    ui->actSmall->setEnabled(isEnable);
    ui->actRealSize->setEnabled(isEnable);
    ui->actHeight->setEnabled(isEnable);
    ui->actionWidth->setEnabled(isEnable);
}

void MainWindow::addFolderItem(QTreeWidgetItem* parItem, const QString& dir)
{
    QIcon            icon(":/icons/35.ico");
    QTreeWidgetItem* item = new QTreeWidgetItem(parItem, GROUP);
    QDir             aDir(dir);
    QString          dirName = aDir.dirName();
    item->setIcon(COLITEM, icon);
    item->setText(COLITEM, dirName);
    item->setText(COLITEMTYPE, "GROUP");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(COLITEM, Qt::Checked);
    item->setData(COLITEM, Qt::UserRole, QVariant(dir));

    QStringList fileList = aDir.entryList(QStringList("*.jpg"));
    foreach (auto& file, fileList)
    {
        QString absFileName = aDir.absoluteFilePath(file);
        addImageItem(item, absFileName);
        QPixmap pix(absFileName);
        QPixmapCache::insert(absFileName, pix);
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << aDir.absoluteFilePath(file);
    }
}

void MainWindow::on_actView_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

void MainWindow::on_actFloat_triggered(bool checked)
{
    ui->dockWidget->setFloating(checked);
}

void MainWindow::on_actAddFile_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "选择一个或多个文件", "", "Images(*.jpg)");
    if (files.empty())
    {
        return;
    }
    if (ui->treeWidget->selectedItems().size() == 0)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    }
    QTreeWidgetItem *item, *parentItem;
    item = ui->treeWidget->currentItem();
    if (item->type() == IMAGE)
    {
        parentItem = item->parent();
    }
    else
    {
        parentItem = item;
    }
    foreach (auto& i, files)
    {
        addImageItem(parentItem, i);
        QPixmap pix(i);
        QPixmapCache::insert(i, pix);
        //        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- QPixmapCache insert key - " << i ;
    }

    QFuture result = QtConcurrent::run([files]() {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
        foreach (auto& file, files)
        {
            //            QPixmap pix(file);
            //            QPixmapCache::insert(file, pix);
            //            qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- ";
        }
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    });
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
}

void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{
    ui->actView->setChecked(visible);
}

void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{
    ui->actFloat->setChecked(topLevel);
}

void MainWindow::on_treeWidget_itemChanged(QTreeWidgetItem* item, int column)
{
    //    if(item->type() == IMAGE)
    //    {
    //        if(it)
    //    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    switch (item->type())
    {
    case IMAGE:
    {
        QString fileName = item->data(COLITEM, Qt::UserRole).toString();
        fileNameLabel->setText(fileName);
        displayImage(fileName);
        changeActStatus(true);
        ui->actDelItem->setEnabled(true);
        break;
    }
    case GROUP:
    {
        QString dir = item->data(COLITEM, Qt::UserRole).toString();
        fileNameLabel->setText(dir);
        break;
    }
    }
}

void MainWindow::on_actHeight_triggered()
{
    if (curPixmap.isNull())
    {
        return;
    }
    int height = ui->scrollArea->height();
    int realH = curPixmap.height();
    pixRatio = qreal(height) / realH;
    QPixmap pix = curPixmap.scaledToHeight(height - 30);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actionWidth_triggered()
{
    int width = ui->scrollArea->width() - 20;
    int realW = curPixmap.width();
    pixRatio = qreal(width) / realW;
    QPixmap pix = curPixmap.scaledToWidth(width - 30);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actSmall_triggered()
{
    pixRatio *= 0.8;
    QSize   size = curPixmap.size() * pixRatio;
    QPixmap pix = curPixmap.scaled(size);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actBig_triggered()
{
    pixRatio *= 1.2;
    QSize   size = curPixmap.size() * pixRatio;
    QPixmap pix = curPixmap.scaled(size);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actRealSize_triggered()
{
    pixRatio = 1;
    ui->label->setPixmap(curPixmap);
}

void MainWindow::on_actDelItem_triggered()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if (item && item->type() != TOP)
    {
        item->parent()->removeChild(item);
        if (item->type() == IMAGE)
        {
            curPixmap.detach();
            ui->label->clear();
            changeActStatus(false);
            imageItemList.removeOne(item);
        }
        delete item;
    }
}

void MainWindow::on_actTraverse_triggered()
{
    if (timer.isActive())
    {
        timer.stop();
        traverseIndex = 0;
    }
    else
    {
        timer.start(1000);
    }
}

void MainWindow::update()
{
    if (imageItemList.empty() || traverseIndex >= imageItemList.size())
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- end timer";
        timer.stop();
        traverseIndex = 0;
        return;
    }
    QTreeWidgetItem* item = imageItemList.at(traverseIndex++);
    QString          fileName = item->data(COLITEM, Qt::UserRole).toString();
    displayImage(fileName);
    ui->treeWidget->setCurrentItem(item);
}

void MainWindow::on_actAddFold_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    if (dir.isEmpty())
    {
        return;
    }
    if (ui->treeWidget->selectedItems().size() == 0)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    }
    QTreeWidgetItem *item, *parentItem;
    item = ui->treeWidget->currentItem();
    if (item->type() == IMAGE)
    {
        parentItem = item->parent();
    }
    else
    {
        parentItem = item;
    }
    addFolderItem(parentItem, dir);
}

void MainWindow::resizeEvent(QResizeEvent* ev)
{
    on_actHeight_triggered();
}
