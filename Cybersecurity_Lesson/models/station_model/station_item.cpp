#include <NetworkManagerQt/WirelessDevice>

#include "station_item.h"
#include "station_common.h"

StationItem::StationItem(StationItem *parent)
    : m_parentItem(parent),
      indent(1,' ')
{
}

StationItem::StationItem (const QVector<QString> &data, StationItem *parent)
    : m_headerData(data),
      m_parentItem(parent),
      m_stmac(QByteArray()),
      indent(1,' ')
{
    for (int i = 0; i < m_headerData.size(); ++i)
    {
        this->insertRole (StationItemRole::StationHeaderRole);
    }
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


/* retrieve data from itemData list at specified column */
QVariant StationItem::data (int column) const
{
    if (column < 0 || column >= m_roles.size())
        return QVariant();

    //qDebug() << "Display Station:" << stmac().toHex(':');
    switch (m_roles.at(column)) {
    case AccessPointRole:
        //return apEssid().constData();
        return apEssid().constData();
    case InterfaceRole:
        return QVariant();
    case StationRole:
        return stmac().toHex(':');
    case StationHeaderRole:
        return m_headerData[column];
    default:
        break;
    }

    return QVariant();
}


/* retrieve data from headerData list at specified column */
QString StationItem::headerData(int column) const
{
    if (column < 0 || column >= m_headerData.size())
        return QString();

    return m_headerData[column];
}



bool StationItem::setHeaderData (int column, const QString &value)
{
    qDebug() << value;

    if (column < 0 || column >= m_headerData.size())
        return false;

    if (m_headerData[column] != value) {
        m_headerData[column] = value;
        return true;
    }
    else {
        return false;
    }
}

/*
 *  Station Items
 */
QByteArray StationItem::apEssid () const
{
    return m_apInfo->essid;
}

QByteArray StationItem::apBssid (ChannelFrequency chan_type) const
{
    if (chan_type == Chan_2GHz)
    {
        return m_apInfo->bssid[0];
    }
    else if (chan_type == Chan_5GHz)
    {
        return m_apInfo->bssid[1];
    }

    qWarning() << "Warning: No BSSID found for this network" << Qt::endl;

    return QByteArray();
}

int StationItem::apChannel(ChannelFrequency chan_type) const
{
    if (chan_type == Chan_2GHz)
    {
        return m_apInfo->channel[0];
    }
    else if (chan_type == Chan_5GHz)
    {
        return m_apInfo->channel[1];
    }

    qWarning() << "Warning: No channel found for this network" << Qt::endl;

    return int();
}


ap_info StationItem::apInfo () const
{
    return *m_apInfo;
}

void StationItem::setApInfo (const ap_info &apInfo)
{
    if (m_apInfo != &apInfo)
    {
        *m_apInfo = apInfo;
        m_changedRoles << StationItemRole::AccessPointRole;
    }
}


QByteArray StationItem::stmac () const
{
    return m_stmac;
}

void StationItem::setStmac (const QByteArray &stmac)
{
    if (m_stmac != stmac)
    {
        m_stmac = stmac;
        m_changedRoles << StationItemRole::StationRole;
    }
}
