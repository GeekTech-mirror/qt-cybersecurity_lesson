#ifndef QNetworkModel_H
#define QNetworkModel_H

#include <QAbstractItemModel>
#include <QQueue>
#include <QTimer>

#include <NetworkManagerQt/WirelessDevice>

#include "network_model_item.h"


class QNetworkModelPrivate;
class QNetworkModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE (QNetworkModel)

public:
    /* Network Model */
    enum HandlerAction {
        ActivateConnection,
        AddAndActivateConnection,
        AddConnection,
        DeactivateConnection,
        RemoveConnection,
        RequestScan,
        UpdateConnection,
        CreateHotspot,
    };

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

    /* Constructor */
    explicit QNetworkModel (const QVector<QNetworkModel::ItemRole> &roles,
                            QObject *parent = nullptr);
    ~QNetworkModel ();

    /* Tree Model */
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData (const QModelIndex &index, const QVariant &value,
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

    bool insertColumns (int position, int columns,
                        const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns (int position, int columns,
                        const QModelIndex &parent = QModelIndex()) override;

    Qt::ItemFlags flags (const QModelIndex &index) const override;



public Q_SLOTS:
    void updateConnection (const NetworkManager::Connection::Ptr &connection,
                           const NMVariantMapMap &map);
    void requestScan (const QString &interface  = QString());

private Q_SLOTS:
//    void replyFinished(QDBusPendingCallWatcher *watcher);
    void wirelessNetworkAppeared (const QString &ssid);

private:
    /* Tree Model */
    QNetworkItem *rootItem;
    QVector<QNetworkModel::ItemRole> columnRoles;
    void setupModelData (QNetworkItem *parent);

    /* Network Model */
    void addConnection (const NetworkManager::Connection::Ptr &connection,
                        QVector<QNetworkModel::ItemRole> &list);
    void addDevice (const NetworkManager::Device::Ptr &device, QNetworkItem *parent);
    void addWirelessNetwork (const NetworkManager::WirelessNetwork::Ptr &network,
                             const NetworkManager::WirelessDevice::Ptr &device,
                             QNetworkItem *parent);

    /* Scan Networks */
    QTimer *m_timer = nullptr;
    QMap<QString, QTimer *> m_wirelessScanRetryTimer;
    bool checkRequestScanRateLimit(const NetworkManager::WirelessDevice::Ptr &wifiDevice);
    void scheduleRequestScan(const QString &interface, int timeout);
    void scanRequestFailed(const QString &interface);

protected:
    QNetworkModel(QNetworkModelPrivate &dd);
    QNetworkModelPrivate *d_ptr;
};

#endif // QNetworkModel_H
