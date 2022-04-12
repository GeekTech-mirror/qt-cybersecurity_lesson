#ifndef QNETWORKSCAN_H
#define QNETWORKSCAN_H

#include <QObject>
#include <QTimer>

#include <NetworkManagerQt/WirelessDevice>

class QNetworkScan : public QObject
{
    Q_OBJECT

public:
    explicit QNetworkScan (QObject *parent = nullptr);
    ~QNetworkScan () override;

public Q_SLOTS:
    void updateConnection (const NetworkManager::Connection::Ptr &connection,
                           const NMVariantMapMap &map);
    void requestScan (const QString &interface  = QString());

private:
    QMap<QString, QTimer *> m_wirelessScanRetryTimer;
    bool checkRequestScanRateLimit(const NetworkManager::WirelessDevice::Ptr &wifiDevice);
    void scheduleRequestScan(const QString &interface, int timeout);
    void scanRequestFailed(const QString &interface);
};

#endif // QNETWORKSCAN_H
