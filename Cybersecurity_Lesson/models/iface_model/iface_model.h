#ifndef IFACE_MODEL_H
#define IFACE_MODEL_H

#include <QAbstractItemModel>
#include <QNetworkInterface>
#include <QTimer>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/WirelessDevice>


class IfaceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /* Constructor */
    explicit IfaceModel(QObject *parent = nullptr);

    ~IfaceModel ();

    /* List Model */
    QVariant data (const QModelIndex &index, int role) const override;

//    bool setData (const QModelIndex &index, const QVariant &device,
//                  int role = Qt::EditRole) override;

    int rowCount (const QModelIndex &parent = QModelIndex()) const override;

    bool insertRow (const QNetworkInterface iface);

    bool removeRows (int position, int rows = 1,
                     const QModelIndex &parent = QModelIndex()) override;

    Qt::ItemFlags flags (const QModelIndex &index) const override;

private Q_SLOTS:
    void updateIface ();

    void removeIface (QNetworkInterface iface);

    void ifaceAdded ();

private:
    QList<QNetworkInterface> m_interfaces;
    QList<QString> interface_name;
    QTimer *m_ifaceUpdate_t = nullptr;

    void initIface ();
};

#endif // IFACE_MODEL_H
