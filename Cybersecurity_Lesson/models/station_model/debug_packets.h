#include <QByteArray>

#include "station_common.h"

QByteArray test_stmac = QByteArray(6, 0x33);

ap_probe test_essid_only = {"Test AP",
                            QByteArray(),
                            int(),
                            ChannelFrequency()};

ap_probe test_ap_2GHz =  {"Test AP 2GHz",
                          QByteArray(6, 0xAB),
                          21,
                          Chan_2GHz};

ap_probe test_ap_5GHz =  {"Test AP 5GHz",
                          QByteArray(6, 0xCD),
                          121,
                          Chan_5GHz};

ap_probe test_ap_chan_chang1 =  {"Test AP 2GHz",
                                 QByteArray(6, 0xAB),
                                 21,
                                 Chan_2GHz};


ap_probe test_ap_chan_chang2 =  {"Test AP 2GHz",
                                 QByteArray(6, 0xAB),
                                 2,
                                 Chan_2GHz};

ap_probe test_ap_bssid_chang1 =  {"Test AP 2GHz",
                                 QByteArray(6, 0xAA),
                                 2,
                                 Chan_2GHz};

ap_probe test_ap_bssid_chang2 =  {"Test AP 2GHz",
                                 QByteArray(6, 0xBB),
                                 2,
                                 Chan_2GHz};
