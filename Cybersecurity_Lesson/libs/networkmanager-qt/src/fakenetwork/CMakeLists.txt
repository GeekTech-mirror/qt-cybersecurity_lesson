include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../settings
)

set(fakeNetwork_SRCS
    accesspoint.cpp
    activeconnection.cpp
    connection.cpp
    device.cpp
    fakenetwork.cpp
    settings.cpp
    wireddevice.cpp
    wirelessdevice.cpp
)

add_library(fakeNetwork STATIC ${fakeNetwork_SRCS})

target_link_libraries(fakeNetwork
    NetworkManagerQt_static
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Network
    Qt${QT_MAJOR_VERSION}::DBus
    PkgConfig::NetworkManager)
