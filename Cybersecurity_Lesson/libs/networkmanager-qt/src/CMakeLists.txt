# add_subdirectory(dbus)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_SOURCE_DIR}/settings
)

set(NetworkManagerQt_PART_SRCS
    device.cpp
    accesspoint.cpp
    activeconnection.cpp
    bridgedevice.cpp
    connection.cpp
    dhcp4config.cpp
    dhcp6config.cpp
    devicestatistics.cpp
    dnsconfiguration.cpp
    dnsdomain.cpp
    ipaddress.cpp
    iproute.cpp
    ipconfig.cpp
    manager.cpp
    secretagent.cpp
    settings.cpp
    utils.cpp
    wireddevice.cpp
    wirelessdevice.cpp
    wirelessnetwork.cpp
    generictypes.cpp
    genericdevice.cpp
    vethdevice.cpp
)

set(NetworkManagerQt_SETTINGS_SRCS
    settings/bridgesetting.cpp
    settings/bridgeportsetting.cpp
    settings/connectionsettings.cpp
    settings/ipv4setting.cpp
    settings/ipv6setting.cpp
    settings/macsecsetting.cpp
    settings/matchsetting.cpp
    settings/setting.cpp
    settings/security8021xsetting.cpp
    #settings/template.cpp
    settings/wiredsetting.cpp
    settings/wirelesssetting.cpp
    settings/wirelesssecuritysetting.cpp
    settings/tcsetting.cpp
    settings/genericsetting.cpp
    settings/usersetting.cpp
)

set(DBUS_INTERFACE_SRCS
    dbus/accesspointinterface.cpp
    dbus/activeconnectioninterface.cpp
    dbus/agentmanagerinterface.cpp
    dbus/bridgedeviceinterface.cpp
    dbus/connectioninterface.cpp
    dbus/deviceinterface.cpp
    dbus/devicestatisticsinterface.cpp
    dbus/dhcp4configinterface.cpp
    dbus/dhcp6configinterface.cpp
    dbus/dnsmanagerinterface.cpp
    dbus/genericdeviceinterface.cpp
    dbus/checkpointinterface.cpp
    dbus/ip4configinterface.cpp
    dbus/ip6configinterface.cpp
    dbus/macsecdeviceinterface.cpp
    dbus/networkmanagerinterface.cpp
    dbus/secretagentadaptor.cpp
    dbus/settingsinterface.cpp
    dbus/vethdeviceinterface.cpp
    dbus/wireddeviceinterface.cpp
    dbus/wirelessdeviceinterface.cpp
)

ecm_qt_export_logging_category(
    IDENTIFIER NMQT
    CATEGORY_NAME kf.networkmanagerqt
    OLD_CATEGORY_NAMES networkmanager-qt
    DEFAULT_SEVERITY Warning
    DESCRIPTION "NetworkManagerQt"
    EXPORT NETWORMANAGERQT
)

add_library(NetworkManagerQt SHARED ${NetworkManagerQt_PART_SRCS} ${NetworkManagerQt_SETTINGS_SRCS} ${DBUS_INTERFACE_SRCS})
generate_export_header(NetworkManagerQt EXPORT_FILE_NAME ${NetworkManagerQt_BINARY_DIR}/networkmanagerqt/networkmanagerqt_export.h BASE_NAME NetworkManagerQt)

target_include_directories(NetworkManagerQt INTERFACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt>")
# for examples to build
target_include_directories(NetworkManagerQt PUBLIC "$<BUILD_INTERFACE:${NetworkManagerQt_BINARY_DIR};${CMAKE_CURRENT_SOURCE_DIR}/settings;${CMAKE_CURRENT_BINARY_DIR}>")

target_link_libraries(NetworkManagerQt PUBLIC Qt${QT_MAJOR_VERSION}::Core
                                              Qt${QT_MAJOR_VERSION}::Network
                                              Qt${QT_MAJOR_VERSION}::DBus
                                              PkgConfig::NetworkManager PkgConfig::Gio)

set_target_properties(NetworkManagerQt PROPERTIES VERSION ${NETWORKMANAGERQT_VERSION}
                                                   SOVERSION ${NETWORKMANAGERQT_SOVERSION}
                                                   EXPORT_NAME NetworkManagerQt
)

########### static lib for tests  ###############
add_library(NetworkManagerQt_static STATIC ${NetworkManagerQt_PART_SRCS} ${NetworkManagerQt_SETTINGS_SRCS} ${DBUS_INTERFACE_SRCS})
set_target_properties(NetworkManagerQt_static PROPERTIES COMPILE_FLAGS -DNMQT_STATIC=1)

target_link_libraries(NetworkManagerQt_static PUBLIC Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Network Qt${QT_MAJOR_VERSION}::DBus PkgConfig::NetworkManager PkgConfig::Gio)
target_include_directories(NetworkManagerQt_static PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..;${CMAKE_CURRENT_BINARY_DIR}/..>")

ecm_generate_headers(NetworkManagerQt_CamelCase_HEADERS
  HEADER_NAMES
  AccessPoint
  ActiveConnection
  BridgeDevice
  Connection
  Device
  DeviceStatistics
  Dhcp4Config
  Dhcp6Config
  DnsConfiguration
  DnsDomain
  GenericDevice
  GenericTypes
  IpAddress
  IpConfig
  IpRoute
  Manager
  SecretAgent
  Settings
  Utils
  VethDevice
  WiredDevice
  WirelessDevice
  WirelessNetwork

  REQUIRED_HEADERS NetworkManagerQt_HEADERS
  PREFIX NetworkManagerQt
)

ecm_generate_headers(NetworkManagerQt_SETTINGS_CamelCase_HEADERS
  HEADER_NAMES
  BridgeSetting
  BridgePortSetting
  ConnectionSettings
  GenericSetting
  Ipv4Setting
  Ipv6Setting
  Security8021xSetting
  Setting
  WiredSetting
  WirelessSecuritySetting
  WirelessSetting

  RELATIVE settings
  REQUIRED_HEADERS NetworkManagerQt_SETTINGS_HEADERS
  PREFIX NetworkManagerQt
)

install(TARGETS NetworkManagerQt EXPORT NetworkManagerQtTargets)

install(FILES
  ${NetworkManagerQt_CamelCase_HEADERS}
  ${NetworkManagerQt_SETTINGS_CamelCase_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt/NetworkManagerQt COMPONENT Devel
)

install(FILES
  ${NetworkManagerQt_BINARY_DIR}/networkmanagerqt/networkmanagerqt_export.h
  ${NetworkManagerQt_HEADERS}
  ${NetworkManagerQt_SETTINGS_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt/networkmanagerqt COMPONENT Devel
)

ecm_qt_install_logging_categories(
    EXPORT NETWORMANAGERQT
    FILE networkmanagerqt.categories
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/qlogging-categories6
)

if(BUILD_QCH)
    ecm_add_qch(
        NetworkManagerQt_QCH
        NAME NetworkManagerQt
        BASE_NAME NetworkManagerQt
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${NetworkManagerQt_HEADERS}
            ${NetworkManagerQt_SETTINGS_HEADERS}
        MD_MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/../README.md"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Network_QCH
            Qt6DBus_QCH
        BLANK_MACROS
            NETWORKMANAGERQT_EXPORT
            NETWORKMANAGERQT_DEPRECATED
            NETWORKMANAGERQT_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/qch
        QCH_INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/qch
        COMPONENT Devel
    )
endif()
