/* Main Menu
** File: main_window.cpp
** --------
** Set-up the main app ui
** --------
*/

/* Qt include files */
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QStringBuilder>

/* local include files */
#include "main_window.h"
#include "ui_main_window.h"

#include "custom_stylesheets.h"
#include "custom_colors.h"


Main_Window::Main_Window (QWidget *parent)
    : QMainWindow (parent),
      ui (new Ui::Main_Window),
      stylesheets (new CustomStyleSheets)
{
    ui->setupUi (this);

    // Embed fonts
    setup_fonts();

    // Set Main Menu fonts
    setup_main_menu();

    /* Set global style sheets
    **     Background color
    **     Scrollbar colors
    */
    ui->Central_Widget->setStyleSheet (QString("* { background-color: rgb(30, 34, 39); }")
                                       + stylesheets->vertical_scrollbar()
                                       + stylesheets->horizontal_scrollbar());

    // Set starting point to main menu
    ui->Stacked_Widget->setCurrentIndex (0);
}


Main_Window::~Main_Window ()
{
    delete ui;
}


void Main_Window::on_man_in_the_middle_button_clicked ()
{
    ui->Stacked_Widget->setCurrentIndex(1);
}


void Main_Window::on_wifi_jammer_button_clicked ()
{
    ui->Stacked_Widget->setCurrentIndex(2);
}


void Main_Window::on_rubber_ducky_button_clicked ()
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


/*
 * Setup UI
 */
void Main_Window::setup_fonts (void)
{
    // Add Dejavu fonts
    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejauSerif.ttf");
    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSans.ttf");

    // Add Liberation fonts
    QFontDatabase::addApplicationFont (":/fonts/liberation/LiberationSans-Bold.ttf");
}

void Main_Window::setup_main_menu (void)
{
    // Set Title Font
    ui->title->setFont (QFont("DejaVuSerif", 45, QFont::Bold));

    // Set Main Menu Button Fonts
    ui->rubber_ducky_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->wifi_jammer_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->man_in_the_middle_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
    ui->war_driving_button->setFont (QFont("DejavuSans", 26, QFont::Bold));
}
