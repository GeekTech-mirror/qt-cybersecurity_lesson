#ifndef QNETWORKTREEVIEW_H
#define QNETWORKTREEVIEW_H

#include <QTreeView>
#include <QObject>

#include "network_list.h"
#include "network_model.h"

class QNetworkTreeView : public QObject
{
    Q_OBJECT
public:
    QNetworkTreeView();

private:

    QNetworkModel *network_model;
    QNetworkList network_list;
};

#endif // QNETWORKTREEVIEW_H
