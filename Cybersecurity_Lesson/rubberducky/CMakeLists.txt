set(RUBBER_DUCKY_SRCS
        rubberducky.cpp
        rubberducky.h
        rubberducky.ui
)

add_library(Rubber_Ducky STATIC ${RUBBER_DUCKY_SRCS})

target_link_libraries(Rubber_Ducky PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Rubber_Ducky PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Rubber_Ducky PRIVATE NetworkManagerQt)

include_directories(${CMAKE_SOURCE_DIR}/styles)
