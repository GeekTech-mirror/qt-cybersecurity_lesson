#ifndef STATION_MODEL_P_H
#define STATION_MODEL_P_H

#include "pcap_common.h"

#include "station_model.h"
#include "station_item.h"

class StationModelPrivate
{
public:
    StationModelPrivate ();


    StationItem *getItem (const QModelIndex &index, const StationModel *dd) const
    {
        if (index.isValid()) {
            StationItem *item = static_cast<StationItem*>(index.internalPointer());
            if (item)
                return item;
        }
        return dd->rootItem;
    }


    QVariant getColumn (const QModelIndex &index, StationModel *dd) const
    {
        StationItem *item = getItem (index, dd);

        switch (dd->rootItem->getRole(index.column())) {
        case StationItemRole::StationRole:
            return item->stmac ();
            break;
        case StationItemRole::InterfaceRole:
            return QVariant ();
            break;
        case StationItemRole::AccessPointRole:
            return item->apEssid ();
            break;
        case StationItemRole::Bssid2Role:
            return item->apBssid(Chan_2GHz);
            break;
        case StationItemRole::Bssid5Role:
            return item->apBssid(Chan_5GHz);
        case StationItemRole::Channel2Role:
            return item->apChannel(Chan_2GHz);
            break;
        case StationItemRole::Channel5Role:
            return item->apChannel(Chan_5GHz);
            break;

        default:
            return "";
        }
    }

    bool setItemRole (const QVariant &role,
                      const QModelIndex &index,
                      const StationModel *dd)
    {
        StationItem *item = getItem (index, dd);

        switch (role.toInt()) {
        case StationItemRole::AccessPointRole:
            item->insertRole (StationItemRole::AccessPointRole);
            break;
        case StationItemRole::Bssid2Role:
            item->insertRole (StationItemRole::Bssid2Role);
            break;
        case StationItemRole::Bssid5Role:
            item->insertRole (StationItemRole::Bssid5Role);
            break;
        case StationItemRole::Channel2Role:
            item->insertRole (StationItemRole::Channel2Role);
            break;
        case StationItemRole::Channel5Role:
            item->insertRole (StationItemRole::Channel5Role);
            break;
        case StationItemRole::InterfaceRole:
            item->insertRole (StationItemRole::InterfaceRole);
            break;
        case StationItemRole::StationRole:
            item->insertRole (StationItemRole::StationRole);
            break;
        default:
            return false;
            break;
        }

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
    bool station_request(const QByteArray &pk, ap_probe &ap)
    {
        // return if subtype is not a probe request
        if (pk.at(0) != IEEE80211_FC0_SUBTYPE_PROBE_REQ
            && pk.at(0) != IEEE80211_FC0_SUBTYPE_ASSOC_REQ)
        {
            return true;
        }

        // start at tagged parameters
        QByteArray tags;
        if (pk.at(0) == IEEE80211_FC0_SUBTYPE_PROBE_REQ)
            tags = pk.sliced(24);

        if (pk.at(0) == IEEE80211_FC0_SUBTYPE_ASSOC_REQ)
            tags = pk.sliced(28);

        int tag_len;

        // timout if process takes too long
        QTimer *timeout = new QTimer();
        timeout->setSingleShot(true);
        timeout->start(2000);


        // Probe tagged parameters
        while (timeout->isActive())
        {
            tag_len = BYTE_TO_UCHAR(tags, 1);


            switch (find_essid(tags, ap))
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
    bool ap_response(const QByteArray &pk, ap_probe &ap)
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

         // start at tagged parameters
        QTimer *timeout = new QTimer();
        timeout->setSingleShot(true);
        timeout->start(2000);


        // Probe tagged parameters
        while (timeout->isActive())
        {
            tag_id = BYTE_TO_UCHAR(tags, 0);
            tag_len = BYTE_TO_UCHAR(tags, 1);

            /* find essid */
            switch (find_essid(tags, ap))
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


    TagSearch find_essid (const QByteArray &tags, ap_probe &ap)
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

    bool find_bssid (QByteArray &pk, ap_probe &ap)
    {
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
                return false;
        }

        return true;
    }

    bool find_stmac (QByteArray &pk, ap_probe &ap, QByteArray &stmac)
    {
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
                return false;
        }

        return true;
    }
};



#endif // STATION_MODEL_P_H
