cmake_minimum_required(VERSION 3.5)

project(Cybersecurity_Lesson VERSION 0.4 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS StateMachine)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)


set(CYBERSECURITY_SRCS
        main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        icons.qrc
        fonts.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Cybersecurity_Lesson
        MANUAL_FINALIZATION
        ${CYBERSECURITY_SRCS}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Cybersecurity_Lesson APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Cybersecurity_Lesson SHARED
                    ${CYBERSECURITY_SRCS})
# Define properties for Android with Qt 6 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Cybersecurity_Lesson
            ${CYBERSECURITY_SRCS}
        )
    endif()
endif()

target_link_libraries(Cybersecurity_Lesson PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Cybersecurity_Lesson PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Cybersecurity_Lesson PRIVATE Qt${QT_VERSION_MAJOR}::StateMachine)
#target_link_libraries(Cybersecurity_Lesson PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_link_libraries(Cybersecurity_Lesson PRIVATE NetworkManagerQt)
#target_link_libraries(Cybersecurity_Lesson STATIC libpcap)

target_link_libraries(Cybersecurity_Lesson PRIVATE Iface_Model)
target_link_libraries(Cybersecurity_Lesson PRIVATE Network_Model)
target_link_libraries(Cybersecurity_Lesson PRIVATE Station_Model)
target_link_libraries(Cybersecurity_Lesson PRIVATE Styles)

target_link_libraries(Cybersecurity_Lesson PRIVATE Rubber_Ducky)
target_link_libraries(Cybersecurity_Lesson PRIVATE WarDriving)
target_link_libraries(Cybersecurity_Lesson PRIVATE WiFi_Deauther)


set_target_properties(Cybersecurity_Lesson PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Cybersecurity_Lesson)
endif()

set(INSTALL_DESTDIR "/usr/bin")

install(TARGETS Cybersecurity_Lesson
    RUNTIME DESTINATION "${INSTALL_DESTDIR}"
    BUNDLE DESTINATION "${INSTALL_DESTDIR}"
    LIBRARY DESTINATION "${INSTALL_DESTDIR}"
)

add_subdirectory(networkmanager-qt)

add_subdirectory(models)
add_subdirectory(styles)

add_subdirectory(rubberducky)
add_subdirectory(wardriving)
add_subdirectory(deauther)



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rubberducky)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/wardriving)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deauther)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/styles)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/models)

#add_subdirectory(network_model)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network_model)
