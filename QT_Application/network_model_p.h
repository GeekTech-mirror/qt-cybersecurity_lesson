#ifndef NETWORK_MODEL_P_H
#define NETWORK_MODEL_P_H

#include <QIcon>

#include "network_model.h"

class QNetworkModelPrivate
{
public:
    QNetworkModelPrivate ();


    QNetworkItem *getItem(const QModelIndex &index, const QNetworkModel *dd) const
    {
        if (index.isValid()) {
            QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());
            if (item)
                return item;
        }
        return dd->rootItem;
    }


    QVariant getColumn(const QModelIndex &index, QNetworkModel *dd) const
    {
        QNetworkItem *item = getItem(index, dd);

        switch (dd->columnRoles.at(index.column())) {
        case QNetworkModel::DeviceName:
            return item->deviceName();
            break;

        case QNetworkModel::DevicePathRole:
            return item->devicePath();
            break;

        case QNetworkModel::ConnectionIconRole:
            return QIcon::fromTheme(item->icon());
            break;

        case QNetworkModel::SpecificPathRole:
            return item->specificPath();
            break;

        case QNetworkModel::SecurityTypeRole:
            return getSecurityString(item->securityType());
            break;

        case QNetworkModel::SsidRole:
            return item->ssid();
            break;

        case QNetworkModel::TypeRole:
            return item->type();
            break;

        default:
            return "";
        }
    }


    QString getSecurityString(NetworkManager::WirelessSecurityType type) const
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


    void initializeSignals () const
    {
    //    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionAdded,
    //        this, &QNetworkModel::activeConnectionAdded, Qt::UniqueConnection);
    //    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded,
    //        this, &QNetworkModel::deviceAdded, Qt::UniqueConnection);
    //    connect(NetworkManager::notifier(), &NetworkManager::Notifier::statusChanged,
    //        this, &QNetworkModel::statusChanged, Qt::UniqueConnection);
    }

    void initializeSignals (const NetworkManager::Connection::Ptr &connection) const
    {
    //    connect(connection.data(), &NetworkManager::Connection::updated,
    //        this, &QNetworkModel::connectionUpdated, Qt::UniqueConnection);
    }

    void initializeSignals (const NetworkManager::Device::Ptr &device, QNetworkModel *dd) const
    {
    //    connect(device.data(), &NetworkManager::Device::stateChanged,
    //        this, &QNetworkModel::deviceStateChanged, Qt::UniqueConnection);

        if (device->type () == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDev =
                device.objectCast<NetworkManager::WirelessDevice>();

            dd->connect (wifiDev.data(),
                         &NetworkManager::WirelessDevice::networkAppeared,
                         dd,
                         &QNetworkModel::wirelessNetworkAppeared,
                         Qt::UniqueConnection);
        }
    }

    void initializeSignals(const NetworkManager::WirelessNetwork::Ptr &network) const
    {
    //    connect(network.data(),
    //            &NetworkManager::WirelessNetwork::referenceAccessPointChanged,
    //            this,
    //            &QNetworkModel::wirelessNetworkReferenceApChanged,
    //            Qt::UniqueConnection);
    }
};

#endif // NETWORK_MODEL_P_H
