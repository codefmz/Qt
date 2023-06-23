#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QSettings>

QTranslator* trans = NULL;
QString readSetting();
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    trans = new QTranslator;
    QString curLang = readSetting(); //读取语言设置
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << curLang;
    if (curLang == "EN")
    {
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- Load en";
        trans->load(":/qm/MultiLanguage_en.qm");
    }
    else
    {
        trans->load(":/qm/MultiLanguage_cn.qm");
    }
    app.installTranslator(trans);

    MainWindow w;
    w.show();
    return app.exec();
}

QString readSetting()
{                                                                     //从注册表读取上次设置的语言
    QString   organization = "WWB-Qt";                                //用于注册表，
    QString   appName = "MultiLanguage";                                   // HKEY_CURRENT_USER/WWB-Qt/amp13_1
    QSettings settings(organization, appName);                        //创建
    QString   Language = settings.value("Language", "EN").toString(); //读取 saved键的值
    return Language;
}
