set(RUBBER_DUCKY_SRCS
    languages.h
    rubberducky.cpp
    rubberducky.h
    rubberducky.ui
)

set(LANGUAGE_SRCS
    languages/gb.h
)

add_library(Rubber_Ducky STATIC ${RUBBER_DUCKY_SRCS})
add_library(Language_Libs STATIC ${LANGUAGE_SRCS})
target_link_libraries(Language_Libs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Rubber_Ducky PRIVATE Language_Libs)

target_link_libraries(Rubber_Ducky PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Rubber_Ducky PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Rubber_Ducky PRIVATE NetworkManagerQt)

include_directories(${CMAKE_SOURCE_DIR}/styles)

#install(TARGETS JSON_Libs
#        LIBRARY DESTINATION lib)
