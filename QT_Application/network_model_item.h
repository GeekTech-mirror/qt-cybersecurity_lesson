#ifndef QNetworkItem_H
#define QNetworkItem_H

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Utils>

class QNetworkItem
{
public:
    explicit QNetworkItem(const QVector<QVariant> &data,
                          QNetworkItem *parent = nullptr);
    ~QNetworkItem();

    /* Tree Items */
    QNetworkItem *parent();
    QNetworkItem *child(int row);
    int childNumber() const;
    int childCount() const;
    int columnCount() const;

    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);

    bool insertColumns(int position, int columns);
    bool removeColumns(int position, int columns);

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);


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

    QVector<int> changedRoles() const
    {
        return m_changedRoles;
    }
    void clearChangedRoles()
    {
        m_changedRoles.clear();
    }

private:
    /* Tree Items */
    QNetworkItem *m_parentItem;
    QVector<QNetworkItem*> m_childItems;
    QVector<QVariant> m_itemData;

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

    QVector<int> m_changedRoles;
};

#endif // QNetworkItem_H
