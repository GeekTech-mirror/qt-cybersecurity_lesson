#include <QScrollBar>
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
    setup_network_list();
}

WarDriving::~WarDriving()
{
    delete ui;
}


void WarDriving::setup_network_list (void)
{
    QVector<ItemRole> roles ({ItemRole::NameRole,
                              ItemRole::SecurityTypeRole});

    NetworkModel *network_model = new NetworkModel (roles);

    // Set network model and column settings
    ui->network_list->setModel (network_model);
    ui->network_list->resizeColumnToContents (network_model->columnCount()-1);
    ui->network_list->setIndentation (10);
    ui->network_list->setIconSize (QSize (36,36));
    ui->network_list->setColumnWidth (0, 290);


    // Set treeview header font
    ui->network_list->header()->setFont(QFont("LiberationSans", 18, QFont::Bold));


    // Set treeview font
    ui->network_list->setFont (QFont("DejavuSerif", 16, QFont::Normal));


    // Set treeview header size for scrollbar gutter
    treeview_stylesheet = stylesheets->
                          treeview_scrollbar
                          (
                              ui->network_list->header()->
                              sizeHint().height()
                          );

    // Set treeview scrollbar colors
    ui->network_list->setStyleSheet (treeview_stylesheet
                                     % stylesheets->treeview_vertical_scrollbar_quirk());

    // install filter to correct horizontal scrollbar quirks
    ui->network_list->horizontalScrollBar()->installEventFilter(this);
}


/* Filter to fix vertical scrollbar border when horizontal scrollbar appears
**
** Add bottom border to vertical scrollbar, on horizontal scrollbar hide
**
** Remove bottom border to vertical scrollbar on horizontal scrollbar appear
**/
bool WarDriving::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Hide)
    {
        QString update_scrollbar;

        update_scrollbar =
                treeview_stylesheet
                % stylesheets->treeview_vertical_scrollbar_quirk();

        ui->network_list->setStyleSheet (update_scrollbar);
    }
    else if (event->type() == QEvent::Show)
    {
        ui->network_list->setStyleSheet (treeview_stylesheet);
    }

    return QWidget::eventFilter (object, event);
}
