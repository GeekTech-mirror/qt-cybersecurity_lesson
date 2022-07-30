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
#include <QTimer>

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
    **     Button font color
    **     Scrollbar colors
    */
    ui->Central_Widget->setStyleSheet (main_window_stylesheet()
                                       + stylesheets->vertical_scrollbar()
                                       + stylesheets->horizontal_scrollbar());

    // Set starting point to main menu
    ui->Stacked_Widget->setCurrentIndex (0);

    // Ensures main menu index is set
    QTimer *timeout = new QTimer();
    timeout->setSingleShot(true);
    timeout->setInterval(500);

    timeout->start();
    while (ui->Stacked_Widget->currentIndex() != 0)
    {
        ui->Stacked_Widget->setCurrentIndex (0);

        if (!timeout->remainingTime())
        {
            timeout->stop();
            break;
        }
    }
    delete timeout;

    // alternative to current layout
    //connect(ui->rubber_ducky_button, &QPushButton::clicked,
    //        this, [&]{ ui->Stacked_Widget->setCurrentIndex(1); });
}


Main_Window::~Main_Window ()
{
    delete ui;
}


/*
 * Menu Navigation
 */
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

void Main_Window::on_action_close_triggered()
{
    Main_Window::close();
}


/*
 * Setup UI
 */
void Main_Window::setup_fonts (void)
{
    // Add Dejavu fonts
    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSerif.ttf");
    QFontDatabase::addApplicationFont (":/fonts/dejavu/DejaVuSans.ttf");

    // Add Liberation fonts
    QFontDatabase::addApplicationFont (":/fonts/liberation/LiberationSans-Bold.ttf");
}

void Main_Window::setup_main_menu (void)
{
    // Set Title Font
    ui->title->setFont (Main_Window::title_font);

    // Set Main Menu Button Fonts
    ui->rubber_ducky_button->setFont (Main_Window::button_font);
    ui->wifi_jammer_button->setFont (Main_Window::button_font);
    ui->man_in_the_middle_button->setFont (Main_Window::button_font);
    ui->war_driving_button->setFont (Main_Window::button_font);
}

QString Main_Window::main_window_stylesheet()
{
    // Set background color and button font color
    QString stylesheet =
    "* { background-color: rgb(30, 34, 39); } \
     QPushButton { color: rgb(209, 213, 218); }";

    return stylesheet;
}


