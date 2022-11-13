#include "network_model.h"
#include "network_model_p.h"

#include "custom_colors.h"

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
        // Display Network icon in first column
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

    const bool result = rootItem->setHeaderData (section, value.toString());

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
