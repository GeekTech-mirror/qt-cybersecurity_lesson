set(IFACE_MODEL_SRCS
    iface_model.cpp
    iface_model.h
)

add_library(Iface_Model STATIC ${IFACE_MODEL_SRCS})

target_link_libraries(Iface_Model PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Iface_Model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Iface_Model PRIVATE NetworkManagerQt)

include_directories(${CMAKE_SOURCE_DIR}/styles)
