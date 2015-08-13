#ifndef NETWORKSESSION_H
#define NETWORKSESSION_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtQml/QJSValue>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkSession>

class NetworkSession : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QVariant configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    Q_PROPERTY (QJSValue interface READ interface NOTIFY interfaceChanged)
    Q_PROPERTY (bool isOpen READ isOpen NOTIFY isOpenChanged)
    Q_PROPERTY (int state READ state NOTIFY stateChanged)
    Q_PROPERTY (int error READ error NOTIFY errorChanged)
    Q_PROPERTY (QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY (int usagePolicies READ usagePolicies NOTIFY usagePoliciesChanged)

    Q_ENUMS (ConfigurationFlags)
    Q_ENUMS (InterfaceFlags)
    Q_ENUMS (State)

public:
    enum ConfigurationFlags {
        ConfigurationUndefined  = 1,
        ConfigurationDefined    = 2,
        ConfigurationDiscovered = 4,
        ConfigurationActive     = 8
    };

    enum InterfaceFlags {
        IsUp = QNetworkInterface::IsUp,
        IsRunning = QNetworkInterface::IsRunning,
        CanBroadcast = QNetworkInterface::CanBroadcast,
        IsLoopBack = QNetworkInterface::IsLoopBack,
        IsPointToPoint = QNetworkInterface::IsPointToPoint,
        CanMulticast = QNetworkInterface::CanMulticast
    };

    enum State {
        Invalid = QNetworkSession::Invalid,
        NotAvailable = QNetworkSession::NotAvailable,
        Connecting = QNetworkSession::Connecting,
        Connected = QNetworkSession::Connected,
        Closing = QNetworkSession::Closing,
        Disconnected = QNetworkSession::Disconnected,
        Roaming = QNetworkSession::Roaming
    };

    NetworkSession();

    QVariant configuration() const;
    void setConfiguration(const QVariant &configuration);

    QJSValue interface() const;

    bool isOpen() const;
    int state() const;
    int error() const;
    QString errorString() const;
    int usagePolicies() const;

signals:
    void configurationChanged() const;
    void interfaceChanged() const;
    void isOpenChanged() const;
    void stateChanged() const;
    void errorChanged() const;
    void errorStringChanged() const;
    void usagePoliciesChanged() const;

private:
    QNetworkSession *m_session;
};

#endif // NETWORKSESSION_H
