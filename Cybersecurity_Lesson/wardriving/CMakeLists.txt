set(WAR_DRIVING_SRCS
        wardriving.cpp
        wardriving.h
        wardriving.ui
)

add_library(WarDriving STATIC ${WAR_DRIVING_SRCS})

target_link_libraries(WarDriving PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(WarDriving PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(WarDriving PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(WarDriving PRIVATE NetworkManagerQt)
target_link_libraries(WarDriving PRIVATE Network_Model)

include_directories(${CMAKE_SOURCE_DIR}/models)
include_directories(${CMAKE_SOURCE_DIR}/styles)
