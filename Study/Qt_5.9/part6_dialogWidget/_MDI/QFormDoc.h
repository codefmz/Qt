#ifndef QFORMDOC_H
#define QFORMDOC_H

#include <QWidget>
#include <QPlainTextEdit>

#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
class QFormDoc : public QWidget
{
    Q_OBJECT

private:
    QString mCurrentFile; //当前文件
    bool    mFileOpened=false; //文件已打开
    QPlainTextEdit * plainTextEdit;
public:
    explicit QFormDoc(QWidget *parent = 0);
    ~QFormDoc();

    void    loadFromFile(QString& aFileName); //打开文件
    QString currentFileName();//返回当前文件名
    bool    isFileOpened();//文件已经打开

    void    setEditFont();//设置字体
    void    textCut(); //cut
    void    textCopy(); //copy
    void    textPaste(); //paste

    virtual void showEvent(QShowEvent *showEvent) override;
};

#endif // QFORMDOC_H
