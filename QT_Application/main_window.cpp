#include <QAbstractSlider>
#include <QCommonStyle>
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QStringBuilder>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/AccessPoint>

#include "main_window.h"
#include "./ui_main_window.h"

#include "custom_stylesheets.h"
#include "custom_colors.h"

#include "network_model.h"
#include "network_delegate.h"
//#include "custom_scrollbar.h"

Main_Window::Main_Window (QWidget *parent)
    : QMainWindow (parent),
      ui(new Ui::Main_Window),
      stylesheets (new CustomStyleSheets)
{
    ui->setupUi (this);

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
    QVector<ItemRole> roles ({ItemRole::ConnectionIconRole,
                              ItemRole::SsidRole,
                              ItemRole::SecurityTypeRole});

    NetworkModel *network_model = new NetworkModel(roles);

    ui->network_list->setModel (network_model);
    ui->network_list->resizeColumnToContents(network_model->columnCount()-1);
    ui->network_list->setIndentation(10);
    ui->network_list->setColumnWidth(0, 64);
    ui->network_list->setColumnWidth(1, 200);

//    ui->network_list->header()->setSectionResizeMode (QHeaderView::ResizeToContents);

    ui->network_list->header()->setStyleSheet ("QHeaderView { font-size: " +
                                               QString::number(16) + "pt; }");

    treeview_stylesheet = stylesheets->
                          treeview_scrollbar
                          (
                              ui->network_list->header()->
                              sizeHint().height()
                          );

    ui->network_list->setStyleSheet (treeview_stylesheet
                                     % stylesheets->treeview_vertical_scrollbar_quirk());

    ui->network_list->horizontalScrollBar()->installEventFilter(this);

    ui->Central_Widget->setStyleSheet(QString("* { background-color: rgb(30, 34, 39); }")
                                      + stylesheets->vertical_scrollbar()
                                      + stylesheets->horizontal_scrollbar());

    // Set starting point to main menu
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


/* Filter to fix vertical scrollbar border when horizontal scrollbar appears*/
bool Main_Window::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Hide)
    {
        QString update_scrollbar;

        update_scrollbar =
                treeview_stylesheet %
                stylesheets->treeview_vertical_scrollbar_quirk();

        ui->network_list->setStyleSheet (update_scrollbar);
    }
    else if (event->type() == QEvent::Show)
    {
        ui->network_list->setStyleSheet (treeview_stylesheet);
    }

    return QMainWindow::eventFilter(object, event);
}

