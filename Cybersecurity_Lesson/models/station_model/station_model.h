#ifndef STATION_MODEL_H
#define STATION_MODEL_H

#include <QAbstractItemModel>
#include <QTimer>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/WirelessDevice>

#include "pcap.h"

#include "iface_model.h"

#include "station_enums.h"
#include "station_item.h"

class StationModelPrivate;
class StationModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE (StationModel)
public:
    /* Constructor */
    explicit StationModel (QObject *parent = nullptr);

    ~StationModel ();

    /* Tree Model */
    QVariant data (const QModelIndex &index, int role) const override;
    bool setData (const QModelIndex &index, const QVariant &network_role,
                  int role = Qt::EditRole) override;

    QVariant headerData (int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const override;
    bool setHeaderData (int section, Qt::Orientation orientation,
                        const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index (int row, int column,
                       const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent (const QModelIndex &index) const override;

    int rowCount (const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows (int position, int rows,
                     const QModelIndex &parent = QModelIndex()) override;
    bool removeRows (int position, int rows,
                     const QModelIndex &parent = QModelIndex()) override;

    int columnCount (const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags (const QModelIndex &index) const override;


    /* Station Model */
    QHash<int, QByteArray> roleNames (void) const override;

    void setIfaceHandle (pcap_t *handle);

    void addStation ();

    void capturePacket ();

private Q_SIGNAL:
    int pcap_loop(pcap_t *handle);

private Q_SLOTS:


private:
    StationItem *rootItem;
    QVector<StationItemRole> columnRoles;

    IfaceModel *m_iface;
    pcap_t *m_ifaceHandle;

protected:
    StationModel (StationModelPrivate &dd);
    StationModelPrivate *d_ptr;

};

#endif // STATION_MODEL_H
