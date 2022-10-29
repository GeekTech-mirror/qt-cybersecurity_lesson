#ifndef STATION_ENUMS_H
#define STATION_ENUMS_H

#include <QObject>
#include <QByteArray>

enum StationItemRole {
    AccessPointRole = Qt::UserRole + 1,
    Bssid2Role,
    Bssid5Role,
    Channel2Role,
    Channel5Role,
    InterfaceRole,
    StationRole,
    StationHeaderRole
};

enum ChannelFrequency
{
    Chan_2GHz,
    Chan_5GHz
};

struct ap_info
{
    QByteArray essid;
    QByteArray bssid[2];
    int channel[2];
};


struct ap_probe
{
    QByteArray essid;
    QByteArray bssid;
    int channel;

    ChannelFrequency chan_type;
};

/* oui struct for list management */
struct oui
{
    QString id[9]; /* TODO: Don't use ASCII chars to compare, use unsigned char[3]
                   (later) with the value (hex ascii will have to be converted)
                   */
    QString manuf[128]; /* TODO: Switch to a char * later to improve memory usage */
};

#endif // STATION_ENUMS_H
