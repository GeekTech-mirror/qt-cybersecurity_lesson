/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>

#include "station_model.h"
#include "station_model_p.h"
#include "station_item.h"

#include "iwconfig.h"
#include "custom_colors.h"

StationModel::StationModel (QObject *parent)
    : QAbstractItemModel (parent)
{
    rootItem = new StationItem ();

    // populate list of network interfaces
    initIface();

    m_ifaceUpdateInterval = new QTimer (this);
    m_ifaceUpdateInterval->setInterval (5000);

    connect (m_ifaceUpdateInterval, &QTimer::timeout, this, &StationModel::updateIface);
}

/* Constructor for private functions */
StationModel::StationModel (StationModelPrivate &dd)
    : d_ptr (&dd)
{
}

/* Destructor */
StationModel::~StationModel ()  = default;

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
QVariant StationModel::data (const QModelIndex &index, int role) const
{
    // Checks for valid index
    if (!index.isValid())
        return QVariant();

    //qDebug() << role;

    switch (role) {
    case Qt::BackgroundRole:
        if (0 == index.row() % 2)
            return CustomColors::frame_color();
        else
            return CustomColors::frame_color().lighter(145);

        break;
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
        break;
    case Qt::DecorationRole:
        // Display Symbol in first column
        if (index.column() == 0)
        {
            return QIcon ();
        }
        break;
    case Qt::DisplayRole:
        return QVariant();
        break;
    case Qt::EditRole:
        return QVariant();
        break;
    }

    return QVariant();
}


/* Store value in model index */
bool StationModel::setData (const QModelIndex &index,
                            const QVariant &network_role,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    StationItem *item = d_ptr->getItem (index, this);

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
QVariant StationModel::headerData (int section,
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
bool StationModel::setHeaderData (int section,
                                  Qt::Orientation orientation,
                                  const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setHeaderData (section, value.toString());

    if (result)
        emit headerDataChanged (orientation, section, section);

    return result;
}


/* Return index to model */
QModelIndex StationModel::index (int row, int column,
                                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    StationItem *parentItem = d_ptr->getItem (parent, this);

    if (!parentItem)
        return QModelIndex();

    StationItem *childItem = parentItem->child (row);
    if (childItem)
        return createIndex (row, column, childItem);

    return QModelIndex();
}


/* Return index to parent in model */
QModelIndex StationModel::parent (const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    StationItem *childItem = static_cast<StationItem*>(index.internalPointer());
    StationItem *parentItem = childItem->parent();

//    if (parentItem == rootItem)
//        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


/* Return total number of rows */
int StationModel::rowCount (const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const StationItem *parentItem = d_ptr->getItem (parent, this);

    return parentItem ? parentItem->childCount() : 0;
}


/* Insert number of rows below specified position */
bool StationModel::insertRows (int position, int rows,
                                const QModelIndex &parent)
{
    StationItem *parentItem = d_ptr->getItem (parent, this);
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
bool StationModel::removeRows (int position, int rows,
                                const QModelIndex &parent)
{
    StationItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginRemoveRows (parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren (position, rows);
    endRemoveRows ();

    return success;
}


/* Return total number of columns */
int StationModel::columnCount (const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}


/* Returns list of item flags present on index (See Qt::ItemFlags) */
Qt::ItemFlags StationModel::flags (const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
}


/* Station Model
** --------
** Implement functions to add stations to the model
** --------
**/


/* Add station roles to the model */
QHash<int, QByteArray> StationModel::roleNames (void) const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[StationItemRole::AccessPointRole] = "AccessPoint";
    roles[StationItemRole::InterfaceRole] = "Interface";
    roles[StationItemRole::StationRole] = "Station";

    return roles;
}

QVector<QString> StationModel::getIface()
{
    QVector<QString> *ifaceList = new QVector<QString> ();
    ifaceList->reserve(m_iface.count());

    for (int i = 0; i < m_iface.count(); ++i)
    {
        ifaceList->append(m_iface.at(i)->interfaceName());
    }

    return *ifaceList;
}

/* Add physical network devices
** Parameters:
**     device: name of the physical wifi adapter
*/
void StationModel::updateIface ()
{
    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        // Look for duplicates
        QString iface_name = dev->interfaceName();
        for (int i = 0; i < m_iface.count(); ++i)
        {
            if (m_iface.at(i)->interfaceName() == iface_name)
                return;
        }

        connect (dev.data(), &NetworkManager::Device::stateChanged,
                 this, &StationModel::ifaceStateChanged, Qt::UniqueConnection);

        if (dev->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice =
                    dev.objectCast<NetworkManager::WirelessDevice>();

            m_iface.append(wifiDevice);
        }
        else
        {
            m_iface.append(dev);
        }
    }
}

void StationModel::initIface ()
{
    m_iface.empty();

    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        connect (dev.data(), &NetworkManager::Device::stateChanged,
                 this, &StationModel::ifaceStateChanged, Qt::UniqueConnection);

        if (dev->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice =
                    dev.objectCast<NetworkManager::WirelessDevice>();

            m_iface.append(wifiDevice);
        }
        else
        {
            m_iface.append(dev);
        }
    }

}

void StationModel::ifaceStateChanged (NetworkManager::Device::State state,
                                       NetworkManager::Device::State oldState,
                                       NetworkManager::Device::StateChangeReason reason)
{
    Q_UNUSED(oldState);
    Q_UNUSED(reason);

    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(qobject_cast<NetworkManager::Device *>(sender())->uni());

    if (!device) {
        return;
    }

    if (state == NetworkManager::Device::State::Disconnected)
    {
        qDebug() << m_iface.removeOne(device);
    }
}

void StationModel::addStation ()
{

}

void StationModel::capturePacket ()
{

}

void StationModel::start_monitoring (NetworkManager::Device::Ptr &device)
{
    // Create monitor mode socket
    int sockfd = iw_sockets_open();

    if (sockfd == -1) {
          qDebug() <<  "socket() creation failed - do you have permissions?";
          return;
    }


    // search for nearby devices
    QString mode = "monitor";
    QString tmp = "wlp5s0";
    set_mode_info(sockfd, tmp.toLocal8Bit().data(), mode.toLocal8Bit().data(), 3);
}
