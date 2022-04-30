#ifndef NetworkItem_H
#define NetworkItem_H

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Utils>

#include "network_enums.h"

class NetworkItem
{
public:
    explicit NetworkItem(NetworkItem *parent = nullptr);
    explicit NetworkItem(const QVector<QVariant> &data,
                          NetworkItem *parent = nullptr);
    ~NetworkItem();

    /* Tree Items */
    NetworkItem *parent();
    NetworkItem *child(int row);
    int childNumber() const;
    int childCount() const;
    int columnCount() const;

    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);

    bool insertColumns(int position, int columns);
    bool removeColumns(int position, int columns);

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    QVariant headerData(int column) const;
    bool setHeaderData(int column, const QVariant &value);


    /* Network Items */
    NetworkManager::ActiveConnection::State connectionState() const;
    void setConnectionState(NetworkManager::ActiveConnection::State state);

    QVariant deviceName() const;
    void setDeviceName(const QVariant &name);

    QVariant devicePath() const;
    void setDevicePath(const QVariant &path);

    void setIcon(const QString &icon);
    QString icon() const;

    QVariant specificPath() const;
    void setSpecificPath(const QVariant &path);

    NetworkManager::WirelessSecurityType securityType() const;
    void setSecurityType(NetworkManager::WirelessSecurityType type);

    QVariant ssid() const;
    void setSsid(const QVariant &ssid);

    QVariant uuid() const;
    void setUuid(const QVariant &uuid);

    NetworkManager::ConnectionSettings::ConnectionType type() const;
    void setType(NetworkManager::ConnectionSettings::ConnectionType type);

    QVariant uni() const;


    int role(int index) const
    {
        return m_roles.at(index);
    }
    void setRole(const ItemRole roles)
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
    QVector<QVariant> m_itemData;
    QVector<QVariant> m_headerData;

    /* Network Items */
    QString computeIcon() const;
    void refreshIcon();

    NetworkManager::ActiveConnection::State m_connectionState;
    QVariant m_deviceName;
    QVariant m_devicePath;
    QVariant m_specificPath;
    NetworkManager::WirelessSecurityType m_securityType;
    QVariant m_ssid;
    NetworkManager::ConnectionSettings::ConnectionType m_type;
    QVariant m_uuid;
    QString m_icon;

    QVector<int> m_roles;
    QVector<int> m_changedRoles;
};

#endif // NetworkItem_H
