#ifndef QNetworkModel_H
#define QNetworkModel_H

#include <QAbstractItemModel>
#include <QQueue>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/WirelessDevice>

#include "network_model_item.h"

class QNetworkModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QNetworkModel(const QStringList &headers, const QString &data,
                           QObject *parent = nullptr);
    ~QNetworkModel();


    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    void setupModelData(const QStringList &lines, QNetworkItem *parent);
    QNetworkItem *getItem(const QModelIndex &index) const;

    QNetworkItem *rootItem;

};

#endif // QNetworkModel_H
