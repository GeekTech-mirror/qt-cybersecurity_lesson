#include "network_sort.h"
#include "network_enums.h"
#include "network_item.h"

NetworkSort::NetworkSort (QObject *parent)
    : QSortFilterProxyModel{parent}
{

}


bool NetworkSort::lessThan(const QModelIndex &left,
                           const QModelIndex &right) const
{
    //QVariant leftData = sourceModel()->data(left);
    //QVariant rightData = sourceModel()->data(right);

    NetworkItem *left_item = static_cast<NetworkItem*>(left.internalPointer());
    NetworkItem *right_item = static_cast<NetworkItem*>(right.internalPointer());

    QString left_data = left_item->data(left.column()).toString().toLower();
    QString right_data = right_item->data(right.column()).toString().toLower();

    if (left_item->columnCount() == 0)
    {
        qDebug() << "Error: sorting item with no roles";
        return false;
    }

    switch (left_item->getRole(left.column())) {
    case (ItemRole::SecurityTypeRole):
        if (left_item->securityType() == right_item->securityType())
        {
            return left_item->ssid().toLower()
                    < right_item->ssid().toLower();
        }
        return left_item->securityType() < right_item->securityType();
        break;
    default:
        return  left_data < right_data;
        break;

    }
}
