#ifndef STATIONITEM_H
#define STATIONITEM_H

#include "station_common.h"

class StationItem : public QObject
{
    Q_OBJECT
public:
    explicit StationItem (StationItem *parent = nullptr);
    explicit StationItem (const QVector<QString> &data,
                          StationItem *parent = nullptr);

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
    void insertRole (const StationItemRole roles)
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

    QByteArray apEssid () const;
    QByteArray apBssid (ChannelFrequency chan_type) const;
    int apChannel (ChannelFrequency chan_type) const;

    ap_info apInfo () const;
    void setApInfo (const ap_info &apInfo);

    QByteArray stmac () const;
    void setStmac (const QByteArray &stmac);

signals:

private:
    StationItem *m_parentItem;
    QVector<StationItem*> m_childItems;
    QVector<QString> m_headerData;
    QString indent;

    /* Station Items */
    QByteArray m_stmac;                                     /* the client's MAC address  */
    ap_info *m_apInfo;
    QString m_manuf;                                        /* the client's manufacturer */

    /* Security Properties */
    int m_wpatype;                                          /* 1=wpa1 2=wpa2             */
    int m_wpahash;                                          /* 1=md5(tkip) 2=sha1(ccmp)  */
    int m_wep;

    QVector<int> m_roles;
    QVector<int> m_changedRoles;
};

#endif // STATIONITEM_H
