#include <QDBusInterface>
#include <QIcon>
#include <QTimer>

#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "network_model.h"
#include "network_model_p.h"
#include "network_model_item.h"

// 1 seconds
#define NM_REQUESTSCAN_LIMIT_RATE 1000

QNetworkModel::QNetworkModel (const QVector<QNetworkModel::ItemRole> &roles,
                              QObject *parent)
    : columnRoles (roles),
      QAbstractItemModel (parent)
{
    QVector<QVariant> rootData;
    for (int i = 0; i < columnRoles.count(); ++i)
    {
        switch (columnRoles.at(i)) {
        case DeviceName:
            rootData << "Device Name";
            break;

        case DevicePathRole:
            rootData << "Device Path";
            break;

        case ConnectionIconRole:
            rootData << "";
            break;

        case SpecificPathRole:
            rootData << "Specific Path";
            break;

        case SecurityTypeRole:
            rootData << "Security";
            break;

        case SsidRole:
            rootData << "Ssid";
            break;

        case TypeRole:
            rootData << "Type";
            break;

        default:
            break;
        }
    }
    rootItem = new QNetworkItem(rootData);

    // Initialize first scan and then scan every 15 seconds
    requestScan();

    m_timer = new QTimer(this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this,
            [&](){QNetworkModel::requestScan();});
    m_timer->start();

    // Initialize existing connections
    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        if (!dev->managed())
            continue;

        addDevice (dev, rootItem);
    }

    setupModelData (rootItem);
}

QNetworkModel::QNetworkModel (QNetworkModelPrivate &dd)
    : d_ptr (&dd)
{
}

QNetworkModel::~QNetworkModel ()
{
    delete rootItem;
}


/* Tree Model */
QVariant QNetworkModel::data (const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole &&
        role != Qt::EditRole &&
        role != Qt::DecorationRole)
    {
        return QVariant();
    }

    QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());

    return item->data(index.column());
}

bool QNetworkModel::setData (const QModelIndex &index,
                             const QVariant &value,
                             int role)
{
    if (role != Qt::EditRole)
        return false;

    QNetworkItem *item = d_ptr->getItem (index, this);
    index.column();

    bool result = item->setData (index.column(), value);
    if (result)
        emit dataChanged (index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}


QVariant QNetworkModel::headerData (int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool QNetworkModel::setHeaderData (int section,
                                   Qt::Orientation orientation,
                                   const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData (section, value);

    if (result)
        emit headerDataChanged (orientation, section, section);

    return result;
}


QModelIndex QNetworkModel::index (int row, int column,
                                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    QNetworkItem *parentItem = d_ptr->getItem (parent, this);

    if (!parentItem)
        return QModelIndex();

    QNetworkItem *childItem = parentItem->child (row);
    if (childItem)
        return createIndex (row, column, childItem);

    return QModelIndex();
}

QModelIndex QNetworkModel::parent (const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QNetworkItem *childItem = static_cast<QNetworkItem*>(index.internalPointer());
    QNetworkItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


int QNetworkModel::rowCount (const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const QNetworkItem *parentItem = d_ptr->getItem(parent, this);

    return parentItem ? parentItem->childCount() : 0;
}


bool QNetworkModel::insertRows (int position, int rows,
                                const QModelIndex &parent)
{
    QNetworkItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginInsertRows (parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows ();

    return success;
}

bool QNetworkModel::removeRows (int position, int rows,
                                const QModelIndex &parent)
{
    QNetworkItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginRemoveRows (parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren (position, rows);
    endRemoveRows ();

    return success;
}


int QNetworkModel::columnCount (const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

bool QNetworkModel::insertColumns (int position, int columns,
                                   const QModelIndex &parent)
{
    beginInsertColumns (parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns (position, columns);
    endInsertColumns ();

    return success;
}

bool QNetworkModel::removeColumns (int position, int columns,
                                   const QModelIndex &parent)
{
    beginRemoveColumns (parent, position, position + columns -1);
    const bool success = rootItem->removeColumns (position, columns);
    endRemoveColumns ();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}


Qt::ItemFlags QNetworkModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
}


void QNetworkModel::setupModelData (QNetworkItem *parent)
{
    for (int i = 0; i < parent->childCount(); ++i)
    {
        QNetworkItem *item = parent->child(i);
        for (int j = 0; j < parent->columnCount(); ++j)
        {

            switch (columnRoles.at(j)) {
            case DeviceName:
                item->setData(j, item->deviceName());
                break;

            case DevicePathRole:
                item->setData(j, item->devicePath());
                break;

            case ConnectionIconRole:
                item->setData(j, QIcon::fromTheme (item->icon()));
                break;

            case SpecificPathRole:
                item->setData(j, item->specificPath());
                break;

            case SecurityTypeRole:
                item->setData(j, d_ptr->getSecurityString (item->securityType()));
                break;

            case SsidRole:
                item->setData(j, item->ssid());
                break;

            case TypeRole:
                item->setData(j, item->type());
                break;

            default:
                item->setData(j, "");
            }
        }
    }

}


/* Network Mode */
QHash<int, QByteArray> QNetworkModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[ConnectionDetailsRole] = "ConnectionDetails";
    roles[ConnectionIconRole] = "ConnectionIcon";
    roles[ConnectionPathRole] = "ConnectionPath";
    roles[ConnectionStateRole] = "ConnectionState";
    roles[DeviceName] = "DeviceName";
    roles[DevicePathRole] = "DevicePath";
    roles[DeviceStateRole] = "DeviceState";
    roles[DuplicateRole] = "Duplicate";
    roles[ItemUniqueNameRole] = "ItemUniqueName";
    roles[ItemTypeRole] = "ItemType";
    roles[NameRole] = "Name";
    roles[SectionRole] = "Section";
    roles[SlaveRole] = "Slave";
    roles[SsidRole] = "Ssid";
    roles[SpecificPathRole] = "SpecificPath";
    roles[TimeStampRole] = "TimeStamp";
    roles[TypeRole] = "Type";
    roles[UniRole] = "Uni";
    roles[UuidRole] = "Uuid";

    return roles;
}


void QNetworkModel::addDevice (const NetworkManager::Device::Ptr &device,
                               QNetworkItem *parent)
{
    d_ptr->initializeSignals(device, this);

    if (device->type() == NetworkManager::Device::Wifi)
    {
        NetworkManager::WirelessDevice::Ptr wifiDev =
                device.objectCast<NetworkManager::WirelessDevice>();

        for (const NetworkManager::WirelessNetwork::Ptr &wifiNetwork :
             wifiDev->networks())
        {
            addWirelessNetwork(wifiNetwork, wifiDev, parent);
        }
    }

}


void QNetworkModel::addWirelessNetwork (const NetworkManager::WirelessNetwork::Ptr &network,
                                        const NetworkManager::WirelessDevice::Ptr &device,
                                        QNetworkItem *parent)
{
    d_ptr->initializeSignals(network);

    QVector<QNetworkItem *> parents;
    parents << parent;

    // BUG: 386342
    // When creating a new hidden wireless network and attempting to connect to it, NM then later reports that AccessPoint appeared, but
    // it doesn't know its SSID from some reason, this also makes Wireless device to advertise a new available connection, which we later
    // attempt to merge with an AP, based on its SSID, but it doesn't find any, because we have AP with empty SSID. After this we get another
    // AccessPoint appeared signal, this time we know SSID, but we don't attempt any merging, because it's usually the other way around, thus
    // we need to attempt to merge it here with a connection we guess it's related to this new AP

    /*
    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Type, NetworkManager::ConnectionSettings::Wireless)) {
        if (item->itemType() != QNetworkItem::AvailableConnection)
            continue;

        NetworkManager::ConnectionSettings::Ptr connectionSettings = NetworkManager::findConnection(item->connectionPath())->settings();
        if (connectionSettings && connectionSettings->connectionType() == NetworkManager::ConnectionSettings::Wireless) {
            NetworkManager::WirelessSetting::Ptr wirelessSetting =
                connectionSettings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
            if (QString::fromUtf8(wirelessSetting->ssid()) == network->ssid()) {
                const QString bssid = NetworkManager::macAddressAsString(wirelessSetting->bssid());
                const QString restrictedHw = NetworkManager::macAddressAsString(wirelessSetting->macAddress());
                if ((bssid.isEmpty() || bssid == network->referenceAccessPoint()->hardwareAddress())
                    && (restrictedHw.isEmpty() || restrictedHw == device->hardwareAddress())) {
                    updateFromWirelessNetwork(item, network, device);
                    return;
                }
            }
        }
    }
    */

    NetworkManager::WirelessSetting::NetworkMode mode = NetworkManager::WirelessSetting::Infrastructure;
    NetworkManager::WirelessSecurityType securityType = NetworkManager::UnknownSecurity;

    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();
    if (ap && (ap->capabilities().testFlag(NetworkManager::AccessPoint::Privacy) || ap->wpaFlags() || ap->rsnFlags()))
    {
        securityType = NetworkManager::findBestWirelessSecurity(device->wirelessCapabilities(), true,
                                                                (device->mode() == NetworkManager::WirelessDevice::Adhoc),
                                                                ap->capabilities(),
                                                                ap->wpaFlags(),
                                                                ap->rsnFlags());
    }

    QNetworkItem *item = parents.last();

    const int index = rootItem->childCount();
    beginInsertRows (QModelIndex(), index, index);
    item->insertChildren (item->childCount(), 1, rootItem->columnCount());
    endInsertRows ();

    if (device->ipInterfaceName().isEmpty()) {
        item->child(item->childCount()-1)->setDeviceName(device->interfaceName());
    }
    else {
        item->child(item->childCount()-1)->setDeviceName (device->ipInterfaceName());
    }
    item->child(item->childCount()-1)->setSsid (network->ssid());
    item->child(item->childCount()-1)->setDevicePath (device->uni());
    item->child(item->childCount()-1)->setSpecificPath (network->referenceAccessPoint()->uni());
    item->child(item->childCount()-1)->setType (NetworkManager::ConnectionSettings::Wireless);
    item->child(item->childCount()-1)->setSecurityType (securityType);
//    item->setMode(mode);
//    item->setName(network->ssid());

//    insertRows(position, 1);
}


void QNetworkModel::updateConnection(const NetworkManager::Connection::Ptr &connection, const NMVariantMapMap &map)
{
    QDBusPendingReply<> reply = connection->update(map);
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    watcher->setProperty("action", UpdateConnection);
    watcher->setProperty("connection", connection->name());
}


void QNetworkModel::requestScan(const QString &interface)
{
    for (const NetworkManager::Device::Ptr &device : NetworkManager::networkInterfaces())
    {
        if (device->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice = device.objectCast<NetworkManager::WirelessDevice>();

            if (wifiDevice && wifiDevice->state() != NetworkManager::WirelessDevice::Unavailable)
            {
                if (!interface.isEmpty() && interface != wifiDevice->interfaceName()) {
                    continue;
                }

                if (!checkRequestScanRateLimit(wifiDevice))
                {
                    QDateTime now = QDateTime::currentDateTime();
                    // for NM < 1.12, lastScan is not available
                    QDateTime lastScan = wifiDevice->lastScan();
                    QDateTime lastRequestScan = wifiDevice->lastRequestScan();
                    // Compute the next time we can run a scan
                    int timeout = NM_REQUESTSCAN_LIMIT_RATE;
                    if (lastScan.isValid() && lastScan.msecsTo(now) < NM_REQUESTSCAN_LIMIT_RATE) {
                        timeout = NM_REQUESTSCAN_LIMIT_RATE - lastScan.msecsTo(now);
                    } else if (lastRequestScan.isValid() && lastRequestScan.msecsTo(now) < NM_REQUESTSCAN_LIMIT_RATE) {
                        timeout = NM_REQUESTSCAN_LIMIT_RATE - lastRequestScan.msecsTo(now);
                    }
//                    qCDebug(PLASMA_NM_LIBS_LOG) << "Rescheduling a request scan for" << wifiDevice->interfaceName() << "in" << timeout;
                    scheduleRequestScan(wifiDevice->interfaceName(), timeout);

                    if (!interface.isEmpty()) {
                        return;
                    }
                    continue;
                } else if (m_wirelessScanRetryTimer.contains(interface)) {
                    m_wirelessScanRetryTimer.value(interface)->stop();
                    delete m_wirelessScanRetryTimer.take(interface);
                }

//                qCDebug(PLASMA_NM_LIBS_LOG) << "Requesting wifi scan on device" << wifiDevice->interfaceName();
                QDBusPendingReply<> reply = wifiDevice->requestScan();
                auto watcher = new QDBusPendingCallWatcher(reply, this);
                watcher->setProperty("action", QNetworkModel::RequestScan);
                watcher->setProperty("interface", wifiDevice->interfaceName());
            }
        }
    }
}


bool QNetworkModel::checkRequestScanRateLimit(const NetworkManager::WirelessDevice::Ptr &wifiDevice)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime lastScan = wifiDevice->lastScan();
    QDateTime lastRequestScan = wifiDevice->lastRequestScan();

    // if the last scan finished within the last 10 seconds
    bool ret = lastScan.isValid() && lastScan.msecsTo(now) < NM_REQUESTSCAN_LIMIT_RATE;
    // or if the last Request was sent within the last 10 seconds
    ret |= lastRequestScan.isValid() && lastRequestScan.msecsTo(now) < NM_REQUESTSCAN_LIMIT_RATE;
    // skip the request scan
    if (ret) {
//        qCDebug(PLASMA_NM_LIBS_LOG) << "Last scan finished " << lastScan.msecsTo(now) << "ms ago and last request scan was sent "
//                                    << lastRequestScan.msecsTo(now) << "ms ago, Skipping scanning interface:" << wifiDevice->interfaceName();
        return false;
    }
    return true;
}


void QNetworkModel::scheduleRequestScan(const QString &interface, int timeout)
{
    QTimer *timer;
    if (!m_wirelessScanRetryTimer.contains(interface)) {
        // create a timer for the interface
        timer = new QTimer();
        timer->setSingleShot(true);
        m_wirelessScanRetryTimer.insert(interface, timer);
        auto retryAction = [this, interface]() {
            requestScan(interface);
        };
        connect(timer, &QTimer::timeout, this, retryAction);
    } else {
        // set the new value for an existing timer
        timer = m_wirelessScanRetryTimer.value(interface);
        if (timer->isActive()) {
            timer->stop();
        }
    }

    // +1 ms is added to avoid having the scan being rejetted by nm
    // because it is run at the exact last millisecond of the requestScan threshold
    timer->setInterval(timeout + 1);
    timer->start();
}

void QNetworkModel::scanRequestFailed(const QString &interface)
{
    scheduleRequestScan(interface, 2000);
}


void QNetworkModel::wirelessNetworkAppeared (const QString &ssid)
{
    NetworkManager::Device::Ptr device =
            NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());
    if (device && device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wirelessDevice = device.objectCast<NetworkManager::WirelessDevice>();
        NetworkManager::WirelessNetwork::Ptr network = wirelessDevice->findNetwork(ssid);

        QString ssid = network->ssid();
        for (int i = 0; i < rootItem->childCount(); ++i)
        {
            if (rootItem->child(i)->ssid() == ssid )
                return;
        }

        addWirelessNetwork(network, wirelessDevice, rootItem);

        int row = rootItem->childCount()-1;
        QNetworkItem *item = rootItem->child(row);
        for (int i = 0; i < rootItem->columnCount(); ++i) {
            item->setData(i, d_ptr->getColumn(index(row, i), this));
        }
    }
}
