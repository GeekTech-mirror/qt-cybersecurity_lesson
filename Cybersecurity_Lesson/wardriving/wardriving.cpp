#include <QScrollBar>
#include <QScroller>
#include <QStringBuilder>

#include "wardriving.h"
#include "ui_wardriving.h"

#include "custom_stylesheets.h"
#include "custom_colors.h"

#include "network_model.h"


WarDriving::WarDriving(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarDriving),
    stylesheets (new CustomStyleSheets)
{
    ui->setupUi(this);

    // Setup Tree View to display networks
    setup_network_view();

    connect(ui->start_stop, &QPushButton::clicked,
            network_model, &NetworkModel::toggle_scan);

    connect(ui->start_stop, &QPushButton::clicked,
            this, [&]{ ui->network_scan_icon->setIcon (network_model->getScanIcon()); });

    connect(ui->export_to_csv, &QPushButton::clicked,
            network_model, &NetworkModel::export_data);

    connect(ui->export_to_csv, &QPushButton::clicked,
            this, [&]{ ui->network_scan_icon->setIcon (network_model->getScanIcon()); });
}

WarDriving::~WarDriving()
{
    delete ui;
}


void WarDriving::setup_network_view (void)
{
    QVector<ItemRole> roles ({ItemRole::NameRole,
                              ItemRole::SecurityTypeRole});

    network_model = new NetworkModel (roles);
    network_model->setWirelessProperties (WirelessProperties::RemoveUnavailableNetworks, false);

    network_sort = new NetworkSort();
    network_sort->setSourceModel (network_model);


    // Set network model and column settings
    ui->network_view->setModel (network_sort);
    ui->network_view->resizeColumnToContents (network_model->columnCount()-1);
    ui->network_view->setIndentation (10);
    ui->network_view->setIconSize (QSize (36,36));
    ui->network_view->setColumnWidth (0, 290);

    ui->network_view->setSortingEnabled (true);


    // Set treeview fonts
    ui->network_view->header()->setFont(QFont("LiberationSans", 18, QFont::Bold));
    ui->network_view->setFont (QFont("LiberationSerif", 16, QFont::Normal));


    // Set treeview header size for scrollbar gutter
    treeview_stylesheet = stylesheets->
                          treeview_scrollbar
                          (
                              ui->network_view->header()->
                              sizeHint().height()
                          );

    // Set treeview scrollbar colors
    ui->network_view->setStyleSheet (treeview_stylesheet
                                     % stylesheets->vertical_scrollbar_quirk());

    // install filter to correct horizontal and vertical scrollbar borders
    ui->network_view->verticalScrollBar()->installEventFilter(this);
    ui->network_view->horizontalScrollBar()->installEventFilter(this);

    //QScroller::grabGesture(ui->network_view, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->network_view, QScroller::TouchGesture);
}


/* Filters to fix vertical/horizontal scrollbar border when scrollbars appear
**
** Add bottom border to vertical scrollbar, on horizontal scrollbar hide
**
** Remove bottom border to vertical scrollbar on horizontal scrollbar appear
**/
void WarDriving::setStylesheetOnly()
{
    ui->network_view->setStyleSheet (treeview_stylesheet);
}

void WarDriving::setVerticalScrollBarQuirk()
{
    QString update_scrollbar =
            treeview_stylesheet
            % stylesheets->vertical_scrollbar_quirk();

    ui->network_view->setStyleSheet (update_scrollbar);
}

void WarDriving::setHorizontalScrollbarQuirk()
{
    QString update_scrollbar =
            treeview_stylesheet
            % stylesheets->horizontal_scrollbar_quirk();

    ui->network_view->setStyleSheet (update_scrollbar);
}

bool WarDriving::eventFilter(QObject *object, QEvent *event)
{
    bool isHorizontalScrollBar =
        (object == ui->network_view->horizontalScrollBar()) ? true : false;

    bool isVerticalScrollBar =
        (object == ui->network_view->verticalScrollBar()) ? true : false;

    bool isHorizontalScrollBarVisible =
        (ui->network_view->horizontalScrollBar()->isVisible()) ? true : false;

    bool isVerticalScrollBarVisible =
        (ui->network_view->verticalScrollBar()->isVisible()) ? true : false;


    if (event->type() == QEvent::Hide)
    {
        if (isHorizontalScrollBar
            && isVerticalScrollBarVisible)
        {
            setVerticalScrollBarQuirk();
        }
    }
    else if (event->type() == QEvent::Show)
    {
        if (isHorizontalScrollBar
            && isVerticalScrollBarVisible)
        {
            setStylesheetOnly();
        }
        else if (isVerticalScrollBar
                 && isHorizontalScrollBarVisible)
        {
            setStylesheetOnly();
        }
        else if (isVerticalScrollBar
                 && !isHorizontalScrollBarVisible)
        {
            setVerticalScrollBarQuirk();
        }
        else if (isHorizontalScrollBar)
        {
            setHorizontalScrollbarQuirk();
        }
    }

    return QWidget::eventFilter (object, event);
}


void WarDriving::stop_wardriving (void)
{
    network_model->stop_scan();

    network_model->clear_data();

    ui->network_scan_icon->setIcon (network_model->getScanIcon());
}
