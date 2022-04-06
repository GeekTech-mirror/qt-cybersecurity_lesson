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
    explicit QNetworkItem(const QVector<QVariant> &data, QNetworkItem *parent = nullptr);
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
    QVariant devicePath() const;
    void setDevicePath(const QString &path);

    QVariant specificPath() const;
    void setSpecificPath(const QVariant &path);

    QVariant ssid() const;
    void setSsid(const QVariant &ssid);

    QVariant uuid() const;
    void setUuid(const QVariant &uuid);

    QVariant uni() const;

private:
    /* Tree Items */
    QNetworkItem *m_parentItem;
    QVector<QNetworkItem*> m_childItems;
    QVector<QVariant> m_itemData;

    /* Network Items */
    QVariant m_devicePath;
    QVariant m_specificPath;
    QVariant m_ssid;
    QVariant m_uuid;

    QVector<int> m_changedRoles;
};

#endif // QNetworkItem_H
