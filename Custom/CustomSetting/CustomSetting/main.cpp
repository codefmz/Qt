#include "OptionDialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("Arial", 12); // 使用Arial字体，大小为12像素
    a.setFont(font);
    OptionDialog::instance()->show();
    return a.exec();
}
