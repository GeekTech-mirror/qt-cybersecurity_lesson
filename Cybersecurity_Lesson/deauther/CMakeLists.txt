set(WIFI_DEAUTHER_SRCS
    deauther.cpp
    deauther.h
    deauther.ui
    iw.h
)

add_library(WiFi_Deauther STATIC ${WIFI_DEAUTHER_SRCS})

target_link_libraries(WiFi_Deauther PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(WiFi_Deauther PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WiFi_Deauther PRIVATE NetworkManagerQt)
target_link_libraries(WiFi_Deauther PRIVATE Network_Model)

include_directories(${CMAKE_SOURCE_DIR}/network_model)
include_directories(${CMAKE_SOURCE_DIR}/styles)


