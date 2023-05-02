#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->splitter);

    //初始化model
    theModel = new QStandardItemModel(2, FixedColumnCount, this);
    theSelectModel = new QItemSelectionModel(theModel);
    connect(theSelectModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(on_currentChanged(QModelIndex, QModelIndex)));

    //初始化tableView
    ui->tableView->setModel(theModel);
    ui->tableView->setSelectionModel(theSelectModel);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection); //允许多选，shift crtl 快捷键功能支持
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //初始化状态栏label
    LabCellPos = new QLabel;
    LabCellText = new QLabel;
    LabCurFile = new QLabel;
    ui->statusbar->addPermanentWidget(LabCellPos, 0);
    ui->statusbar->addWidget(LabCurFile);
    ui->statusbar->addPermanentWidget(LabCellText, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initModelFromStringList(const QStringList& fileList)
{
    int rowCount = fileList.count();
    theModel->setRowCount(rowCount - 1);
    QString     header = fileList.at(0);
    QStringList headerList = header.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    theModel->setHorizontalHeaderLabels(headerList);
    foreach (auto it, headerList)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << it;
    }
    QStandardItem* aItem;
    QStringList    tmpList;
    int            j;
    for (int i = 1; i < rowCount; i++)
    {
        QString aLineText = fileList.at(i);
        tmpList = aLineText.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        for (j = 0; j < FixedColumnCount - 1; j++)
        {
            aItem = new QStandardItem(tmpList.at(j));
            theModel->setItem(i - 1, j, aItem);
        }
        aItem = new QStandardItem(headerList.at(j));
        aItem->setCheckable(true);
        if (tmpList.at(j) == "0")
        {
            aItem->setCheckState(Qt::Unchecked);
        }
        else
        {
            aItem->setCheckState(Qt::Checked);
        }
        theModel->setItem(i - 1, j, aItem);
    }
}

void MainWindow::on_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    if (current.isValid())
    {
        LabCellPos->setText(QString::asprintf("当前单元格：%d行， %d列", current.row(), current.column()));
        QStandardItem* curItem = theModel->itemFromIndex(current);
        this->LabCellText->setText(curItem->text());
        QFont font = curItem->font();
        ui->actFontBold->setChecked(font.bold());
    }
}

void MainWindow::on_actOpen_triggered()
{
    //    QString dirPath = QCoreApplication::applicationDirPath();
    QString dirPath = QDir::currentPath();
    QString aFileName = QFileDialog::getOpenFileName(this, "打开一个文件", dirPath, "数据文件(*.txt)");
    if (aFileName.isEmpty())
    {
        return;
    }
    QStringList fFileList;
    QFile       file(aFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        ui->plainTextEdit->clear();
        while (!stream.atEnd())
        {
            QString str = stream.readLine();
            ui->plainTextEdit->appendPlainText(str);
            fFileList << str;
        }
        file.close();
        this->LabCurFile->setText("当前文件" + aFileName);
        initModelFromStringList(fFileList);
    }
}

void MainWindow::on_actAppend_triggered()
{

    QList<QStandardItem*> aItemList;
    QStandardItem*        aItem;
    for (int i = 0; i < FixedColumnCount - 1; i++)
    {
        aItem = new QStandardItem("0");
        aItemList << aItem;
    }
    QString str = theModel->headerData(theModel->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    aItem = new QStandardItem(str);
    aItem->setCheckable(true);
    aItemList << aItem;
    theModel->insertRow(theModel->rowCount(), aItemList); //插入一列
    QModelIndex index = theModel->index(theModel->rowCount() - 1, 0);
    theSelectModel->clearSelection();
    theSelectModel->setCurrentIndex(index, QItemSelectionModel::Select);
}

void MainWindow::on_actDelete_triggered()
{
    QModelIndex curIndex = theSelectModel->currentIndex();
    if (curIndex.row() == theModel->rowCount() - 1)
    {
        theModel->removeRow(curIndex.row());
    }
    else
    {
        //删除之后依然选择原来的索引
        theModel->removeRow(curIndex.row());
        theSelectModel->setCurrentIndex(curIndex, QItemSelectionModel::Select);
    }
}

void MainWindow::on_actAlignLeft_triggered()
{
    if (!theSelectModel->hasSelection())
    {
        return;
    }
    QModelIndexList selectedIndexList = theSelectModel->selectedIndexes();
    foreach (auto index, selectedIndexList)
    {
        QStandardItem* item = theModel->itemFromIndex(index);
        item->setTextAlignment(Qt::AlignLeft); //设置文本对齐方式
    }
}

void MainWindow::on_actAlignCenter_triggered()
{
    if (!theSelectModel->hasSelection())
    {
        return;
    }
    QModelIndexList selectedIndexList = theSelectModel->selectedIndexes();
    foreach (auto index, selectedIndexList)
    {
        QStandardItem* item = theModel->itemFromIndex(index);
        item->setTextAlignment(Qt::AlignCenter); //设置文本对齐方式
    }
}

void MainWindow::on_actAlignRight_triggered()
{
    if (!theSelectModel->hasSelection())
    {
        return;
    }
    QModelIndexList selectedIndexList = theSelectModel->selectedIndexes();
    foreach (auto index, selectedIndexList)
    {
        QStandardItem* item = theModel->itemFromIndex(index);
        item->setTextAlignment(Qt::AlignRight); //设置文本对齐方式
    }
}

void MainWindow::on_actFontBold_triggered()
{
    if (!theSelectModel->hasSelection())
    {
        return;
    }
    QModelIndexList selectedIndexList = theSelectModel->selectedIndexes();
    foreach (auto index, selectedIndexList)
    {
        QStandardItem* item = theModel->itemFromIndex(index);
        QFont          font = item->font();
        font.setBold(true);
        item->setFont(font);
    }
}

void MainWindow::on_actSave_triggered()
{
    QString curPath = QApplication::applicationDirPath();
    QString aFileName = QFileDialog::getSaveFileName(this, "选择一个文件夹", curPath, "数据文件(*.txt)");
    if (aFileName.isEmpty())
    {
        return;
    }
    QFile file(aFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return;
    }
    QTextStream    textStream(&file);
    QStandardItem* aItem;
    int            i;
    QString        str;
    ui->plainTextEdit->clear();
    //获取表头数据
    for (i = 0; i < theModel->columnCount(); i++)
    {
        aItem = theModel->horizontalHeaderItem(i);
        str = str + aItem->text() + "\t\t";
    }
    textStream << str + "\n";
    ui->plainTextEdit->appendPlainText(str);
    //获取内容
    for (i = 0; i < theModel->rowCount(); i++)
    {
        str = "";
        int j = 0;
        for (j = 0; j < theModel->columnCount() - 1; j++)
        {
            aItem = theModel->item(i, j);
            str = str + aItem->text() + QString::asprintf("\t\t");
        }
        aItem = theModel->item(i, j);
        if (aItem->checkState() == Qt::Checked)
        {
            str += "1";
        }
        else
        {
            str += "0";
        }
        ui->plainTextEdit->appendPlainText(str);
        textStream << str << "\n";
    }
}

void MainWindow::on_actModelData_triggered()
{
    qDebug() << "MainWindow::on_actModelData_triggered";
    ui->plainTextEdit->clear();
    //获取表头数据
    QStandardItem* aItem;
    int            i;
    QString        str;
    ui->plainTextEdit->clear();
    //获取表头数据
    for (i = 0; i < theModel->columnCount(); i++)
    {
        aItem = theModel->horizontalHeaderItem(i);
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- " << aItem->text();
        str = str + aItem->text().leftJustified(20, ' ');
    }
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- str = " << str;
    ui->plainTextEdit->appendPlainText(str);
    //获取内容
    for (i = 0; i < theModel->rowCount(); i++)
    {
        str = "";
        int j = 0;
        for (j = 0; j < theModel->columnCount() - 1; j++)
        {
            aItem = theModel->item(i, j);
            str = str + aItem->text().leftJustified(20, ' ');
        }
        aItem = theModel->item(i, j);
        if (aItem->checkState() == Qt::Checked)
        {
            str += QString("1").leftJustified(20, ' ');

        }
        else
        {
            str +=  QString("0").leftJustified(20, ' ');
        }
        ui->plainTextEdit->appendPlainText(str);
    }
}
