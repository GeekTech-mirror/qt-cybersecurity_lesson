#include <QIcon>

#include <NetworkManagerQt/BridgeDevice>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WirelessSetting>
#include <NetworkManagerQt/AccessPoint>

#include "network_item.h"
#include "network_enums.h"

NetworkItem::NetworkItem(NetworkItem *parent)
    : m_parentItem(parent)
{
}

NetworkItem::NetworkItem(const QVector<QVariant> &data, NetworkItem *parent)
    : m_headerData(data)
    , m_parentItem(parent)
{
    for (int i = 0; i < m_headerData.size(); ++i)
    {
        this->setRole(ItemRole::HeaderRole);
    }
}

NetworkItem::~NetworkItem()
{
    qDeleteAll(m_childItems);
}


/* return pointer to parent */
NetworkItem *NetworkItem::parent()
{
    return m_parentItem;
}

/* return pointer to child at "row" */
NetworkItem *NetworkItem::child(int number)
{
    if (number < 0 || number >= m_childItems.size())
        return nullptr;
    return m_childItems.at(number);
}

/* return total number of children */
int NetworkItem::childCount() const
{
    return m_childItems.count();
}

/* return index of child in its parent's list of children */
/* return zero for the root item */
int NetworkItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<NetworkItem*>(this));
    return 0;
}

/* return number of elements in the internal itemData list */
int NetworkItem::columnCount() const
{
    return m_roles.count();
}

/* insert row below specified index  */
bool NetworkItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        NetworkItem *item = new NetworkItem(this);
        m_childItems.insert(position, item);
    }

    return true;
}

/* remove row at specified index */
bool NetworkItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

/* insert column (perform action on every row) */
bool NetworkItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, int());

    for (NetworkItem *child : qAsConst(m_childItems))
        child->insertColumns(position, columns);

    return true;
}

/* remove column */
bool NetworkItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.remove(position);

    for (NetworkItem *child : qAsConst(m_childItems))
        child->removeColumns(position, columns);

    return true;
}


/* temporary until better role handling */
QString getSecurityString(NetworkManager::WirelessSecurityType type)
{
    switch (type) {
    case NetworkManager::NoneSecurity:
        return QStringLiteral("None");
        break;

    case NetworkManager::StaticWep:
        return QStringLiteral("Static-Wep");
        break;

    case NetworkManager::DynamicWep:
        return QStringLiteral("Dynamic-Wep");
        break;

    case NetworkManager::Leap:
        return QStringLiteral("Leap");
        break;

    case NetworkManager::WpaPsk:
        return QStringLiteral("Wpa-Psk");
        break;

    case NetworkManager::WpaEap:
        return QStringLiteral("Wpa-Eap");
        break;

    case NetworkManager::Wpa2Psk:
        return QStringLiteral("Wpa2-Psk");
        break;

    case NetworkManager::Wpa2Eap:
        return QStringLiteral("Wpa2-Eap");
        break;

    case NetworkManager::SAE:
        return QStringLiteral("SAE");
        break;

    case NetworkManager::Wpa3SuiteB192:
        return QStringLiteral("Wpa3-Suite-B192");
        break;

    default:
        return QStringLiteral("Unknown");
    }

    return QStringLiteral("error");
}


/* retrieve data from itemData list at specified column */
QVariant NetworkItem::data(int column) const
{
    if (column < 0 || column >= m_roles.size())
        return QVariant();

    switch (m_roles.at(column)) {
    case ConnectionIconRole:
        return QIcon (m_icon);
    case ConnectionStateRole:
        return m_connectionState;
    case DeviceName:
        return m_deviceName;
    case DevicePathRole:
        return m_devicePath;
    case HeaderRole:
        return m_headerData[column];
    case SsidRole:
        return m_ssid;
    case SpecificPathRole:
        return m_specificPath;
    case SecurityTypeRole:
        return getSecurityString(m_securityType);
    case TypeRole:
        return m_type;
    case UuidRole:
        return m_uuid;
    default:
        break;
    }

    return m_itemData.at(column);
}

/* store values in the itemData list for valid list indexes */
bool NetworkItem::setData(int column, const QVariant &value)
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


/* retrieve data from headerData list at specified column */
QVariant NetworkItem::headerData(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();

    return m_headerData[column];
}



bool NetworkItem::setHeaderData(int column, const QVariant &value)
{

    if (column < 0 || column >= m_itemData.size())
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
 *  Network Items
 */
NetworkManager::ActiveConnection::State NetworkItem::connectionState() const
{
    return m_connectionState;
}

void NetworkItem::setConnectionState(NetworkManager::ActiveConnection::State state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        m_changedRoles << ItemRole::ConnectionStateRole;
        refreshIcon();
    }
}


QVariant NetworkItem::deviceName() const
{
    return m_deviceName;
}

void NetworkItem::setDeviceName(const QVariant &name)
{
    if (m_deviceName != name) {
        m_deviceName = name;
        m_changedRoles << ItemRole::DeviceName;
    }
}


QVariant NetworkItem::devicePath() const
{
    return m_devicePath;
}

void NetworkItem::setDevicePath(const QVariant &path)
{
    if (m_devicePath != path) {
        m_devicePath = path;
        m_changedRoles << ItemRole::DevicePathRole;
    }
}


QString NetworkItem::icon() const
{
    return m_icon;
}

void NetworkItem::setIcon(const QString &icon)
{
    if (icon != m_icon) {
        m_icon = icon;
        m_changedRoles << ItemRole::ConnectionIconRole;
    }
}

void NetworkItem::refreshIcon()
{
    setIcon(computeIcon());
}

QString NetworkItem::computeIcon() const
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
        if (m_securityType == NetworkManager::NoneSecurity)
            return QStringLiteral(":/icons/network/24/network-wireless.svg");
        else if (m_securityType == NetworkManager::UnknownSecurity)
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


QVariant NetworkItem::specificPath() const
{
    return m_specificPath;
}

void NetworkItem::setSpecificPath(const QVariant &path)
{
    if (m_specificPath != path) {
        m_specificPath = path;
        m_changedRoles << ItemRole::SpecificPathRole;
    }
}


NetworkManager::WirelessSecurityType NetworkItem::securityType() const
{
    return m_securityType;
}

void NetworkItem::setSecurityType(NetworkManager::WirelessSecurityType type)
{
    if (m_securityType != type) {
        m_securityType = type;
        m_changedRoles << ItemRole::SecurityTypeStringRole << ItemRole::SecurityTypeRole;
        refreshIcon();
    }
}


QVariant NetworkItem::ssid() const
{
    return m_ssid;
}

void NetworkItem::setSsid(const QVariant &ssid)
{
    if (m_ssid != ssid) {
        m_ssid = ssid;
        m_changedRoles << ItemRole::SsidRole << ItemRole::UniRole;
    }
}


QVariant NetworkItem::uuid() const
{
    return m_uuid;
}

void NetworkItem::setUuid(const QVariant &uuid)
{
    if (m_uuid != uuid) {
        m_uuid = uuid;
        m_changedRoles << ItemRole::UuidRole;
    }
}


NetworkManager::ConnectionSettings::ConnectionType NetworkItem::type() const
{
    return m_type;
}

void NetworkItem::setType(NetworkManager::ConnectionSettings::ConnectionType type)
{
    if (m_type != type) {
        m_type = type;
        m_changedRoles << ItemRole::TypeRole << ItemRole::ItemTypeRole;

        refreshIcon();
    }
}


/*
QVariant NetworkItem::uni() const
{
    if (m_type == NetworkManager::ConnectionSettings::Wireless && m_uuid.toString().isEmpty()) {
        return m_ssid.toString() + '%' + m_devicePath.toString();
    }
    else {
        return m_connectionPath.toString() + '%' + m_devicePath.toString();
    }
}
*/
