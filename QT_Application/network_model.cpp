#include <QIcon>

#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "network_model.h"
#include "network_model_item.h"

QNetworkModel::QNetworkModel(const QVector<QNetworkModel::ItemRole> &columnRoles, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (int i = 0; i < columnRoles.count(); ++i) {
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

    // Initialize existing connections
    for (const NetworkManager::Device::Ptr &dev : NetworkManager::networkInterfaces()) {
        if (!dev->managed()) {
            continue;
        }
        addDevice(dev, rootItem);
    }

    setupModelData(columnRoles, rootItem);
}

QNetworkModel::~QNetworkModel()
{
    delete rootItem;
}

/* Tree Model */
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
}

bool QNetworkModel::setData(const QModelIndex &index, const QVariant &value,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    QNetworkItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

QVariant QNetworkModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool QNetworkModel::setHeaderData(int section, Qt::Orientation orientation,
                   const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}


QModelIndex QNetworkModel::index(int row, int column,
                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    QNetworkItem *parentItem = getItem(parent);

    if (!parentItem)
        return QModelIndex();

    QNetworkItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

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

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


int QNetworkModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const QNetworkItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool QNetworkModel::insertRows(int position, int rows,
                               const QModelIndex &parent)
{
    QNetworkItem *parentItem = getItem(parent);
    if(!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows();

    return success;
}

bool QNetworkModel::removeRows(int position, int rows,
                               const QModelIndex &parent)
{
    QNetworkItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int QNetworkModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

bool QNetworkModel::insertColumns(int position, int columns,
                                  const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool QNetworkModel::removeColumns(int position, int columns,
                   const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns -1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}


QNetworkItem *QNetworkModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

Qt::ItemFlags QNetworkModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}


void QNetworkModel::setupModelData(const QVector<QNetworkModel::ItemRole> &roles, QNetworkItem *parent)
{
    QList<QNetworkItem *> parents;
    parents << parent;

    for (int i = 0; i < parent->childCount(); ++i) {
        QNetworkItem *item = parent->child(i);

        for (int j = 0; j < parent->columnCount(); ++j) {
            switch (roles.at(j)) {
            case DeviceName:
                item->setData(j, item->deviceName());
                break;

            case DevicePathRole:
                item->setData(j, item->devicePath());
                break;

            case ConnectionIconRole:
                item->setData(j, QIcon::fromTheme(item->icon()));
                break;

            case SpecificPathRole:
                item->setData(j, item->specificPath());
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


void QNetworkModel::addDevice(const NetworkManager::Device::Ptr &device, QNetworkItem *parent)
{
    initializeSignals(device);

    if (device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wifiDev = device.objectCast<NetworkManager::WirelessDevice>();
        for (const NetworkManager::WirelessNetwork::Ptr &wifiNetwork : wifiDev->networks()) {
            addWirelessNetwork(wifiNetwork, wifiDev, parent);
        }
    }

}

void QNetworkModel::addWirelessNetwork(const NetworkManager::WirelessNetwork::Ptr &network,
                                       const NetworkManager::WirelessDevice::Ptr &device,
                                       QNetworkItem *parent)
{
    initializeSignals(network);

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


    QNetworkItem *item = parents.last();

    item->insertChildren(item->childCount(), 1,
                         rootItem->columnCount());

    if (device->ipInterfaceName().isEmpty()) {
        item->child(item->childCount()-1)->setDeviceName(device->interfaceName());
    }
    else {
        item->child(item->childCount()-1)->setDeviceName(device->ipInterfaceName());
    }
    item->child(item->childCount()-1)->setSsid(network->ssid());
    item->child(item->childCount()-1)->setDevicePath(device->uni());
    item->child(item->childCount()-1)->setSpecificPath(network->referenceAccessPoint()->uni());
    item->child(item->childCount()-1)->setType(NetworkManager::ConnectionSettings::Wireless);
//    item->setMode(mode);
//    item->setName(network->ssid());

//    item->setSsid(network->ssid());
//    item->setType(NetworkManager::ConnectionSettings::Wireless);

//    insertRows(position, 1);
}


void QNetworkModel::initializeSignals()
{
//    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionAdded, this, &QNetworkModel::activeConnectionAdded, Qt::UniqueConnection);
//    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionRemoved, this, &QNetworkModel::activeConnectionRemoved, Qt::UniqueConnection);
//    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded, this, &QNetworkModel::deviceAdded, Qt::UniqueConnection);
//    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceRemoved, this, &QNetworkModel::deviceRemoved, Qt::UniqueConnection);
//    connect(NetworkManager::notifier(), &NetworkManager::Notifier::statusChanged, this, &QNetworkModel::statusChanged, Qt::UniqueConnection);
}

void QNetworkModel::initializeSignals(const NetworkManager::Connection::Ptr &connection)
{
//    connect(connection.data(), &NetworkManager::Connection::updated, this, &QNetworkModel::connectionUpdated, Qt::UniqueConnection);
}

void QNetworkModel::initializeSignals(const NetworkManager::Device::Ptr &device)
{
//    connect(device.data(), &NetworkManager::Device::ipV4ConfigChanged, this, &QNetworkModel::ipConfigChanged, Qt::UniqueConnection);
//    connect(device.data(), &NetworkManager::Device::ipV6ConfigChanged, this, &QNetworkModel::ipConfigChanged, Qt::UniqueConnection);
//    connect(device.data(), &NetworkManager::Device::ipInterfaceChanged, this, &QNetworkModel::ipInterfaceChanged);
//    connect(device.data(), &NetworkManager::Device::stateChanged, this, &QNetworkModel::deviceStateChanged, Qt::UniqueConnection);

    if (device->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice::Ptr wifiDev = device.objectCast<NetworkManager::WirelessDevice>();
//        connect(wifiDev.data(), &NetworkManager::WirelessDevice::networkAppeared, this, &QNetworkModel::wirelessNetworkAppeared, Qt::UniqueConnection);
    }
}

void QNetworkModel::initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network)
{
//    connect(network.data(), &NetworkManager::WirelessNetwork::signalStrengthChanged, this, &QNetworkModel::wirelessNetworkSignalChanged, Qt::UniqueConnection);
//    connect(network.data(),
//            &NetworkManager::WirelessNetwork::referenceAccessPointChanged,
//            this,
//            &QNetworkModel::wirelessNetworkReferenceApChanged,
//            Qt::UniqueConnection);
}
