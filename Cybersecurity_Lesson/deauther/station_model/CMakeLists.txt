set(STATION_MODEL_SRCS
    station_enums.h
    station_model.cpp
    station_model.h
    station_model_p.h
    station_item.cpp
    station_item.h
    ap_item.cpp
    ap_item.h
)

add_library(Station_Model STATIC ${STATION_MODEL_SRCS})

target_link_libraries(Station_Model PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Station_Model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Station_Model PRIVATE NetworkManagerQt)
target_link_libraries(Station_Model PRIVATE Network_Model)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/networkmanager-qt)
include_directories(${CMAKE_SOURCE_DIR}/network_model)
include_directories(${CMAKE_SOURCE_DIR}/styles)
