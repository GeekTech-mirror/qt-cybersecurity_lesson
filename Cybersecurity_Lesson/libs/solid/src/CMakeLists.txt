# Debugging
include(CMakePrintHelpers)

add_subdirectory(solid)
add_subdirectory(tools)
add_subdirectory(imports)

set(CMAKE_INSTALL_LOGGINGCATEGORIESDIR share/qlogging-categories6)
ecm_qt_install_logging_categories(
    EXPORT SOLID
    FILE solid.categories
    DESTINATION ${CMAKE_INSTALL_LOGGINGCATEGORIESDIR}
)
