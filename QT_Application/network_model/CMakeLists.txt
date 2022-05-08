set(NETWORK_MODEL_SOURCES
        network_delegate.cpp
        network_delegate.h
        network_enums.h
        network_item.cpp
        network_item.h
        network_model.cpp
        network_model.h
        network_model_p.h
        network_scan.cpp
        network_scan.h
)

add_library(Network_Model STATIC ${NETWORK_MODEL_SOURCES})

target_link_libraries(Network_Model PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Network_Model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Network_Model PRIVATE NetworkManagerQt)

