#include <NetworkManagerQt/BridgeDevice>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WirelessSetting>
#include <NetworkManagerQt/AccessPoint>

#include "network_model_item.h"
#include "network_model.h"

QNetworkItem::QNetworkItem(const QVector<QVariant> &data, QNetworkItem *parent)
    : m_itemData(data)
    , m_parentItem(parent)
{
}

QNetworkItem::~QNetworkItem()
{
    qDeleteAll(m_childItems);
}


/* new */
QNetworkItem *QNetworkItem::child(int number)
{
    if (number < 0 || number >= m_childItems.size())
        return nullptr;
    return m_childItems.at(number);
}

int QNetworkItem::childCount() const
{
    return m_childItems.count();
}

int QNetworkItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<QNetworkItem*>(this));
    return 0;
}


int QNetworkItem::columnCount() const
{
    return m_itemData.count();
}

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

bool QNetworkItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, QVariant());

    for (QNetworkItem *child : qAsConst(m_childItems))
        child->insertColumns(position, columns);

    return true;
}

QNetworkItem *QNetworkItem::parent()
{
    return m_parentItem;
}

bool QNetworkItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

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

int QNetworkItem::row() const
{
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<QNetworkItem*>(this));

        return 0;
}

QVariant QNetworkItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

bool QNetworkItem::setData(int column, const QVariant &value)
{

    if (column < 0 || column >= m_itemData.size())
        return false;

    if (m_itemData != value) {
        m_itemData[column] = value;
        return true;
    }
    else {
        return false;
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
        m_changedRoles << QNetworkModel::SsidRole << QNetworkModel::UniRole;
    }
}

QVariant QNetworkItem::uni() const
{
    if (m_type == NetworkManager::ConnectionSettings::Wireless && m_uuid.toString().isEmpty()) {
        return m_ssid.toString() + '%' + m_devicePath.toString();
    } else {
        return m_connectionPath.toString() + '%' + m_devicePath.toString();
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
        m_changedRoles << QNetworkModel::UuidRole;
    }
}

QVariant QNetworkItem::devicePath() const
{
    return m_devicePath;
}

void QNetworkItem::setDevicePath(const QString &path)
{
    if (m_devicePath != path) {
        m_devicePath = path;
        m_changedRoles << QNetworkModel::DevicePathRole << QNetworkModel::ItemTypeRole << QNetworkModel::UniRole;
    }
}

QVariant QNetworkItem::connectionPath() const
{
    return m_connectionPath;
}

void QNetworkItem::setConnectionPath(const QVariant &path)
{
    if (m_connectionPath != path) {
        m_connectionPath = path;
        m_changedRoles << QNetworkModel::ConnectionPathRole << QNetworkModel::UniRole;
    }
}

void QNetworkItem::setType(NetworkManager::ConnectionSettings::ConnectionType type)
{
    if (m_type != type) {
        m_type = type;
        m_changedRoles << QNetworkModel::TypeRole << QNetworkModel::ItemTypeRole << QNetworkModel::UniRole;
    }
}

QNetworkItem::ItemType QNetworkItem::itemType() const
{
    if (!m_devicePath.toString().isEmpty() //
        || m_type == NetworkManager::ConnectionSettings::Bridge //
        || ((NetworkManager::status() == NetworkManager::Connected //
             || NetworkManager::status() == NetworkManager::ConnectedLinkLocal //
             || NetworkManager::status() == NetworkManager::ConnectedSiteOnly))) {
        if (m_connectionPath.toString().isEmpty() && m_type == NetworkManager::ConnectionSettings::Wireless) {
            return QNetworkItem::AvailableAccessPoint;
        } else {
            return QNetworkItem::AvailableConnection;
        }
    }
    return QNetworkItem::UnavailableConnection;
}

NetworkManager::WirelessSetting::NetworkMode QNetworkItem::mode() const
{
    return m_mode;
}

void QNetworkItem::setMode(const NetworkManager::WirelessSetting::NetworkMode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
    }
}

/* old*/
/*
QVariant QNetworkItem::activeConnectionPath(int column) const
{
    return m_activeConnectionPath[column];
}

void QNetworkItem::setActiveConnectionPath(int column, const QVariant &path)
{
    m_activeConnectionPath[column] = path;
}

NetworkManager::ActiveConnection::State QNetworkItem::connectionState() const
{
    return m_connectionState;
}

void QNetworkItem::setConnectionState(NetworkManager::ActiveConnection::State state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
    }
}


QVariant QNetworkItem::deviceName(int column) const
{
    return m_deviceName.at(column);
}

void QNetworkItem::setDeviceName(int column, const QVariant &name)
{
    if (m_deviceName != name) {
        m_deviceName[column] = name;
        m_changedRoles << QNetworkModel::DeviceName;
    }
}



void QNetworkItem::setDeviceState(const NetworkManager::Device::State state)
{
    if (m_deviceState != state) {
        m_deviceState = state;
        m_changedRoles << QNetworkModel::DeviceStateRole;
    }
}

bool QNetworkItem::duplicate() const
{
    return m_duplicate;
}

void QNetworkItem::setIcon(int column, const QVariant &icon)
{
    if (icon != m_icon) {
        m_icon[column] = icon;
        m_changedRoles << QNetworkModel::ConnectionIconRole;
    }
}

QVariant QNetworkItem::name(int column) const
{
    return m_name.at(column);
}

void QNetworkItem::setName(int column, const QVariant &name)
{
    if (m_name != name) {
        m_name[column] = name;
        m_changedRoles << QNetworkModel::ItemUniqueNameRole << QNetworkModel::NameRole;
    }
}

QVariant QNetworkItem::originalName(int column) const
{
    if (m_deviceName.isEmpty()) {
        return m_name;
    }
    return m_name.at(column) + QLatin1String(" (") + m_deviceName.at(column) + ')';
}

QString QNetworkItem::sectionType() const
{
    if (m_connectionState == NetworkManager::ActiveConnection::Deactivated) {
        return QStringLiteral("Available connections");
    } else {
        return {};
    }
}

bool QNetworkItem::slave() const
{
    return m_slave;
}

void QNetworkItem::setSlave(bool slave)
{
    if (m_slave != slave) {
        m_slave = slave;
        m_changedRoles << QNetworkModel::SlaveRole;
    }
}

QVariant QNetworkItem::specificPath() const
{
    return m_specificPath;
}

void QNetworkItem::setSpecificPath(const QVariant &path)
{
    if (m_specificPath != path) {
        m_specificPath = path;
        m_changedRoles << QNetworkModel::SpecificPathRole;
    }
}

NetworkManager::ConnectionSettings::ConnectionType QNetworkItem::type() const
{
    return m_type;
}


QDateTime QNetworkItem::timestamp() const
{
    return m_timestamp;
}


void QNetworkItem::setTimestamp(const QDateTime &date)
{
    if (m_timestamp != date) {
        m_timestamp = date;
        m_changedRoles << QNetworkModel::TimeStampRole;
    }
}

bool QNetworkItem::operator==(const QNetworkItem *item) const
{
    if (!item->uuid().isEmpty() && !uuid().isEmpty()) {
        if (item->devicePath() == devicePath() && item->uuid() == uuid()) {
            return true;
        }
    }
    else if (item->type() == NetworkManager::ConnectionSettings::Wireless && type() == NetworkManager::ConnectionSettings::Wireless) {
        if (item->ssid() == ssid() && item->devicePath() == devicePath()) {
            return true;
        }
    }

    return false;
}
*/
