#include <QDir>
#include <QStandardPaths>
#include <QThread>

#include "station_model.h"
#include "station_model_p.h"

#include "custom_colors.h"


StationModel::StationModel (const QVector<StationItemRole> &roles, QObject *parent)
    : QAbstractItemModel (parent),
      m_pcapThread (new QThread(this)),
      m_apInfo (QVector<ap_info*>()),
      m_ouiFile ("oui.txt")
{
    qDebug() << QDir::currentPath();
    qDebug() << m_ouiFile.open (QIODevice::ReadOnly | QIODevice::Text);

    // Create header titles based on column roles
    QVector<QString> rootData;
    for (int i = 0; i < roles.count(); ++i)
    {
        switch (roles.at(i)) {
        case StationItemRole::StationRole:
            rootData << "Stations";
            break;
        case StationItemRole::AccessPointRole:
            rootData << "Access Points";
            break;
        case StationItemRole::Bssid2Role:
            rootData << "BSSID 2GHz";
            break;
        case StationItemRole::Bssid5Role:
            rootData << "BSSID 5GHz";
            break;
        case StationItemRole::Channel2Role:
            rootData << "Ch 2GHz";
            break;
        case StationItemRole::Channel5Role:
            rootData << "Ch 5GHz";
            break;
        default:
            break;
        }
    }
    rootItem = new StationItem (rootData);
    columnRoles = roles;


    connect (this, &StationModel::packetCaptured,
             this, &StationModel::filterPacket);
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

    StationItem *item = static_cast<StationItem*>(index.internalPointer());

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
        // Display Symbol in first column
        if (index.column() == 0)
        {
            return QIcon ();
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
bool StationModel::setData (const QModelIndex &index,
                            const QVariant &station_role,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    StationItem *item = d_ptr->getItem (index, this);

    // Set pcap info to display
    bool result = d_ptr->setItemRole (station_role, index, this);
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

    if (parentItem == rootItem)
        return QModelIndex();

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


/* Add station roles to the model */
QHash<int, QByteArray> StationModel::roleNames (void) const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[StationItemRole::AccessPointRole] = "AccessPoint";
    roles[StationItemRole::Bssid2Role] = "BSSID 2GHz";
    roles[StationItemRole::Bssid5Role] = "BSSID 5GHz";
    roles[StationItemRole::Channel2Role] = "Channel 2GHz";
    roles[StationItemRole::Channel5Role] = "Channel 5GHz";
    roles[StationItemRole::InterfaceRole] = "Interface";
    roles[StationItemRole::StationRole] = "Station";
    roles[StationItemRole::StationHeaderRole] = "Header";

    return roles;
}
