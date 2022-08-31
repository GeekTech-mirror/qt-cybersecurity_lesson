#ifndef NetworkModel_H
#define NetworkModel_H

#include <QAbstractItemModel>
#include <QTimer>

#include <NetworkManagerQt/WirelessDevice>

#include "network_enums.h"
#include "network_item.h"
#include "network_scan.h"
#include "network_sort.h"


class NetworkModelPrivate;
class NetworkModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE (NetworkModel)

public:
    /* Constructor */
    explicit NetworkModel (const QVector<ItemRole> &roles,
                           QObject *parent = nullptr);
    ~NetworkModel ();

    /* Tree Model */
    QVariant data (const QModelIndex &index, int role) const override;
    bool setData (const QModelIndex &index, const QVariant &network_role,
                  int role = Qt::EditRole) override;

    QVariant headerData (int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const override;
    bool setHeaderData (int section, Qt::Orientation orientation,
                        const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index (int row, int column,
                       const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent (const QModelIndex &index) const override;

    int rowCount (const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows (int position, int rows,
                     const QModelIndex &parent = QModelIndex()) override;
    bool removeRows (int position, int rows,
                     const QModelIndex &parent = QModelIndex()) override;

    int columnCount (const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags (const QModelIndex &index) const override;


    /* Network Model */
    QHash<int, QByteArray> roleNames (void) const override;


    QIcon getScanIcon();

    void toggle_scan();

    void start_scan();
    void stop_scan();

    void export_data();


    // Network Properties


private Q_SLOTS:
    void wirelessNetworkAppeared (const QString &ssid);

    void wirelessNetworkDisappeared(const QString &ssid);

private:
    /* Tree Model */
    NetworkItem *rootItem;
    QVector<ItemRole> columnRoles;
    void setupModelData (const QVector<ItemRole> &roles, NetworkItem *parent);

    /* Network Model */
    void addConnection (const NetworkManager::Connection::Ptr &connection,
                        QVector<ItemRole> &list);
    void addDevice (const NetworkManager::Device::Ptr &device, NetworkItem *parent);
    void addWirelessNetwork (const NetworkManager::WirelessNetwork::Ptr &network,
                             const NetworkManager::WirelessDevice::Ptr &device);

    void remove_network(NetworkItem *item);

    void update_networks (const NetworkManager::WirelessNetwork::Ptr &network,
                          NetworkManager::WirelessDevice::Ptr &wifiDev);

    /* Scan Networks */
    bool first_scan = true;

    NetworkScan *m_scanHandler;
    QTimer *m_timer = nullptr;

    QString m_scanIcon;

protected:
    NetworkModel (NetworkModelPrivate &dd);
    NetworkModelPrivate *d_ptr;
};

#endif // NetworkModel_H
