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


StationModel::StationModel (const QVector<StationItemRole> &roles, QObject *parent)
    : QAbstractItemModel (parent),
      m_pcapThread (new QThread(this)),
      m_apInfo (QVector<ap_info*>())
{
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

    // Set WiFi info to display
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


/* Return index to model */
QModelIndex StationModel::index (int row, int column,
                                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    StationItem *parentItem = d_ptr->getItem (parent, this);

    if (!parentItem)
        return QModelIndex();

    StationItem *childItem = parentItem->child (row);
    if (childItem)
        return createIndex (row, column, childItem);

    return QModelIndex();
}


/* Return index to parent in model */
QModelIndex StationModel::parent (const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    StationItem *childItem = static_cast<StationItem*>(index.internalPointer());
    StationItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


/* Return total number of rows */
int StationModel::rowCount (const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const StationItem *parentItem = d_ptr->getItem (parent, this);

    return parentItem ? parentItem->childCount() : 0;
}


/* Insert number of rows below specified position */
bool StationModel::insertRows (int position, int rows,
                                const QModelIndex &parent)
{
    StationItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginInsertRows (parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows ();

    return success;
}

/* Remove number of rows below specified position */
bool StationModel::removeRows (int position, int rows,
                                const QModelIndex &parent)
{
    StationItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginRemoveRows (parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren (position, rows);
    endRemoveRows ();

    return success;
}


/* Return total number of columns */
int StationModel::columnCount (const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}


/* Returns list of item flags present on index (See Qt::ItemFlags) */
Qt::ItemFlags StationModel::flags (const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
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
    roles[StationItemRole::InterfaceRole] = "Interface";
    roles[StationItemRole::StationRole] = "Station";
    roles[StationItemRole::StationHeaderRole] = "Header";

    return roles;
}


void StationModel::setIfaceHandle (pcap_t *handle)
{
    if (handle == NULL)
    {
        qWarning() << "setIfaceHandle failed: handle does not exist";
        return;
    }

    if (m_ifaceHandle != handle)
    {
        m_ifaceHandle = handle;
    }

    struct pcap_pkthdr *packet_header;
    const u_char *packet;

    this->create_pcapThread (m_ifaceHandle);
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
        //ChannelFrequency chan_type;

        const u_char *pk_data;
        struct pcap_pkthdr *packet_header;


        int n = 0;

        while (true)
        {
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

        }
    });
    m_pcapThread->start();
}


void StationModel::addStation(QByteArray &stmac, ap_probe &apProbe)
{
    bool apFound = false;
    ap_info *apInfo;

    /* Skip Station */
    if (stmac.isNull())
    {
        qWarning() << "Warning: tried to add station with null mac address";
        qWarning() << "Skipping Station" << Qt::endl;
        return;
    }

    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        if (rootItem->child(i)->stmac() == stmac)
        {
            qInfo() << "station:" << stmac.toHex(':') << "is already in model";
            qInfo() << "Skipping Station" << Qt::endl;
            return;
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
        return;
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
    qDebug() << "added station";

    endInsertRows ();   // END ADDING NETWORK

    return;
}



bool StationModel::addAccessPoint (ap_probe &ap_cur)
{
    QDebug debug = qDebug();

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
        debug << "Warning: Channel type not found";
        debug << "Skipping Packet" << Qt::endl << Qt::endl;
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

                return true;
            }

            continue;
        }

        // update channel
        if (ap->channel[i] != ap_cur.channel)
        {
            debug << ap->essid << Qt::endl;
            debug << "channel changed:" <<
                     ap->channel[i] << "->" << ap_cur.channel;
            debug << Qt::endl << Qt::endl;

            ap->channel[i] = ap_cur.channel;
        }

        return true;
    }

    // add new access point
    ap_info *apInfo = new ap_info();

    apInfo->essid = ap_cur.essid;
    apInfo->bssid[i] = ap_cur.bssid;
    apInfo->channel[i] = ap_cur.channel;

    m_apInfo.append(apInfo);

    //if (!m_apInfo.contains(bssid))
    //{
    //    m_accessPoint.append(bssid);

        //find manufacturer
    //}
    return true;
}


void StationModel::filterPacket (const QByteArray &packet, int caplen)
{
    QByteArray pk = packet;
    QByteArray stmac;
    ap_probe ap;

    int channel = -1;
    ChannelFrequency chan_type;

    // broadcast address
    QByteArray wildcard;
    wildcard.fill(0xFF, 6);

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

    // skip control frame
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


    /* find access point - bssid */
    switch (pk.at(1) & IEEE80211_FC1_DIR_MASK)
    {
        case IEEE80211_FC1_DIR_NODS:
            ap.bssid = pk.sliced(16, 6);
            break; // Adhoc

        case IEEE80211_FC1_DIR_TODS:
            ap.bssid = pk.sliced(4, 6);
            break; // ToDS

        case IEEE80211_FC1_DIR_FROMDS:
        case IEEE80211_FC1_DIR_DSTODS:
            ap.bssid = pk.sliced(10, 6);
            break; // WDS -> Transmitter taken as BSSID

        default:
            qDebug() << "returned";
            return;
    }

    /* find station - mac address */
    switch (pk.at(1) & IEEE80211_FC1_DIR_MASK)
    {
        case IEEE80211_FC1_DIR_NODS:

            // if management, check that SA != BSSID
            if (pk.sliced(10,6) == ap.bssid)
            {
                // skip station
                break;
            }

            stmac = pk.sliced(10, 6);
            break;

        case IEEE80211_FC1_DIR_TODS:

            // ToDS packet, must come from a client
            stmac = pk.sliced(10, 6);
            break;

        case IEEE80211_FC1_DIR_FROMDS:

            // FromDS packet, reject broadcast MACs
            if ((pk.at(4) % 2) != 0)
            {
                // skip station
                break;
            }

            stmac = pk.sliced(4, 6);
            break;

        case IEEE80211_FC1_DIR_DSTODS:
            break;

        default:
            qDebug() << "returned";
            return;
    }


    /* Probe Request: ST -> AP */
    if (pk.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_REQ
        && !stmac.isNull())
    {
        if (ap.bssid == wildcard)
        {
            ap.bssid.clear();
        }

        if(!probe_request(pk, ap))
        {
            return;
        }

        //m_mutex.lock();
        addStation(stmac, ap);
    }

    if (((uchar)pk.at(0) == IEEE80211_FC0_SUBTYPE_BEACON
         || pk.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_RESP)
        && !ap.bssid.isNull())
    {
        if (!probe_response(pk, ap))
        {
            return;
        }

        if (!addAccessPoint(ap))
        {
            return;
        }

    }


//////////////////////// DEBUGGING ////////////////////////

    // print captured packet to console
    QDebug info = qInfo();
    info << "capture length" << pk.size() << caplen << Qt::endl;
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
    info << stmac.toHex(':') << Qt::endl;
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


/* Request from device to connect to ap
** Parameters:
**     packet:
** Return:
** Notes:
**     index 0: indicates the tag type
**     index 1: inicates the size of the tag
*/
bool StationModel::probe_request(const QByteArray &pk, ap_probe &ap)
{
    // start parsing at Wireless Management Header
    QByteArray tags = pk.sliced(24);

    int tag_len;

    // timout if process takes too long
    QTimer *timeout = new QTimer();
    timeout->setSingleShot(true);
    timeout->start(2000);

    // return if subtype is not a probe request
    if (pk.at(0) != IEEE80211_FC0_SUBTYPE_PROBE_REQ)
    {
        return true;
    }

    // Probe tagged parameters
    while (timeout->isActive())
    {
        tag_len = BYTE_TO_UCHAR(tags, 1);


        switch (find_ssid(tags, ap))
        {
            case TagSearch::Error:
                return false;

            case TagSearch::TagFound:
                return true;

            case TagSearch::TagNotFound:
                break;
        }


        // avoid going out of bounds
        if ((2+tag_len) > tags.size() )
        {
            qCritical() << "Error: tag parsing attempted to move out of bounds";
            qCritical() << "Skipping Packet" << Qt::endl;

            return false;
        }

        // move to the next tag
        tags = tags.sliced (2+tag_len);
    }

    qWarning() << "Warning: probe_request() timed out";
    qWarning() << "Skipping Packet" << Qt::endl;
    return false;
}


/* Access Point's response to device probe request
** Parameters:
**     packet:
** Return:
** Notes:
**     index 0: indicates the tag type
**     index 1: inicates the size of the tag
**     index 2+: contains the tag data
*/
bool StationModel::probe_response(const QByteArray &pk, ap_probe &ap)
{
    // type cast to uchar (0x80 or b1000 0000 results in a negative with signed char)
    if ((uchar)pk.at(0) != IEEE80211_FC0_SUBTYPE_BEACON
        && pk.at(0) != IEEE80211_FC0_SUBTYPE_PROBE_RESP)
    {
        return true;
    }

    // store preamble

    // store timestamp

    QByteArray tags = pk.sliced(36);
    QVector<bool> tags_found;
    tags_found.fill (false, 2);

    int tag_id;
    int tag_len;

    // timout if process takes too long
    QTimer *timeout = new QTimer();
    timeout->setSingleShot(true);
    timeout->start(2000);

    // Probe tagged parameters
    while (timeout->isActive())
    {
        tag_id = BYTE_TO_UCHAR(tags, 0);
        tag_len = BYTE_TO_UCHAR(tags, 1);

        /* find essid */
        switch (find_ssid(tags, ap))
        {
            case TagSearch::Error:
                return false;

            case TagSearch::TagFound:
                tags_found[0] = true;

            case TagSearch::TagNotFound:
                break;

        }

        /* find channel */
        if (tag_id == TAG_PARAM_AP_CHANNEL)
        {
            ap.channel = BYTE_TO_UCHAR(tags, 2);
            tags_found[1] = true;

            // Find HT Capabilities n, ac, etc
        }


        /* found all tags */
        if (tags_found.count(true) == 2)
        {
            return true;
        }


        // avoid going out of bounds
        if ((2+tag_len) > tags.size() )
        {
            qCritical() << "Error: tag parsing attempted to move out of bounds";
            qCritical() << "Skipping Packet" << Qt::endl;

            return false;
        }

        // move to the next tag
        tags = tags.sliced (2+tag_len);
    }

    qWarning() << "Warning: probe_request() timed out";
    qWarning() << "Skipping Packet" << Qt::endl;
    return false;
}


TagSearch StationModel::find_ssid(const QByteArray &tags, ap_probe &ap)
{
    int tag_id;             // index 0
    int tag_len;            // index 1
    QByteArray tag_data;    // index 2+

    tag_id = BYTE_TO_UCHAR(tags, 0);
    tag_len = BYTE_TO_UCHAR(tags, 1);


    if (tag_len == 0)
    {
        //qDebug() << "Info: packet contains a wildcard ssid";
        //qDebug() << "Skipping Packet" << Qt::endl;

        return TagSearch::Error;
    }
    else if (tag_len == 1 && tags.at(2) == ' ')
    {
        qInfo() << "Info: packet contains a empty ssid";
        qInfo() << "Skipping Packet" << Qt::endl;

        return TagSearch::Error;
    }
    else if (tags.at(2) == '\0')
    {
    //    qDebug() << "Info: packet contains a null ssid";
    //    qDebug() << "Skipping Packet" << Qt::endl;

        return TagSearch::Error;
    }


    /* find access point - essid */
    if (tag_id == TAG_PARAM_SSID)
    {
        tag_data = tags.sliced (2, tag_len);

        for (int i=0; i < tag_len; ++i)
        {
            // avoid ascii special characters
            if (tag_data.at(i) < 0x20)
            {
                tag_data[i] = '.';
            }
        }

        if (tag_data.isValidUtf8())
        {
            ap.essid = tag_data;

            return TagSearch::TagFound;
        }
        else
        {
            qWarning() << "Warning: essid" << tag_data
                     << "is not a valid Utf8 string";
            qWarning() << "Skipping Packet";

            return TagSearch::Error;
        }


    }

    return TagSearch::TagNotFound;
}

