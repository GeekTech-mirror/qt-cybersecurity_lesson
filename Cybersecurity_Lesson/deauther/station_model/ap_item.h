#ifndef STATIONITEM_H
#define STATIONITEM_H

#include <QObject>

#include "station_enums.h"

class AccessPointItem : public QObject
{
    Q_OBJECT
public:
    explicit AccessPointItem(QObject *parent = nullptr);

    ~AccessPointItem();

    /* Tree Items */
    AccessPointItem *parent();
    AccessPointItem *child (int row);
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

signals:

private:
    AccessPointItem *m_parentItem;
    QVector<AccessPointItem*> m_childItems;
    QVector<QString> m_headerData;
    QString indent;

    /* Access Point Items */
    int m_channel;
    enum channel_width_enum m_channel_width;

    /* Channel Protocol */
    QString m_standard;                                     /* 802.11 standard: n or ac  */
    struct n_channel_info n_channel;                        /* 802.11n channel info      */
    struct ac_channel_info ac_channel;                      /* 802.11ac channel info     */
    unsigned int m_security;                                /* ENC_*, AUTH_*, STD_*      */

    /* packet details */
    struct pkt_buf * m_packets;                             /* list of captured packets (last few seconds) */
    unsigned long m_nb_bcn;                                 /* total number of beacons   */
    unsigned long m_nb_pkt;                                 /* total number of packets   */
    unsigned long m_nb_data;                                /* number of  data packets   */
    unsigned long m_nb_data_old;                            /* number of data packets/sec*/
    int m_nb_dataps;                                        /* number of data packets/sec*/


    QVector<int> m_roles;
    QVector<int> m_changedRoles;
};

#endif // STATIONITEM_H
