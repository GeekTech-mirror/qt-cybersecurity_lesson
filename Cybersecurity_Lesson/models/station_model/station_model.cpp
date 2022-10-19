#include <QThread>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>

#include "station_model.h"
#include "station_model_p.h"
#include "station_item.h"

#include "pcap.h"
#include "custom_colors.h"

#include <netinet/in.h>
#include <net/ethernet.h>

StationModel::StationModel (QObject *parent)
    : QAbstractItemModel (parent),
      m_pcapThread (new QThread(this))
{
    rootItem = new StationItem ();

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

    switch (role) {
    case Qt::BackgroundRole:
        if (0 == index.row() % 2)
            return CustomColors::frame_color();
        else
            return CustomColors::frame_color().lighter(145);

        break;
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
        return QVariant();
        break;
    case Qt::EditRole:
        return QVariant();
        break;
    }

    return QVariant();
}


/* Store value in model index */
bool StationModel::setData (const QModelIndex &index,
                            const QVariant &network_role,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    StationItem *item = d_ptr->getItem (index, this);

    // Set WiFi info to display
    bool result = d_ptr->setItemRole (network_role, index, this);
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

//    if (parentItem == rootItem)
//        return QModelIndex();

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

void StationModel::addStation ()
{

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
        struct pcap_pkthdr *packet_header;
        const u_char *pk_data;


        int n = 0;

        while (true)
        {
            if (handle == NULL)
            {
                continue;
            }

            // capture the next packet
            switch (pcap_next_ex (handle, &packet_header, &pk_data))
            {
            case PCAP_ERROR:
                qWarning() << "Error: Failed while reading packet";
                qDebug() << pcap_geterr (handle);
                break;

            case PCAP_ERROR_BREAK:
                qWarning() << "Warning: No more packets to read";
                break;

            default:

                // convert unsigned char* array to QByteArray
                packet = new QByteArray (reinterpret_cast<const char*>(pk_data),
                                         packet_header->caplen);

                // skip radiotap header
                int n = static_cast<uint>(packet->at(RADIOTAP_HDR_LEN_LOC));
                *packet = packet->sliced(n);


                // filtering packets subtype for debugging
                static int i = 0;
                if ((uchar)packet->at(0) == IEEE80211_FC0_SUBTYPE_PROBE_REQ)
                {
                    qDebug() << "capture length" << packet_header->caplen;

                    Q_EMIT packetCaptured (pk_data, *packet);

                    ++i;
                    if (i > 10)
                    {
                        return;
                    }
                }
            }

        }
    });
    m_pcapThread->start();
}


void StationModel::filterPacket (const uchar *pk_data, const QByteArray &packet)
{
    QByteArray stmac;
    QByteArray bssid;
    QByteArray essid;

    // broadcast address
    QByteArray wildcard;
    wildcard.fill(0xFF, 6);


    /* packets to skip */

    // skip packets smaller than IEEE 802.11 Frames
    if (packet.size() < IEEE80211_FRAME_LEN)
    {
        // go to write packet?
        return;
    }

    // skip control frame
    if ((packet.at(0) & IEEE80211_FC0_TYPE_MASK)
        == IEEE80211_FC0_TYPE_CTL)
    {
        // go to write packet?
        return;
    }

    // skip LLC null packets
    if (((packet.at(0) & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_DATA)
        && (packet.size() > 28))
    {
        if (packet.sliced(24,4) == llcnull)
            return;
    }


    /* find access point - bssid */
    switch (packet.at(1) & IEEE80211_FC1_DIR_MASK)
    {
        case IEEE80211_FC1_DIR_NODS:
            bssid = packet.sliced(16, 6);
            break; // Adhoc

        case IEEE80211_FC1_DIR_TODS:
            bssid = packet.sliced(4, 6);
            break; // ToDS

        case IEEE80211_FC1_DIR_FROMDS:
        case IEEE80211_FC1_DIR_DSTODS:
            bssid = packet.sliced(10, 6);
            break; // WDS -> Transmitter taken as BSSID

        default:
            qDebug() << "returned";
            return;
    }

    /* find station - mac address */
    switch (packet.at(1) & IEEE80211_FC1_DIR_MASK)
    {
        case IEEE80211_FC1_DIR_NODS:

            // if management, check that SA != BSSID
            if (packet.sliced(10,6) == bssid)
            {
                // skip station
                break;
            }

            stmac = packet.sliced(10, 6);
            break;

        case IEEE80211_FC1_DIR_TODS:

            // ToDS packet, must come from a client
            stmac = packet.sliced(10, 6);
            break;

        case IEEE80211_FC1_DIR_FROMDS:

            // FromDS packet, reject broadcast MACs
            if ((packet.at(4) % 2) != 0)
            {
                // skip station
                break;
            }

            stmac = packet.sliced(4, 6);
            break;

        case IEEE80211_FC1_DIR_DSTODS:
            break;

        default:
            qDebug() << "returned";
            return;
    }

    if (!m_accessPoint.contains(bssid))
    {
        m_accessPoint.append(bssid);

        //find manufacturer
    }


    /* Probe Request: ST -> AP */
    if (packet.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_REQ
        && !stmac.isNull())
    {
        if (bssid == wildcard)
        {
            bssid.clear();
        }

        if(!probe_request(packet, essid))
        {
            return;
        }
    }

    if (!probe_response(packet, essid))
    {
        return;
    }


//////////////////////// DEBUGGING ////////////////////////

    // print captured packet to console
    QDebug debug = qDebug();
    debug << packet << Qt::endl << Qt::endl;
    debug << packet.toHex(' ') << Qt::endl << Qt::endl;


    // print access point
    debug << "access point" << Qt::endl;
    debug << "bssid:" << bssid.toHex(':');
    debug << Qt::endl;

    debug << "essid:" << essid;
    debug << Qt::endl << Qt::endl;


    // print station
    debug << "station" << Qt::endl;
    debug << stmac.toHex(':') << Qt::endl;
}


/* Request from device to connect to ap
** Parameters:
**     packet:
** Return:
** Notes:
**     index 0: indicates the tag type
**     index 1: inicates the size of the tag
*/
bool StationModel::probe_request(const QByteArray &pk, QByteArray &essid)
{
    // start parsing at Wireless Management Header
    QByteArray tags = pk.sliced(24);

    int tag_id;
    int tag_len;
    QByteArray tag_data;

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
        tag_id = BYTE_TO_UCHAR(tags, 0);
        tag_len = BYTE_TO_UCHAR(tags, 1);


        switch (find_ssid(tags, essid))
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
            qDebug() << "Error: tag parsing attempted to move out of bounds";
            qDebug() << "Skipping Packet" << Qt::endl;

            return false;
        }

        // move to the next tag
        tags = tags.sliced (2+tag_len);
    }

    qDebug() << "Warning: probe_request() timed out";
    qDebug() << "Skipping Packet" << Qt::endl;
    return false;
}


/* Access Point response from device probe request
** Parameters:
**     packet:
** Return:
** Notes:
**     index 0: indicates the tag type
**     index 1: inicates the size of the tag
**     index 2->tag_size: contains the tag data
*/
bool StationModel::probe_response(const QByteArray &pk, QByteArray &essid)
{
    // type cast to uchar (0x80 or b1000 0000 results in a negative with signed char)
    if ((uchar)pk.at(0) != IEEE80211_FC0_SUBTYPE_BEACON
        && pk.at(0) != IEEE80211_FC0_SUBTYPE_PROBE_RESP)
    {
        return true;
    }

    if ((uchar)pk.at(0) == IEEE80211_FC0_SUBTYPE_BEACON)
    {
        qDebug() << "Beacon Frame";
    }

    if (pk.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_RESP)
    {
        qDebug() << "Probe Response";
    }

    // store preamble

    // store timestamp

    QByteArray tags = pk.sliced(36);

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


        switch (find_ssid(tags, essid))
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
            qDebug() << "Error: tag parsing attempted to move out of bounds";
            qDebug() << "Skipping Packet" << Qt::endl;

            return false;
        }

        // move to the next tag
        tags = tags.sliced (2+tag_len);
    }

    qDebug() << "Warning: probe_request() timed out";
    qDebug() << "Skipping Packet" << Qt::endl;
    return false;
}


TagSearch StationModel::find_ssid(const QByteArray &tags, QByteArray &essid)
{
    int tag_id;             // index 0
    int tag_len;            // index 1
    QByteArray tag_data;    // index 2+

    tag_id = BYTE_TO_UCHAR(tags, 0);
    tag_len = BYTE_TO_UCHAR(tags, 1);


    if (tag_len == 0)
    {
        qDebug() << "Info: packet contains a wildcard ssid";
        qDebug() << "Skipping Packet" << Qt::endl;

        return TagSearch::Error;
    }
    else if (tag_len == 1 && tags.at(2) == ' ')
    {
        qDebug() << "Info: packet contains a empty ssid";
        qDebug() << "Skipping Packet" << Qt::endl;

        return TagSearch::Error;
    }
    else if (tags.at(2) == '\0')
    {
        qDebug() << "Info: packet contains a null ssid";
        qDebug() << "Skipping Packet" << Qt::endl;

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
            essid = tag_data;
        }

        return TagSearch::TagFound;
    }

    return TagSearch::TagNotFound;
}

