#ifndef QUICK_NETWORK_PLUGIN_H
#define QUICK_NETWORK_PLUGIN_H

#include <QQmlExtensionPlugin>

class QuicknetworkPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA (IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QUICK_NETWORK_PLUGIN_H
