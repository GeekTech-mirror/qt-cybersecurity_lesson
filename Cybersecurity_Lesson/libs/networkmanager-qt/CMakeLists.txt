# Debugging
include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.96.0") # handled by release scripts
project(NetworkManagerQt VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.92.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

find_package(PkgConfig)
pkg_search_module(NetworkManager REQUIRED IMPORTED_TARGET libnm>=1.4.0)
pkg_search_module(Gio REQUIRED IMPORTED_TARGET gio-2.0)


################ CMake settings ####################################

find_package(Qt6 COMPONENTS CoreTools REQUIRED CONFIG)
get_target_property(qtpaths_executable Qt6::qtpaths LOCATION)
execute_process(COMMAND ${qtpaths_executable} --query QT_INSTALL_PREFIX OUTPUT_VARIABLE qt_install_prefix_dir)

set(CMAKE_INSTALL_LIBDIR "lib")

##### RPATH settings #####
if(NOT CMAKE_INSTALL_LIBDIR)
    message(FATAL_ERROR "CMAKE_INSTALL_LIBDIR is not set. Setting one is necessary for using the RPATH settings.")
else()
    set(_abs_LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
endif()

if (NOT IS_ABSOLUTE "${_abs_LIB_INSTALL_DIR}")
    set(_abs_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()


# add our LIB_INSTALL_DIR to the RPATH (but only when it is not one of
# the standard system link directories - such as /usr/lib on UNIX)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${_abs_LIB_INSTALL_DIR}" _isSystemLibDir)
list(FIND CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES      "${_abs_LIB_INSTALL_DIR}" _isSystemCxxLibDir)
list(FIND CMAKE_C_IMPLICIT_LINK_DIRECTORIES        "${_abs_LIB_INSTALL_DIR}" _isSystemCLibDir)
if("${_isSystemLibDir}" STREQUAL "-1"  AND  "${_isSystemCxxLibDir}" STREQUAL "-1"  AND  "${_isSystemCLibDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${_abs_LIB_INSTALL_DIR}")
endif()

# Append directories in the linker search path (but outside the project)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


##### Test settings #####
find_program(APPSTREAMCLI appstreamcli)
function(appstreamtest)
    if(APPSTREAMCLI AND NOT appstreamtest_added)
        set(appstreamtest_added TRUE PARENT_SCOPE)
        add_test(NAME appstreamtest COMMAND ${CMAKE_COMMAND} -DAPPSTREAMCLI=${APPSTREAMCLI} -DINSTALL_FILES=${CMAKE_BINARY_DIR}/install_manifest.txt -P ${CMAKE_CURRENT_LIST_DIR}/appstreamtest.cmake)
    else()
        message(STATUS "Could not set up the appstream test. appstreamcli is missing.")
    endif()
endfunction()

if(NOT SKIP_TEST_SETTINGS)

   # If there is a CTestConfig.cmake, include CTest.
   # Otherwise, there will not be any useful settings, so just
   # fake the functionality we care about from CTest.
   option(BUILD_TESTING "Build the testing tree." ON)
   if(BUILD_TESTING)
     enable_testing()
       appstreamtest()
   endif ()
endif ()


##### Build settings #####
# Always include srcdir and builddir in include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# put the include dirs which are in the source or build tree
# before all other include dirs, so the headers in the sources
# are preferred over the already installed ones
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

# Add the src and build dir to the BUILD_INTERFACE include directories
# of all targets
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# Default to shared libs, if no type is explicitly given to add_library()
set(BUILD_SHARED_LIBS TRUE CACHE BOOL "If enabled, shared libs will be built by default, otherwise static libs")

# By default, don't put a prefix on MODULE targets
set(CMAKE_SHARED_MODULE_PREFIX "")

unset(EXECUTABLE_OUTPUT_PATH)
unset(LIBRARY_OUTPUT_PATH)
unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
unset(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

##### Additional settings #####

find_package(Qt6Core)
set(ENABLE_BSYMBOLICFUNCTIONS ${QT_FEATURE_reduce_relocations})


################ Compiler settings #################################

add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    -DQT_NO_KEYWORDS
    -DQT_NO_FOREACH
)

# Do not merge uninitialized global variables.
# This is mostly a "principle of least surprise" thing, but also
# has performance benefits.
if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_C_COMPILER_ID MATCHES "Clang" OR
        (CMAKE_C_COMPILER_ID STREQUAL "Intel" AND NOT WIN32))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
endif()

# Do not treat the operator name keywords and, bitand, bitor, compl, not, or and xor as synonyms as keywords.
# They're not supported under Visual Studio out of the box thus using them limits the portability of code
if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_C_COMPILER_ID MATCHES "Clang" OR
        (CMAKE_C_COMPILER_ID STREQUAL "Intel" AND NOT WIN32))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-operator-names")
endif()

# Default to hidden visibility for symbols
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
if (POLICY CMP0063)
    # No sane project should be affected by CMP0063, so suppress the warnings
    # generated by the above visibility settings in CMake >= 3.3
    cmake_policy(SET CMP0063 NEW)
endif()

##### Better diagnostics #####

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT APPLE) OR
        (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT APPLE) OR
        (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" AND NOT WIN32))
    # Linker warnings should be treated as errors
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--fatal-warnings ${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings ${CMAKE_MODULE_LINKER_FLAGS}")

    # Do not allow undefined symbols, even in non-symbolic shared libraries
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined ${CMAKE_MODULE_LINKER_FLAGS}")
endif()

include("${ECM_MODULE_DIR}/ECMEnableSanitizers.cmake")
include("${ECM_MODULE_DIR}/ECMCoverageOption.cmake")

####################################################################

find_package(QT NAMES Qt6 REQUIRED Core DBus Network)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f02)

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)


include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)
include(ECMQtDeclareLoggingCategory)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX NETWORKMANAGERQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfigVersion.cmake"
                        SOVERSION 6)

# Enable unit testing
if (BUILD_TESTING)
    add_subdirectory(src/fakenetwork)
    add_subdirectory(autotests)
endif ()

add_subdirectory(src)
##################  examples  ################################
option(BUILD_EXAMPLES "Build example applications" FALSE)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/NetworkManagerQt")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS NetworkManagerQt_QCH
        FILE NetworkManagerQtQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/NetworkManagerQtQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/NetworkManagerQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/NetworkManagerQtConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT NetworkManagerQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE NetworkManagerQtTargets.cmake)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_version.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt COMPONENT Devel
)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
