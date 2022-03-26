#include <arpa/inet.h>
#include <QThread>
#include <QTextStream>
#include <QStandardItem>
#include <QStringListModel>
#include <QAbstractItemModel>
#include <QDebug>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/AccessPoint>

#include "main_window.h"
#include "./ui_main_window.h"

#include "network_model.h"
#include "network_list.h
Main_Window::Main_Window (QWidget *parent)
    : QMainWindow (parent)
    , ui(new Ui::Main_Window)
{
    ui->setupUi (this);

    // Add icons to project
    QIcon::setThemeName("breeze-icons");
    ui->action_home->setIcon (QIcon::fromTheme("user-home"));
    ui->action_help->setIcon (QIcon::fromTheme("help-about"));

    // Add DejaVu Serif font
    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSerif.ttf");
    ui->title->setFont (QFont("DejaVuSerif", 45, QFont::Bold));

    // Add DejaVu Sans font
    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSans.ttf");
    ui->rubber_ducky_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->wifi_jammer_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->man_in_the_middle_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->war_driving_button->setFont (QFont("DejavuSans", 26, QFont::Bold));

    QNetworkModel *network_model = new QNetworkModel();

    ui->network_list->setModel (network_model);


}


Main_Window::~Main_Window ()
{
    delete ui;
}


void Main_Window::on_rubber_ducky_button_clicked ()
{
    ui->Stacked_Widget->setCurrentIndex(1);
}


void Main_Window::on_wifi_jammer_button_clicked ()
{
    ui->Stacked_Widget->setCurrentIndex(2);
}


void Main_Window::on_man_in_the_middle_button_clicked ()
{
    ui->Stacked_Widget->setCurrentIndex(3);
}


void Main_Window::on_war_driving_button_clicked ()
{
    ui->Stacked_Widget->setCurrentIndex (4);
/*
    QStringList results;

    // getting all of the devices, and iterate through them
    NetworkManager::Device::List list = NetworkManager::networkInterfaces();
    Q_FOREACH (NetworkManager::Device::Ptr dev, list)
    {
        if((NMDeviceType)dev->type() != NM_DEVICE_TYPE_WIFI)
        {
            //skipping non-wifi interfaces
            continue;
        }
        // creating a Wifi device with this object path
        NetworkManager::WirelessDevice wifi_dev(dev->uni());
        wifi_dev.requestScan();
        QThread::sleep(2); // still not the best solution:w
        //get the Object Path of all the visible access points
        // and iterate through
        foreach(QString ap_path, wifi_dev.accessPoints())
        {
            // creating an AccessPoint object with this path
            NetworkManager::AccessPoint ap(ap_path);

            // and finally get the SSID
            results << ap.ssid();
        }

        QAbstractItemModel *model = new QStringListModel (results);
        ui->network_list->setModel(model);
    }
*/
}


void Main_Window::on_action_home_triggered ()
{
    ui->Stacked_Widget->setCurrentIndex (0);
}

