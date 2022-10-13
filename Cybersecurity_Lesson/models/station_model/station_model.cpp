#include <QThread>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>

#include "station_model.h"
#include "station_model_p.h"
#include "station_item.h"

#include "pcap.h"
#include "iwconfig.h"
#include "custom_colors.h"

#include <netinet/in.h>
#include <arpa/inet.h>

StationModel::StationModel (QObject *parent)
    : QAbstractItemModel (parent)
{
    rootItem = new StationItem ();

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

void StationModel::setIfaceHandle (pcap_t *handle)
{
    if (m_ifaceHandle == NULL)
    {
        qDebug() << "setIfaceHandle failed: handle does not exist";
        return;
    }

    if (m_ifaceHandle != handle)
    {
        m_ifaceHandle = handle;
    }

    struct pcap_pkthdr *packet_header;
    const u_char *packet;

    this->pcap_loop (m_ifaceHandle);
}

void StationModel::addStation ()
{

}


int StationModel::pcap_loop(pcap_t *handle)
{
    int n;
    int status;

    QThread *thread = QThread::create([handle]
    {
        struct pcap_pkthdr *packet_header;

        const u_char *packet;

        while (true)
        {
            if (handle == NULL)
            {
                continue;
            }

            switch (pcap_next_ex (handle, &packet_header, &packet))
            {
            case PCAP_ERROR:
                qDebug() << "Error: Failed while reading packet";
                qDebug() << pcap_geterr (handle);
                break;
            case PCAP_ERROR_BREAK:
                qDebug() << "Warning: No more packets to read";
                break;
            default:
                qDebug() << "packet captured";
                qDebug() << "capture lenght" << packet_header->caplen;
                qDebug() << "length" << packet_header->len;
                break;
            }

        }
    });
    thread->start();

    return 0;
}


void StationModel::capturePacket ()
{

}

