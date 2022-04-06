#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "network_model.h"
#include "network_model_item.h"

QNetworkModel::QNetworkModel(QObject *parent)
    : QAbstractListModel(parent)
{
    initialize();
}

QNetworkModel::~QNetworkModel() = default;

QVariant QNetworkModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();

    if (row >= 0 && row < m_list.count()) {
        QNetworkModelItem *item = m_list.itemAt(row);

        switch (role) {
//        case ConnectionDetailsRole:
//            return item->details();
        case ConnectionIconRole:
            return item->icon();
        case ConnectionPathRole:
            return item->connectionPath();
        case ConnectionStateRole:
            return item->connectionState();
        case DeviceName:
            return item->deviceName();
        case DevicePathRole:
            return item->devicePath();
//        case DeviceStateRole:
//            return item->deviceState();
        case DuplicateRole:
            return item->duplicate();
        case ItemUniqueNameRole:
            return item->name();
        case ItemTypeRole:
            return item->itemType();
        case NameRole:
            return item->name();
        case SectionRole:
            return item->sectionType();
        case SignalRole:
            return item->signal();
        case SlaveRole:
            return item->slave();
        case SsidRole:
            return item->ssid();
        case SpecificPathRole:
            return item->specificPath();
        case SecurityTypeRole:
            return item->securityType();
        case TimeStampRole:
            return item->timestamp();
        case TypeRole:
            return item->type();
        case UniRole:
            return item->uni();
        case UuidRole:
            return item->uuid();
        case RxBytesRole:
            return item->rxBytes();
        case TxBytesRole:
            return item->txBytes();
        default:
            break;
        }
    }

    return {};
}

int QNetworkModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return parent.isValid() ? 0 : m_list.count();
}

QHash<int, QByteArray> QNetworkModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
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
    roles[SignalRole] = "Signal";
    roles[SlaveRole] = "Slave";
    roles[SsidRole] = "Ssid";
    roles[SpecificPathRole] = "SpecificPath";
    roles[SecurityTypeRole] = "SecurityType";
    roles[SecurityTypeStringRole] = "SecurityTypeString";
    roles[TimeStampRole] = "TimeStamp";
    roles[TypeRole] = "Type";
    roles[UniRole] = "Uni";
    roles[UuidRole] = "Uuid";
    roles[RxBytesRole] = "RxBytes";
    roles[TxBytesRole] = "TxBytes";

    return roles;
}

void QNetworkModel::setDelayModelUpdates(bool delayUpdates)
{
    m_delayModelUpdates = delayUpdates;

    // Process queue
    if (!delayUpdates) {
        while (!m_updateQueue.isEmpty()) {
            QPair<QNetworkModel::ModelChangeType, QNetworkModelItem *> update = m_updateQueue.dequeue();
            if (update.first == ItemAdded) {
                insertItem(update.second);
            } else if (update.first == ItemRemoved) {
                removeItem(update.second);
            } else if (update.first == ItemPropertyChanged) {
                updateItem(update.second);
            }
        }
    }
}

void QNetworkModel::initialize()
{
    // Initialize existing connections
    for (const NetworkManager::Connection::Ptr &connection : NetworkManager::listConnections()) {
        addConnection(connection);
    }

    // Initialize existing devices
    for (const NetworkManager::Device::Ptr &dev : NetworkManager::networkInterfaces()) {
        if (!dev->managed()) {
            continue;
        }
        addDevice(dev);
    }

    // Initialize existing active connections
    for (const NetworkManager::ActiveConnection::Ptr &active : NetworkManager::activeConnections()) {
        addActiveConnection(active);
    }

    initializeSignals();
}

void QNetworkModel::initializeSignals()
{
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionAdded, this, &QNetworkModel::activeConnectionAdded, Qt::UniqueConnection);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionRemoved, this, &QNetworkModel::activeConnectionRemoved, Qt::UniqueConnection);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded, this, &QNetworkModel::deviceAdded, Qt::UniqueConnection);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceRemoved, this, &QNetworkModel::deviceRemoved, Qt::UniqueConnection);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::statusChanged, this, &QNetworkModel::statusChanged, Qt::UniqueConnection);
}

void QNetworkModel::initializeSignals(const NetworkManager::ActiveConnection::Ptr &activeConnection)
{

    connect(activeConnection.data(),
            &NetworkManager::ActiveConnection::stateChanged,
            this,
            &QNetworkModel::activeConnectionStateChanged,
            Qt::UniqueConnection);
}

void QNetworkModel::initializeSignals(const NetworkManager::Connection::Ptr &connection)
{
    connect(connection.data(), &NetworkManager::Connection::updated, this, &QNetworkModel::connectionUpdated, Qt::UniqueConnection);
}

void QNetworkModel::initializeSignals(const NetworkManager::Device::Ptr &device)
{
    connect(device.data(), &NetworkManager::Device::availableConnectionAppeared, this, &QNetworkModel::availableConnectionAppeared, Qt::UniqueConnection);
    connect(device.data(), &NetworkManager::Device::availableConnectionDisappeared, this, &QNetworkModel::availableConnectionDisappeared, Qt::UniqueConnection);
    connect(device.data(), &NetworkManager::Device::ipV4ConfigChanged, this, &QNetworkModel::ipConfigChanged, Qt::UniqueConnection);
    connect(device.data(), &NetworkManager::Device::ipV6ConfigChanged, this, &QNetworkModel::ipConfigChanged, Qt::UniqueConnection);
    connect(device.data(), &NetworkManager::Device::ipInterfaceChanged, this, &QNetworkModel::ipInterfaceChanged);
    connect(device.data(), &NetworkManager::Device::stateChanged, this, &QNetworkModel::deviceStateChanged, Qt::UniqueConnection);

    auto deviceStatistics = device->deviceStatistics();
    connect(deviceStatistics.data(), &NetworkManager::DeviceStatistics::rxBytesChanged, this, [this, device](qulonglong rxBytes) {
        for (auto item : m_list.returnItems(QNetworkList::Device, device->uni())) {
            item->setRxBytes(rxBytes);
            updateItem(item);
        }
    });
    connect(deviceStatistics.data(), &NetworkManager::DeviceStatistics::txBytesChanged, this, [this, device](qulonglong txBytes) {
        for (auto item : m_list.returnItems(QNetworkList::Device, device->uni())) {
            item->setTxBytes(txBytes);
            updateItem(item);
        }
    });

    if (device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wifiDev = device.objectCast<NetworkManager::WirelessDevice>();
        connect(wifiDev.data(), &NetworkManager::WirelessDevice::networkAppeared, this, &QNetworkModel::wirelessNetworkAppeared, Qt::UniqueConnection);
        connect(wifiDev.data(), &NetworkManager::WirelessDevice::networkDisappeared, this, &QNetworkModel::wirelessNetworkDisappeared, Qt::UniqueConnection);

    }
}

void QNetworkModel::initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network)
{
    connect(network.data(), &NetworkManager::WirelessNetwork::signalStrengthChanged, this, &QNetworkModel::wirelessNetworkSignalChanged, Qt::UniqueConnection);
    connect(network.data(),
            &NetworkManager::WirelessNetwork::referenceAccessPointChanged,
            this,
            &QNetworkModel::wirelessNetworkReferenceApChanged,
            Qt::UniqueConnection);
}

void QNetworkModel::addActiveConnection(const NetworkManager::ActiveConnection::Ptr &activeConnection)
{
    initializeSignals(activeConnection);

    NetworkManager::Device::Ptr device;
    NetworkManager::Connection::Ptr connection = activeConnection->connection();

    // Not necessary to have device for VPN connections
    if (activeConnection && !activeConnection->vpn() && !activeConnection->devices().isEmpty()) {
        device = NetworkManager::findNetworkInterface(activeConnection->devices().first());
    }

    // Check whether we have a base connection
    if (!m_list.contains(QNetworkList::Uuid, connection->uuid())) {
        // Active connection appeared before a base connection, so we have to add its base connection first
        addConnection(connection);
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::QNetworkList::Uuid, connection->uuid())) {
        if (((device && device->uni() == item->devicePath()) || item->devicePath().isEmpty()) || item->type() == NetworkManager::ConnectionSettings::Vpn) {
            item->setActiveConnectionPath(activeConnection->path());
            item->setConnectionState(activeConnection->state());

            if (device && device->uni() == item->devicePath()) {
                auto deviceStatistics = device->deviceStatistics();
                item->setRxBytes(deviceStatistics->rxBytes());
                item->setTxBytes(deviceStatistics->txBytes());
            }
        }
        updateItem(item);
    }
}

void QNetworkModel::addAvailableConnection(const QString &connection, const NetworkManager::Device::Ptr &device)
{
    if (!device) {
        return;
    }

    checkAndCreateDuplicate(connection, device->uni());

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Connection, connection)) {
        // The item is already associated with another device
        if (!device || !item->devicePath().isEmpty()) {
            continue;
        }

        if (device->ipInterfaceName().isEmpty()) {
            item->setDeviceName(device->interfaceName());
        } else {
            item->setDeviceName(device->ipInterfaceName());
        }

        item->setDevicePath(device->uni());
        item->setDeviceState(device->state());

        if (item->type() == NetworkManager::ConnectionSettings::Wireless && item->mode() == NetworkManager::WirelessSetting::Infrastructure) {
            // Find an accesspoint which could be removed, because it will be merged with a connection
            for (QNetworkModelItem *secondItem : m_list.returnItems(QNetworkList::Ssid, item->ssid())) {
                if (secondItem->itemType() == QNetworkModelItem::AvailableAccessPoint && secondItem->devicePath() == item->devicePath()) {
                    removeItem(secondItem);
                    break;
                }
            }

            NetworkManager::WirelessDevice::Ptr wifiDevice = device.objectCast<NetworkManager::WirelessDevice>();
            if (wifiDevice) {
                NetworkManager::WirelessNetwork::Ptr wifiNetwork = wifiDevice->findNetwork(item->ssid());
                if (wifiNetwork) {
                    updateFromWirelessNetwork(item, wifiNetwork, wifiDevice);
                }
            }
        }

        updateItem(item);
        break;
    }
}

void QNetworkModel::addConnection(const NetworkManager::Connection::Ptr &connection)
{
    // Can't add a connection without name or uuid
    if (connection->name().isEmpty() || connection->uuid().isEmpty()) {
        return;
    }

    initializeSignals(connection);

    NetworkManager::ConnectionSettings::Ptr settings = connection->settings();
    NetworkManager::WirelessSetting::Ptr wirelessSetting;

    if (settings->connectionType() == NetworkManager::ConnectionSettings::Wireless) {
        wirelessSetting = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    }

    // Check whether the connection is already in the model to avoid duplicates, but this shouldn't happen
    if (m_list.contains(QNetworkList::Connection, connection->path())) {
        return;
    }

    auto item = new QNetworkModelItem();
    item->setConnectionPath(connection->path());
    item->setName(settings->id());
    item->setTimestamp(settings->timestamp());
    item->setType(settings->connectionType());
    item->setUuid(settings->uuid());
    item->setSlave(settings->isSlave());

    if (item->type() == NetworkManager::ConnectionSettings::Wireless) {
        item->setMode(wirelessSetting->mode());
        item->setSecurityType(NetworkManager::securityTypeFromConnectionSetting(settings));
        item->setSsid(QString::fromUtf8(wirelessSetting->ssid()));
    }

    item->invalidateDetails();

    insertItem(item);
}

void QNetworkModel::addDevice(const NetworkManager::Device::Ptr &device)
{
    initializeSignals(device);

    if (device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wifiDev = device.objectCast<NetworkManager::WirelessDevice>();
        for (const NetworkManager::WirelessNetwork::Ptr &wifiNetwork : wifiDev->networks()) {
            addWirelessNetwork(wifiNetwork, wifiDev);
        }
    }

    for (const NetworkManager::Connection::Ptr &connection : device->availableConnections()) {
        addAvailableConnection(connection->path(), device);
    }
}

void QNetworkModel::addWirelessNetwork(const NetworkManager::WirelessNetwork::Ptr &network, const NetworkManager::WirelessDevice::Ptr &device)
{
    initializeSignals(network);

    // BUG: 386342
    // When creating a new hidden wireless network and attempting to connect to it, NM then later reports that AccessPoint appeared, but
    // it doesn't know its SSID from some reason, this also makes Wireless device to advertise a new available connection, which we later
    // attempt to merge with an AP, based on its SSID, but it doesn't find any, because we have AP with empty SSID. After this we get another
    // AccessPoint appeared signal, this time we know SSID, but we don't attempt any merging, because it's usually the other way around, thus
    // we need to attempt to merge it here with a connection we guess it's related to this new AP
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Type, NetworkManager::ConnectionSettings::Wireless)) {
        if (item->itemType() != QNetworkModelItem::AvailableConnection)
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

    NetworkManager::WirelessSetting::NetworkMode mode = NetworkManager::WirelessSetting::Infrastructure;
    NetworkManager::WirelessSecurityType securityType = NetworkManager::UnknownSecurity;

    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();
    if (ap && (ap->capabilities().testFlag(NetworkManager::AccessPoint::Privacy) || ap->wpaFlags() || ap->rsnFlags())) {
        securityType = NetworkManager::findBestWirelessSecurity(device->wirelessCapabilities(),
                                                                true,
                                                                (device->mode() == NetworkManager::WirelessDevice::Adhoc),
                                                                ap->capabilities(),
                                                                ap->wpaFlags(),
                                                                ap->rsnFlags());
        if (network->referenceAccessPoint()->mode() == NetworkManager::AccessPoint::Infra) {
            mode = NetworkManager::WirelessSetting::Infrastructure;
        } else if (network->referenceAccessPoint()->mode() == NetworkManager::AccessPoint::Adhoc) {
            mode = NetworkManager::WirelessSetting::Adhoc;
        } else if (network->referenceAccessPoint()->mode() == NetworkManager::AccessPoint::ApMode) {
            mode = NetworkManager::WirelessSetting::Ap;
        }
    }

    auto item = new QNetworkModelItem();
    if (device->ipInterfaceName().isEmpty()) {
        item->setDeviceName(device->interfaceName());
    } else {
        item->setDeviceName(device->ipInterfaceName());
    }
    item->setDevicePath(device->uni());
    item->setMode(mode);
    item->setName(network->ssid());
    item->setSignal(network->signalStrength());
    item->setSpecificPath(network->referenceAccessPoint()->uni());
    item->setSsid(network->ssid());
    item->setType(NetworkManager::ConnectionSettings::Wireless);
    item->setSecurityType(securityType);
    item->invalidateDetails();

    insertItem(item);
}

void QNetworkModel::checkAndCreateDuplicate(const QString &connection, const QString &deviceUni)
{
    bool createDuplicate = false;
    QNetworkModelItem *originalItem = nullptr;

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Connection, connection)) {
        if (!item->duplicate()) {
            originalItem = item;
        }

        if (!item->duplicate() && item->itemType() == QNetworkModelItem::AvailableConnection
            && (item->devicePath() != deviceUni && !item->devicePath().isEmpty())) {
            createDuplicate = true;
        }
    }

    if (createDuplicate) {
        auto duplicatedItem = new QNetworkModelItem(originalItem);
        duplicatedItem->invalidateDetails();

        insertItem(duplicatedItem);
    }
}

void QNetworkModel::onItemUpdated()
{
    auto item = static_cast<QNetworkModelItem *>(sender());
    if (item) {
        updateItem(item);
    }
}

void QNetworkModel::setDeviceStatisticsRefreshRateMs(const QString &devicePath, uint refreshRate)
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(devicePath);

    if (device) {
        device->deviceStatistics()->setRefreshRateMs(refreshRate);
    }
}

void QNetworkModel::insertItem(QNetworkModelItem *item)
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkModelItem *>(QNetworkModel::ItemAdded, item));
        return;
    }

    const int index = m_list.count();
    beginInsertRows(QModelIndex(), index, index);
    m_list.insertItem(item);
    endInsertRows();
}

void QNetworkModel::removeItem(QNetworkModelItem *item)
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkModelItem *>(QNetworkModel::ItemRemoved, item));
        return;
    }

    const int row = m_list.indexOf(item);
    if (row >= 0) {
        beginRemoveRows(QModelIndex(), row, row);
        m_list.removeItem(item);
        item->deleteLater();
        endRemoveRows();
    }
}

void QNetworkModel::updateItem(QNetworkModelItem *item)
{
    const QVector<int> changedRoles = item->changedRoles();
    // Check only primary roles which can change item order
    if (m_delayModelUpdates && (changedRoles.contains(ConnectionStateRole) || changedRoles.contains(ItemTypeRole) || changedRoles.contains(SignalRole))) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkModelItem *>(QNetworkModel::ItemPropertyChanged, item));
        return;
    }

    const int row = m_list.indexOf(item);

    if (row >= 0) {
        item->invalidateDetails();
        QModelIndex index = createIndex(row, 0);
        Q_EMIT dataChanged(index, index, item->changedRoles());
        item->clearChangedRoles();
    }
}

void QNetworkModel::accessPointSignalStrengthChanged(int signal)
{
    auto apPtr = qobject_cast<NetworkManager::AccessPoint *>(sender());
    if (!apPtr) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Ssid, apPtr->ssid())) {
        if (item->specificPath() == apPtr->uni()) {
            item->setSignal(signal);
            updateItem(item);
        }
    }
}

void QNetworkModel::activeConnectionAdded(const QString &activeConnection)
{
    NetworkManager::ActiveConnection::Ptr activeCon = NetworkManager::findActiveConnection(activeConnection);

    if (activeCon) {
        addActiveConnection(activeCon);
    }
}

void QNetworkModel::activeConnectionRemoved(const QString &activeConnection)
{
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::ActiveConnection, activeConnection)) {
        item->setActiveConnectionPath(QString());
        item->setConnectionState(NetworkManager::ActiveConnection::Deactivated);
        updateItem(item);
    }
}

void QNetworkModel::activeConnectionStateChanged(NetworkManager::ActiveConnection::State state)
{
    auto activePtr = qobject_cast<NetworkManager::ActiveConnection *>(sender());

    if (!activePtr) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::ActiveConnection, activePtr->path())) {
        item->setConnectionState(state);
        updateItem(item);
    }
}

void QNetworkModel::availableConnectionAppeared(const QString &connection)
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());
    if (!device) {
        return;
    }

    addAvailableConnection(connection, device);
}

void QNetworkModel::availableConnectionDisappeared(const QString &connection)
{
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Connection, connection)) {
        bool available = false;
        const QString devicePath = item->devicePath();
        const QString specificPath = item->specificPath();

        // We have to check whether the connection is still available, because it might be
        // presented in the model for more devices and we don't want to remove it for all of them.

        // Check whether the device is still available
        NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(devicePath);
        if (device) {
            // Check whether the connection is still listed as available
            for (const NetworkManager::Connection::Ptr &connection : device->availableConnections()) {
                if (connection->path() == item->connectionPath()) {
                    available = true;
                    break;
                }
            }
        }

        if (!available) {
            item->setDeviceName(QString());
            item->setDevicePath(QString());
            item->setDeviceState(NetworkManager::Device::UnknownState);
            item->setSignal(0);
            item->setSpecificPath(QString());
            // Check whether the connection is still available as an access point, this happens
            // when we change its properties, like ssid, bssid, security etc.
            if (item->type() == NetworkManager::ConnectionSettings::Wireless && !specificPath.isEmpty()) {
                if (device && device->type() == NetworkManager::Device::Wifi) {
                    NetworkManager::WirelessDevice::Ptr wifiDevice = device.objectCast<NetworkManager::WirelessDevice>();
                    if (wifiDevice) {
                        NetworkManager::AccessPoint::Ptr ap = wifiDevice->findAccessPoint(specificPath);
                        if (ap) {
                            NetworkManager::WirelessNetwork::Ptr network = wifiDevice->findNetwork(ap->ssid());
                            if (network) {
                                addWirelessNetwork(network, wifiDevice);
                            }
                        }
                    }
                }
            }

            if (item->duplicate()) {
                removeItem(item);
            } else {
                updateItem(item);
            }
        }
        available = false;
    }
}

void QNetworkModel::connectionAdded(const QString &connection)
{
    NetworkManager::Connection::Ptr newConnection = NetworkManager::findConnection(connection);
    if (newConnection) {
        addConnection(newConnection);
    }
}

void QNetworkModel::connectionRemoved(const QString &connection)
{
    bool remove = false;
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Connection, connection)) {
        // When the item type is wireless, we can remove only the connection and leave it as an available access point
        if (item->type() == NetworkManager::ConnectionSettings::Wireless && !item->devicePath().isEmpty()) {
            for (QNetworkModelItem *secondItem : m_list.items()) {
                // Remove it entirely when there is another connection with the same configuration and for the same device
                // or it's a shared connection
                if ((item->mode() != NetworkManager::WirelessSetting::Infrastructure)
                    || (item->connectionPath() != secondItem->connectionPath() //
                        && item->devicePath() == secondItem->devicePath() //
                        && item->mode() == secondItem->mode() //
                        && item->securityType() == secondItem->securityType() //
                        && item->ssid() == secondItem->ssid())) {
                    remove = true;
                    break;
                }
            }

            if (!remove) {
                item->setConnectionPath(QString());
                item->setName(item->ssid());
                item->setSlave(false);
                item->setTimestamp(QDateTime());
                item->setUuid(QString());
                updateItem(item);
            }
        } else {
            remove = true;
        }

        if (remove) {
            removeItem(item);
        }
        remove = false;
    }
}

void QNetworkModel::connectionUpdated()
{
    auto connectionPtr = qobject_cast<NetworkManager::Connection *>(sender());
    if (!connectionPtr) {
        return;
    }

    NetworkManager::ConnectionSettings::Ptr settings = connectionPtr->settings();
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Connection, connectionPtr->path())) {
        item->setConnectionPath(connectionPtr->path());
        item->setName(settings->id());
        item->setTimestamp(settings->timestamp());
        item->setType(settings->connectionType());
        item->setUuid(settings->uuid());

        if (item->type() == NetworkManager::ConnectionSettings::Wireless) {
            NetworkManager::WirelessSetting::Ptr wirelessSetting;
            wirelessSetting = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
            item->setMode(wirelessSetting->mode());
            item->setSecurityType(NetworkManager::securityTypeFromConnectionSetting(settings));
            item->setSsid(QString::fromUtf8(wirelessSetting->ssid()));
            // TODO check whether BSSID has changed and update the wireless info
        }

        updateItem(item);
    }
}

void QNetworkModel::deviceAdded(const QString &device)
{
    NetworkManager::Device::Ptr dev = NetworkManager::findNetworkInterface(device);
    if (dev) {
        addDevice(dev);
    }
}

void QNetworkModel::deviceRemoved(const QString &device)
{
    // Make all items unavailable
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Device, device)) {
        availableConnectionDisappeared(item->connectionPath());
    }
}

void QNetworkModel::deviceStateChanged(NetworkManager::Device::State state,
                                      NetworkManager::Device::State oldState,
                                      NetworkManager::Device::StateChangeReason reason)
{
    Q_UNUSED(oldState);
    Q_UNUSED(reason);

    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());

    if (!device) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Device, device->uni())) {
        item->setDeviceState(state);
        updateItem(item);
    }
}

void QNetworkModel::ipConfigChanged()
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());

    if (!device) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Device, device->uni())) {
        updateItem(item);
    }
}

void QNetworkModel::ipInterfaceChanged()
{
    auto device = qobject_cast<NetworkManager::Device *>(sender());
    if (!device) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Device, device->uni())) {
        if (device->ipInterfaceName().isEmpty()) {
            item->setDeviceName(device->interfaceName());
        } else {
            item->setDeviceName(device->ipInterfaceName());
        }
    }
}

void QNetworkModel::statusChanged(NetworkManager::Status status)
{
    Q_UNUSED(status);

    // This has probably effect only for VPN connections
    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Type, NetworkManager::ConnectionSettings::Vpn)) {
        updateItem(item);
    }
}

void QNetworkModel::wirelessNetworkAppeared(const QString &ssid)
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());
    if (device && device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wirelessDevice = device.objectCast<NetworkManager::WirelessDevice>();
        NetworkManager::WirelessNetwork::Ptr network = wirelessDevice->findNetwork(ssid);
        addWirelessNetwork(network, wirelessDevice);
    }
}

void QNetworkModel::wirelessNetworkDisappeared(const QString &ssid)
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());
    if (!device) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Ssid, ssid, device->uni())) {
        // Remove the entire item, because it's only AP or it's a duplicated available connection
        if (item->itemType() == QNetworkModelItem::AvailableAccessPoint || item->duplicate()) {
            removeItem(item);
            // Remove only AP and device from the item and leave it as an unavailable connection
        } else {
            if (item->mode() == NetworkManager::WirelessSetting::Infrastructure) {
                item->setDeviceName(QString());
                item->setDevicePath(QString());
                item->setSpecificPath(QString());
            }
            item->setSignal(0);
            updateItem(item);
        }
    }
}

void QNetworkModel::wirelessNetworkReferenceApChanged(const QString &accessPoint)
{
    auto networkPtr = qobject_cast<NetworkManager::WirelessNetwork *>(sender());

    if (!networkPtr) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Ssid, networkPtr->ssid(), networkPtr->device())) {
        NetworkManager::Connection::Ptr connection = NetworkManager::findConnection(item->connectionPath());
        if (!connection) {
            continue;
        }

        NetworkManager::WirelessSetting::Ptr wirelessSetting =
            connection->settings()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
        if (!wirelessSetting) {
            continue;
        }

        if (wirelessSetting->bssid().isEmpty()) {
            item->setSpecificPath(accessPoint);
            updateItem(item);
        }
    }
}

void QNetworkModel::wirelessNetworkSignalChanged(int signal)
{
    auto networkPtr = qobject_cast<NetworkManager::WirelessNetwork *>(sender());
    if (!networkPtr) {
        return;
    }

    for (QNetworkModelItem *item : m_list.returnItems(QNetworkList::Ssid, networkPtr->ssid(), networkPtr->device())) {
        if (item->specificPath() == networkPtr->referenceAccessPoint()->uni()) {
            item->setSignal(signal);
            updateItem(item);
            // qCDebug(PLASMA_NM_LIBS_LOG) << "Wireless network " << item->name() << ": signal changed to " << item->signal();
        }
    }
}

NetworkManager::WirelessSecurityType QNetworkModel::alternativeWirelessSecurity(const NetworkManager::WirelessSecurityType type)
{
    if (type == NetworkManager::WpaPsk) {
        return NetworkManager::Wpa2Psk;
    } else if (type == NetworkManager::WpaEap) {
        return NetworkManager::Wpa2Eap;
    } else if (type == NetworkManager::Wpa2Psk) {
        return NetworkManager::WpaPsk;
    } else if (type == NetworkManager::Wpa2Eap) {
        return NetworkManager::WpaEap;
    }
    return type;
}

void QNetworkModel::updateFromWirelessNetwork(QNetworkModelItem *item,
                                             const NetworkManager::WirelessNetwork::Ptr &network,
                                             const NetworkManager::WirelessDevice::Ptr &device)
{
    NetworkManager::WirelessSecurityType securityType = NetworkManager::UnknownSecurity;
    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();
    if (ap && ap->capabilities().testFlag(NetworkManager::AccessPoint::Privacy)) {
        securityType = NetworkManager::findBestWirelessSecurity(device->wirelessCapabilities(),
                                                                true,
                                                                (device->mode() == NetworkManager::WirelessDevice::Adhoc),
                                                                ap->capabilities(),
                                                                ap->wpaFlags(),
                                                                ap->rsnFlags());
    }

    // Check whether the connection is associated with some concrete AP
    NetworkManager::Connection::Ptr connection = NetworkManager::findConnection(item->connectionPath());
    if (connection) {
        NetworkManager::WirelessSetting::Ptr wirelessSetting =
            connection->settings()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
        if (wirelessSetting) {
            if (!wirelessSetting->bssid().isEmpty()) {
                for (const NetworkManager::AccessPoint::Ptr &ap : network->accessPoints()) {
                    if (ap->hardwareAddress() == NetworkManager::macAddressAsString(wirelessSetting->bssid())) {
                        item->setSignal(ap->signalStrength());
                        item->setSpecificPath(ap->uni());
                        // We need to watch this AP for signal changes
                        connect(ap.data(),
                                &NetworkManager::AccessPoint::signalStrengthChanged,
                                this,
                                &QNetworkModel::accessPointSignalStrengthChanged,
                                Qt::UniqueConnection);
                    }
                }
            } else {
                item->setSignal(network->signalStrength());
                item->setSpecificPath(network->referenceAccessPoint()->uni());
            }
        }
    }
    item->setSecurityType(securityType);
    updateItem(item);
}
