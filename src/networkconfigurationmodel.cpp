#include <QtCore/QModelIndex>
#include <QtNetwork/QNetworkConfigurationManager>

#include "networkconfigurationmodel.h"

NetworkConfigurationModel::NetworkConfigurationModel(QObject *parent) :
    QAbstractListModel (parent)
{
    m_ncm = new QNetworkConfigurationManager(this);

    QObject::connect(m_ncm.data(), &QNetworkConfigurationManager::configurationAdded,
                     this, &NetworkConfigurationModel::onConfigurationAdded);
    QObject::connect(m_ncm.data(), &QNetworkConfigurationManager::configurationRemoved,
                     this, &NetworkConfigurationModel::onConfigurationRemoved);
    QObject::connect(m_ncm.data(), &QNetworkConfigurationManager::configurationChanged,
                     this, &NetworkConfigurationModel::onConfigurationChanged);

    emit beginResetModel();
    m_configurations = m_ncm->allConfigurations();
    emit endResetModel();
}

int NetworkConfigurationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent)
    return m_configurations.count();
}

QVariant NetworkConfigurationModel::data(const QModelIndex &index, int role) const
{
    const auto &res = m_configurations.at(index.row());
    switch (role) {
    case ConfigurationRole:
        return QVariant::fromValue(res);
    case BearerTypeRole:
        return res.bearerType();
    case BearerTypeFamilyRole:
        return res.bearerTypeFamily();
    case BearerTypeNameRole:
        return res.bearerTypeName();
    case IdentifierRole:
        return res.identifier();
    case IsRoamingAvailableRole:
        return res.isRoamingAvailable();
    case IsValidRole:
        return res.isValid();
    case NameRole:
        return res.name();
    case PurposeRole:
        return res.purpose();
    case StateRole:
        return QVariant(res.state());
    case TypeRole:
        return res.type();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> NetworkConfigurationModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { ConfigurationRole, "configuration" },
        { BearerTypeRole, "bearerType" },
        { BearerTypeFamilyRole, "bearerTypeFamily" },
        { BearerTypeNameRole, "bearerTypeName" },
        { IdentifierRole, "identifier" },
        { IsRoamingAvailableRole, "isRoamingAvailable" },
        { IsValidRole, "isValid" },
        { NameRole, "name" },
        { PurposeRole, "purpose" },
        { StateRole, "state" },
        { TypeRole, "type" }
    };
    return roles;
}

void NetworkConfigurationModel::onConfigurationAdded(const QNetworkConfiguration &config)
{
    int index = m_configurations.indexOf(config);
    if (index != -1)
        return;

    index = m_configurations.count();
    emit beginInsertRows(QModelIndex(), index, index);
    m_configurations.append(config);
    emit endInsertRows();
}

void NetworkConfigurationModel::onConfigurationRemoved(const QNetworkConfiguration &config)
{
    int index = m_configurations.indexOf(config);
    if (index == -1)
        return;

    emit beginRemoveRows(QModelIndex(), index, index);
    m_configurations.removeAt(index);
    emit endRemoveRows();
}

void NetworkConfigurationModel::onConfigurationChanged(const QNetworkConfiguration &config)
{
    int index = m_configurations.indexOf(config);
    if (index == -1)
        return;

    m_configurations.replace(index, config);

    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
}
