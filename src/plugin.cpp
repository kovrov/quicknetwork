#include <qqml.h>

#include "networkconfigurationmodel.h"
#include "networksession.h"

#include "plugin.h"

void QuicknetworkPlugin::registerTypes(const char *uri)
{
    // @uri Network
    qmlRegisterType<NetworkSession>(uri, 1, 0, "NetworkSession");
    qmlRegisterType<NetworkConfigurationModel>(uri, 1, 0, "NetworkConfigurationModel");
}
