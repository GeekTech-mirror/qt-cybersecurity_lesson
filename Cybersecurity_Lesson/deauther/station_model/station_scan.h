#ifndef STATIONSCAN_H
#define STATIONSCAN_H

#include <QObject>
#include <QTimer>

#include <NetworkManagerQt/WirelessDevice>

class StationScan : public QObject
{
    Q_OBJECT

public:
    explicit StationScan (QObject *parent = nullptr);
    ~StationScan () override;

public Q_SLOTS:
    void updateConnection (const NetworkManager::Connection::Ptr &connection,
                           const NMVariantMapMap &map);
    void requestScan (const QString &interface  = QString());

private:
    QMap<QString, QTimer *> m_wirelessScanRetryTimer;
    bool checkRequestScanRateLimit(const NetworkManager::WirelessDevice::Ptr
                                   &wifiDevice);
    void scheduleRequestScan (const QString &interface, int timeout);
    void scanRequestFailed (const QString &interface);
};

#endif // STATIONSCAN_H
