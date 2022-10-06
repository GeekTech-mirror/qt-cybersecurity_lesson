#include <NetworkManagerQt/Manager>

#include "station_enums.h"
#include "station_scan.h"

// 1 seconds
#define NM_REQUESTSCAN_LIMIT_RATE 1000


StationScan::StationScan(QObject *parent)
    : QObject{parent}
{
}

StationScan::~StationScan() = default;

void StationScan::updateConnection (const NetworkManager::Connection::Ptr &connection,
                                     const NMVariantMapMap &map)
{
    QDBusPendingReply<> reply = connection->update(map);
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    watcher->setProperty("action", HandlerAction::UpdateConnection);
    watcher->setProperty("connection", connection->name());
}


void StationScan::requestScan (const QString &interface)
{
    for (const NetworkManager::Device::Ptr &device
         : NetworkManager::networkInterfaces())
    {
        if (device->type() == NetworkManager::Device::Wifi)
        {
            NetworkManager::WirelessDevice::Ptr wifiDevice =
                    device.objectCast<NetworkManager::WirelessDevice>();

            if (wifiDevice && wifiDevice->state() !=
                NetworkManager::WirelessDevice::Unavailable)
            {
                if (!interface.isEmpty()
                    && interface != wifiDevice->interfaceName())
                {
                    continue;
                }

                if (!checkRequestScanRateLimit(wifiDevice))
                {
                    QDateTime now = QDateTime::currentDateTime();

                    // for NM < 1.12, lastScan is not available
                    QDateTime lastScan = wifiDevice->lastScan();
                    QDateTime lastRequestScan = wifiDevice->lastRequestScan();

                    // Compute the next time we can run a scan&&
                    int timeout = NM_REQUESTSCAN_LIMIT_RATE;
                    if (lastScan.isValid()
                        && (lastScan.msecsTo(now)
                            < NM_REQUESTSCAN_LIMIT_RATE))
                    {
                        timeout = NM_REQUESTSCAN_LIMIT_RATE
                                  - lastScan.msecsTo(now);
                    }
                    else if (lastRequestScan.isValid()
                             && (lastRequestScan.msecsTo(now)
                                 < NM_REQUESTSCAN_LIMIT_RATE))
                    {
                        timeout = NM_REQUESTSCAN_LIMIT_RATE
                                  - lastRequestScan.msecsTo(now);
                    }
                    scheduleRequestScan (wifiDevice->interfaceName(), timeout);

                    if (!interface.isEmpty())
                    {
                        return;
                    }
                    continue;
                }
                else if (m_wirelessScanRetryTimer.contains(interface))
                {
                    m_wirelessScanRetryTimer.value(interface)->stop();
                    delete m_wirelessScanRetryTimer.take(interface);
                }

                QDBusPendingReply<> reply = wifiDevice->requestScan();
                auto watcher = new QDBusPendingCallWatcher (reply, this);
                watcher->setProperty ("action", HandlerAction::RequestScan);
                watcher->setProperty ("interface", wifiDevice->interfaceName());
            }
        }
    }
}


bool StationScan::checkRequestScanRateLimit (const NetworkManager::WirelessDevice::Ptr &wifiDevice)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime lastScan = wifiDevice->lastScan();
    QDateTime lastRequestScan = wifiDevice->lastRequestScan();

    // if the last scan finished within the last 10 seconds
    bool ret = lastScan.isValid()
               && (lastScan.msecsTo(now)
                   < NM_REQUESTSCAN_LIMIT_RATE);

    // or if the last Request was sent within the last 10 seconds
    ret |= lastRequestScan.isValid()
           && (lastRequestScan.msecsTo(now)
               < NM_REQUESTSCAN_LIMIT_RATE);

    // skip the request scan
    if (ret)
    {
//        qCDebug(PLASMA_NM_LIBS_LOG) << "Last scan finished " << lastScan.msecsTo(now) << "ms ago and last request scan was sent "
//                                    << lastRequestScan.msecsTo(now) << "ms ago, Skipping scanning interface:" << wifiDevice->interfaceName();
        return false;
    }
    return true;
}


void StationScan::scheduleRequestScan (const QString &interface, int timeout)
{
    QTimer *timer;

    if (!m_wirelessScanRetryTimer.contains(interface))
    {
        // create a timer for the interface
        timer = new QTimer();
        timer->setSingleShot (true);
        m_wirelessScanRetryTimer.insert (interface, timer);
        auto retryAction = [this, interface]()
        {
            requestScan (interface);
        };
        connect (timer, &QTimer::timeout, this, retryAction);
    }
    else
    {
        // set the new value for an existing timer
        timer = m_wirelessScanRetryTimer.value(interface);
        if (timer->isActive())
        {
            timer->stop();
        }
    }

    // +1 ms is added to avoid having the scan being rejetted by nm
    // because it is run at the exact last millisecond of the requestScan threshold
    timer->setInterval (timeout + 1);
    timer->start();
}

void StationScan::scanRequestFailed (const QString &interface)
{
    scheduleRequestScan (interface, 2000);
}
