#ifndef STATION_ENUMS_H
#define STATION_ENUMS_H

#include <QObject>
#include <QByteArray>

enum StationItemRole {
    StationRole = Qt::UserRole + 1,
    InterfaceRole,
    AccessPointRole,
};

/* IEEE 802.11 wireless LAN protocol filters */
#define IEEE80211_FRAME_LEN 24

/* Traffic direction */
#define	IEEE80211_FC1_DIR_MASK			0x03
#define	IEEE80211_FC1_DIR_NODS			0x00	/* STA->STA */
#define	IEEE80211_FC1_DIR_TODS			0x01	/* STA->AP  */
#define	IEEE80211_FC1_DIR_FROMDS		0x02	/* AP ->STA */
#define	IEEE80211_FC1_DIR_DSTODS		0x03	/* AP ->AP  */

/* Probe types */
#define	IEEE80211_FC0_TYPE_MASK			0x0c
#define	IEEE80211_FC0_TYPE_SHIFT		2
#define	IEEE80211_FC0_TYPE_MGT			0x00
#define	IEEE80211_FC0_TYPE_CTL			0x04
#define	IEEE80211_FC0_TYPE_DATA			0x08

/* Probe subtypes */
#define	IEEE80211_FC0_SUBTYPE_ASSOC_REQ		0x00
#define	IEEE80211_FC0_SUBTYPE_ASSOC_RESP	0x10
#define	IEEE80211_FC0_SUBTYPE_REASSOC_REQ	0x20
#define	IEEE80211_FC0_SUBTYPE_REASSOC_RESP	0x30
#define	IEEE80211_FC0_SUBTYPE_PROBE_REQ		0x40
#define	IEEE80211_FC0_SUBTYPE_PROBE_RESP	0x50
#define	IEEE80211_FC0_SUBTYPE_BEACON		0x80
#define	IEEE80211_FC0_SUBTYPE_ATIM          0x90
#define	IEEE80211_FC0_SUBTYPE_DISASSOC		0xa0
#define	IEEE80211_FC0_SUBTYPE_AUTH          0xb0
#define	IEEE80211_FC0_SUBTYPE_DEAUTH		0xc0

/* Frame locations */
// location of radiotap header length
#define RADIOTAP_HDR_LEN        0x02

/* Tag properties */
#define TAG_PARAM_SSID          0x00

/* Data properties */
#define ESSID_LENGTH 32 /* The spec. says 32 maximum. */

// returns the raw uchar* from a QByteArray at the position pos
#define BYTE_TO_INT(arr,pos) \
    *reinterpret_cast<const uchar*>(arr.mid(pos,1).constData())


/* oui struct for list management */
struct oui
{
    QString id[9]; /* TODO: Don't use ASCII chars to compare, use unsigned char[3]
                   (later) with the value (hex ascii will have to be converted)
                   */
    QString manuf[128]; /* TODO: Switch to a char * later to improve memory usage */
};

#endif // STATION_ENUMS_H
