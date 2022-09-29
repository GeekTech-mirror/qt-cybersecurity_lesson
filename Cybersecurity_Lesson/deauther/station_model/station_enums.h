#ifndef STATION_ENUMS_H
#define STATION_ENUMS_H

#include <QObject>

QT_BEGIN_NAMESPACE

using namespace Qt;

enum StationItemRole {
    StationRole = Qt::UserRole + 1,
    AccessPointRole,
};
Q_ENUM_NS(StationItemRole);

/* size of probed ESSID ring buffer */
#define NB_PRB 10

/* size of signal power ring buffer */
#define NB_PWR 5

/* maximum length of an Information Element */
#define MAX_IE_ELEMENT_SIZE 256

/* maximum value for modulation and coding set index */
#define MAX_AC_MCS_INDEX 8

enum channel_width_enum
{
    CHANNEL_UNKNOWN_WIDTH,
    CHANNEL_3MHZ,
    CHANNEL_5MHZ,
    CHANNEL_10MHZ,
    CHANNEL_20MHZ,
    CHANNEL_22MHZ,
    CHANNEL_30MHZ,
    CHANNEL_20_OR_40MHZ,
    CHANNEL_40MHZ,
    CHANNEL_80MHZ,
    CHANNEL_80_80MHZ,
    CHANNEL_160MHZ
};
Q_ENUM_NS(channel_width_enum);

/* 802.11ac channel information */
struct ac_channel_info
{
    QString center_sgmt[2];
    /* 802.11ac Center segment 0*/
    QChar mu_mimo; /* MU-MIMO support          */
    QChar short_gi_80; /* Short GI for 80MHz       */
    QChar short_gi_160; /* Short GI for 160MHz      */
    QChar split_chan; /* 80+80MHz Channel support */
    QChar mhz_160_chan; /* 160 MHz channel support  */
    QChar wave_2; /* Wave 2                   */
    QString mcs_index[MAX_AC_MCS_INDEX];
    /* Maximum TX rate          */
};

/* 802.11n channel information */
struct n_channel_info
{
    char mcs_index; /* Maximum MCS TX index     */
    char sec_channel; /* 802.11n secondary channel*/
    unsigned char short_gi_20; /* Short GI for 20MHz       */
    unsigned char short_gi_40; /* Short GI for 40MHz       */
    unsigned char any_chan_width; /* Support for 20 or 40MHz
                                    as opposed to only 20 or
                                    only 40MHz               */
};

/* linked list of received packets for the last few seconds */
struct pkt_buf
{
    unsigned char * packet; /* packet */
    unsigned short length; /* packet length */
    struct timeval ctime; /* capture time */
};

/* oui struct for list management */
struct oui
{
    QString id[9]; /* TODO: Don't use ASCII chars to compare, use unsigned char[3]
                   (later) with the value (hex ascii will have to be converted)
                   */
    QString manuf[128]; /* TODO: Switch to a char * later to improve memory usage */
};

QT_END_NAMESPACE

#endif // STATION_ENUMS_H
