#include "MainWindow.h"
#include "QSplitter"
#include "QTextEdit"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("ZYSong18030",12); //指定字体
    a.setFont(font);

    QSplitter * splitterMain = new QSplitter(Qt::Horizontal,0);
    QTextEdit * textLeft = new QTextEdit();
    textLeft->setText(QObject::tr("Left Widget"));
    textLeft->setAlignment(Qt::AlignCenter); //设置文本对齐方式  还有 Qt::AlignLeft  Qt::Ali
    splitterMain->addWidget(textLeft);

    QSplitter * splitterRight = new QSplitter(Qt::Vertical);
    splitterRight->setOpaqueResize(false); //设置拖拽时是否显示分割条

    QTextEdit * textUp = new QTextEdit();
    textUp->setText(QObject::tr("Top Widget"));
    textUp->setAlignment(Qt::AlignCenter);

    QTextEdit * textBottom = new QTextEdit();
    textBottom->setText(QObject::tr("Bottom Widget"));
    textBottom->setAlignment(Qt::AlignCenter);

    splitterRight->addWidget(textUp);
    splitterRight->addWidget(textBottom);
    splitterMain->addWidget(splitterRight);

    splitterMain->setStretchFactor(1,1); //设定可伸缩控件， 第一个参数指定设置的控件序号，序号按照插入的先后顺序， 第二个参数大于0，表示控件可以收缩
    splitterMain->setWindowTitle(QObject::tr("Splitter"));
    splitterMain->show();


    //分割窗口


//    MainWindow w;
//    w.show();
    return a.exec();
}
