set(WIFI_DEAUTHER_SRCS
    deauther.cpp
    deauther.h
    deauther.ui
)

add_library(WiFi_Deauther STATIC ${WIFI_DEAUTHER_SRCS})

target_link_libraries(WiFi_Deauther PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WiFi_Deauther PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(WiFi_Deauther PRIVATE NetworkManagerQt)
#target_link_libraries(WiFi_Deauther PRIVATE PolkitQt${QT_VERSION_MAJOR}-1::Core
#                                            PolkitQt${QT_VERSION_MAJOR}-1::Gui
#                                            PolkitQt${QT_VERSION_MAJOR}-1::Agent)
target_link_libraries(WiFi_Deauther PRIVATE pcap)

target_link_libraries(WiFi_Deauther PRIVATE Iface_Model)
target_link_libraries(WiFi_Deauther PRIVATE Network_Model)
target_link_libraries(WiFi_Deauther PRIVATE Station_Model)

include_directories(${CMAKE_SOURCE_DIR}/models/iface_model)
include_directories(${CMAKE_SOURCE_DIR}/models/network_model)
include_directories(${CMAKE_SOURCE_DIR}/models/station_model)
include_directories(${CMAKE_SOURCE_DIR}/styles)
