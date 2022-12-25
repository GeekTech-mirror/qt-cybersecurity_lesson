# Debugging
include(CMakePrintHelpers)

set(RUBBER_DUCKY_SRCS
    languages.h
    rubberducky.cpp
    rubberducky-commands.cpp
    rubberducky-encoder.cpp
    rubberducky.h
    rubberducky.ui
)

set(LANGUAGE_SRCS
    languages/gb.h
)

add_library(Language_Libs STATIC ${LANGUAGE_SRCS})
target_link_libraries(Language_Libs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_library(Rubber_Ducky STATIC ${RUBBER_DUCKY_SRCS})
target_link_libraries(Rubber_Ducky PRIVATE Language_Libs)

target_link_libraries(Rubber_Ducky PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Rubber_Ducky PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Rubber_Ducky PRIVATE NetworkManagerQt)
target_link_libraries(Rubber_Ducky PRIVATE KF5Solid)

include_directories(${CMAKE_SOURCE_DIR}/styles)


# install example payload
configure_file(payload_example.txt payload_example.txt COPYONLY)
install (FILES payload_example.txt DESTINATION ${CMAKE_INSTALL_DATADIR}/cybersecurity_lesson)

# install pico setup files
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/pico_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/pico_files
        ${CMAKE_CURRENT_BINARY_DIR}/pico_files
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pico_files
)

add_custom_target(copy_directory ALL DEPENDS ${CMAKE_BINARY_DIR}/pico_files)

install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pico_files
         DESTINATION ${CMAKE_INSTALL_DATADIR}/cybersecurity_lesson)

# add pico setup files to clean command
set_property(DIRECTORY PROPERTY
    ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/pico_files
    TARGET Rubber_ducky
)

# add search path for pico setup files
set(CMAKE_DATADIR ${CMAKE_INSTALL_DATADIR}/cybersecurity_lesson)
add_definitions(-DCMAKE_DATADIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_DATADIR}")


#install(TARGETS JSON_Libs
#        LIBRARY DESTINATION lib)
