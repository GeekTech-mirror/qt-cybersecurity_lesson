#include <QTimer>
#include <QNetworkInterface>

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

    m_ifaceUpdate_t = new QTimer (this);
    m_ifaceUpdate_t->setInterval (2000);

    connect (m_ifaceUpdate_t, &QTimer::timeout,
             this, &IfaceModel::updateIface);

    m_ifaceUpdate_t->start();

    // Testing with the following commands
    // nmcli con add type bridge con-name br0 ifname br
    // nmcli con delete br0
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
    QNetworkInterface item = m_interfaces.at(row);

    switch (role) {
        case Qt::ForegroundRole:
            return CustomColors::frame_font_color();
            break;

        case Qt::DisplayRole: {
            QString iface_name = item.humanReadableName();

            return iface_name;
            break;
        }
        case Qt::EditRole: {
            QString iface_name = item.humanReadableName();

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
bool IfaceModel::insertRow (const QNetworkInterface iface)
{
    int position = m_interfaces.size();
    QNetworkInterface item = iface;

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
    bool isDuplicate = false;

    QList<QNetworkInterface> iface_list = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : iface_list)
    {
        // Error handling
        if (rowCount() < 0)
        {
            // add error message
            return;
        }

        // Look for duplicates (skip duplicate interfaces)
        if (m_interfaces.contains(iface))
        {
            continue;
        }

        insertRow (iface);
    }


    for (const QNetworkInterface &iface : m_interfaces)
    {
        if (!iface_list.contains(iface))
        {
            qInfo() << "Info:" << iface.humanReadableName() << "removed";
            qInfo() << Qt::endl;
            removeIface(iface);
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

    QList<QNetworkInterface> iface_list = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : iface_list)
    {
        // Add new network interfaces to model
        insertRow (iface);
    }

}


void IfaceModel::removeIface (QNetworkInterface iface)
{
    removeRows(m_interfaces.indexOf(iface));
}


void IfaceModel::ifaceAdded ()
{
    qDebug() << "added device";
}


bool operator==(const QNetworkInterface &a, const QNetworkInterface &b)
{
    return a.hardwareAddress() == b.hardwareAddress();
}
