/* Tree Network Model
** File: network_model.cpp
** --------
** Scan for local networks and display relevent info
** in a Tree Model
** --------
*/

/* Qt include files */
#include <QDBusInterface>
#include <QIcon>
#include <QTimer>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

/* local include files */
#include "network_model.h"
#include "network_model_p.h"
#include "network_item.h"


/* Constructor */
NetworkModel::NetworkModel (const QVector<ItemRole> &roles,
                              QObject *parent)
    : QAbstractItemModel (parent),
      m_scanHandler (new QNetworkScan (this))

{
    // Create header titles based on column roles
    QVector<QVariant> rootData;
    for (int i = 0; i < roles.count(); ++i)
    {
        switch (roles.at(i)) {
        case ItemRole::DeviceName:
            rootData << "Device Name";
            break;

        case ItemRole::DevicePathRole:
            rootData << "Device Path";
            break;

        case ItemRole::ConnectionIconRole:
            rootData << "";
            break;

        case ItemRole::SpecificPathRole:
            rootData << "Specific Path";
            break;

        case ItemRole::SecurityTypeRole:
            rootData << "Security";
            break;

        case ItemRole::SsidRole:
            rootData << "Ssid";
            break;

        case ItemRole::TypeRole:
            rootData << "Type";
            break;

        default:
            break;
        }
    }
    rootItem = new NetworkItem(rootData);
    columnRoles = roles;

    // Initialize first scan and then scan every 2 seconds
    m_scanHandler = new QNetworkScan (this);
    m_scanHandler->requestScan();

    m_timer = new QTimer (this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this,
            [&](){m_scanHandler->requestScan();});
    m_timer->start();


    // Initialize existing connections
    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        if (!dev->managed())
            continue;

        addDevice (dev, rootItem);
    }
}

/* Constructor for private functions */
NetworkModel::NetworkModel (NetworkModelPrivate &dd)
    : d_ptr (&dd)
{
}

/* Destructor */
NetworkModel::~NetworkModel ()
{
    delete rootItem;
}


/* Tree Model
** --------
** reimplement functions from Tree Model
** --------
**/

/* Return data from model
** Parameters:
**     index: specify model index for row and column
**     role: controls the display method for the model contents
** Return: value stored in the model index
*/
QVariant NetworkModel::data (const QModelIndex &index, int role) const
{
    // Checks for valid index
    if (!index.isValid())
        return QVariant();

    //qDebug() << role;

    NetworkItem *item = static_cast<NetworkItem*>(index.internalPointer());
    const int row = index.row();
/*
    if (row >= 0 && row < item->childCount())
    {
        switch (role) {
        case ConnectionIconRole:
            return item->icon();
        case ConnectionStateRole:
            return item->connectionState();
        case DeviceName:
            return item->deviceName();
        case DevicePathRole:
            return item->devicePath();
        case SsidRole:
            return item->ssid();
        case SpecificPathRole:
            return item->specificPath();
        case SecurityTypeRole:
            return item->securityType();
        case TypeRole:
            return item->type();
        case UuidRole:
            return item->uuid();
        default:
            break;
        }
    }
*/
    // Define roles that are allowed
    // (all others return empty value)
    if (role != Qt::DisplayRole &&
        role != Qt::EditRole &&
        role != Qt::DecorationRole)
    {
        return QVariant();
    }

    return item->data(index.column());
}

/* Store value in model index */
bool NetworkModel::setData (const QModelIndex &index,
                             const QVariant &value,
                             int role)
{
    if (role != Qt::EditRole)
        return false;

    NetworkItem *item = d_ptr->getItem (index, this);
    index.column();

    bool result = item->setData (index.column(), value);
    if (result)
        Q_EMIT dataChanged (index, index, item->changedRoles());
        item->clearChangedRoles();


    return result;
}


/* Return header data from model
** Parameters:
**     index: specify model index for row and column
**     orientation: control location of header (vertical or horizontal)
**     role: controls the display method for the model contents
** Return: value stored in the model index
*/
QVariant NetworkModel::headerData (int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

/* Store header data in model index */
bool NetworkModel::setHeaderData (int section,
                                   Qt::Orientation orientation,
                                   const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setHeaderData (section, value);

    if (result)
        emit headerDataChanged (orientation, section, section);

    return result;
}


/* Return index to model */
QModelIndex NetworkModel::index (int row, int column,
                                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    NetworkItem *parentItem = d_ptr->getItem (parent, this);

    if (!parentItem)
        return QModelIndex();

    NetworkItem *childItem = parentItem->child (row);
    if (childItem)
        return createIndex (row, column, childItem);

    return QModelIndex();
}


/* Return index to parent in model */
QModelIndex NetworkModel::parent (const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    NetworkItem *childItem = static_cast<NetworkItem*>(index.internalPointer());
    NetworkItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


/* Return total number of rows */
int NetworkModel::rowCount (const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const NetworkItem *parentItem = d_ptr->getItem(parent, this);

    return parentItem ? parentItem->childCount() : 0;
}


/* Insert number of rows below specified position */
bool NetworkModel::insertRows (int position, int rows,
                                const QModelIndex &parent)
{
    NetworkItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginInsertRows (parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows ();

    return success;
}

/* Remove number of rows below specified position */
bool NetworkModel::removeRows (int position, int rows,
                                const QModelIndex &parent)
{
    NetworkItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginRemoveRows (parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren (position, rows);
    endRemoveRows ();

    return success;
}


/* Return total number of columns */
int NetworkModel::columnCount (const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

/* Insert number of columns below specified position */
bool NetworkModel::insertColumns (int position, int columns,
                                   const QModelIndex &parent)
{
    beginInsertColumns (parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns (position, columns);
    endInsertColumns ();

    return success;
}

/* Remove number of columns below specified position */
bool NetworkModel::removeColumns (int position, int columns,
                                   const QModelIndex &parent)
{
    beginRemoveColumns (parent, position, position + columns -1);
    const bool success = rootItem->removeColumns (position, columns);
    endRemoveColumns ();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}


/* Returns list of item flags present on index (See Qt::ItemFlags) */
Qt::ItemFlags NetworkModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
}


void NetworkModel::sort(int column, Qt::SortOrder order)
{

}



/* Network Model
** --------
** Implement functions to add wireless networks to the model
** --------
**/
QHash<int, QByteArray> NetworkModel::roleNames() const
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
    roles[HeaderRole] = "Header";
    roles[ItemUniqueNameRole] = "ItemUniqueName";
    roles[ItemTypeRole] = "ItemType";
    roles[NameRole] = "Name";
    roles[SectionRole] = "Section";
    roles[SlaveRole] = "Slave";
    roles[SsidRole] = "Ssid";
    roles[SpecificPathRole] = "SpecificPath";
    roles[SecurityTypeRole] = "SecurityType";
    roles[SecurityTypeStringRole] = "SecurityTypeString";
    roles[TimeStampRole] = "TimeStamp";
    roles[TypeRole] = "Type";
    roles[UniRole] = "Uni";
    roles[UuidRole] = "Uuid";

    return roles;
}


void NetworkModel::addDevice (const NetworkManager::Device::Ptr &device,
                               NetworkItem *parent)
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


void NetworkModel::addWirelessNetwork (const NetworkManager::WirelessNetwork::Ptr &network,
                                        const NetworkManager::WirelessDevice::Ptr &device,
                                        NetworkItem *parent)
{
    d_ptr->initializeSignals(network);

    QVector<NetworkItem *> parents;
    parents << parent;

    // Set default security info
    NetworkManager::WirelessSetting::NetworkMode mode =
            NetworkManager::WirelessSetting::Infrastructure;
    NetworkManager::WirelessSecurityType securityType =
            NetworkManager::UnknownSecurity;

    // Look for security info
    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();
    if (ap && (ap->capabilities().testFlag(NetworkManager::AccessPoint::Privacy)
        || ap->wpaFlags() || ap->rsnFlags()))
    {
        securityType =
                NetworkManager::findBestWirelessSecurity
                (device->wirelessCapabilities(), true,
                 (device->mode() == NetworkManager::WirelessDevice::Adhoc),
                 ap->capabilities(),
                 ap->wpaFlags(),
                 ap->rsnFlags());
    }

    // Insert new row at end of table
    const int index = rootItem->childCount();
    beginInsertRows (QModelIndex(), index, index);
    parent->insertChildren (parent->childCount(), 1, rootItem->columnCount());
    endInsertRows ();

    // Fill Network Item with WiFi info
    NetworkItem *item = parent->child(parent->childCount()-1);
    if (device->ipInterfaceName().isEmpty()) {
        item->setDeviceName(device->interfaceName());
    }
    else {
        item->setDeviceName (device->ipInterfaceName());
    }
    item->setSsid (network->ssid());
    item->setDevicePath (device->uni());
    item->setSpecificPath (network->referenceAccessPoint()->uni());
    item->setType (NetworkManager::ConnectionSettings::Wireless);
    item->setSecurityType (securityType);

    // Set WiFi info to display
    d_ptr->setItemRoles(columnRoles, parent->child(parent->childCount()-1));
}


void NetworkModel::wirelessNetworkAppeared (const QString &ssid)
{
    NetworkManager::Device::Ptr device =
            NetworkManager::findNetworkInterface (qobject_cast<NetworkManager::Device *>
                                                  (sender())->uni());

    if (device && device->type() == NetworkManager::Device::Wifi)
    {
        NetworkManager::WirelessDevice::Ptr wirelessDevice =
                device.objectCast<NetworkManager::WirelessDevice>();
        NetworkManager::WirelessNetwork::Ptr network =
                wirelessDevice->findNetwork(ssid);

        QString ssid = network->ssid();
        for (int i = 0; i < rootItem->childCount(); ++i)
        {
            if (rootItem->child(i)->ssid() == ssid )
                return;
        }

        addWirelessNetwork(network, wirelessDevice, rootItem);

        int row = rootItem->childCount()-1;
        NetworkItem *item = rootItem->child(row);
        for (int i = 0; i < rootItem->columnCount(); ++i)
        {
            item->setData(i, d_ptr->getColumn(index(row, i), this));
        }
    }
}
