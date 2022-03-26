#ifndef QNetworkModelItem_H
#define QNetworkModelItem_H

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Utils>

#include "network_model.h"

class QNetworkModelItem : public QObject
{
    Q_OBJECT
public:
    enum ItemType { UnavailableConnection, AvailableConnection, AvailableAccessPoint };

    explicit QNetworkModelItem(QObject *parent = nullptr);
    explicit QNetworkModelItem(const QNetworkModelItem *item, QObject *parent = nullptr);
    ~QNetworkModelItem() override;

    QString activeConnectionPath() const;
    void setActiveConnectionPath(const QString &path);

    QString connectionPath() const;
    void setConnectionPath(const QString &path);

    NetworkManager::ActiveConnection::State connectionState() const;
    void setConnectionState(NetworkManager::ActiveConnection::State state);

    QStringList details() const;

    QString deviceName() const;
    void setDeviceName(const QString &name);

    QString devicePath() const;
    void setDevicePath(const QString &path);

    QString deviceState() const;
    void setDeviceState(const NetworkManager::Device::State state);

    bool duplicate() const;

    void setIcon(const QString &icon);
    QString icon() const
    {
        return m_icon;
    }

    ItemType itemType() const;

    NetworkManager::WirelessSetting::NetworkMode mode() const;
    void setMode(const NetworkManager::WirelessSetting::NetworkMode mode);

    QString name() const;
    void setName(const QString &name);

    QString originalName() const;

    QString sectionType() const;

    NetworkManager::WirelessSecurityType securityType() const;
    void setSecurityType(NetworkManager::WirelessSecurityType type);

    int signal() const;
    void setSignal(int signal);

    bool slave() const;
    void setSlave(bool slave);

    QString specificPath() const;
    void setSpecificPath(const QString &path);

    QString ssid() const;
    void setSsid(const QString &ssid);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &date);

    NetworkManager::ConnectionSettings::ConnectionType type() const;
    void setType(NetworkManager::ConnectionSettings::ConnectionType type);

    QString uni() const;

    QString uuid() const;
    void setUuid(const QString &uuid);

    qulonglong rxBytes() const;
    void setRxBytes(qulonglong bytes);

    qulonglong txBytes() const;
    void setTxBytes(qulonglong bytes);

    bool operator==(const QNetworkModelItem *item) const;

    QVector<int> changedRoles() const
    {
        return m_changedRoles;
    }
    void clearChangedRoles()
    {
        m_changedRoles.clear();
    }

public Q_SLOTS:
    void invalidateDetails();

private:
    QString computeIcon() const;
    void refreshIcon();
//    void updateDetails() const;

    QString m_activeConnectionPath;
    QString m_connectionPath;
    NetworkManager::ActiveConnection::State m_connectionState;
    QString m_devicePath;
    QString m_deviceName;
    NetworkManager::Device::State m_deviceState;
    mutable QStringList m_details;
    mutable bool m_detailsValid;
    bool m_duplicate;
    NetworkManager::WirelessSetting::NetworkMode m_mode;
    QString m_name;
    NetworkManager::WirelessSecurityType m_securityType;
    int m_signal;
    bool m_slave;
    QString m_specificPath;
    QString m_ssid;
    QDateTime m_timestamp;
    NetworkManager::ConnectionSettings::ConnectionType m_type;
    QString m_uuid;
    qulonglong m_rxBytes;
    qulonglong m_txBytes;
    QString m_icon;
    QVector<int> m_changedRoles;

signals:

};

#endif // QNetworkModelItem_H
