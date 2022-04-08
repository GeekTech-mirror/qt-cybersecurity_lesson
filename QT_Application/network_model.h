#ifndef QNetworkModel_H
#define QNetworkModel_H

#include <QAbstractItemModel>
#include <QQueue>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/WirelessDevice>

#include "network_model_item.h"

class QNetworkModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    /* Network Model */
    enum ItemRole {
        ConnectionDetailsRole = Qt::UserRole + 1,
        ConnectionIconRole,
        ConnectionPathRole,
        ConnectionStateRole,
        DeviceName,
        DevicePathRole,
        DeviceStateRole,
        DuplicateRole,
        ItemUniqueNameRole,
        ItemTypeRole,
        LastUsedRole,
        LastUsedDateOnlyRole,
        NameRole,
        SecurityTypeRole,
        SecurityTypeStringRole,
        SectionRole,
        SignalRole,
        SlaveRole,
        SsidRole,
        SpecificPathRole,
        TimeStampRole,
        TypeRole,
        UniRole,
        UuidRole,
    };
    Q_ENUM(ItemRole)
    QHash<int, QByteArray> roleNames() const override;

    explicit QNetworkModel(const QVector<QNetworkModel::ItemRole> &columnRoles,
                           QObject *parent = nullptr);
    ~QNetworkModel();

    /* Tree Model */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;


private:
    /* Tree Model */
    void setupModelData(const QVector<QNetworkModel::ItemRole> &roles, QNetworkItem *parent);
    QNetworkItem *getItem(const QModelIndex &index) const;

    QNetworkItem *rootItem;

    /* Network Model */
    void addConnection(const NetworkManager::Connection::Ptr &connection, QVector<QNetworkModel::ItemRole> &list);
    void addDevice(const NetworkManager::Device::Ptr &device, QNetworkItem *parent);
    void addWirelessNetwork(const NetworkManager::WirelessNetwork::Ptr &network,
                            const NetworkManager::WirelessDevice::Ptr &device,
                            QNetworkItem *parent);

    void initializeSignals();
    void initializeSignals(const NetworkManager::Connection::Ptr &connection);
    void initializeSignals(const NetworkManager::Device::Ptr &device);
    void initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network);
};

#endif // QNetworkModel_H
