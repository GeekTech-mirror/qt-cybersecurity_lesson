#ifndef NetworkItem_H
#define NetworkItem_H

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Utils>

#include "network_enums.h"

class NetworkItem : public QObject
{
    Q_OBJECT
public:
    explicit NetworkItem(NetworkItem *parent = nullptr);
    explicit NetworkItem(const QVector<QString> &data,
                          NetworkItem *parent = nullptr);
    ~NetworkItem();

    /* Tree Items */
    NetworkItem *parent();
    NetworkItem *child (int row);
    int childNumber() const;
    int childCount() const;
    int columnCount() const;

    bool insertChildren (int position, int count, int columns);
    bool removeChildren (int position, int count);

    QVariant data (int column) const;
    bool setData (int column, const QVariant &value);

    QString headerData (int column) const;
    bool setHeaderData (int column, const QString &value);


    /* Network Items */
    NetworkManager::ActiveConnection::State connectionState() const;
    void setConnectionState (NetworkManager::ActiveConnection::State state);

    QString deviceName() const;
    void setDeviceName (const QString &name);

    QString devicePath() const;
    void setDevicePath (const QString &path);

    QString icon() const;
    void setIcon (const QString &icon);
    void refreshIcon();

    QString name() const;
    void setName (const QString &network);

    QString specificPath() const;
    void setSpecificPath (const QString &path);

    NetworkManager::WirelessSecurityType securityType() const;
    void setSecurityType (NetworkManager::WirelessSecurityType type);

    QString ssid() const;
    void setSsid (const QString &ssid);

    QString uuid() const;
    void setUuid (const QString &uuid);

    NetworkManager::ConnectionSettings::ConnectionType type() const;
    void setType (NetworkManager::ConnectionSettings::ConnectionType type);

    QVariant uni() const;


    int getRole (int index) const
    {
        return m_roles.at(index);
    }
    void insertRole (const ItemRole roles)
    {
            m_roles << roles;
    }

    QVector<int> changedRoles()
    {
        return m_changedRoles;
    }
    void clearChangedRoles()
    {
        m_changedRoles.clear();
    }

private:
    /* Tree Items */
    NetworkItem *m_parentItem;
    QVector<NetworkItem*> m_childItems;
    QVector<QString> m_headerData;
    QString indent;

    /* Network Items */
    QString computeIcon() const;

    NetworkManager::ActiveConnection::State m_connectionState;
    NetworkManager::WirelessSecurityType m_securityType;
    NetworkManager::ConnectionSettings::ConnectionType m_type;
    QString m_deviceName;
    QString m_devicePath;
    QString m_name;
    QString m_specificPath;
    QString m_ssid;
    QString m_uuid;
    QString m_icon;

    QVector<int> m_roles;
    QVector<int> m_changedRoles;
};

#endif // NetworkItem_H
