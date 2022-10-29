#include <QTimer>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Settings>

#include "iface_model.h"
#include "custom_colors.h"

IfaceModel::IfaceModel (QObject *parent)
    : QAbstractListModel (parent)
{
    // populate list of network interfaces
    initIface();

    m_ifaceUpdateInterval = new QTimer (this);
    m_ifaceUpdateInterval->setInterval (2000);

    connect (m_ifaceUpdateInterval, &QTimer::timeout,
             this, &IfaceModel::updateIface);

    m_ifaceUpdateInterval->start();

    // Testing with the following commands
    // nmcli con add type bridge con-name br0 ifname br
    // nmcli con delete br0

    connect (NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded,
             this, &IfaceModel::ifaceAdded, Qt::UniqueConnection);

    //connect (NetworkManager::notifier(), &NetworkManager::Notifier::serviceAppeared,
    //         this, &IfaceModel::ifaceAdded, Qt::UniqueConnection);

    //connect(NetworkManager::settingsNotifier(),
    //        &NetworkManager::SettingsNotifier::connectionAdded,
    //        this,
    //        &IfaceModel::ifaceAdded,
    //        Qt::UniqueConnection);

//    connect(NetworkManager::settingsNotifier(),
//            &NetworkManager::SettingsNotifier::connectionRemoved,
//            this,
//            &IfaceModel::ifaceAdded,
//            Qt::UniqueConnection);
}

/* Destructor */
IfaceModel::~IfaceModel ()  = default;


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
QVariant IfaceModel::data (const QModelIndex &index, int role) const
{
    // Checks for valid index
    if (!index.isValid())
        return QVariant();

    const int row = index.row();
    NetworkManager::Device::Ptr item = m_interfaces.at(row);

    switch (role) {
        case Qt::ForegroundRole:
            return CustomColors::frame_font_color();
            break;

        case Qt::DisplayRole: {
            QString iface_name = item->interfaceName();

            return iface_name;
            break;
        }
        case Qt::EditRole: {
            QString iface_name = item->interfaceName();

            return iface_name;
            break;
        }
    }

    return QVariant();
}


/* Return total number of rows */
int IfaceModel::rowCount (const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return parent.isValid() ?  -1 : m_interfaces.count();
}


/* Insert number of rows below specified position */
bool IfaceModel::insertRow (const NetworkManager::Device::Ptr iface)
{
    int position = m_interfaces.size();
    NetworkManager::Device::Ptr item = iface;

    beginInsertRows (QModelIndex(), position, position);
    if (position < 0
        || position > m_interfaces.size())
    {
        return false;
    }

    m_interfaces.insert (position, item);
    endInsertRows ();

    return true;
}


/* Remove number of rows below specified position */
bool IfaceModel::removeRows (int position, int rows,
                 const QModelIndex &parent)
{
    if (position < 0
        || position > m_interfaces.size()
        || position+rows > m_interfaces.size())
    {
        return false;
    }

    beginRemoveRows (parent, position, position + rows - 1);
    for (int i = position; i < position + rows; ++i)
    {
       m_interfaces.takeAt(i);
    }
    endRemoveRows ();

    return true;
}


/* Returns list of item flags present on index (See Qt::ItemFlags) */
Qt::ItemFlags IfaceModel::flags (const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
}


/* Iface Model
** --------
** Implement functions to add network interfaces to the model
** --------
**/

/* Add physical network devices
** Parameters:
**     device: name of the physical wifi adapter
*/
void IfaceModel::updateIface ()
{
    // Not working
    bool isDuplicate = false;
    NetworkManager::networkInterfaces();

    for (NetworkManager::Device::Ptr &device :
         NetworkManager::networkInterfaces())
    {
        // Error handling
        if (rowCount() < 0)
        {
            // add error message
        }

        if (!device->isAvailable())
        {
            qDebug() << device->interfaceName() << "  Unavailable";
            qDebug() << "State: " << device->state();
            continue;
        }

        //qDebug() << device->interfaceName();

        // Look for duplicates (skip duplicate devices)
        int row = rowCount();
        QString iface_name = device->interfaceName();
        for (int i = 0; i < row; ++i)
        {
            if (m_interfaces.at(i)->interfaceName() == iface_name)
                isDuplicate = true;
        }

        if (isDuplicate)
        {
            continue;
        }

        // Add new network interfaces to model
        connect (device.data(), &NetworkManager::Device::stateChanged,
                 this, &IfaceModel::ifaceRemoved, Qt::UniqueConnection);

        if (device->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice =
                    device.objectCast<NetworkManager::WirelessDevice>();

            insertRow (wifiDevice);
        }
        else
        {
            insertRow (device);
        }
    }
}


void IfaceModel::initIface ()
{
    // Ensure model is empty
    if (!m_interfaces.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_interfaces.size() - 1);
        m_interfaces.clear();
        endRemoveRows();
    }

    for (const NetworkManager::Device::Ptr &device :
         NetworkManager::networkInterfaces())
    {
        if (!device->isAvailable())
        {
            qDebug() << device->interfaceName() << "  Unavailable";
            qDebug() << "State: " << device->state();
            continue;
        }

        if (device->interfaceName() == "br0")
        {
            qDebug() << device->udi();
        }

        // Add new network interfaces to model
        connect (device.data(), &NetworkManager::Device::stateChanged,
                 this, &IfaceModel::ifaceRemoved, Qt::UniqueConnection);

        if (device->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice =
                    device.objectCast<NetworkManager::WirelessDevice>();

            insertRow (wifiDevice);
        }
        else
        {
            insertRow (device);
        }
    }

}


void IfaceModel::ifaceRemoved (NetworkManager::Device::State newState,
                               NetworkManager::Device::State oldState,
                               NetworkManager::Device::StateChangeReason reason)
{
    // finds interface from signal sender
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface
            (qobject_cast<NetworkManager::Device *>(sender())->uni());

    if (!device)
    {
        // add error handling
        return;
    }

    // Remove device if no longer available
    if (!device->isAvailable()
        || reason == NetworkManager::Device::ConnectionRemovedReason)
    {
        removeRows(m_interfaces.indexOf(device));
    }

}


// Not working properly
void IfaceModel::ifaceAdded ()
{
    // finds interface from signal sender
    //NetworkManager::Connection::Ptr connection = NetworkManager::findConnection(path);
    //NetworkManager::Device device() =


    qDebug() << "added device";
    //qDebug() << path;
    //qDebug() << connection->name();
    //qDebug() << connection->uuid();
    //NetworkManager::Device::Ptr device;
    //qDebug() << device->interfaceName();
}
