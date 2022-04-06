#ifndef QNetworkModel_H
#define QNetworkModel_H

#include <QAbstractListModel>
#include <QQueue>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/WirelessDevice>

#include "network_list.h"

class QNetworkModel : public QAbstractListModel
{
public:
    explicit QNetworkModel(QObject *parent = nullptr);
    ~QNetworkModel() override;

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
        RxBytesRole,
        TxBytesRole,
    };
    Q_ENUM(ItemRole)

    enum ModelChangeType { ItemAdded, ItemRemoved, ItemPropertyChanged };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setDelayModelUpdates(bool delayUpdates);

public Q_SLOTS:
    void onItemUpdated();
    void setDeviceStatisticsRefreshRateMs(const QString &devicePath, uint refreshRate);

private Q_SLOTS:
    void accessPointSignalStrengthChanged(int signal);
    void activeConnectionAdded(const QString &activeConnection);
    void activeConnectionRemoved(const QString &activeConnection);
    void activeConnectionStateChanged(NetworkManager::ActiveConnection::State state);
    void availableConnectionAppeared(const QString &connection);
    void availableConnectionDisappeared(const QString &connection);
    void connectionAdded(const QString &connection);
    void connectionRemoved(const QString &connection);
    void connectionUpdated();
    void deviceAdded(const QString &device);
    void deviceRemoved(const QString &device);
    void deviceStateChanged(NetworkManager::Device::State state, NetworkManager::Device::State oldState, NetworkManager::Device::StateChangeReason reason);
    void ipConfigChanged();
    void ipInterfaceChanged();
    void statusChanged(NetworkManager::Status status);
    void wirelessNetworkAppeared(const QString &ssid);
    void wirelessNetworkDisappeared(const QString &ssid);
    void wirelessNetworkSignalChanged(int signal);
    void wirelessNetworkReferenceApChanged(const QString &accessPoint);

    void initialize();

private:
    bool m_delayModelUpdates = false;
    QNetworkList m_list;
    QQueue<QPair<ModelChangeType, QNetworkModelItem *>> m_updateQueue;

    void addActiveConnection(const NetworkManager::ActiveConnection::Ptr &activeConnection);
    void addAvailableConnection(const QString &connection, const NetworkManager::Device::Ptr &device);
    void addConnection(const NetworkManager::Connection::Ptr &connection);
    void addDevice(const NetworkManager::Device::Ptr &device);
    void addWirelessNetwork(const NetworkManager::WirelessNetwork::Ptr &network, const NetworkManager::WirelessDevice::Ptr &device);
    void checkAndCreateDuplicate(const QString &connection, const QString &deviceUni);
    void initializeSignals();
    void initializeSignals(const NetworkManager::ActiveConnection::Ptr &activeConnection);
    void initializeSignals(const NetworkManager::Connection::Ptr &connection);
    void initializeSignals(const NetworkManager::Device::Ptr &device);
    void initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network);
    void
    updateFromWirelessNetwork(QNetworkModelItem *item, const NetworkManager::WirelessNetwork::Ptr &network, const NetworkManager::WirelessDevice::Ptr &device);

    void insertItem(QNetworkModelItem *item);
    void removeItem(QNetworkModelItem *item);
    void updateItem(QNetworkModelItem *item);

    NetworkManager::WirelessSecurityType alternativeWirelessSecurity(const NetworkManager::WirelessSecurityType type);
};

#endif // QNetworkModel_H
