#ifndef AIRODUMP_H
#define AIRODUMP_H

#include <string.h>

// macros from ieee80211 header
#define	IEEE80211_FC1_DIR_MASK			0x03
#define	IEEE80211_FC1_DIR_NODS			0x00	/* STA->STA */
#define	IEEE80211_FC1_DIR_TODS			0x01	/* STA->AP  */
#define	IEEE80211_FC1_DIR_FROMDS		0x02	/* AP ->STA */
#define	IEEE80211_FC1_DIR_DSTODS		0x03	/* AP ->AP  */

#define	IEEE80211_FC0_SUBTYPE_PROBE_REQ		0x40 // for TYPE_MGT

// macros from aircrack-ng (crypto_engine.h)
#define ESSID_LENGTH 32 /* The spec. says 32 maximum. */

// macros from aircrack-ng (common.h)
#ifndef MIN
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif

//static int dump_add_packet(unsigned char * h80211,
//                           int caplen,
//                           struct rx_info * ri,
//                           int cardnum)
//{
//    size_t i;
//    size_t n;

//    unsigned char *p;
//    unsigned char bssid[6];
//    unsigned char stmac[6];


//    switch (h80211[1] & IEEE80211_FC1_DIR_MASK)
//    {
//        case IEEE80211_FC1_DIR_NODS:

//            /* if management, check that SA != BSSID */

//            if (memcmp(h80211 + 10, bssid, 6) == 0) goto skip_station;

//            memcpy(stmac, h80211 + 10, 6);
//            break;

//        case IEEE80211_FC1_DIR_TODS:

//            /* ToDS packet, must come from a client */

//            memcpy(stmac, h80211 + 10, 6);
//            break;

//        case IEEE80211_FC1_DIR_FROMDS:

//            /* FromDS packet, reject broadcast MACs */

//            if ((h80211[4] % 2) != 0) goto skip_station;
//            memcpy(stmac, h80211 + 4, 6);
//            break;

//        case IEEE80211_FC1_DIR_DSTODS:
//            goto skip_station;

//        default:
//            abort();
//    }

//skip_station:

//    /* packet parsing: Probe Request */

//    if (h80211[0] == IEEE80211_FC0_SUBTYPE_PROBE_REQ && st_cur != NULL)
//    {
//        p = h80211 + 24;

//        while (p < h80211 + caplen)
//        {
//            if (p + 2 + p[1] > h80211 + caplen) break;

//            if (p[0] == 0x00 && p[1] > 0 && p[2] != '\0'
//                && (p[1] > 1 || p[2] != ' '))
//            {
//                n = MIN(ESSID_LENGTH, p[1]);

//                for (i = 0; i < n; i++)
//                    if (p[2 + i] > 0 && p[2 + i] < ' ') goto skip_probe;

//                /* got a valid ASCII probed ESSID, check if it's
//                   already in the ring buffer */

//                for (i = 0; i < NB_PRB; i++)
//                    if (memcmp(st_cur->probes[i], p + 2, n) == 0)
//                        goto skip_probe;

//                st_cur->probe_index = (st_cur->probe_index + 1) % NB_PRB;
//                memset(st_cur->probes[st_cur->probe_index], 0, 256);
//                memcpy(
//                    st_cur->probes[st_cur->probe_index], p + 2, n); // twice?!
//                st_cur->ssid_length[st_cur->probe_index] = (int) n;

//                if (verifyssid((const unsigned char *)
//                                   st_cur->probes[st_cur->probe_index])
//                    == 0)
//                    for (i = 0; i < n; i++)
//                    {
//                        c = p[2 + i];
//                        if (c < 32) c = '.';
//                        st_cur->probes[st_cur->probe_index][i] = c;
//                    }
//            }

//            p += 2 + p[1];
//        }
//    }

//}

#endif // AIRODUMP_H
