cmake_minimum_required(VERSION 3.5)

project(QT_Application VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        style_custom.cpp
        style_custom.h
        style_custom_p.h
        network_model_item.cpp
        network_model_item.h
        network_model.cpp
        network_model.h
        war_driving.cpp
        war_driving.h
        icons.qrc
        fonts.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QT_Application
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QT_Application APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QT_Application SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 6 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QT_Application
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QT_Application PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(QT_Application PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QT_Application PRIVATE NetworkManagerQt)

set_target_properties(QT_Application PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QT_Application)
endif()

set(INSTALL_DESTDIR "/usr/bin")

install(TARGETS QT_Application
    RUNTIME DESTINATION "${INSTALL_DESTDIR}"
    BUNDLE DESTINATION "${INSTALL_DESTDIR}"
    LIBRARY DESTINATION "${INSTALL_DESTDIR}"
)

add_subdirectory(networkmanager-qt)
