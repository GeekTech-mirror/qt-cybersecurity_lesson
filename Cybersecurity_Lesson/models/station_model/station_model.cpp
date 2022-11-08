#include <QDir>
#include <QThread>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>

#include "station_model.h"
#include "station_model_p.h"
#include "station_item.h"
#include "pcap_common.h"

#include "pcap.h"
#include "custom_colors.h"

#include <netinet/in.h>
#include <net/ethernet.h>

#include "debug_packets.h"


StationModel::StationModel (const QVector<StationItemRole> &roles, QObject *parent)
    : QAbstractItemModel (parent),
      m_pcapThread (new QThread(this)),
      m_apInfo (QVector<ap_info*>()),
      m_ouiFile (QDir::currentPath()+"/oui.txt")
{
    qDebug() << QDir::current();
    qDebug() << QDir::currentPath();
    qDebug() << m_ouiFile.open (QIODevice::ReadOnly | QIODevice::Text);

    // Create header titles based on column roles
    QVector<QString> rootData;
    for (int i = 0; i < roles.count(); ++i)
    {
        switch (roles.at(i)) {
        case StationItemRole::StationRole:
            rootData << "Stations";
            break;
        case StationItemRole::AccessPointRole:
            rootData << "Access Points";
            break;
        case StationItemRole::Bssid2Role:
            rootData << "BSSID 2GHz";
            break;
        case StationItemRole::Bssid5Role:
            rootData << "BSSID 5GHz";
            break;
        case StationItemRole::Channel2Role:
            rootData << "Ch 2GHz";
            break;
        case StationItemRole::Channel5Role:
            rootData << "Ch 5GHz";
            break;
        default:
            break;
        }
    }
    rootItem = new StationItem (rootData);
    columnRoles = roles;


    connect (this, &StationModel::packetCaptured,
             this, &StationModel::filterPacket);
}

/* Constructor for private functions */
StationModel::StationModel (StationModelPrivate &dd)
    : d_ptr (&dd)
{
}

/* Destructor */
StationModel::~StationModel ()  = default;


/* Tree Model
** --------
** reimplement functions from Tree Model
** --------
**/

/* Return data from model
** Parameters:
**     index: specify model index for row and column
**     role: controls the display method for the model contents
** Return: value stored in the model index
*/
QVariant StationModel::data (const QModelIndex &index, int role) const
{
    // Checks for valid index
    if (!index.isValid())
        return QVariant();

    //qDebug() << role;

    StationItem *item = static_cast<StationItem*>(index.internalPointer());

    switch (role) {
//    case Qt::BackgroundRole:
//        if (0 == index.row() % 2)
//            return CustomColors::frame_color();
//        else
//            return CustomColors::frame_color().lighter(145);

//        break;
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
        break;
    case Qt::DecorationRole:
        // Display Symbol in first column
        if (index.column() == 0)
        {
            return QIcon ();
        }
        break;
    case Qt::DisplayRole:
        return item->data (index.column());
        break;
    case Qt::EditRole:
        return item->data (index.column());
        break;
    }

    return QVariant();
}


/* Store value in model index */
bool StationModel::setData (const QModelIndex &index,
                            const QVariant &station_role,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    StationItem *item = d_ptr->getItem (index, this);

    // Set pcap info to display
    bool result = d_ptr->setItemRole (station_role, index, this);
    if (result)
        Q_EMIT dataChanged (index, index, item->changedRoles());
        item->clearChangedRoles();

    return result;
}


/* Return header data from model
** Parameters:
**     index: specify model index for row and column
**     orientation: control location of header (vertical or horizontal)
**     role: controls the display method for the model contents
** Return: value stored in the model index
*/
QVariant StationModel::headerData (int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    switch (role) {
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
    case Qt::DisplayRole:
        return rootItem->data (section);
        break;
    }
    return QVariant();
}

/* Store header data in model index */
bool StationModel::setHeaderData (int section,
                                  Qt::Orientation orientation,
                                  const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setHeaderData (section, value.toString());

    if (result)
        emit headerDataChanged (orientation, section, section);

    return result;
}




/* Station Model
** --------
** Implement functions to add stations to the model
** --------
**/


/* Add station roles to the model */
QHash<int, QByteArray> StationModel::roleNames (void) const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[StationItemRole::AccessPointRole] = "AccessPoint";
    roles[StationItemRole::Bssid2Role] = "BSSID 2GHz";
    roles[StationItemRole::Bssid5Role] = "BSSID 5GHz";
    roles[StationItemRole::Channel2Role] = "Channel 2GHz";
    roles[StationItemRole::Channel5Role] = "Channel 5GHz";
    roles[StationItemRole::InterfaceRole] = "Interface";
    roles[StationItemRole::StationRole] = "Station";
    roles[StationItemRole::StationHeaderRole] = "Header";

    return roles;
}


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
    int timeout = 3000;

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

        //addAccessPoint(Netgear);
        //addStation(precision_stmac, Netgear);

        while (!QThread::currentThread()->isInterruptionRequested())
        {
            //QElapsedTimer timer;
            //timer.start();

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

            //if (timer.elapsed() > 2000)
            //{
            //    qDebug() << "loop time" << timer.elapsed() << "milliseconds"
            //             << "caplen:" << packet_header->caplen << Qt::endl;
            //}
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
        qWarning() << "Warning: Radiotap Header Filter Failed ";
        qWarning() << "Skipping Packet" << Qt::endl;
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


    /* temporary debugging stuff */
    if (pk.at(0) != IEEE80211_FC0_SUBTYPE_PROBE_REQ
        && pk.at(0) != IEEE80211_FC0_SUBTYPE_ASSOC_REQ)
    {
        return;
    }
    //else
    //{
    //    return;
    //}


//////////////////////// DEBUGGING ////////////////////////

        // print captured packet to console
        QDebug info = qInfo();
        info << "capture length" << caplen << Qt::endl;
        info << pk << Qt::endl << Qt::endl;
        info << pk.toHex(' ') << Qt::endl << Qt::endl;


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
    if (pk.size() < RADIOTAP_CHAN_FLAGS)
    {
        return false;
    }

    QByteArray channel_flags = pk.sliced(RADIOTAP_CHAN_FLAGS, 2);

    // Radiotap header uses Little Endian
    std::reverse(channel_flags.begin(), channel_flags.end());

    // Flags are 2 bytes wide (16-bit)
    QDataStream dataStream (channel_flags);
    quint16 flags;

    dataStream >> flags;

    /* find packet frequency */
    if (flags & RADIOTAP_CHAN_2GHZ)
    {
        ap.chan_type = Chan_2GHz;
    }
    if (flags & RADIOTAP_CHAN_5GHZ)
    {
        ap.chan_type = Chan_5GHz;
    }

    // skip radiotap header
    int n = BYTE_TO_UCHAR(pk, RADIOTAP_HDR_LEN);
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
    int i;
    if (ap_cur.chan_type == Chan_2GHz)
    {
        i = 0;
    }
    else if (ap_cur.chan_type == Chan_5GHz)
    {
        i = 1;
    }
    else
    {
        qWarning() << "Warning: Channel type not found";
        qWarning() << "Skipping Packet" << Qt::endl << Qt::endl;
        return false;
    }

    for (ap_info *ap : m_apInfo)
    {
        // look for new bssids
        if (ap->bssid[i] != ap_cur.bssid)
        {
            // same network with different bssid
            if (ap->essid == ap_cur.essid)
            {
                ap->bssid[i] = ap_cur.bssid;
                ap->channel[i] = ap_cur.channel;

                updateStations();

                return true;
            }

            continue;
        }

        // update channel
        if (ap->channel[i] != ap_cur.channel)
        {
            qInfo() << "Info:" << ap->essid
                    << "channel changed:"
                    <<  ap->channel[i] << "->" << ap_cur.channel;

            qInfo() << Qt::endl;

            ap->channel[i] = ap_cur.channel;

            updateStations();
        }

        return true;
    }

    // add new access point
    ap_info *apInfo = new ap_info();

    apInfo->essid = ap_cur.essid;
    apInfo->bssid[i] = ap_cur.bssid;
    apInfo->channel[i] = ap_cur.channel;

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
