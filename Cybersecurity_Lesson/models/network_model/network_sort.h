#ifndef NETWORKSORTFILTER_H
#define NETWORKSORTFILTER_H

#include <QSortFilterProxyModel>

class NetworkSort : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit NetworkSort(QObject *parent = nullptr);

private:
    bool lessThan (const QModelIndex &left, const QModelIndex &right) const override;

signals:

};

#endif // NETWORKSORTFILTER_H
