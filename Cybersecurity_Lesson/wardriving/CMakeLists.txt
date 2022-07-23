set(WAR_DRIVING_SRCS
        wardriving.cpp
        wardriving.h
        wardriving.ui
)

add_library(War_Driving STATIC ${WAR_DRIVING_SRCS})

target_link_libraries(War_Driving PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(War_Driving PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(War_Driving PRIVATE NetworkManagerQt)
target_link_libraries(War_Driving PRIVATE Network_Model)

include_directories(${CMAKE_SOURCE_DIR}/styles)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network_model)

add_subdirectory(network_model)
