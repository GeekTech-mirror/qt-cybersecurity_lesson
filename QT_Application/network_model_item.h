#ifndef QNetworkItem_H
#define QNetworkItem_H

#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Utils>

class QNetworkItem
{
public:
    explicit QNetworkItem(const QVector<QVariant> &data, QNetworkItem *parent = nullptr);
    ~QNetworkItem();


    /* new */
    QNetworkItem *parent();
    QNetworkItem *child(int row);
    int childNumber() const;
    int childCount() const;
    int columnCount() const;

    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);

    bool insertColumns(int position, int columns);
    bool removeColumns(int position, int columns);



    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);


private:
    QNetworkItem *m_parentItem;
    QVector<QNetworkItem*> m_childItems;
    QVector<QVariant> m_itemData;

};

#endif // QNetworkItem_H
