#include "station_item.h"

StationItem::StationItem(QObject *parent)
    : QObject{parent}
{

}

StationItem::~StationItem() = default;


/* return pointer to parent */
StationItem *StationItem::parent()
{
    return m_parentItem;
}

/* return pointer to child at "row" */
StationItem *StationItem::child (int number)
{
    if (number < 0 || number >= m_childItems.size())
        return nullptr;
    return m_childItems.at(number);
}

/* return total number of children */
int StationItem::childCount() const
{
    return m_childItems.count();
}

/* return index of child in its parent's list of children */
/* return zero for the root item */
int StationItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf
                (const_cast<StationItem*>(this));
    return 0;
}

/* return number of elements in the internal itemData list */
int StationItem::columnCount() const
{
    return m_roles.count();
}

/* insert row below specified index  */
bool StationItem::insertChildren (int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        StationItem *item = new StationItem (this);
        m_childItems.insert (position, item);
    }

    return true;
}

/* remove row at specified index */
bool StationItem::removeChildren (int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        m_childItems.takeAt(position)->deleteLater();

    return true;
}


/*
 *  Station Items
 */
