#ifndef STATION_MODEL_P_H
#define STATION_MODEL_P_H

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
};



#endif // STATION_MODEL_P_H
