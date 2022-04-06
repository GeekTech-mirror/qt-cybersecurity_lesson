#ifndef QNetworkModel_H
#define QNetworkModel_H

#include <QAbstractListModel>
#include <QQueue>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/WirelessDevice>

#include "network_model_item.h"

class QNetworkModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QNetworkModel(const QStringList &headers, const QString &data,
                           QObject *parent = nullptr);
    ~QNetworkModel();

    /* new */
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override; // new implementation
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role) const override; // new implementation

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /* old */
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
        SectionRole,
        SlaveRole,
        SsidRole,
        SpecificPathRole,
        TimeStampRole,
        TypeRole,
        UniRole,
        UuidRole,
    };
    Q_ENUM(ItemRole)

    enum ModelChangeType { ItemAdded, ItemRemoved, ItemPropertyChanged };

    //int rowCount(const QModelIndex &parent) const override;
    //QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setDelayModelUpdates(bool delayUpdates);

public Q_SLOTS:
    void onItemUpdated();
    void setDeviceStatisticsRefreshRateMs(const QVariant &devicePath, uint refreshRate);

private Q_SLOTS:
    void wirelessNetworkAppeared(const QVariant &ssid);
    void wirelessNetworkDisappeared(const QVariant &ssid);
    void connectionAdded(const QVariant &connection);
    void connectionRemoved(const QVariant &connection);
    void connectionUpdated();
    void deviceAdded(const QVariant &device);
    void deviceRemoved(const QVariant &device);

    /*
    void accessPointSignalStrengthChanged(int signal);
    void activeConnectionAdded(const QVariant &activeConnection);
    void activeConnectionRemoved(const QVariant &activeConnection);
    void activeConnectionStateChanged(NetworkManager::ActiveConnection::State state);
    void deviceStateChanged(NetworkManager::Device::State state, NetworkManager::Device::State oldState, NetworkManager::Device::StateChangeReason reason);
    void ipConfigChanged();
    void ipInterfaceChanged();
    void statusChanged(NetworkManager::Status status);
    void wirelessNetworkSignalChanged(int signal);
    void wirelessNetworkReferenceApChanged(const QVariant &accessPoint);
    void initialize();
    */

private:
    /* new */
    QVector<QNetworkItem *> m_items;

    void setupModelData(const QStringList &lines, QNetworkItem *parent);
    QNetworkItem *getItem(const QModelIndex &index) const;

    QNetworkItem *rootItem;

    /* old */
    void addConnection(const NetworkManager::Connection::Ptr &connection, QVector<QVariant> &list);
    void addDevice(const NetworkManager::Device::Ptr &device);
    void addWirelessNetwork(const NetworkManager::WirelessNetwork::Ptr &network, const NetworkManager::WirelessDevice::Ptr &device);
    void updateFromWirelessNetwork(QNetworkItem *item, const NetworkManager::WirelessNetwork::Ptr &network, const NetworkManager::WirelessDevice::Ptr &device);

    void insertItem(QNetworkItem *item);
    void removeItem(QNetworkItem *item);
    void updateItem(QNetworkItem *item);

    bool m_delayModelUpdates = false;
    QQueue<QPair<ModelChangeType, QNetworkItem *>> m_updateQueue;

    /*
    QNetworkList m_list;
    void checkAndCreateDuplicate(const QString &connection, const QString &deviceUni);
    void initializeSignals();
    void initializeSignals(const NetworkManager::Connection::Ptr &connection);
    void initializeSignals(const NetworkManager::Device::Ptr &device);
    void initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network);
    */

};

#endif // QNetworkModel_H
