cmake_minimum_required(VERSION 3.5)

project(QT_Application VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        style_custom.cpp
        style_custom.h
        style_custom_p.h
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
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QT_Application
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QT_Application PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QT_Application PRIVATE Qt${QT_VERSION_MAJOR}::Network)

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


#
# Define networkmanager-qt module
#
#find_package(ECM 5.92.0  NO_MODULE)
#set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")

#set(CMAKE_MODULE_PATH
#	${CMAKE_MODULE_PATH}
#	${CMAKE_CURRENT_SOURCE_DIR}/cmake
#	${ECM_MODULE_PATH}
#)

#find_package(PkgConfig)
#pkg_search_module(NetworkManager REQUIRED IMPORTED_TARGET libnm>=1.4.0)
#pkg_search_module(Gio REQUIRED IMPORTED_TARGET gio-2.0)

#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f02)

#remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)

#include(GenerateExportHeader)

#include(ECMSetupVersion)
#include(ECMGenerateHeaders)
#include(ECMQtDeclareLoggingCategory)


#ecm_setup_version(PROJECT VARIABLE_PREFIX NETWORKMANAGERQT
#                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_version.h"
#                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5NetworkManagerQtConfigVersion.cmake"
#                        SOVERSION 6)

#add_subdirectory(networkmanager-qt)

# create a Config.cmake and a ConfigVersion.cmake file and install them
#set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/NetworkManagerQt")

#include(CMakePackageConfigHelpers)

#configure_package_config_file(
#	"${CMAKE_CURRENT_SOURCE_DIR}/NetworkManagerQtConfig.cmake.in"
#	"${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfig.cmake"
#        INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
#)

#install(FILES
#	"${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfig.cmake"
#	"${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfigVersion.cmake"
#        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#)

#install(EXPORT
#	NetworkManagerQtTargets
#	DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#	FILE NetworkManagerQtTargets.cmake
#)

#install(FILES
#	${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_version.h
#        DESTINATION ${INCLUDE_INSTALL_DIR}/NetworkManagerQt
#)
