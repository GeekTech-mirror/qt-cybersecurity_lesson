include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/settings
)

include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED Test)
set_package_properties(Qt${QT_MAJOR_VERSION}Test PROPERTIES PURPOSE "Required for autotests")

ecm_add_test(managertest.cpp LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test NetworkManagerQt_static PkgConfig::NetworkManager fakeNetwork)
ecm_add_test(settingstest.cpp LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test NetworkManagerQt_static PkgConfig::NetworkManager fakeNetwork)
ecm_add_test(activeconnectiontest.cpp LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test NetworkManagerQt_static PkgConfig::NetworkManager fakeNetwork)

add_subdirectory(settings)
