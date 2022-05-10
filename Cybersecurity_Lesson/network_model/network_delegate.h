#ifndef QNETWORKDELEGATE_H
#define QNETWORKDELEGATE_H

#include <QStyledItemDelegate>


class QNetworkDelegatePrivate;

class QNetworkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const override;

protected:


private:
};

#endif // QNETWORKDELEGATE_H
