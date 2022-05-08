#ifndef NETWORK_ENUMS_H
#define NETWORK_ENUMS_H

#include <QObject>

QT_BEGIN_NAMESPACE

using namespace Qt;

enum HandlerAction {
    ActivateConnection,
    AddAndActivateConnection,
    AddConnection,
    DeactivateConnection,
    RemoveConnection,
    RequestScan,
    UpdateConnection,
};
Q_ENUM_NS(HandlerAction)

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
Q_ENUM_NS(ItemRole)

QT_END_NAMESPACE

#endif // NETWORK_ENUMS_H
