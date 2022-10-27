#ifndef PCAP_ENUM_H
#define PCAP_ENUM_H

/* IEEE 802.11 wireless LAN protocol filters
 *
 * definitions
 *
 * FC: Frame Control Byte
 * DIR: Direction
 * TO/FROM DS: To/From Distribution System
 */
#define IEEE80211_FRAME_LEN 24

/* Traffic direction
 *
 * DS bit usage
 *
 * TA = transmitter address
 * RA = receiver address
 * DA = destination address
 * SA = source address
 *
 * ToDS    FromDS  A1(RA)  A2(TA)  A3      A4      Use
 * -----------------------------------------------------------------
 *  0       0       DA      SA      BSSID   -       IBSS/DLS
 *  0       1       DA      BSSID   SA      -       AP -> STA
 *  1       0       BSSID   SA      DA      -       AP <- STA
 *  1       1       RA      TA      DA      SA      unspecified (WDS)
 */
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
#define	IEEE80211_FC0_SUBTYPE_PROBE_REQ		0x40    /* STA->AP  */
#define	IEEE80211_FC0_SUBTYPE_PROBE_RESP	0x50    /* AP ->STA */
#define	IEEE80211_FC0_SUBTYPE_BEACON		0x80
#define	IEEE80211_FC0_SUBTYPE_ATIM          0x90
#define	IEEE80211_FC0_SUBTYPE_DISASSOC		0xa0
#define	IEEE80211_FC0_SUBTYPE_AUTH          0xb0
#define	IEEE80211_FC0_SUBTYPE_DEAUTH		0xc0


/* Radiotap properties */
#define RADIOTAP_HDR_LEN           2

#define RADIOTAP_CHAN_FLAGS        28
#define RADIOTAP_CHAN_2GHZ         0x0080
#define RADIOTAP_CHAN_5GHZ         0x0100


/* Tag properties */
#define TAG_PARAM_SSID              0x00
#define TAG_PARAM_AP_CHANNEL        0x03

/* Data properties */
#define ESSID_LEN 32 /* The spec. says 32 maximum. */

// returns the raw uchar* from a QByteArray at the position pos
#define BYTE_TO_UCHAR(arr, pos) \
    *reinterpret_cast<uchar*>(arr.sliced(pos,2).data())

#define BYTES_TO_UCHAR(arr, pos, size) \
    *reinterpret_cast<uchar*>(arr.sliced(pos,size).data())

#define BYTE_P_TO_UCHAR(arr, pos) \
    *reinterpret_cast<uchar*>(arr->sliced(pos,1).data())

enum TagSearch {
    Error,
    TagNotFound,
    TagFound
};

#endif // PCAP_ENUM_H
