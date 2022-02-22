#include "main_window.h"
#include "./ui_main_window.h"

Main_Window::Main_Window (QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Main_Window)
{
    ui->setupUi(this);
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

