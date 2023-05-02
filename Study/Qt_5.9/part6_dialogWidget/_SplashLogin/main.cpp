#include "qwmainwindow.h"
#include <QApplication>

#include "qdlgLogin.h"
#include <QPixmap>
#include <QSplashScreen>

#include <QTextCodec>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QDlgLogin*   dlgLogin = new QDlgLogin; //创建对话框
    QSplashScreen splash;
    splash.setPixmap(QPixmap(":/images/images/splash2.jpg"));
    splash.showMessage("Loading...", Qt::AlignTop | Qt::AlignCenter,Qt::white);
    splash.show();
    if (dlgLogin->exec() == QDialog::Accepted)
    {

//        dlgLogin->deleteLater();
//        dlgLogin = nullptr;
        QWMainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
