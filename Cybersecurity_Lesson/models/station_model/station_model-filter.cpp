#include <QDir>
#include <QThread>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>

#include "station_model.h"
#include "station_model_p.h"
#include "station_item.h"
#include "pcap_common.h"

#include <pcap/pcap.h>

#include <netinet/in.h>
#include <net/ethernet.h>
#include "radiotap.h"
#include "radiotap_iter.h"

#include "debug_packets.h"


/* Station Model
** --------
** Implement functions to add stations to the model
** --------
**/

bool StationModel::setIfaceHandle (pcap_t *handle)
{
    if (handle == NULL)
    {
        qWarning() << "Warning: failed to set iface handle";
        qWarning() << "Handle does not exist" << Qt::endl;
        return false;
    }

    if (m_ifaceHandle != handle)
    {
        m_ifaceHandle = handle;
    }

    return true;
}


void StationModel::start_pcapThread (pcap_t *handle)
{
    if (!setIfaceHandle(handle))
    {
        return;
    }

    create_pcapThread (m_ifaceHandle);

    m_pcapThread->start(QThread::HighPriority);
}

void StationModel::stop_pcapThread ()
{
    int result = -1;
    int timeout = 10000;

    if (!m_pcapThread->isRunning())
    {
        return;
    }

    // tell pcap loop to stop
    m_pcapThread->requestInterruption();

    m_pcapThread->exit(result);
    if (!result)
    {
        qCritical() << "Error: thread did not exit safely" << Qt::endl;
        qCritical() << "Terminating pcap thread" << Qt::endl;
        m_pcapThread->terminate();
    }

    // wait for thread process to end
    m_pcapThread->wait(timeout);

    if(!m_pcapThread->isFinished())
    {
        qCritical() << "Error: thread did not exit safely";
        qCritical() << "Terminating pcap thread" << Qt::endl;
        m_pcapThread->terminate();
    }
}


void StationModel::create_pcapThread (pcap_t *handle)
{
    int n=0;
    int status;

    if (m_pcapThread->isRunning())
    {
        m_pcapThread->quit();
    }

    m_pcapThread = QThread::create([handle, this]
    {
        QByteArray *packet;

        const u_char *pk_data;
        struct pcap_pkthdr *packet_header;

        // test packets
        addAccessPoint(tp_link_2GHz);
        addAccessPoint(tp_link_5GHz);
        addStation(samsung_stmac, tp_link_2GHz);

        addAccessPoint(Netgear);
        addStation(precision_stmac, Netgear);

        while (!QThread::currentThread()->isInterruptionRequested())
        {
            QElapsedTimer timer;
            timer.start();

            if (handle == NULL)
            {
                continue;
            }

            /* capture the next packet */
            switch (pcap_next_ex (handle, &packet_header, &pk_data))
            {
            case PCAP_ERROR:
                qCritical() << "Error: Failed while reading packet";
                qCritical() << pcap_geterr (handle);
                break;

            case PCAP_ERROR_BREAK:
                qWarning() << "Warning: No more packets to read";
                break;

            default:

                /* convert unsigned char* array to QByteArray */
                packet = new QByteArray (reinterpret_cast<const char*>(pk_data),
                                         packet_header->caplen);

                Q_EMIT packetCaptured (*packet, packet_header->caplen);
            }

            if (timer.elapsed() > 2000)
            {
                qDebug() << "loop time" << timer.elapsed() << "milliseconds"
                         << "caplen:" << packet_header->caplen << Qt::endl;
            }
        }
    });
}


void StationModel::filterPacket (const QByteArray &packet, int caplen)
{
    QByteArray pk = packet;
    QByteArray stmac;
    ap_probe ap;

    int channel = -1;
    ChannelFrequency chan_type;

    // broadcast address
    QByteArray wildcard = QByteArray(6, 0xFF);

    // llc null packet
    const QByteArray llcnull = QByteArray(4, 0);


    if (!filterRadiotapHdr(pk, ap))
    {
        return;
    }


    /* Skip Packets */

    // skip packets smaller than IEEE 802.11 Frames
    if (pk.size() < IEEE80211_FRAME_LEN)
    {
        // go to write packet?
        return;
    }

    // skip uninteresting control frame
    if ((pk.at(0) & IEEE80211_FC0_TYPE_MASK)
        == IEEE80211_FC0_TYPE_CTL)
    {
        // go to write packet?
        return;
    }

    // skip LLC null packets
    if (((pk.at(0) & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_DATA)
        && (pk.size() > 28))
    {
        if (pk.sliced(24,4) == llcnull)
            return;
    }


    /* find access point mac address */
    if (!d_ptr->find_bssid (pk, ap))
    {
        qWarning() << "Warning: Could not find matching IEEE 802.11 control byte";
        qWarning() << "Skipping Packet" << Qt::endl;

        return;
    }

    /* find station mac address */
    if (!d_ptr->find_stmac (pk, ap, stmac))
    {
        qWarning() << "Warning: Could not find matching IEEE 802.11 control byte";
        qWarning() << "Skipping Packet" << Qt::endl;

        return;
    }


    /* filter station request */
    if (pk.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_REQ
        || pk.at(0) == IEEE80211_FC0_SUBTYPE_ASSOC_REQ)
    {
        if (ap.bssid == wildcard)
        {
            ap.bssid.clear();
        }

        if (stmac.isNull())
        {
            return;
        }

        if(!d_ptr->station_request(pk, ap))
        {
            return;
        }

        //m_mutex.lock();
        if (!addStation(stmac, ap))
        {
            return;
        }
    }


    /* filter ap Response*/
    if (((uchar)pk.at(0) == IEEE80211_FC0_SUBTYPE_BEACON
         || pk.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_RESP))
    {
        if (ap.bssid.isNull())
        {
            return;
        }

        if (!d_ptr->ap_response(pk, ap))
        {
            return;
        }

        if (!addAccessPoint(ap))
        {
            return;
        }

    }


//////////////////////// DEBUGGING ////////////////////////
    // only print specified packet types
    if (
        pk.at(0) != IEEE80211_FC0_SUBTYPE_PROBE_REQ
        && pk.at(0) != IEEE80211_FC0_SUBTYPE_ASSOC_REQ
//        && (uchar)pk.at(0) != IEEE80211_FC0_SUBTYPE_BEACON
        )
    {
        return;
    }


    // print captured packet to console
    QDebug info = qInfo();
    info << "capture length" << caplen << Qt::endl;
    d_ptr->print_packet(packet, info);

    info << Qt::endl << Qt::endl;

    // print access point
    info << "access point" << Qt::endl;
    info << "bssid:" << ap.bssid.toHex(':') << Qt::endl;
    info << "essid:" << ap.essid << Qt::endl;
    info << "channel:" << ap.channel << Qt::endl;
    info << "channel frequency:" <<
            ((ap.chan_type == Chan_2GHz) ? "2GHZ" :
             (ap.chan_type == Chan_5GHz) ? "5GHZ" : "?") << Qt::endl;
    info << Qt::endl;


    // print station
    info << "station" << Qt::endl;
    info << "stmac:" << stmac.toHex(':') << Qt::endl;
}


bool StationModel::filterRadiotapHdr (QByteArray &pk, ap_probe &ap)
{
    struct ieee80211_radiotap_iterator iterator;
    struct ieee80211_radiotap_header * radiotap_h;
    const uchar* pk_data = reinterpret_cast<const uchar*>(pk.data());
    radiotap_h = (struct ieee80211_radiotap_header *) pk_data;
    int ret = ieee80211_radiotap_iterator_init(&iterator, radiotap_h, pk.size(), NULL);

    while (!ret) {
        ret = ieee80211_radiotap_iterator_next(&iterator);

        if (ret)
            continue;

        switch (iterator.this_arg_index) {
        case IEEE80211_CHAN_2GHZ:
            ap.chan_type = Chan_2GHz;
            break;

        case IEEE80211_CHAN_5GHZ:
            ap.chan_type = Chan_5GHz;
            break;

        default:
            break;
        }
    }  /* while more rt headers */

    if (ret == -EINVAL)
    {
        qInfo() << "Error: Malformed Radiotap Header";
        qInfo() << "Skipping Packet" << Qt::endl;

        return false;
    }


    // find radiotap header size
    int n = iterator._max_length;

    if (pk.size() < n)
    {
        qCritical() << "Error: Malformed Radiotap Header";
        qCritical() << "Skipping Packet" << Qt::endl;

        return false;
    }

    // skip radiotap header
    pk = pk.sliced(n);
    return true;
}


bool StationModel::addStation(QByteArray &stmac, ap_probe &apProbe)
{
    bool apFound = false;
    ap_info *apInfo;

    /* Skip Station */
    if (stmac.isNull())
    {
        qWarning() << "Warning: tried to add station with null mac address";
        qWarning() << "Skipping Station" << Qt::endl;
        return false;
    }

    if (apProbe.essid.isNull())
    {
        return false;
    }


    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        if (rootItem->child(i)->stmac() == stmac)
        {
            qInfo() << "Info: station" << stmac.toHex(':') << "is already in model";
            qInfo() << "Skipping Station" << Qt::endl;

            updateStations();

            return false;
        }
    }

    for (ap_info *ap : m_apInfo)
    {
        if (ap->essid == apProbe.essid)
        {
            apFound = true;
            apInfo = ap;
        }
    }

    if (!apFound)
    {
        apInfo = new ap_info();

        apInfo->essid = apProbe.essid;
    }


    /* Insert new row into model */
    const int index = rootItem->childCount();
    beginInsertRows (QModelIndex(), index, index);  // BEGIN ADDING NETWORK

    rootItem->insertChildren (rootItem->childCount(), 1, rootItem->columnCount());
    StationItem *item = rootItem->child (rootItem->childCount() - 1);

    item->setStmac(stmac);
    item->setApInfo(*apInfo);

    // Set column role
    int row = rootItem->childCount()-1;
    for (int i=0; i < columnRoles.count(); ++i)
    {
        setData (this->index(row,i), columnRoles.at(i));
    }

    endInsertRows ();   // END ADDING NETWORK

    qInfo() << "Info: station" << stmac.toHex(':') << "added" << Qt::endl;

    return true;;
}


bool StationModel::addAccessPoint (ap_probe &ap_cur)
{
    QDebug info = qInfo();

    if (ap_cur.essid.isEmpty())
    {
        return false;
    }

    //find manufacturer

    // identify channel type
    int ch_type;
    if (ap_cur.chan_type == Chan_2GHz)
    {
        ch_type = 0;
    }
    else if (ap_cur.chan_type == Chan_5GHz)
    {
        ch_type = 1;
    }
    else
    {
        //qWarning() << "Warning: Channel type not found";
        //qWarning() << "Skipping Packet" << Qt::endl << Qt::endl;
        return false;
    }

    for (ap_info *ap : m_apInfo)
    {
        // look for new bssids
        if (ap->bssid[ch_type] != ap_cur.bssid)
        {
            // same network with different bssid
            if (ap->essid == ap_cur.essid)
            {
                ap->bssid[ch_type] = ap_cur.bssid;
                ap->channel[ch_type] = ap_cur.channel;

                updateStations();

                return true;
            }

            continue;
        }

        // update channel
        if (ap->channel[ch_type] != ap_cur.channel)
        {
            qInfo() << "Info:" << ap->essid
                    << "channel changed:"
                    <<  ap->channel[ch_type] << "->" << ap_cur.channel;

            qInfo() << Qt::endl;

            ap->channel[ch_type] = ap_cur.channel;

            updateStations();
        }

        return true;
    }

    // add new access point
    ap_info *apInfo = new ap_info();

    apInfo->essid = ap_cur.essid;
    apInfo->bssid[ch_type] = ap_cur.bssid;
    apInfo->channel[ch_type] = ap_cur.channel;

    m_apInfo.append(apInfo);

    updateStations();

    //if (!m_apInfo.contains(bssid))
    //{
    //    m_accessPoint.append(bssid);

        //find manufacturer
    //}
    return true;
}


void StationModel::updateStations()
{
    bool updateAp = false;

    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        for (ap_info *ap : m_apInfo)
        {
            if (rootItem->child(i)->apEssid() != ap->essid )
            {
                continue;
            }

            if (rootItem->child(i)->apBssid(Chan_2GHz) != ap->bssid[0])
                updateAp = true;

            if (rootItem->child(i)->apBssid(Chan_5GHz) != ap->bssid[1])
                updateAp = true;

            if (rootItem->child(i)->apChannel(Chan_2GHz) != ap->channel[0])
                updateAp = true;

            if (rootItem->child(i)->apChannel(Chan_5GHz) != ap->channel[1])
                updateAp = true;

            if (updateAp)
            {
                QModelIndex index = this->index(i, 1);
                StationItem *item = d_ptr->getItem (index, this);

                item->setApInfo(*ap);

                Q_EMIT dataChanged (index, index, item->changedRoles());
            }
        }
    }
}
