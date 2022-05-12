//#include <QItemEditorFactory>
//#include <QModelRoleDataSpan>
#include <QTreeView>

#include "network_delegate.h"
#include "network_enums.h"

/*

void paint (QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const;

QSize sizeHint (const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
*/


/*
void QNetworkDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{

    if (index.data().canConvert<StarRating>()) {
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        model->setData(index, QVariant::fromValue(starEditor->starRating()));
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
*/

void NetworkDelegate::paint (QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    //if it's column 0 just draw the original
    /*
    if (index.column() == 0)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }
    */

    //get the style to draw with
    QStyle* style = option.widget->style();

    //calculate tree level
    int level = 0;
    for(auto i = index; (i = i.parent()).isValid(); level++);

    //calculate indentation
    auto view = qobject_cast<const QTreeView*>(option.widget);
    int indent = level * (view ? view->indentation() : 20 ); //some arbitrary default value
    //create a styled option object
    QStyleOptionViewItem opt = option;
    QStyledItemDelegate::initStyleOption(&opt, index);

    //draw indented item
    opt.rect.adjust(indent, 0, 0, 0);
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, option.widget);

    //fill the gap space
    opt.rect = option.rect;
    opt.rect.setWidth(indent);
    opt.text.clear(); //remove text
    opt.viewItemPosition = QStyleOptionViewItem::Middle; //remove selection frame sides
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, option.widget);
    /*
    switch () {
    case ConnectionIconRole:

    case ConnectionStateRole:

    case DeviceName:

    case DevicePathRole:

    case SsidRole:

    case SpecificPathRole:

    case SecurityTypeRole:

    case TypeRole:

    case UuidRole:

    default:
        break;
    }
    */
}
