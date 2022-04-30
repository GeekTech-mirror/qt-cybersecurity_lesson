#include <QItemEditorFactory>
#include <QModelRoleDataSpan>

#include "network_delegate.h"

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
