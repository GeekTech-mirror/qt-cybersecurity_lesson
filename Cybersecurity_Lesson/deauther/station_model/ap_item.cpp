#include "ap_item.h"

AccessPointItem::AccessPointItem(QObject *parent)
    : QObject{parent}
{

}

AccessPointItem::~AccessPointItem() = default;


/* return pointer to parent */
AccessPointItem *AccessPointItem::parent()
{
    return m_parentItem;
}

/* return pointer to child at "row" */
AccessPointItem *AccessPointItem::child (int number)
{
    if (number < 0 || number >= m_childItems.size())
        return nullptr;
    return m_childItems.at(number);
}

/* return total number of children */
int AccessPointItem::childCount() const
{
    return m_childItems.count();
}

/* return index of child in its parent's list of children */
/* return zero for the root item */
int AccessPointItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf
                (const_cast<AccessPointItem*>(this));
    return 0;
}

/* return number of elements in the internal itemData list */
int AccessPointItem::columnCount() const
{
    return m_roles.count();
}

/* insert row below specified index  */
bool AccessPointItem::insertChildren (int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        AccessPointItem *item = new AccessPointItem (this);
        m_childItems.insert (position, item);
    }

    return true;
}

/* remove row at specified index */
bool AccessPointItem::removeChildren (int position, int count)
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
