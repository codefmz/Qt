#include "TetPlugin.h"
#include "TetPluginPlugin.h"

#include <QtPlugin>

TetPluginPlugin::TetPluginPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void TetPluginPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool TetPluginPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TetPluginPlugin::createWidget(QWidget *parent)
{
    return new TetPlugin(parent);
}

QString TetPluginPlugin::name() const
{
    return QLatin1String("TetPlugin");
}

QString TetPluginPlugin::group() const
{
    return QLatin1String("");
}

QIcon TetPluginPlugin::icon() const
{
    return QIcon();
}

QString TetPluginPlugin::toolTip() const
{
    return QLatin1String("");
}

QString TetPluginPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool TetPluginPlugin::isContainer() const
{
    return false;
}

QString TetPluginPlugin::domXml() const
{
    return QLatin1String("<widget class=\"TetPlugin\" name=\"tetPlugin\">\n</widget>\n");
}

QString TetPluginPlugin::includeFile() const
{
    return QLatin1String("TetPlugin.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tetpluginplugin, TetPluginPlugin)
#endif // QT_VERSION < 0x050000
