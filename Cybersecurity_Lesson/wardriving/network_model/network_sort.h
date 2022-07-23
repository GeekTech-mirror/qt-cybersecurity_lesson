#ifndef NETWORKSORTFILTER_H
#define NETWORKSORTFILTER_H

#include <QSortFilterProxyModel>

class NetworkSortFilter : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit NetworkSortFilter(QObject *parent = nullptr);

signals:

};

#endif // NETWORKSORTFILTER_H
