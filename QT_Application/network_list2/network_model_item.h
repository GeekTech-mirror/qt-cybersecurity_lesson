#ifndef QNetworkItem_H
#define QNetworkItem_H

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Utils>

//#include "network_model.h"

class QNetworkItem
{
public:
    enum ItemType { UnavailableConnection, AvailableConnection, AvailableAccessPoint };

    explicit QNetworkItem(const QVector<QVariant> &data, QNetworkItem *parent = nullptr);
    ~QNetworkItem();


    /* new */
    QNetworkItem *parent();
    QNetworkItem *child(int row);
    int childNumber() const;
    int childCount() const;
    int columnCount() const;

    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);

    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);



    QVariant ssid() const;
    void setSsid(const QVariant &ssid);

    QVariant uni() const;

    QVariant uuid() const;
    void setUuid(const QVariant &uuid);

    QVariant devicePath() const;
    void setDevicePath(const QString &path);

    QVariant connectionPath() const;
    void setConnectionPath(const QVariant &path);

    NetworkManager::ConnectionSettings::ConnectionType type() const;
    void setType(NetworkManager::ConnectionSettings::ConnectionType type);
    ItemType itemType() const;

    NetworkManager::WirelessSetting::NetworkMode mode() const;
    void setMode(const NetworkManager::WirelessSetting::NetworkMode mode);

    QVector<int> changedRoles() const
    {
        return m_changedRoles;
    }
    void clearChangedRoles()
    {
        m_changedRoles.clear();
    }

    /* old */
    /*
    QVariant activeConnectionPath(int column) const;
    void setActiveConnectionPath(int column, const QVariant &path);

    NetworkManager::ActiveConnection::State connectionState() const;
    void setConnectionState(NetworkManager::ActiveConnection::State state);

    QStringList details() const;

    QVariant deviceName(int column) const;
    void setDeviceName(int column, const QVariant &name);

    QVariant deviceState() const;
    void setDeviceState(const NetworkManager::Device::State state);

    bool duplicate() const;

    void setIcon(int column, const QVariant &icon);
    QVariant icon() const
    {
        return m_icon;
    }


    QVariant name(int column) const;
    void setName(int column, const QVariant &name);

    QVariant originalName(int column) const;

    QVariant sectionType() const;

    int signal() const;
    void setSignal(int signal);

    bool slave() const;
    void setSlave(bool slave);

    QVariant specificPath() const;
    void setSpecificPath(const QVariant &path);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &date);

    bool operator==(const QNetworkItem *item) const;
*/
private:
    /* new */
    QNetworkItem *m_parentItem;
    QVector<QNetworkItem*> m_childItems;
    QVector<QVariant> m_itemData;
    QVariant m_ssid;

    QVariant m_uuid;
    QVariant m_devicePath;
    QVariant m_connectionPath;
    NetworkManager::ConnectionSettings::ConnectionType m_type;
    NetworkManager::WirelessSetting::NetworkMode m_mode;
    QVector<int> m_changedRoles;


    /* old */
    /*
    QVariant computeIcon() const;
    QVector<QVariant> m_activeConnectionPath;
    NetworkManager::ActiveConnection::State m_connectionState;
    QVector<QVariant> m_deviceName;
    NetworkManager::Device::State m_deviceState;
    mutable QStringList m_details;
    mutable bool m_detailsValid;
    bool m_duplicate;
    QVector<QVariant> m_name;
    int m_signal;
    bool m_slave;
    QVector<QVariant> m_specificPath;
    //QString m_ssid;
    QDateTime m_timestamp;
    QVector<QVariant> m_icon;
    */

signals:

};

#endif // QNetworkItem_H
