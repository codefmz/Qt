#ifndef QWBATTERYPLUGINPLUGIN_H
#define QWBATTERYPLUGINPLUGIN_H

#include "qwbatteryplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace QwBatteryPlugin {
namespace Internal {

class QwBatteryPluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QwBatteryPlugin.json")

public:
    QwBatteryPluginPlugin();
    ~QwBatteryPluginPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    void triggerAction();
};

} // namespace Internal
} // namespace QwBatteryPlugin

#endif // QWBATTERYPLUGINPLUGIN_H
