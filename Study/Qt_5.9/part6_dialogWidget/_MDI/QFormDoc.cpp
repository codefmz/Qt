#include    "QFormDoc.h"
#include    <QVBoxLayout>
#include    <QFile>
#include    <QTextStream>
#include    <QFontDialog>
#include    <QFileInfo>
#include    <QMessageBox>
#include    <QDebug>
QFormDoc::QFormDoc(QWidget *parent) :QWidget(parent)
{

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
    plainTextEdit = new QPlainTextEdit;
    QVBoxLayout * vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(plainTextEdit);
    vboxLayout->setMargin(2);
    setLayout(vboxLayout);

}

QFormDoc::~QFormDoc()
{
}

void QFormDoc::loadFromFile(QString &aFileName)
{//打开文件
    QFile aFile(aFileName);  //以文件方式读出
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        QTextStream aStream(&aFile); //用文本流读取文件
        plainTextEdit->clear();//清空
        plainTextEdit->setPlainText(aStream.readAll()); //读取文本文件
        aFile.close();//关闭文件

        mCurrentFile = aFileName;//保存当前文件名
        QFileInfo  fileInfo(aFileName); //文件信息
        QString str=fileInfo.fileName(); //去除路径后的文件名
        this->setWindowTitle(str);
        mFileOpened=true;
    }
}

QString QFormDoc::currentFileName()
{
    return  mCurrentFile;
}

bool QFormDoc::isFileOpened()
{ //文件是否已打开
    return mFileOpened;
}

void QFormDoc::setEditFont()
{
    QFont   font;
    font=plainTextEdit->font();

    bool    ok;
    font=QFontDialog::getFont(&ok,font);
    plainTextEdit->setFont(font);
}

void QFormDoc::textCut()
{
    plainTextEdit->cut();
}

void QFormDoc::textCopy()
{
    plainTextEdit->copy();
}

void QFormDoc::textPaste()
{
    plainTextEdit->paste();
}

void QFormDoc::showEvent(QShowEvent *showEvent)
{
    qDebug() << "QFormDoc::showEvent";
    showEvent->accept();
}
