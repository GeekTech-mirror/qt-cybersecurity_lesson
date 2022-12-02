set(IFACE_MODEL_SRCS
    iface_model.cpp
    iface_model.h
)

add_library(Iface_Model STATIC ${IFACE_MODEL_SRCS})

target_link_libraries(Iface_Model PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Iface_Model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Iface_Model PRIVATE NetworkManagerQt)
#target_link_libraries(Iface_Model PRIVATE pcap)

install(FILES ${IFACE_MODEL_SRCS} DESTINATION include)
target_include_directories(Iface_Model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/styles)
