#include "network_list.h"
#include "network_model_item.h"

QNetworkList::QNetworkList(QObject *parent)
    : QObject(parent)
{
}

QNetworkList::~QNetworkList()
{
    qDeleteAll(m_items);
}

bool QNetworkList::contains(const QNetworkList::FilterType type, const QString &parameter) const
{
    for (QNetworkModelItem *item : m_items) {
        switch (type) {
        case QNetworkList::ActiveConnection:
            if (item->activeConnectionPath() == parameter) {
                return true;
            }
            break;
        case QNetworkList::Connection:
            if (item->connectionPath() == parameter) {
                return true;
            }
            break;
        case QNetworkList::Device:
            if (item->devicePath() == parameter) {
                return true;
            }
            break;
        case QNetworkList::Name:
            if (item->name() == parameter) {
                return true;
            }
            break;
        case QNetworkList::Ssid:
            if (item->ssid() == parameter) {
                return true;
            }
            break;
        case QNetworkList::Uuid:
            if (item->uuid() == parameter) {
                return true;
            }
            break;
        case QNetworkList::Type:
            break;
        default:
            break;
        }
    }

    return false;
}

int QNetworkList::count() const
{
    return m_items.count();
}

int QNetworkList::indexOf(QNetworkModelItem *item) const
{
    return m_items.indexOf(item);
}

void QNetworkList::insertItem(QNetworkModelItem *item)
{
    m_items << item;
}

QNetworkModelItem *QNetworkList::itemAt(int index) const
{
    return m_items.at(index);
}

QList<QNetworkModelItem *> QNetworkList::items() const
{
    return m_items;
}

void QNetworkList::removeItem(QNetworkModelItem *item)
{
    m_items.removeAll(item);
}

QList<QNetworkModelItem *>
QNetworkList::returnItems(const QNetworkList::FilterType type, const QString &parameter, const QString &additionalParameter) const
{
    QList<QNetworkModelItem *> result;

    for (QNetworkModelItem *item : m_items) {
        switch (type) {
        case QNetworkList::ActiveConnection:
            if (item->activeConnectionPath() == parameter) {
                result << item;
            }
            break;
        case QNetworkList::Connection:
            if (item->connectionPath() == parameter) {
                if (additionalParameter.isEmpty()) {
                    result << item;
                } else {
                    if (item->devicePath() == additionalParameter) {
                        result << item;
                    }
                }
            }
            break;
        case QNetworkList::Device:
            if (item->devicePath() == parameter) {
                result << item;
            }
            break;
        case QNetworkList::Name:
            if (item->name() == parameter) {
                result << item;
            }
            break;
        case QNetworkList::Ssid:
            if (item->ssid() == parameter) {
                if (additionalParameter.isEmpty()) {
                    result << item;
                } else {
                    if (item->devicePath() == additionalParameter) {
                        result << item;
                    }
                }
            }
            break;
        case QNetworkList::Uuid:
            if (item->uuid() == parameter) {
                result << item;
            }
            break;
        case QNetworkList::Type:
            break;
        }
    }

    return result;
}

QList<QNetworkModelItem *> QNetworkList::returnItems(const QNetworkList::FilterType type,
                                                        NetworkManager::ConnectionSettings::ConnectionType typeParameter) const
{
    QList<QNetworkModelItem *> result;

    for (QNetworkModelItem *item : m_items) {
        if (type == QNetworkList::Type) {
            if (item->type() == typeParameter) {
                result << item;
            }
        }
    }
    return result;
}
