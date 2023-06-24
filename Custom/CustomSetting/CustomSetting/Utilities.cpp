#include "Utilities.h"

QSettings *Utilities::getApplicationSettings()
{
    static bool       init = false;
    static QSettings* pSettings;
    if (!init)
    {
        init = true;
        //用户级别(windows: )
        pSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "QtCustom", "CustomSetting");
        pSettings->setIniCodec(QString("UTF-8").toUtf8().constData());
    }
    return pSettings;
}
