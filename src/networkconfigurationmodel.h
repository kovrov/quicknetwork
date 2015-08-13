#ifndef NETWORKCONFIGURATIONMODEL_H
#define NETWORKCONFIGURATIONMODEL_H

#include <QtCore/QPointer>
#include <QtCore/QAbstractListModel>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkConfigurationManager>

class NetworkConfigurationModel : public QAbstractListModel
{
    Q_OBJECT

    enum ItemDataRole {
        ConfigurationRole = Qt::UserRole,
        BearerTypeRole,
        BearerTypeFamilyRole,
        BearerTypeNameRole,
        IdentifierRole,
        IsRoamingAvailableRole,
        IsValidRole,
        NameRole,
        PurposeRole,
        StateRole,
        TypeRole,
    };

public:
    NetworkConfigurationModel(QObject *parent=nullptr);

    //QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    //QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    //int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private slots:
    void onConfigurationAdded(const QNetworkConfiguration &config);
    void onConfigurationRemoved(const QNetworkConfiguration &config);
    void onConfigurationChanged(const QNetworkConfiguration &config);

private:
    QList<QNetworkConfiguration> m_configurations;
    QPointer<QNetworkConfigurationManager> m_ncm;
};

#endif // NETWORKCONFIGURATIONMODEL_H
