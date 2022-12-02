set(STATION_MODEL_SRCS
    debug_packets.h
    pcap_common.h
    station_common.h
    station_model-filter.cpp
    station_model-tree.cpp
    station_model.h
    station_model_p.h
    station_item.cpp
    station_item.h
    oui.txt
)

add_library(Station_Model STATIC ${STATION_MODEL_SRCS})

target_link_libraries(Station_Model PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Station_Model PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Station_Model PRIVATE NetworkManagerQt)
target_link_libraries(Station_Model PRIVATE radiotap)
target_link_libraries(Station_Model PRIVATE pcap)

target_link_libraries(Station_Model PRIVATE Iface_Model)


target_include_directories(Station_Model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#include_directories(${CMAKE_SOURCE_DIR}/models/network_model)
#include_directories(${CMAKE_SOURCE_DIR}/models/iface_model)
include_directories(${CMAKE_SOURCE_DIR}/styles)

#set(LIB_DESTDIR "/usr/lib/cybersecurity_lesson")

#install(FILES
#    "oui.txt"
#    LIBRARY DESTINATION ${LIB_DESTDIR}
#)
