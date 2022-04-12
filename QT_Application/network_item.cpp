#include <NetworkManagerQt/BridgeDevice>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WirelessSetting>
#include <NetworkManagerQt/AccessPoint>

#include "network_item.h"
#include "network_enums.h"

QNetworkItem::QNetworkItem(const QVector<QVariant> &data, QNetworkItem *parent)
    : m_itemData(data)
    , m_parentItem(parent)
{
}

QNetworkItem::~QNetworkItem()
{
    qDeleteAll(m_childItems);
}

/* return pointer to parent */
QNetworkItem *QNetworkItem::parent()
{
    return m_parentItem;
}

/* return pointer to child at "row" */
QNetworkItem *QNetworkItem::child(int number)
{
    if (number < 0 || number >= m_childItems.size())
        return nullptr;
    return m_childItems.at(number);
}

/* return total number of children */
int QNetworkItem::childCount() const
{
    return m_childItems.count();
}

/* return index of child in its parent's list of children */
/* return zero for the root item */
int QNetworkItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<QNetworkItem*>(this));
    return 0;
}

/* return number of elements in the internal itemData list */
int QNetworkItem::columnCount() const
{
    return m_itemData.count();
}

/* insert row below specified index  */
bool QNetworkItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        QNetworkItem *item = new QNetworkItem(data, this);
        m_childItems.insert(position, item);
    }

    return true;
}

/* remove row at specified index */
bool QNetworkItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

/* insert column (perform action on every row) */
bool QNetworkItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, int());

    for (QNetworkItem *child : qAsConst(m_childItems))
        child->insertColumns(position, columns);

    return true;
}

/* remove column */
bool QNetworkItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.remove(position);

    for (QNetworkItem *child : qAsConst(m_childItems))
        child->removeColumns(position, columns);

    return true;
}


/* retrieve data from itemData list at specified column */
QVariant QNetworkItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();

    return m_itemData.at(column);
}

/* store values in the itemData list for valid list indexes */
bool QNetworkItem::setData(int column, const QVariant &value)
{

    if (column < 0 || column >= m_itemData.size())
        return false;

    if (m_itemData[column] != value) {
        m_itemData[column] = value;
        return true;
    }
    else {
        return false;
    }
}



/*
 *  Network Items
 */
NetworkManager::ActiveConnection::State QNetworkItem::connectionState() const
{
    return m_connectionState;
}

void QNetworkItem::setConnectionState(NetworkManager::ActiveConnection::State state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        m_changedRoles << ItemRole::ConnectionStateRole << ItemRole::SectionRole;
        refreshIcon();
    }
}


QVariant QNetworkItem::deviceName() const
{
    return m_deviceName;
}

void QNetworkItem::setDeviceName(const QVariant &name)
{
    if (m_deviceName != name) {
        m_deviceName = name;
        m_changedRoles << ItemRole::DeviceName;
    }
}


QVariant QNetworkItem::devicePath() const
{
    return m_devicePath;
}

void QNetworkItem::setDevicePath(const QVariant &path)
{
    if (m_devicePath != path) {
        m_devicePath = path;
        m_changedRoles << ItemRole::DevicePathRole
                       << ItemRole::ItemTypeRole
                       << ItemRole::UniRole;
    }
}


QString QNetworkItem::icon() const
{
    return m_icon;
}

void QNetworkItem::setIcon(const QString &icon)
{
    if (icon != m_icon) {
        m_icon = icon;
        m_changedRoles << ItemRole::ConnectionIconRole;
    }
}

void QNetworkItem::refreshIcon()
{
    setIcon(computeIcon());
}

QString QNetworkItem::computeIcon() const
{
    switch (m_type) {
    case NetworkManager::ConnectionSettings::Bridge:
        break;

    case NetworkManager::ConnectionSettings::Wired:
        if (m_connectionState == NetworkManager::ActiveConnection::Activated)
            return QStringLiteral("network-wired");
        else
            return QStringLiteral("network-wired");
        break;

    case NetworkManager::ConnectionSettings::Wireless:
        if (m_securityType <= NetworkManager::NoneSecurity)
            return QStringLiteral(":/icons/network/24/network-wireless.svg");
        else if (m_securityType <= NetworkManager::UnknownSecurity)
            return QStringLiteral(":/icons/network/24/network-wireless-available.svg");
        else
            return QStringLiteral(":/icons/network/24/network-wireless-locked.svg");

    default:
        break;
    }

    if (m_connectionState == NetworkManager::ActiveConnection::Activated)
        return QStringLiteral("network-wired");
    else
        return QStringLiteral("network-wired");
}


QVariant QNetworkItem::specificPath() const
{
    return m_specificPath;
}

void QNetworkItem::setSpecificPath(const QVariant &path)
{
    if (m_specificPath != path) {
        m_specificPath = path;
        m_changedRoles << ItemRole::SpecificPathRole;
    }
}


NetworkManager::WirelessSecurityType QNetworkItem::securityType() const
{
    return m_securityType;
}

void QNetworkItem::setSecurityType(NetworkManager::WirelessSecurityType type)
{
    if (m_securityType != type) {
        m_securityType = type;
        m_changedRoles << ItemRole::SecurityTypeStringRole << ItemRole::SecurityTypeRole;
        refreshIcon();
    }
}


QVariant QNetworkItem::ssid() const
{
    return m_ssid;
}

void QNetworkItem::setSsid(const QVariant &ssid)
{
    if (m_ssid != ssid) {
        m_ssid = ssid;
        m_changedRoles << ItemRole::SsidRole << ItemRole::UniRole;
    }
}


QVariant QNetworkItem::uuid() const
{
    return m_uuid;
}

void QNetworkItem::setUuid(const QVariant &uuid)
{
    if (m_uuid != uuid) {
        m_uuid = uuid;
        m_changedRoles << ItemRole::UuidRole;
    }
}


NetworkManager::ConnectionSettings::ConnectionType QNetworkItem::type() const
{
    return m_type;
}

void QNetworkItem::setType(NetworkManager::ConnectionSettings::ConnectionType type)
{
    if (m_type != type) {
        m_type = type;
        m_changedRoles << ItemRole::TypeRole << ItemRole::ItemTypeRole << ItemRole::UniRole;

        refreshIcon();
    }
}


/*
QVariant QNetworkItem::uni() const
{
    if (m_type == NetworkManager::ConnectionSettings::Wireless && m_uuid.toString().isEmpty()) {
        return m_ssid.toString() + '%' + m_devicePath.toString();
    }
    else {
        return m_connectionPath.toString() + '%' + m_devicePath.toString();
    }
}
*/
