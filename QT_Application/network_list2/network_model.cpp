#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "network_model.h"
#include "network_model_item.h"

QNetworkModel::QNetworkModel(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (const QString &header : headers) {
        rootData << header;
    }
    rootItem = new QNetworkItem(rootData);
    setupModelData(data.split('\n'), rootItem);

    // Initialize existing connections
    QVector<QVariant> list;
    for (const NetworkManager::Connection::Ptr &connection : NetworkManager::listConnections()) {

        addConnection(connection, list);
    }

    // Initialize existing devices
    for (const NetworkManager::Device::Ptr &dev : NetworkManager::networkInterfaces()) {
        if (!dev->managed()) {
            continue;
        }
        addDevice(dev);
    }
}

QNetworkModel::~QNetworkModel()
{
    delete rootItem;
}



/* new */
QModelIndex QNetworkModel::index(int row, int column,
                  const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QNetworkItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
       parentItem = static_cast<QNetworkItem*>(parent.internalPointer());

    QNetworkItem *childItem = parentItem->child(row);

    return QModelIndex();

}

QModelIndex QNetworkModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QNetworkItem *childItem = static_cast<QNetworkItem*>(index.internalPointer());
    QNetworkItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QNetworkModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<QNetworkItem*>(parent.internalPointer())->columnCount();

    return rootItem->columnCount();
}

int QNetworkModel::rowCount(const QModelIndex &parent) const
{
    QNetworkItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<QNetworkItem*>(parent.internalPointer());

    return parentItem->childCount();
}

/* remove */
QNetworkItem *QNetworkModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}


QVariant QNetworkModel::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QVariant QNetworkModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());

    return item->data(index.column());

/*
    const int row = index.row();

    if (row >= 0 && row < m_list.count()) {
        QNetworkItem *item = m_itemData;

        switch (role) {
        case ConnectionPathRole:
            return item->connectionPath();
        case DevicePathRole:
            return item->devicePath();
        case ItemTypeRole:
            return item->itemType();
        case SsidRole:
            return item->ssid();
        case TypeRole:
            return item->type();
        case UniRole:
            return item->uni();
        case UuidRole:
            return item->uuid();
        default:
            break;
        }
    }

    return {};
*/
}

Qt::ItemFlags QNetworkModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}


void QNetworkModel::setupModelData(const QStringList &lines, QNetworkItem *parent)
{
    QList<QNetworkItem *> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings =
                lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
            QList<QVariant> columnData;
            columnData.reserve(columnStrings.size());
            for (const QString &columnString : columnStrings)
                columnData << columnString;

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            QNetworkItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }
        ++number;
    }
}


void QNetworkModel::addConnection(const NetworkManager::Connection::Ptr &connection, QVector<QVariant> &list)
{
    // Can't add a connection without name or uuid
    if (connection->name().isEmpty() || connection->uuid().isEmpty()) {
        return;
    }

    NetworkManager::ConnectionSettings::Ptr settings = connection->settings();
    NetworkManager::WirelessSetting::Ptr wirelessSetting;

    if (settings->connectionType() == NetworkManager::ConnectionSettings::Wireless) {
        wirelessSetting = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    }

    // Check whether the connection is already in the model to avoid duplicates, but this shouldn't happen
//    if (list.contains(QNetworkList::Connection, connection->path())) {
//        return;
//    }

    auto item = new QNetworkItem(list);
    item->setConnectionPath(connection->path());
    item->setType(settings->connectionType());
    item->setUuid(settings->uuid());

    if (item->type() == NetworkManager::ConnectionSettings::Wireless) {
        item->setMode(wirelessSetting->mode());
        item->setSsid(QString::fromUtf8(wirelessSetting->ssid()));
    }

    insertItem(item);
}

bool QNetworkModel::insertRows(int position, int rows,
                               const QModelIndex &parent);
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemAdded, item));
        return;
    }

    const int index = m_list.count();
    beginInsertRows(QModelIndex(), index, index);
    m_list.insertItem(item);
    endInsertRows();
}

/*
void QNetworkModel::addConnection(const NetworkManager::Connection::Ptr &connection)
{
    // Can't add a connection without name or uuid
    if (connection->name().isEmpty() || connection->uuid().isEmpty()) {
        return;
    }

    NetworkManager::ConnectionSettings::Ptr settings = connection->settings();
    NetworkManager::WirelessSetting::Ptr wirelessSetting;

    if (settings->connectionType() == NetworkManager::ConnectionSettings::Wireless) {
        wirelessSetting = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    }

    // Check whether the connection is already in the model to avoid duplicates, but this shouldn't happen
    if (m_list.contains(QNetworkList::Connection, connection->path())) {
        return;
    }

    auto item = new QNetworkItem();
    item->setConnectionPath(connection->path());
    item->setName(settings->id());
    item->setTimestamp(settings->timestamp());
    item->setType(settings->connectionType());
    item->setUuid(settings->uuid());
    item->setSlave(settings->isSlave());

    if (item->type() == NetworkManager::ConnectionSettings::Wireless) {
        item->setMode(wirelessSetting->mode());
        item->setSsid(QString::fromUtf8(wirelessSetting->ssid()));
    }

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

    NetworkManager::WirelessSetting::NetworkMode mode = NetworkManager::WirelessSetting::Infrastructure;
    NetworkManager::WirelessSecurityType securityType = NetworkManager::UnknownSecurity;

    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();

    auto item = new QNetworkItem();
    if (device->ipInterfaceName().isEmpty()) {
        item->setDeviceName(device->interfaceName());
    } else {
        item->setDeviceName(device->ipInterfaceName());
    }
    item->setDevicePath(device->uni());
    item->setMode(mode);
    item->setName(network->ssid());
    item->setSpecificPath(network->referenceAccessPoint()->uni());
    item->setSsid(network->ssid());
    item->setType(NetworkManager::ConnectionSettings::Wireless);

    insertItem(item);
}

void QNetworkModel::connectionAdded(const QString &connection)
{
    NetworkManager::Connection::Ptr newConnection = NetworkManager::findConnection(connection);
    if (newConnection) {
        addConnection(newConnection);
    }
}

void QNetworkModel::connectionUpdated()
{
    auto connectionPtr = qobject_cast<NetworkManager::Connection *>(sender());
    if (!connectionPtr) {
        return;
    }

    NetworkManager::ConnectionSettings::Ptr settings = connectionPtr->settings();
    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Connection, connectionPtr->path())) {
        item->setConnectionPath(connectionPtr->path());
        item->setName(settings->id());
        item->setTimestamp(settings->timestamp());
        item->setType(settings->connectionType());
        item->setUuid(settings->uuid());

        if (item->type() == NetworkManager::ConnectionSettings::Wireless) {
            NetworkManager::WirelessSetting::Ptr wirelessSetting;
            wirelessSetting = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
            item->setMode(wirelessSetting->mode());
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

void QNetworkModel::updateFromWirelessNetwork(QNetworkItem *item,
                                             const NetworkManager::WirelessNetwork::Ptr &network,
                                             const NetworkManager::WirelessDevice::Ptr &device)
{
    NetworkManager::WirelessSecurityType securityType = NetworkManager::UnknownSecurity;
    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();

    // Check whether the connection is associated with some concrete AP
    NetworkManager::Connection::Ptr connection = NetworkManager::findConnection(item->connectionPath());
    if (connection) {
        NetworkManager::WirelessSetting::Ptr wirelessSetting =
            connection->settings()->setting(NetworkManager::Setting::Wireless).staticCast<NetworkManager::WirelessSetting>();
        if (wirelessSetting) {
            if (!wirelessSetting->bssid().isEmpty()) {
                for (const NetworkManager::AccessPoint::Ptr &ap : network->accessPoints()) {
                    if (ap->hardwareAddress() == NetworkManager::macAddressAsString(wirelessSetting->bssid())) {
                        item->setSpecificPath(ap->uni());
                        // We need to watch this AP for signal changes
                        connect(ap.data(),
                                &NetworkManager::AccessPoint::signalStrengthChanged,
                                this,
                                &QNetworkModel::accessPointSignalStrengthChanged,
                                Qt::UniqueConnection);
                    }
                }
            }
            else {
                item->setSpecificPath(network->referenceAccessPoint()->uni());
            }
        }
    }
    updateItem(item);
}


void QNetworkModel::insertItem(QNetworkItem *item)
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemAdded, item));
        return;
    }

    const int index = m_list.count();
    beginInsertRows(QModelIndex(), index, index);
    m_list.insertItem(item);
    endInsertRows();
}

void QNetworkModel::removeItem(QNetworkItem *item)
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemRemoved, item));
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

void QNetworkModel::updateItem(QNetworkItem *item)
{
    const QVector<int> changedRoles = item->changedRoles();
    // Check only primary roles which can change item order
    if (m_delayModelUpdates && (changedRoles.contains(ConnectionStateRole) || changedRoles.contains(ItemTypeRole))) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemPropertyChanged, item));
        return;
    }

    const int row = m_list.indexOf(item);

    if (row >= 0) {
        QModelIndex index = createIndex(row, 0);
        Q_EMIT dataChanged(index, index, item->changedRoles());
        item->clearChangedRoles();
    }
}

void QNetworkModel::setDelayModelUpdates(bool delayUpdates)
{
    m_delayModelUpdates = delayUpdates;

    // Process queue
    if (!delayUpdates) {
        while (!m_updateQueue.isEmpty()) {
            QPair<QNetworkModel::ModelChangeType, QNetworkItem *> update = m_updateQueue.dequeue();
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
*/



/* old */
/*
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
}

void QNetworkModel::initializeSignals(const NetworkManager::Connection::Ptr &connection)
{
    connect(connection.data(), &NetworkManager::Connection::updated, this, &QNetworkModel::connectionUpdated, Qt::UniqueConnection);
}

void QNetworkModel::initializeSignals(const NetworkManager::Device::Ptr &device)
{
    connect(device.data(), &NetworkManager::Device::ipV4ConfigChanged, this, &QNetworkModel::ipConfigChanged, Qt::UniqueConnection);
    connect(device.data(), &NetworkManager::Device::ipV6ConfigChanged, this, &QNetworkModel::ipConfigChanged, Qt::UniqueConnection);
    connect(device.data(), &NetworkManager::Device::ipInterfaceChanged, this, &QNetworkModel::ipInterfaceChanged);
    connect(device.data(), &NetworkManager::Device::stateChanged, this, &QNetworkModel::deviceStateChanged, Qt::UniqueConnection);

    if (device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wifiDev = device.objectCast<NetworkManager::WirelessDevice>();
        connect(wifiDev.data(), &NetworkManager::WirelessDevice::networkAppeared, this, &QNetworkModel::wirelessNetworkAppeared, Qt::UniqueConnection);
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

void QNetworkModel::checkAndCreateDuplicate(const QString &connection, const QString &deviceUni)
{
    bool createDuplicate = false;
    QNetworkItem *originalItem = nullptr;

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Connection, connection)) {
        if (!item->duplicate()) {
            originalItem = item;
        }

        if (!item->duplicate() && item->itemType() == QNetworkItem::AvailableConnection
            && (item->devicePath() != deviceUni && !item->devicePath().isEmpty())) {
            createDuplicate = true;
        }
    }

    if (createDuplicate) {
        auto duplicatedItem = new QNetworkItem(originalItem);
        insertItem(duplicatedItem);
    }
}

void QNetworkModel::onItemUpdated()
{
    auto item = static_cast<QNetworkItem *>(sender());
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

void QNetworkModel::insertItem(QNetworkItem *item)
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemAdded, item));
        return;
    }

    const int index = m_list.count();
    beginInsertRows(QModelIndex(), index, index);
    m_list.insertItem(item);
    endInsertRows();
}

void QNetworkModel::removeItem(QNetworkItem *item)
{
    if (m_delayModelUpdates) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemRemoved, item));
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

void QNetworkModel::updateItem(QNetworkItem *item)
{
    const QVector<int> changedRoles = item->changedRoles();
    // Check only primary roles which can change item order
    if (m_delayModelUpdates && (changedRoles.contains(ConnectionStateRole) || changedRoles.contains(ItemTypeRole))) {
        m_updateQueue.enqueue(QPair<QNetworkModel::ModelChangeType, QNetworkItem *>(QNetworkModel::ItemPropertyChanged, item));
        return;
    }

    const int row = m_list.indexOf(item);

    if (row >= 0) {
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

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Ssid, apPtr->ssid())) {
        if (item->specificPath() == apPtr->uni()) {
            updateItem(item);
        }
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

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Device, device->uni())) {
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

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Device, device->uni())) {
        updateItem(item);
    }
}

void QNetworkModel::ipInterfaceChanged()
{
    auto device = qobject_cast<NetworkManager::Device *>(sender());
    if (!device) {
        return;
    }

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Device, device->uni())) {
        if (device->ipInterfaceName().isEmpty()) {
            item->setDeviceName(device->interfaceName());
        } else {
            item->setDeviceName(device->ipInterfaceName());
        }
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

void QNetworkModel::wirelessNetworkReferenceApChanged(const QString &accessPoint)
{
    auto networkPtr = qobject_cast<NetworkManager::WirelessNetwork *>(sender());

    if (!networkPtr) {
        return;
    }

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Ssid, networkPtr->ssid(), networkPtr->device())) {
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

    for (QNetworkItem *item : m_list.returnItems(QNetworkList::Ssid, networkPtr->ssid(), networkPtr->device())) {
        if (item->specificPath() == networkPtr->referenceAccessPoint()->uni()) {
            updateItem(item);
            // qCDebug(PLASMA_NM_LIBS_LOG) << "Wireless network " << item->name() << ": signal changed to " << item->signal();
        }
    }
}


*/
