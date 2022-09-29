#ifndef STATIONITEM_H
#define STATIONITEM_H

#include <QObject>

#include "station_enums.h"

class StationItem : public QObject
{
    Q_OBJECT
public:
    explicit StationItem(QObject *parent = nullptr);

    ~StationItem();

    /* Tree Items */
    StationItem *parent();
    StationItem *child (int row);
    int childNumber() const;
    int childCount() const;
    int columnCount() const;

    bool insertChildren (int position, int count, int columns);
    bool removeChildren (int position, int count);

    QVariant data (int column) const;
    bool setData (int column, const QVariant &value);

    QString headerData (int column) const;
    bool setHeaderData (int column, const QString &value);

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

signals:

private:
    StationItem *m_parentItem;
    QVector<StationItem*> m_childItems;
    QVector<QString> m_headerData;
    QString indent;

    QString m_ap;
    QString m_na;
    QString m_station;
    QVector<int> m_roles;
    QVector<int> m_changedRoles;
};

#endif // STATIONITEM_H
