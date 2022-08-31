#ifndef QNETWORKDELEGATE_H
#define QNETWORKDELEGATE_H

#include <QStyledItemDelegate>


class NetworkDelegatePrivate;

class NetworkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint (QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const override;
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const override;

protected:


private:
};

#endif // QNETWORKDELEGATE_H
