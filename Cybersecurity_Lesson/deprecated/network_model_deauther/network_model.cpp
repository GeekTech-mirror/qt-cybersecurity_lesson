/* Network Model
** File: network_model.cpp
** --------
** Scan for local networks and display relevent info
** in a Tree Model
** --------
*/

/* Qt include files */
#include <QDBusInterface>
#include <QIcon>
#include <QStringBuilder>
#include <QTimer>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

/* local include files */
#include "network_model.h"
#include "network_model_p.h"
#include "network_item.h"

#include "custom_colors.h"


/* Constructor */
NetworkModel::NetworkModel (const QVector<ItemRole> &roles, QObject *parent)
    : QAbstractItemModel (parent),
      m_scanHandler (new NetworkScan (this))
{
    // Create header titles based on column roles
    QVector<QVariant> rootData;
    for (int i = 0; i < roles.count(); ++i)
    {
        switch (roles.at(i)) {
        case ItemRole::ConnectionIconRole:
            rootData << "";
            break;

        case ItemRole::DeviceName:
            rootData << "Device Name";
            break;

        case ItemRole::DevicePathRole:
            rootData << "Device Path";
            break;

        case ItemRole::NameRole:
            rootData << "Networks";
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
    rootItem = new NetworkItem (rootData);
    columnRoles = roles;


    // Initialize first scan and then scan every 2 seconds
    m_scanHandler = new NetworkScan (this);
    m_scanHandler->requestScan();

    m_timer = new QTimer (this);
    m_timer->setInterval (2000);
    connect (m_timer, &QTimer::timeout, this,
             [&](){m_scanHandler->requestScan();});
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
    delete m_scanHandler;
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

    switch (role) {
//    case Qt::BackgroundRole:
//        if (0 == index.row() % 2)
//            return CustomColors::frame_color();
//        else
//            return CustomColors::frame_color().lighter(145);

//        break;
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
        break;
    case Qt::DecorationRole:
        // Display Network Symbol in first column
        if (index.column() == 0)
        {
            return QIcon (item->icon());
        }
        break;
    case Qt::DisplayRole:
        return item->data (index.column());
        break;
    case Qt::EditRole:
        return item->data (index.column());
        break;
    }

    return QVariant();
}


/* Store value in model index */
bool NetworkModel::setData (const QModelIndex &index,
                            const QVariant &network_role,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    NetworkItem *item = d_ptr->getItem (index, this);

    // Set WiFi info to display
    bool result = d_ptr->setItemRole (network_role, index, this);
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
    switch (role) {
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
    case Qt::DisplayRole:
        return rootItem->data (section);
        break;
    }
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

    const NetworkItem *parentItem = d_ptr->getItem (parent, this);

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


/* Returns list of item flags present on index (See Qt::ItemFlags) */
Qt::ItemFlags NetworkModel::flags (const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
}


/* Network Model
** --------
** Implement functions to add wireless networks to the model
** --------
**/

/* Add network roles to the model */
QHash<int, QByteArray> NetworkModel::roleNames (void) const
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


/* Add physical network devices
** Parameters:
**     device: name of the physical wifi adapter
*/
void NetworkModel::addDevice (const NetworkManager::Device::Ptr &device,
                              NetworkItem *parent)
{
    // Connect NetworkManager::networkAppear SIGNAL to
    // NetworkModel::wirlessNetworkAppeared SLOT
    d_ptr->initializeSignals (device, this);

    if (device->type() == NetworkManager::Device::Wifi)
    {
        NetworkManager::WirelessDevice::Ptr wifiDev =
                device.objectCast<NetworkManager::WirelessDevice>();

        for (const NetworkManager::WirelessNetwork::Ptr &wifiNetwork :
             wifiDev->networks())
        {
            addWirelessNetwork (wifiNetwork, wifiDev);
        }
    }

}


/* Adds new network
** Parameters:
**     network: the new network to be added
**     device: name of the physical wifi adapter
*/
void NetworkModel::addWirelessNetwork (const NetworkManager::WirelessNetwork::Ptr &network,
                                       const NetworkManager::WirelessDevice::Ptr &device)
{
    d_ptr->initializeSignals (network);

    // Set default security info
    NetworkManager::WirelessSetting::NetworkMode mode =
            NetworkManager::WirelessSetting::Infrastructure;
    NetworkManager::WirelessSecurityType securityType =
            NetworkManager::UnknownSecurity;

    // Look for security info
    NetworkManager::AccessPoint::Ptr ap = network->referenceAccessPoint();
    if (ap)
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
    beginInsertRows (QModelIndex(), index, index);  // BEGIN ADDING NETWORK

    rootItem->insertChildren (rootItem->childCount(), 1, rootItem->columnCount());


    // Fill Network Item with wifi info
    NetworkItem *item = rootItem->child (rootItem->childCount()-1);
    if (device->ipInterfaceName().isEmpty()) {
        item->setDeviceName (device->interfaceName());
    }
    else
    {
        item->setDeviceName (device->ipInterfaceName());
    }
    item->setName (network->ssid());
    item->setSsid (network->ssid());
    item->setDevicePath (device->uni());
    item->setSpecificPath (network->referenceAccessPoint()->uni());
    item->setType (NetworkManager::ConnectionSettings::Wireless);
    item->setSecurityType (securityType);

    //qDebug() << network->ssid() << " " << network->referenceAccessPoint()->hardwareAddress();

    // Set column role
    int row = rootItem->childCount()-1;
    for (int i=0; i < columnRoles.count(); ++i)
    {
        setData (this->index(row,i), columnRoles.at(i));
    }

    endInsertRows ();   // END ADDING NETWORK
}


/* Triggers when new network appears
** Parameters:
**     ssid: takes a network name
*/
void NetworkModel::wirelessNetworkAppeared (const QString &ssid)
{
    // Find the physical network device
    NetworkManager::Device::Ptr device =
            NetworkManager::findNetworkInterface
            (qobject_cast<NetworkManager::Device *>
             (sender())->uni());

    // Ensure the device is a wifi adapter
    if (device && device->type() == NetworkManager::Device::Wifi)
    {
        // Find the specific wirless device
        NetworkManager::WirelessDevice::Ptr wirelessDevice =
                device.objectCast<NetworkManager::WirelessDevice>();

        // Find the network with the specific ssid
        NetworkManager::WirelessNetwork::Ptr network =
                wirelessDevice->findNetwork (ssid);

        // Look for duplicates
        QString ssid = network->ssid();
        for (int i = 0; i < rootItem->childCount(); ++i)
        {
            if (rootItem->child(i)->ssid() == ssid )
                return;
        }

        // Add new network
        addWirelessNetwork (network, wirelessDevice);
    }
}


void NetworkModel::wirelessNetworkDisappeared(const QString &ssid)
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());
    if (!device) {
        qDebug() << "Warning: Attempting to open device failed";
        return;
    }

    // Ensure the device is a wifi adapter
    if (device && device->type() == NetworkManager::Device::Wifi)
    {
        // Find the specific wirless device
        NetworkManager::WirelessDevice::Ptr wirelessDevice =
                device.objectCast<NetworkManager::WirelessDevice>();

        // Find the network with the specific ssid
        NetworkManager::WirelessNetwork::Ptr network =
                wirelessDevice->findNetwork (ssid);

        QString ssid = network->ssid();
        for (int i = 0; i < rootItem->childCount(); ++i)
        {
            if (rootItem->child(i)->ssid() == ssid )
                remove_network(rootItem->child(i));
        }
    }

    /*
    for (NetworkModelItem *item : m_list.returnItems(NetworkItemsList::Ssid, ssid, device->uni())) {
        // Remove the entire item, because it's only AP or it's a duplicated available connection
        if (item->itemType() == NetworkModelItem::AvailableAccessPoint || item->duplicate()) {
            removeItem(item);
            qCDebug(PLASMA_NM_LIBS_LOG) << "Wireless network " << item->name() << " removed completely";
            // Remove only AP and device from the item and leave it as an unavailable connection
        } else {
            if (item->mode() == NetworkManager::WirelessSetting::Infrastructure) {
                item->setDeviceName(QString());
                item->setDevicePath(QString());
                item->setSpecificPath(QString());
            }
            item->setSignal(0);
            updateItem(item);
            qCDebug(PLASMA_NM_LIBS_LOG) << "Item " << item->name() << ": wireless network removed";
        }
    }
    */
}


QIcon NetworkModel::getScanIcon()
{
    return QIcon(m_scanIcon);
}


void NetworkModel::remove_network(NetworkItem *item)
{
    const int row = item->childNumber();
    qDebug() << item->ssid() << row;
    if (row >= 0) {
        beginRemoveRows(QModelIndex(), row, row);
        //m_list.removeItem(item);
        //item->deleteLater();
        endRemoveRows();
    }
}


void NetworkModel::update_networks (const NetworkManager::WirelessNetwork::Ptr &network,
                                    NetworkManager::WirelessDevice::Ptr &wifiDev)
{
    // Look for duplicates
    QString ssid = network->ssid();
    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        if (rootItem->child(i)->ssid() == ssid )
            return;
    }

    // Add new network
    addWirelessNetwork (network, wifiDev);
}

void NetworkModel::toggle_scan()
{
    /* Stop scanning networks*/
    if(m_timer->isActive())
    {
        m_scanIcon = ":/icons/network/24/network_scan_inactive.svg";
        m_timer->stop();

        // Block network appear signals
        for (const NetworkManager::Device::Ptr &dev :
             NetworkManager::networkInterfaces())
        {
            if (!dev->managed())
                continue;

            dev->blockSignals(true);
        }

        return;
    }


    /* Start scanning networks */
    m_scanIcon = ":/icons/network/24/network_scan_active.svg";
    m_timer->start();

    // Initialize network devices for scanning
    if (first_scan)
    {
        for (const NetworkManager::Device::Ptr &dev :
             NetworkManager::networkInterfaces())
        {
            if (!dev->managed())
                continue;

            addDevice (dev, rootItem);
        }

        first_scan = false;

        return;
    }

    // Unblock network appear signals
    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        if (!dev->managed())
            continue;

        dev->blockSignals(false);

        // Update network list since last scan
        if (dev->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDev =
                    dev.objectCast<NetworkManager::WirelessDevice>();

            for (const NetworkManager::WirelessNetwork::Ptr &network :
                 wifiDev->networks())
            {
                update_networks (network, wifiDev);
            }
        }
    }
}


void NetworkModel::start_scan()
{
    /* Start scanning networks */
    m_scanIcon = ":/icons/network/24/network_scan_active.svg";
    m_timer->start();

    // Initialize network devices for scanning
    if (first_scan)
    {
        for (const NetworkManager::Device::Ptr &dev :
             NetworkManager::networkInterfaces())
        {
            if (!dev->managed())
                continue;

            addDevice (dev, rootItem);
        }

        first_scan = false;

        return;
    }

    // Unblock network appear signals
    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        if (!dev->managed())
            continue;

        dev->blockSignals(false);

        // Update network list since last scan
        if (dev->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDev =
                    dev.objectCast<NetworkManager::WirelessDevice>();

            for (const NetworkManager::WirelessNetwork::Ptr &network :
                 wifiDev->networks())
            {
                update_networks (network, wifiDev);
            }
        }
    }
}


void NetworkModel::stop_scan()
{
    if (first_scan)
    {
        qDebug() << "Warning: No scan handles running";
        return;
    }

    /* Stop scanning networks*/
    if(m_timer->isActive())
    {
        m_scanIcon = ":/icons/network/24/network_scan_inactive.svg";
        m_timer->stop();

        // Block network appear signals
        for (const NetworkManager::Device::Ptr &dev :
             NetworkManager::networkInterfaces())
        {
            if (!dev->managed())
                continue;

            dev->blockSignals(true);
        }

        return;
    }
}


void NetworkModel::export_data()
{
    if (m_timer->isActive())
    {
        m_timer->stop();
    }


}
