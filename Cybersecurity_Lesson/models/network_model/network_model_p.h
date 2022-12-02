#ifndef NETWORK_MODEL_P_H
#define NETWORK_MODEL_P_H

#include <QIcon>

#include <NetworkManagerQt/Manager>

#include "network_model.h"


class NetworkModelPrivate
{
public:
    NetworkModelPrivate ();

    NetworkItem *getItem (const QModelIndex &index, const NetworkModel *dd) const
    {
        if (index.isValid()) {
            NetworkItem *item = static_cast<NetworkItem*>(index.internalPointer());
            if (item)
                return item;
        }
        return dd->rootItem;
    }


    QVariant getColumn (const QModelIndex &index, NetworkModel *dd) const
    {
        NetworkItem *item = getItem (index, dd);

        switch (dd->rootItem->getRole(index.column())) {
        case DeviceName:
            return item->deviceName();
            break;

        case DevicePathRole:
            return item->devicePath();
            break;

        case ConnectionIconRole:
            return QIcon (item->icon());
            break;

        case NameRole:
            return item->name();
            break;

        case SpecificPathRole:
            return item->specificPath();
            break;

        case SecurityTypeRole:
            return getSecurityString(item->securityType());
            break;

        case SsidRole:
            return item->ssid();
            break;

        case TypeRole:
            return item->type();
            break;

        default:
            return "";
        }
    }


    QString getSecurityString (NetworkManager::WirelessSecurityType type) const
    {
        switch (type) {
        case NetworkManager::NoneSecurity:
            return QStringLiteral("None");
            break;

        case NetworkManager::StaticWep:
            return QStringLiteral("Static-Wep");
            break;

        case NetworkManager::DynamicWep:
            return QStringLiteral("Dynamic-Wep");
            break;

        case NetworkManager::Leap:
            return QStringLiteral("Leap");
            break;

        case NetworkManager::WpaPsk:
            return QStringLiteral("Wpa-Psk");
            break;

        case NetworkManager::WpaEap:
            return QStringLiteral("Wpa-Eap");
            break;

        case NetworkManager::Wpa2Psk:
            return QStringLiteral("Wpa2-Psk");
            break;

        case NetworkManager::Wpa2Eap:
            return QStringLiteral("Wpa2-Eap");
            break;

        case NetworkManager::SAE:
            return QStringLiteral("SAE");
            break;

        case NetworkManager::Wpa3SuiteB192:
            return QStringLiteral("Wpa3-Suite-B192");
            break;

        default:
            return QStringLiteral("Unknown");
        }

        return QStringLiteral("error");
    }


    bool setItemRole (const QVariant &role,
                      const QModelIndex &index,
                      const NetworkModel *dd)
    {
        NetworkItem *item = getItem (index, dd);

        switch (role.toInt()) {
        case ItemRole::DeviceName:
            item->insertRole (ItemRole::DeviceName);
            break;

        case ItemRole::DevicePathRole:
            item->insertRole (ItemRole::DevicePathRole);
            break;

        case ItemRole::ConnectionIconRole:
            item->insertRole (ItemRole::ConnectionIconRole);
            break;

        case ItemRole::NameRole:
            item->insertRole (ItemRole::NameRole);
            break;

        case ItemRole::SpecificPathRole:
            item->insertRole (ItemRole::SpecificPathRole);
            break;

        case ItemRole::SecurityTypeRole:
            item->insertRole (ItemRole::SecurityTypeRole);
            break;

        case ItemRole::SsidRole:
            item->insertRole (ItemRole::SsidRole);
            break;

        case ItemRole::TypeRole:
            item->insertRole (ItemRole::TypeRole);
            break;

        default:
            return false;
            break;
        }

        return true;
    }


    void initializeSignals () const
    {
    //    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionAdded,
    //        this, &NetworkModel::activeConnectionAdded, Qt::UniqueConnection);
    //    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded,
    //        this, &NetworkModel::deviceAdded, Qt::UniqueConnection);
    //    connect(NetworkManager::notifier(), &NetworkManager::Notifier::statusChanged,
    //        this, &NetworkModel::statusChanged, Qt::UniqueConnection);
    }

    void initializeSignals (const NetworkManager::Connection::Ptr &connection) const
    {
    //    connect(connection.data(), &NetworkManager::Connection::updated,
    //        this, &NetworkModel::connectionUpdated, Qt::UniqueConnection);
    }

    void initializeSignals (const NetworkManager::Device::Ptr &device, NetworkModel *dd) const
    {
    //    connect(device.data(), &NetworkManager::Device::stateChanged,
    //        this, &NetworkModel::deviceStateChanged, Qt::UniqueConnection);

        if (device->type () == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDev =
                device.objectCast<NetworkManager::WirelessDevice>();

            dd->connect (wifiDev.data(), &NetworkManager::WirelessDevice::networkAppeared,
                         dd, &NetworkModel::wirelessNetworkAppeared, Qt::UniqueConnection);
        }
    }

    void initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network) const
    {
    //    connect(network.data(),
    //            &NetworkManager::WirelessNetwork::referenceAccessPointChanged,
    //            this,
    //            &NetworkModel::wirelessNetworkReferenceApChanged,
    //            Qt::UniqueConnection);
    }
};

#endif // NETWORK_MODEL_P_H
