#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkSession>

#include "networksession.h"

NetworkSession::NetworkSession() :
    m_session (nullptr)
{
}

QVariant NetworkSession::configuration() const
{
    return m_session ? QVariant::fromValue(m_session->configuration()) : QVariant();
}

void NetworkSession::setConfiguration(const QVariant &variant)
{
    Q_ASSERT (variant.canConvert<QNetworkConfiguration>());
    QNetworkConfiguration configuration = variant.value<QNetworkConfiguration>();

    if (!(m_session && m_session->configuration() == configuration)) {
        if (m_session)
            delete m_session;

        m_session = new QNetworkSession(configuration, this);

        QObject::connect(m_session, &QNetworkSession::stateChanged, this, &NetworkSession::stateChanged);
        QObject::connect(m_session, &QNetworkSession::stateChanged, this, &NetworkSession::interfaceChanged);
        QObject::connect(m_session, &QNetworkSession::opened, this, &NetworkSession::isOpenChanged);
        QObject::connect(m_session, &QNetworkSession::closed, this, &NetworkSession::isOpenChanged);
        QObject::connect(m_session, &QNetworkSession::usagePoliciesChanged, this, &NetworkSession::usagePoliciesChanged);

        QObject::connect(m_session,
            static_cast<void(QNetworkSession::*)(QNetworkSession::SessionError)>(&QNetworkSession::error),
                         this, &NetworkSession::errorChanged);
        QObject::connect(m_session,
            static_cast<void(QNetworkSession::*)(QNetworkSession::SessionError)>(&QNetworkSession::error),
                         this, &NetworkSession::errorStringChanged);
    }

    emit configurationChanged();
    emit interfaceChanged();
    emit isOpenChanged();
    emit stateChanged();
    emit errorChanged();
    emit errorStringChanged();
    emit usagePoliciesChanged();
}

QJSValue NetworkSession::interface() const
{
    QQmlEngine *js = QQmlEngine::contextForObject(this)->engine(); //QJSEngine::qjsEngine(this);
    QJSValue js_interface = js->newObject();

    if (!m_session || !(m_session->configuration().state() & ConfigurationActive))
        return js_interface;


    const QNetworkInterface &iface = m_session->interface();

    QJSValue js_address_entries = js->newArray();
    for (int i = 0; i < iface.addressEntries().length(); ++i) {
        const QNetworkAddressEntry &network_address_entry = iface.addressEntries().at(i);
        QJSValue js_network_address_entry = js->newObject();

        const QHostAddress &ip = network_address_entry.ip();

        js_network_address_entry.setProperty(QStringLiteral("protocol"),
                                             ip.protocol() == QAbstractSocket::IPv4Protocol ? QStringLiteral("IPv4") :
                                                                                              ip.protocol() == QAbstractSocket::IPv6Protocol ? QStringLiteral("IPv6") :
                                                                                                                                               QStringLiteral("unknown"));

        js_network_address_entry.setProperty(QStringLiteral("broadcast"),
                                             network_address_entry.broadcast().toString());
        js_network_address_entry.setProperty(QStringLiteral("ip"), ip.toString());
        js_network_address_entry.setProperty(QStringLiteral("netmask"),
                                             network_address_entry.netmask().toString());

        js_address_entries.setProperty(i, js_network_address_entry );
    }
    js_interface.setProperty(QStringLiteral("addressEntries"), js_address_entries);

    js_interface.setProperty(QStringLiteral("flags"), static_cast<int>(iface.flags()));
    js_interface.setProperty(QStringLiteral("hardwareAddress"), iface.hardwareAddress());
    js_interface.setProperty(QStringLiteral("humanReadableName"), iface.humanReadableName());
    js_interface.setProperty(QStringLiteral("index"), iface.index());
    js_interface.setProperty(QStringLiteral("isValid"), iface.isValid());
    js_interface.setProperty(QStringLiteral("name"), iface.name());

    return js_interface;
}

bool NetworkSession::isOpen() const
{
    return m_session ? m_session->isOpen() : false;
}

int NetworkSession::state() const
{
    if (!m_session)
        return QNetworkSession::Invalid;
    if (!(m_session->configuration().state() & ConfigurationActive))
        return QNetworkSession::Disconnected;
    return m_session->state();
}

int NetworkSession::error() const
{
    return m_session ? m_session->error() : QNetworkSession::UnknownSessionError;
}

QString NetworkSession::errorString() const
{
    return m_session ? m_session->errorString() : QString();
}

int NetworkSession::usagePolicies() const
{
    return m_session ? m_session->usagePolicies() : QNetworkSession::NoPolicy;
}
