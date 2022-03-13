#include "main_window.h"
#include "./ui_main_window.h"

Main_Window::Main_Window (QWidget *parent)
    : QMainWindow (parent)
    , ui(new Ui::Main_Window)
{
    ui->setupUi (this);

    QIcon::setThemeName("breeze-icons");
    ui->action_home->setIcon (QIcon::fromTheme("user-home"));
    ui->action_help->setIcon (QIcon::fromTheme("help-about"));

    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSerif.ttf");
    ui->title->setFont (QFont("DejaVuSerif", 45, QFont::Bold));

    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSans.ttf");
    ui->rubber_ducky_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->wifi_jammer_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->man_in_the_middle_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->war_driving_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
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


void Main_Window::on_man_in_the_middle_button_clicked()
{
    ui->Stacked_Widget->setCurrentIndex(3);
}


void Main_Window::on_war_driving_button_clicked()
{
    ui->Stacked_Widget->setCurrentIndex(4);
}


void Main_Window::on_action_home_triggered()
{
    ui->Stacked_Widget->setCurrentIndex(0);
}

