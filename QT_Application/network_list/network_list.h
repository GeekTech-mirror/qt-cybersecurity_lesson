#ifndef QNETWORKLIST_H
#define QNETWORKLIST_H

#include <QAbstractListModel>

#include <NetworkManagerQt/ConnectionSettings>

#include <QObject>

class QNetworkModelItem;

class QNetworkList : public QObject
{
    Q_OBJECT
public:
    enum FilterType {
        ActiveConnection,
        Connection,
        Device,
        Name,
        Ssid,
        Uuid,
        Type,
    };

    explicit QNetworkList(QObject *parent = nullptr);
    ~QNetworkList() override;

    bool contains(const FilterType type, const QString &parameter) const;
    int count() const;
    int indexOf(QNetworkModelItem *item) const;
    QNetworkModelItem *itemAt(int index) const;
    QList<QNetworkModelItem *> items() const;
    QList<QNetworkModelItem *> returnItems(const FilterType type, const QString &parameter, const QString &additionalParameter = QString()) const;
    QList<QNetworkModelItem *> returnItems(const FilterType type, NetworkManager::ConnectionSettings::ConnectionType typeParameter) const;

    void insertItem(QNetworkModelItem *item);
    void removeItem(QNetworkModelItem *item);

private:
    QList<QNetworkModelItem *> m_items;
};

#endif // QNETWORKLIST_H
