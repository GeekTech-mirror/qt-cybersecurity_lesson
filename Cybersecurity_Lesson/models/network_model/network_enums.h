#ifndef NETWORK_ENUMS_H
#define NETWORK_ENUMS_H

#include <QObject>

enum HandlerAction {
    ActivateConnection,
    AddAndActivateConnection,
    AddConnection,
    DeactivateConnection,
    RemoveConnection,
    RequestScan,
    UpdateConnection,
};

enum ItemRole {
    ConnectionDetailsRole = Qt::UserRole + 1,
    ConnectionIconRole,
    ConnectionPathRole,
    ConnectionStateRole,
    DeviceName,
    DevicePathRole,
    DeviceStateRole,
    DuplicateRole,
    HeaderRole,
    ItemUniqueNameRole,
    ItemTypeRole,
    LastUsedRole,
    LastUsedDateOnlyRole,
    NameRole,
    NetworkItemRole,
    SecurityTypeRole,
    SecurityTypeStringRole,
    SectionRole,
    SignalRole,
    SlaveRole,
    SsidRole,
    SpecificPathRole,
    TimeStampRole,
    TypeRole,
    UniRole,
    UuidRole,
};

enum WirelessProperties {
    RemoveUnavailableNetworks,
    DisplayNetworkIcons,
};

#endif // NETWORK_ENUMS_H
