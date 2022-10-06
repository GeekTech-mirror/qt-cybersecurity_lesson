set(NETWORK_MODEL_SRCS
    network_enums.h
    network_item.cpp
    network_item.h
    network_model.cpp
    network_model.h
    network_model_p.h
    network_scan.cpp
    network_scan.h
    network_sort.cpp
    network_sort.h
)

add_library(Network_Model STATIC ${NETWORK_MODEL_SRCS})

target_link_libraries(Network_Model PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Network_Model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Network_Model PRIVATE NetworkManagerQt)
target_include_directories(Network_Model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/styles)
