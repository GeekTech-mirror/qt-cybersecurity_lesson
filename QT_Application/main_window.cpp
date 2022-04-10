#include <arpa/inet.h>
#include <QThread>
#include <QTextStream>
#include <QStringListModel>
#include <QAbstractItemModel>
#include <QDebug>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/AccessPoint>

#include "main_window.h"
#include "./ui_main_window.h"

#include "network_model.h"

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

    // Setup Tree View to display networks
    QVector<QNetworkModel::ItemRole> roles ({QNetworkModel::ConnectionIconRole,
                                            QNetworkModel::SsidRole,
                                            QNetworkModel::SecurityTypeRole});

    QNetworkModel *network_model = new QNetworkModel(roles);

    ui->network_list->setModel (network_model);
    ui->network_list->setColumnWidth(0, 64);
    ui->network_list->setColumnWidth(1, 220);

    ui->Stacked_Widget->setCurrentIndex(0);
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
}


void Main_Window::on_action_home_triggered ()
{
    ui->Stacked_Widget->setCurrentIndex (0);
}

