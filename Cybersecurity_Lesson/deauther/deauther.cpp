#include <QTimer>

/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "deauther.h"
#include "ui_deauther.h"

#include "pcap.h"
#include "network_model.h"

Deauther::Deauther(QWidget *parent) :
    QWidget(parent),
    search_animation_timer (new QTimer (this)),
    ui(new Ui::Deauther)
{
    ui->setupUi(this);

    setup_network_list();

    station_model = new StationModel();
    iface_model = new IfaceModel();

    // create drop down list for network interfaces
    ui->iface_list->setModel(iface_model);

    // set up toggle for monitor mode (creates a pcap handle)
    ui->monitor_status->
        setIcon(QIcon(":/icons/actions/24/edit-find-replace_inactive.svg"));
    ui->monitor_status->setIconSize(QSize(50, 50));

    connect(ui->toggle_monitoring, &QPushButton::clicked,
            this, &Deauther::toggle_monitoring);

    // set up packet search icon
    search_animation_timer->setInterval (1000);
    connect (search_animation_timer, &QTimer::timeout,
             this, [&]() {search_animation(ui->monitor_status);});


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


void Deauther::toggle_monitoring ()
{
    static bool monitoring = true;

    const QIcon search_inactive = QIcon (":/icons/actions/24/edit-find-replace_inactive.svg");
    const QIcon search_active = QIcon (":/icons/actions/24/edit-find-replace_state0.svg");

    // create a pcap handle using the selected network interface
    QByteArray iface = ui->iface_list->currentText().toUtf8();
    QByteArray error_buffer[PCAP_ERRBUF_SIZE];

    if (monitoring)
    {
        iface_handle = pcap_create(iface.data(), error_buffer->data());

        if (iface_handle == NULL)
        {
            qWarning() << "pcap_create failed:" << error_buffer->data();
            return;
        }

        // check if iface supports monitoring
        if (!pcap_can_set_rfmon (iface_handle))
        {
            qWarning() << "Monitor mode is not supported for" << iface.data();
            pcap_close (iface_handle);
            return;
        }

        // set handle to monitor mode
        if (pcap_set_rfmon (iface_handle, 1))
        {
            qWarning() << "Setting" << iface.data() << "to monitor mode failed";
            pcap_close (iface_handle);
            return;
        }

        // activate new monitor iface
        if (pcap_activate (iface_handle))
        {
            qWarning() << "Creating monitoring interface for" << iface.data() << "failed";
            qWarning() << "Do you have permissions?";
            pcap_close (iface_handle);
            return;
        }

        station_model->setIfaceHandle (iface_handle);

        // start packet search icon
        search_animation_timer->start();
        ui->monitor_status->setIcon (search_active);
        search_animation_state = 1;
    }
    else
    {
        pcap_close (iface_handle);

        // stop packet search icon
        search_animation_timer->stop();
        ui->monitor_status->setIcon (search_inactive);
    }

    // toggle monitoring mode
    monitoring ^= 1;

}


void Deauther::search_animation (QPushButton *button)
{
    switch (search_animation_state) {
    case 1:
        button->setIcon (QIcon (":/icons/actions/24/edit-find-replace_state1.svg"));
        break;
    case 2:
        button->setIcon (QIcon (":/icons/actions/24/edit-find-replace_state2.svg"));
        break;
    case 3:
        button->setIcon (QIcon (":/icons/actions/24/edit-find-replace_state3.svg"));
        break;
    default:
        search_animation_state = 1;
        break;
    }
    ++search_animation_state;


    if (search_animation_state > 3)
    {
        search_animation_state = 1;
    }
}
