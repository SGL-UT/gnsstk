#============================================================
# Name    = $GNSSTK/CMakeLists.txt
# Purpose = Generator of build framework (e.g. Makefiles) for GNSSTk
# Notes   = This is the top-level CMake input file
#           Depends on $GNSSTK/BuildSetup.cmake
#           Is dependend on by $GNSSTK/build.sh
#============================================================

cmake_minimum_required( VERSION 3.7.2 )

project( GNSSTK )
set( GNSSTK_VERSION_MAJOR "14" )
set( GNSSTK_VERSION_MINOR "3" )
set( GNSSTK_VERSION_PATCH "0" )
set( GNSSTK_VERSION "${GNSSTK_VERSION_MAJOR}.${GNSSTK_VERSION_MINOR}.${GNSSTK_VERSION_PATCH}" )

# Test data file source directory
set( GNSSTK_TEST_DATA_DIR ${PROJECT_SOURCE_DIR}/data )
# Test output directory
set( GNSSTK_TEST_OUTPUT_DIR ${PROJECT_BINARY_DIR}/Testing/Temporary )

# This sets up variables contining GNU standard installation locations.
include( GNUInstallDirs )

# Set a filename for collecting exported targets.
set( EXPORT_TARGETS_FILENAME "GNSSTKTargets" )

if ( WIN32 )
   # Make cmake do most of the work of creating the DLL exports
   set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
   # put everything in the same directory to make it easier for tests
   # to find the DLL.
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else ( WIN32 )
   # This disables the Windows-specific export things for non-Windows platforms.
   add_definitions(-DGNSSTK_STATIC_DEFINE)
endif ( WIN32 )
include(GenerateExportHeader)


option( DEBUG_SWITCH "HELP: DEBUG_SWITCH: Default = OFF, print some CMake variable values to stdout." OFF )
option( DEBUG_VERBOSE "HELP: DEBUG_VERBOSE: Default = OFF, print all CMake variable values." OFF )
option( BUILD_EXT "HELP: BUILD_EXT: SWITCH, Default = OFF, Build the ext library, in addition to the core library." OFF )
option( TEST_SWITCH "HELP: TEST_SWITCH: SWITCH, Default = OFF, Turn on test mode." OFF )
option( COVERAGE_SWITCH "HELP: COVERAGE_SWITCH: SWITCH, Default = OFF, Turn on coverage instrumentation." OFF )
option( BUILD_PYTHON "HELP: BUILD_PYTHON: SWITCH, Default = OFF, Turn on processing of python extension package." OFF )
option( USE_RPATH "HELP: USE_RPATH: SWITCH, Default= ON, Set RPATH in libraries and binaries." ON )
option( PIP_WHEEL_SWITCH "HELP: PIP_WHEEL_SWITCH: SWITCH, Default= OFF, Build a PIP installable wheel." OFF )
option( BUILD_FOR_PACKAGE_SWITCH "HELP: BUILD_FOR_PACKAGE_SWITCH: SWITCH, Default= OFF, Modify python install paths assuming creation of deb/rpm." OFF )
option( PYTHON_USER_INSTALL "HELP: PYTHON_USER_INSTALL: SWITCH, Default= OFF, Install python in user mode." OFF )
option( VERSIONED_HEADER_INSTALL "HELP: VERSIONED_HEADER_INSTALL: SWITCH, Default= OFF, Install header files into maj/min versioned directory." OFF )

set(PYTHON_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Directory to install SWIG files for Python")

if( PYTHON_USER_INSTALL AND NOT BUILD_PYTHON )
    message( WARNING "Combination of PYTHON_USER_INSTALL=ON and BUILD_PYTHON=OFF is not allowed. " )
    message( WARNING "Setting BUILD_PYTHON = TRUE" )
    set( BUILD_PYTHON TRUE )
endif()

if( PIP_WHEEL_SWITCH AND NOT BUILD_PYTHON )
    message( WARNING "Combination of PIP_WHEEL_SWITCH=ON and BUILD_PYTHON=OFF is not allowed. Wheels depend on python module." )
    message( WARNING "Setting BUILD_PYTHON = TRUE" )
    set( BUILD_PYTHON TRUE )
endif()

if( BUILD_FOR_PACKAGE_SWITCH AND NOT BUILD_PYTHON )
    message( WARNING "Combination of BUILD_FOR_PACKAGE_SWITCH=ON and BUILD_PYTHON=OFF is not allowed. Packages depend on python module." )
    message( WARNING "Setting BUILD_PYTHON = TRUE" )
    set( BUILD_PYTHON TRUE )
endif()

if( BUILD_PYTHON AND NOT BUILD_EXT )
    message( WARNING "Combination of BUILD_PYTHON=ON and BUILD_EXT=OFF is not allowed. Python swig bindings depend on gnsstk/ext." )
    message( WARNING "Setting BUILD_EXT = TRUE" )
    set( BUILD_EXT TRUE )
endif()

# GNSSTk header file install target (whether it is version dependent changes based on user flag)
if( VERSIONED_HEADER_INSTALL )
    set( GNSSTK_INCLUDE_WRAPPER_DIR "gnsstk${GNSSTK_VERSION_MAJOR}" )
    set( GNSSTK_INCLUDE_TGT "${GNSSTK_INCLUDE_WRAPPER_DIR}/gnsstk" )
    set( GNSSTK_INCLUDE_PARENT "${CMAKE_INSTALL_INCLUDEDIR}/${GNSSTK_INCLUDE_WRAPPER_DIR}" )
    set( CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${GNSSTK_INCLUDE_TGT}" )
    message( WARNING "Setting GNSSTK_INCLUDE_TGT_DIR = ${CMAKE_INSTALL_INCLUDEDIR}" )
else()
    set( GNSSTK_INCLUDE_PARENT "${CMAKE_INSTALL_INCLUDEDIR}" )
    set( CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/gnsstk" )
endif()



include( BuildSetup.cmake )

#============================================================
# Core Library Target Files
#============================================================

#----------------------------------------
# Define $GNSSTK/core/ library source files
#----------------------------------------

file( GLOB_RECURSE CORE_SRC_FILES "core/lib/*.cpp" "core/lib/*.c" )
file( GLOB_RECURSE CORE_INC_FILES "core/lib/*.h" "core/lib/*.hpp" )

#----------------------------------------
# Define /core library include directories
#----------------------------------------

# initialize list of include directories
set( CORE_INC_DIRS "" )

foreach( _headerFile ${CORE_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND CORE_INC_DIRS ${_dir} )
endforeach()

list( REMOVE_DUPLICATES CORE_INC_DIRS )

# Add every directory containing a header file
# to the project(gnsstk) include_directories
include_directories( ${CORE_INC_DIRS} )

# define src and include files needed to build library target
set( GNSSTK_SRC_FILES "" )
set( GNSSTK_INC_FILES "" )
list( APPEND GNSSTK_SRC_FILES ${CORE_SRC_FILES} )
list( APPEND GNSSTK_INC_FILES ${CORE_INC_FILES} )

# Remove getopt.h from non-Windows installs
if( NOT WIN32 )
  foreach( _headerFile ${GNSSTK_INC_FILES} )
    get_filename_component( _name ${_headerFile} NAME )
    if( ${_name} MATCHES "getopt.h" )
      list( REMOVE_ITEM GNSSTK_INC_FILES ${_headerFile} )
    endif()
  endforeach()
endif()

#============================================================
# Define $GNSSTK/ext/ additions to Library Target Files
#============================================================

if( BUILD_EXT )
  add_definitions(-DBUILD_EXT)
  file( GLOB_RECURSE EXT_SRC_FILES "ext/lib/*.cpp" "ext/lib/*.c" )
  file( GLOB_RECURSE EXT_INC_FILES "ext/lib/*.h" "ext/lib/*.hpp" )

  # Define ext library include directories
  set( EXT_INC_DIRS "" )

  foreach( _headerFile ${EXT_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    get_filename_component( _name ${_headerFile} NAME )
    list( APPEND EXT_INC_DIRS ${_dir} )
  endforeach()

  list( REMOVE_DUPLICATES EXT_INC_DIRS )

  # Add every directory containing a header file
  # to the project(gnsstk) include_directories
  include_directories( ${EXT_INC_DIRS} )

  # append ext include files needed to build library target
  list( APPEND GNSSTK_SRC_FILES ${EXT_SRC_FILES} )
  list( APPEND GNSSTK_INC_FILES ${EXT_INC_FILES} )

endif()

#============================================================
# GNSSTK Library, Build and Install Targets
#============================================================

if ( ${PROFILER} )
  # don't build the shared library with profiler enabled
  add_library( gnsstk STATIC ${GNSSTK_SRC_FILES} ${GNSSTK_INC_FILES} )
elseif( WIN32 )
  add_library( gnsstk ${GNSSTK_SRC_FILES} ${GNSSTK_INC_FILES} )
else()
  add_library( gnsstk SHARED ${GNSSTK_SRC_FILES} ${GNSSTK_INC_FILES} )
endif()

# always generate the header because it's an include file whose
# absence would break the build on non-windows.
generate_export_header(gnsstk)
include_directories(AFTER SYSTEM ${PROJECT_BINARY_DIR})
install( FILES ${PROJECT_BINARY_DIR}/gnsstk_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

install( TARGETS gnsstk DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT "${EXPORT_TARGETS_FILENAME}" )

# GNSSTk header file install target (whether it is version dependent changes based on user flag)
install( FILES ${GNSSTK_INC_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )


#============================================================
# GNSSTK Library parameters
#============================================================

set_target_properties(gnsstk PROPERTIES VERSION "${GNSSTK_VERSION_MAJOR}.${GNSSTK_VERSION_MINOR}.${GNSSTK_VERSION_PATCH}"
                                       SOVERSION "${GNSSTK_VERSION_MAJOR}")

#============================================================
# Testing
#============================================================

if( TEST_SWITCH )
  enable_testing()
endif( )

#============================================================
# Coverage
#============================================================

if( COVERAGE_SWITCH )
  if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"
    OR ((${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "4.9.0" ) AND CMAKE_COMPILER_IS_GNUCXX))
    message(STATUS "Enabling test coverage (gcov)")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage --coverage" )
  endif()
endif( )


#----------------------------------------
# Add sub-directories
#----------------------------------------

add_subdirectory( core )

# keep cmake quiet
set(PYTHON_VERSION_MAJOR 0)
set(PYTHON_VERSION_MINOR 0)
set(GNSSTK_SWIG_MODULE_DIR "")

if( BUILD_EXT )
   add_subdirectory( examples )
   if( BUILD_PYTHON )
       include( swig/PythonSetup.cmake )
       add_subdirectory( swig )
   endif()
endif()


#----------------------------------------
# Export the project import cmake files.
#----------------------------------------
include( CMakePackageConfigHelpers )
set( INSTALL_CONFIG_DIR ${CMAKE_INSTALL_DATADIR}/cmake/GNSSTK)
configure_package_config_file( ${CMAKE_CURRENT_SOURCE_DIR}/GNSSTKConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTKConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIG_DIR}
    PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_BINDIR CMAKE_INSTALL_PREFIX INSTALL_CONFIG_DIR PYTHON_VERSION_MAJOR PYTHON_VERSION_MINOR BUILD_PYTHON GNSSTK_SWIG_MODULE_DIR )
write_basic_package_version_file( ${CMAKE_CURRENT_BINARY_DIR}/GNSSTKConfigVersion.cmake
    VERSION ${GNSSTK_VERSION}
    COMPATIBILITY SameMajorVersion )
install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTKConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GNSSTKConfigVersion.cmake
    DESTINATION
    ${INSTALL_CONFIG_DIR} )

install( EXPORT ${EXPORT_TARGETS_FILENAME} DESTINATION ${INSTALL_CONFIG_DIR} )
