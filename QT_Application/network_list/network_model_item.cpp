#include <NetworkManagerQt/BridgeDevice>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WirelessSetting>
#include <NetworkManagerQt/AccessPoint>

#include "network_model_item.h"

QNetworkModelItem::QNetworkModelItem(QObject *parent)
    : QObject(parent)
    , m_connectionState(NetworkManager::ActiveConnection::Deactivated)
    , m_deviceState(NetworkManager::Device::UnknownState)
    , m_detailsValid(false)
    , m_duplicate(false)
    , m_mode(NetworkManager::WirelessSetting::Infrastructure)
    , m_securityType(NetworkManager::NoneSecurity)
    , m_signal(0)
    , m_slave(false)
    , m_type(NetworkManager::ConnectionSettings::Unknown)
    , m_rxBytes(0)
    , m_txBytes(0)
{
}

QNetworkModelItem::QNetworkModelItem(const QNetworkModelItem *item, QObject *parent)
    : QObject(parent)
    , m_connectionPath(item->connectionPath())
    , m_connectionState(NetworkManager::ActiveConnection::Deactivated)
    , m_detailsValid(false)
    , m_duplicate(true)
    , m_mode(item->mode())
    , m_name(item->name())
    , m_securityType(item->securityType())
    , m_slave(item->slave())
    , m_ssid(item->ssid())
    , m_timestamp(item->timestamp())
    , m_type(item->type())
    , m_uuid(item->uuid())
    , m_rxBytes(0)
    , m_txBytes(0)
{
}

QNetworkModelItem::~QNetworkModelItem() = default;

QString QNetworkModelItem::activeConnectionPath() const
{
    return m_activeConnectionPath;
}

void QNetworkModelItem::setActiveConnectionPath(const QString &path)
{
    m_activeConnectionPath = path;
}

QString QNetworkModelItem::connectionPath() const
{
    return m_connectionPath;
}

void QNetworkModelItem::setConnectionPath(const QString &path)
{
    if (m_connectionPath != path) {
        m_connectionPath = path;
        m_changedRoles << QNetworkModel::ConnectionPathRole << QNetworkModel::UniRole;
    }
}

NetworkManager::ActiveConnection::State QNetworkModelItem::connectionState() const
{
    return m_connectionState;
}

void QNetworkModelItem::setConnectionState(NetworkManager::ActiveConnection::State state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        m_changedRoles << QNetworkModel::ConnectionStateRole << QNetworkModel::SectionRole;
        refreshIcon();
    }
}

/*
QStringList QNetworkModelItem::details() const
{
    if (!m_detailsValid) {
        updateDetails();
    }
    return m_details;
}
*/

QString QNetworkModelItem::devicePath() const
{
    return m_devicePath;
}

QString QNetworkModelItem::deviceName() const
{
    return m_deviceName;
}

void QNetworkModelItem::setDeviceName(const QString &name)
{
    if (m_deviceName != name) {
        m_deviceName = name;
        m_changedRoles << QNetworkModel::DeviceName;
    }
}

void QNetworkModelItem::setDevicePath(const QString &path)
{
    if (m_devicePath != path) {
        m_devicePath = path;
        m_changedRoles << QNetworkModel::DevicePathRole << QNetworkModel::ItemTypeRole << QNetworkModel::UniRole;
    }
}

void QNetworkModelItem::setDeviceState(const NetworkManager::Device::State state)
{
    if (m_deviceState != state) {
        m_deviceState = state;
        m_changedRoles << QNetworkModel::DeviceStateRole;
    }
}

bool QNetworkModelItem::duplicate() const
{
    return m_duplicate;
}

void QNetworkModelItem::setIcon(const QString &icon)
{
    if (icon != m_icon) {
        m_icon = icon;
        m_changedRoles << QNetworkModel::ConnectionIconRole;
    }
}

void QNetworkModelItem::refreshIcon()
{
    setIcon(computeIcon());
}

QString QNetworkModelItem::computeIcon() const
{
    switch (m_type) {
    case NetworkManager::ConnectionSettings::Bridge:
        break;
    case NetworkManager::ConnectionSettings::Wired:
        if (m_connectionState == NetworkManager::ActiveConnection::Activated) {
            return QStringLiteral("network-wired-activated");
        } else {
            return QStringLiteral("network-wired");
        }
        break;
    case NetworkManager::ConnectionSettings::Wireless:
        if (m_signal == 0) {
            if (m_mode == NetworkManager::WirelessSetting::Adhoc || m_mode == NetworkManager::WirelessSetting::Ap) {
                return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-100")
                                                                        : QStringLiteral("network-wireless-100-locked");
            }
            return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-0") : QStringLiteral("network-wireless-0-locked");
        } else if (m_signal < 20) {
            return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-20") : QStringLiteral("network-wireless-20-locked");
        } else if (m_signal < 40) {
            return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-40") : QStringLiteral("network-wireless-40-locked");
        } else if (m_signal < 60) {
            return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-60") : QStringLiteral("network-wireless-60-locked");
        } else if (m_signal < 80) {
            return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-80") : QStringLiteral("network-wireless-80-locked");
        } else {
            return (m_securityType <= NetworkManager::NoneSecurity) ? QStringLiteral("network-wireless-100") : QStringLiteral("network-wireless-100-locked");
        }
        break;
    default:
        break;
    }

    if (m_connectionState == NetworkManager::ActiveConnection::Activated) {
        return QStringLiteral("network-wired-activated");
    } else {
        return QStringLiteral("network-wired");
    }
}

QNetworkModelItem::ItemType QNetworkModelItem::itemType() const
{
    if (!m_devicePath.isEmpty() //
        || m_type == NetworkManager::ConnectionSettings::Bridge //
        || ((NetworkManager::status() == NetworkManager::Connected //
             || NetworkManager::status() == NetworkManager::ConnectedLinkLocal //
             || NetworkManager::status() == NetworkManager::ConnectedSiteOnly))) {
        if (m_connectionPath.isEmpty() && m_type == NetworkManager::ConnectionSettings::Wireless) {
            return QNetworkModelItem::AvailableAccessPoint;
        } else {
            return QNetworkModelItem::AvailableConnection;
        }
    }
    return QNetworkModelItem::UnavailableConnection;
}

NetworkManager::WirelessSetting::NetworkMode QNetworkModelItem::mode() const
{
    return m_mode;
}

void QNetworkModelItem::setMode(const NetworkManager::WirelessSetting::NetworkMode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
        refreshIcon();
    }
}

QString QNetworkModelItem::name() const
{
    return m_name;
}

void QNetworkModelItem::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        m_changedRoles << QNetworkModel::ItemUniqueNameRole << QNetworkModel::NameRole;
    }
}

QString QNetworkModelItem::originalName() const
{
    if (m_deviceName.isEmpty()) {
        return m_name;
    }
    return m_name + QLatin1String(" (") + m_deviceName + ')';
}

QString QNetworkModelItem::sectionType() const
{
    if (m_connectionState == NetworkManager::ActiveConnection::Deactivated) {
        return QStringLiteral("Available connections");
    } else {
        return {};
    }
}

NetworkManager::WirelessSecurityType QNetworkModelItem::securityType() const
{
    return m_securityType;
}

void QNetworkModelItem::setSecurityType(NetworkManager::WirelessSecurityType type)
{
    if (m_securityType != type) {
        m_securityType = type;
        m_changedRoles << QNetworkModel::SecurityTypeStringRole << QNetworkModel::SecurityTypeRole;
        refreshIcon();
    }
}

int QNetworkModelItem::signal() const
{
    return m_signal;
}

void QNetworkModelItem::setSignal(int signal)
{
    if (m_signal != signal) {
        m_signal = signal;
        m_changedRoles << QNetworkModel::SignalRole;
        refreshIcon();
    }
}

bool QNetworkModelItem::slave() const
{
    return m_slave;
}

void QNetworkModelItem::setSlave(bool slave)
{
    if (m_slave != slave) {
        m_slave = slave;
        m_changedRoles << QNetworkModel::SlaveRole;
    }
}

QString QNetworkModelItem::specificPath() const
{
    return m_specificPath;
}

void QNetworkModelItem::setSpecificPath(const QString &path)
{
    if (m_specificPath != path) {
        m_specificPath = path;
        m_changedRoles << QNetworkModel::SpecificPathRole;
    }
}

QString QNetworkModelItem::ssid() const
{
    return m_ssid;
}

void QNetworkModelItem::setSsid(const QString &ssid)
{
    if (m_ssid != ssid) {
        m_ssid = ssid;
        m_changedRoles << QNetworkModel::SsidRole << QNetworkModel::UniRole;
    }
}

NetworkManager::ConnectionSettings::ConnectionType QNetworkModelItem::type() const
{
    return m_type;
}


QDateTime QNetworkModelItem::timestamp() const
{
    return m_timestamp;
}


void QNetworkModelItem::setTimestamp(const QDateTime &date)
{
    if (m_timestamp != date) {
        m_timestamp = date;
        m_changedRoles << QNetworkModel::TimeStampRole;
    }
}


void QNetworkModelItem::setType(NetworkManager::ConnectionSettings::ConnectionType type)
{
    if (m_type != type) {
        m_type = type;
        m_changedRoles << QNetworkModel::TypeRole << QNetworkModel::ItemTypeRole << QNetworkModel::UniRole;

        refreshIcon();
    }
}

QString QNetworkModelItem::uni() const
{
    if (m_type == NetworkManager::ConnectionSettings::Wireless && m_uuid.isEmpty()) {
        return m_ssid + '%' + m_devicePath;
    } else {
        return m_connectionPath + '%' + m_devicePath;
    }
}

QString QNetworkModelItem::uuid() const
{
    return m_uuid;
}

void QNetworkModelItem::setUuid(const QString &uuid)
{
    if (m_uuid != uuid) {
        m_uuid = uuid;
        m_changedRoles << QNetworkModel::UuidRole;
    }
}

qulonglong QNetworkModelItem::rxBytes() const
{
    return m_rxBytes;
}

void QNetworkModelItem::setRxBytes(qulonglong bytes)
{
    if (m_rxBytes != bytes) {
        m_rxBytes = bytes;
        m_changedRoles << QNetworkModel::RxBytesRole;
    }
}

qulonglong QNetworkModelItem::txBytes() const
{
    return m_txBytes;
}

void QNetworkModelItem::setTxBytes(qulonglong bytes)
{
    if (m_txBytes != bytes) {
        m_txBytes = bytes;
        m_changedRoles << QNetworkModel::TxBytesRole;
    }
}

bool QNetworkModelItem::operator==(const QNetworkModelItem *item) const
{
    if (!item->uuid().isEmpty() && !uuid().isEmpty()) {
        if (item->devicePath() == devicePath() && item->uuid() == uuid()) {
            return true;
        }
    } else if (item->type() == NetworkManager::ConnectionSettings::Wireless && type() == NetworkManager::ConnectionSettings::Wireless) {
        if (item->ssid() == ssid() && item->devicePath() == devicePath()) {
            return true;
        }
    }

    return false;
}

void QNetworkModelItem::invalidateDetails()
{
    m_detailsValid = false;
    m_changedRoles << QNetworkModel::ConnectionDetailsRole;
}

/*
void QNetworkModelItem::updateDetails() const
{
    m_detailsValid = true;
    m_details.clear();

    if (itemType() == QNetworkModelItem::UnavailableConnection) {
        return;
    }

    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(m_devicePath);

    // Get IPv[46]Address and related nameservers + IPv[46] default gateway
    if (device && device->ipV4Config().isValid() && m_connectionState == NetworkManager::ActiveConnection::Activated) {
        if (!device->ipV4Config().addresses().isEmpty()) {
            QHostAddress addr = device->ipV4Config().addresses().first().ip();
            if (!addr.isNull() && addr.isGlobal()) {
                m_details << i18n("IPv4 Address") << addr.toString();
            }
        }
        if (!device->ipV4Config().gateway().isEmpty()) {
            QString addr = device->ipV4Config().gateway();
            if (!addr.isNull()) {
                m_details << i18n("IPv4 Default Gateway") << addr;
            }
        }
        if (!device->ipV4Config().nameservers().isEmpty()) {
            QHostAddress addr1 = device->ipV4Config().nameservers().first();
            QHostAddress addr2 = device->ipV4Config().nameservers().last();
            if (!addr1.isNull()) {
                m_details << i18n("IPv4 Primary Nameserver") << addr1.toString();
            }
            if (!addr2.isNull() && !addr1.isNull()) {
                if (addr2 != addr1) {
                    m_details << i18n("IPv4 Secondary Nameserver") << addr2.toString();
                }
            }
        }
    }

    if (device && device->ipV6Config().isValid() && m_connectionState == NetworkManager::ActiveConnection::Activated) {
        if (!device->ipV6Config().addresses().isEmpty()) {
            QHostAddress addr = device->ipV6Config().addresses().first().ip();
            if (!addr.isNull() && addr.isGlobal() && !addr.isUniqueLocalUnicast()) {
                m_details << i18n("IPv6 Address") << addr.toString();
            } else if (!addr.isNull() && addr.isGlobal() && addr.isUniqueLocalUnicast()) {
                m_details << i18n("IPv6 ULA Address") << addr.toString();
            }
        }
        if (!device->ipV6Config().gateway().isEmpty()) {
            QString addr = device->ipV6Config().gateway();
            if (!addr.isNull()) {
                m_details << i18n("IPv6 Default Gateway") << addr;
            }
        }
        if (!device->ipV6Config().nameservers().isEmpty()) {
            QHostAddress addr1 = device->ipV6Config().nameservers().first();
            QHostAddress addr2 = device->ipV6Config().nameservers().last();
            if (!addr1.isNull()) {
                m_details << i18n("IPv6 Primary Nameserver") << addr1.toString();
            }
            if (!addr2.isNull() && !addr1.isNull()) {
                if (addr2 != addr1) {
                    m_details << i18n("IPv6 Secondary Nameserver") << addr2.toString();
                }
            }
        }
    }
    if (m_type == NetworkManager::ConnectionSettings::Wired) {
        NetworkManager::WiredDevice::Ptr wiredDevice = device.objectCast<NetworkManager::WiredDevice>();
        if (wiredDevice) {
            if (m_connectionState == NetworkManager::ActiveConnection::Activated) {
                m_details << i18n("Connection speed") << UiUtils::connectionSpeed(wiredDevice->bitRate());
            }
            m_details << i18n("MAC Address") << wiredDevice->permanentHardwareAddress();
        }
    } else if (m_type == NetworkManager::ConnectionSettings::Wireless) {
        NetworkManager::WirelessDevice::Ptr wirelessDevice = device.objectCast<NetworkManager::WirelessDevice>();
        m_details << i18n("Access point (SSID)") << m_ssid;
        if (m_mode == NetworkManager::WirelessSetting::Infrastructure) {
            m_details << i18n("Signal strength") << QStringLiteral("%1%").arg(m_signal);
        }
        m_details << i18n("Security type") << UiUtils::labelFromWirelessSecurity(m_securityType);
        if (wirelessDevice) {
            if (m_connectionState == NetworkManager::ActiveConnection::Activated) {
                m_details << i18n("Connection speed") << UiUtils::connectionSpeed(wirelessDevice->bitRate());
            }
            m_details << i18n("MAC Address") << wirelessDevice->permanentHardwareAddress();
        }
    } else if (m_type == NetworkManager::ConnectionSettings::Bridge) {
        NetworkManager::BridgeDevice::Ptr bridgeDevice = device.objectCast<NetworkManager::BridgeDevice>();
        m_details << i18n("Type") << i18n("Bridge");
        if (bridgeDevice) {
            m_details << i18n("MAC Address") << bridgeDevice->hwAddress();
        }
    }


    if (device && m_connectionState == NetworkManager::ActiveConnection::Activated) {
        m_details << i18n("Device") << device->interfaceName();
    }
}
*/
