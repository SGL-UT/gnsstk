#----------------------------------------
# gnsstk/swig/CMakeLists.txt
#----------------------------------------

cmake_minimum_required( VERSION 2.8.4  )
include( make_copy_targets.cmake )

include( PythonSetup.cmake )

find_package( SWIG REQUIRED )
include( ${SWIG_USE_FILE} )

# These warnings have been restored by commenting out the SWIG flags
# as it's better to resolve the warnings using %ignore, %rename or
# some other SWIG directive.
#----------------------------------------
# Compiler warning disables
#----------------------------------------
#     362: operator= ignored
#     383: operator++ ignored
#     384: operator-- ignored
#     389: operator[] ignored
#     503: can't wrap 'identifer' unless renamed to a valid identifier (e.g. operator<<)
#     325: Nested class not currently supported

#set( CMAKE_SWIG_FLAGS "-w362,383,384,389,503,325;-builtin;-debug-classes;-debug-symtabs;-debug-symbols;-debug-csymbols;-debug-lsymbols;-debug-tags;-debug-template;-debug-typedef;-debug-tmsearch;-debug-tmused;-MF;swigdeps.txt" )

#----------------------------------------
# Don't set the RPATH for dependencies.
#----------------------------------------
set( CMAKE_SKIP_RPATH TRUE )

#----------------------------------------
# Compiler flags that are used only
# in the compiling of the swig bindings
# @todo: see if -std=c++11 can be removed from all builds
#----------------------------------------
# -Wno-write-strings disables warnings that g++ generates by the
# truckload because the Python C API uses char* instead of
# const char*
if( ${CMAKE_SYSTEM_NAME} MATCHES "SunOS" )
    set( CMAKE_CXX_FLAGS "-std=c++11" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++11" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( CMAKE_CXX_FLAGS "-std=c++11 -Wno-write-strings" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( CMAKE_CXX_FLAGS "-std=c++11" )
endif()


set_property(SOURCE gnsstk.i PROPERTY CPLUSPLUS ON )
set_property(SOURCE gnsstk.i PROPERTY USE_SWIG_DEPENDENCIES TRUE)

if( DEBUG_SWITCH )
    message( STATUS "SWIG_EXECUTABLE          = ${SWIG_EXECUTABLE}" )
    message( STATUS "SWIG_USE_FILE            = ${SWIG_USE_FILE}" )
    message( STATUS "SWIG_DOC_DIR             = ${SWIG_DOC_DIR}")
    message( STATUS "PYTHON_INSTALL_PREFIX    = ${PYTHON_INSTALL_PREFIX}")
    message( STATUS "PROJECT_BINARY_DIR       = ${PROJECT_BINARY_DIR}/libgnsstk.so.${GNSSTK_VERSION_MAJOR}.${GNSSTK_VERSION_MINOR}.${GNSSTK_VERSION_PATCH}")
endif()

#----------------------------------------
# Swig module build target
#----------------------------------------
# Note: the cmake swig module doesn't seem to know
#       how to track dependancy on other .i files...
#       so if the other .i files are changed,
#       you must rerun cmake, not just make.
#
# Note: list of all headers files are included in
#       gnsstk_swig.hpp which is included in gnsstk.i
set( SWIG_MODULE "gnsstk" )

include_directories( ${PYTHON_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FileHandling )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/Binex )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/RINEX )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/RINEX3 )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Geomatics )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/GNSSCore )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/GNSSEph )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Math )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/NavFilter )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/NewNav )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/SWIGHelpers )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/TimeHandling )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Utilities )

swig_add_module( ${SWIG_MODULE} python gnsstk.i )
swig_link_libraries( ${SWIG_MODULE} gnsstk )
#swig_link_libraries( ${SWIG_MODULE} ${PYTHON_LIBRARIES} )

# Install the gnsstk module into the package file tree
set( MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR}/module )
set( CMAKE_SWIG_OUTDIR ${MODULE_PATH}/${SWIG_MODULE} )


#----------------------------------------
# Add custom commands to the _gnsstk.so build target
#----------------------------------------
#    ...to relocate the output from teh swig build target to
#    the correct directory within the python package file tree
#        CMake documentation claims that all SWIG target output
#        will be written to $CMAKE_SWIG_OUTDIR, but
#        testing shows that this ONLY works for the gnsstk.py
#        and does NOT work for the _gnsstk.so
#----------------------------------------
add_custom_target(swig-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MODULE_PATH}/gnsstk)
add_dependencies("_${SWIG_MODULE}" swig-make-directory)
add_custom_command( TARGET "_${SWIG_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "_${SWIG_MODULE}.so" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${SWIG_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${SWIG_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${SWIG_MODULE}"
                    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/libgnsstk.so.${GNSSTK_VERSION}" ${CMAKE_SWIG_OUTDIR}
                    DEPENDS gnsstk)

# Find all files in the gnsstk, apps, and tests directories and set up dependancies to
# copy them to the python module directory
file(GLOB_RECURSE module_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/gnsstk/*.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/*.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.py"
  )
make_copy_targets(module_files ${CMAKE_CURRENT_SOURCE_DIR} ${MODULE_PATH})

# Do the same for the boilerplate files
set(meta_files "" )
list(APPEND meta_files AUTHORS.md COPYING.md COPYING.LESSER.md ChangeLog.md INSTALL.md README.md )
make_copy_targets(meta_files ${CMAKE_SOURCE_DIR} ${MODULE_PATH})


#--------------------------------------------
# Initialize a working space to build the doxygen/sphinx docs
#--------------------------------------------

set( SWIG_DOC_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc )
include_directories( ${SWIG_DOC_DIR} )
file( MAKE_DIRECTORY ${SWIG_DOC_DIR} )
file( WRITE ${SWIG_DOC_DIR}/doc.i "// dummy file. Is replaced when docs are generated for bindings." )

# Provide ability to use CMake variables to define some values within the setup.py script
set( SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" )
set( SETUP_PY    "${MODULE_PATH}/setup.py" )
configure_file( ${SETUP_PY_IN} ${SETUP_PY} )

set( GNSSTK_ROOT_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/gnsstk/__init__.py.in" )
set( GNSSTK_ROOT_PY    "${MODULE_PATH}/gnsstk/__init__.py" )
configure_file( ${GNSSTK_ROOT_PY_IN} ${GNSSTK_ROOT_PY} )


# Note: For setup.py install options, see https://docs.python.org/2/install/
if( BUILD_FOR_PACKAGE_SWITCH )
    # If building for a package, use the "root" switch to imitate the system package path
    install( DIRECTORY DESTINATION ${PYTHON_INSTALL_PREFIX}/lib/python2.7/site-packages )
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --root=${PYTHON_INSTALL_PREFIX} WORKING_DIRECTORY ${MODULE_PATH})" )
elseif( PYTHON_USER_INSTALL )
    # If performing a user-install, use the `--user` flag, otherwise it may not create the correct directores.
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --user WORKING_DIRECTORY ${MODULE_PATH})" )
else()
    install( DIRECTORY DESTINATION ${PYTHON_INSTALL_PREFIX}/lib/python2.7/site-packages )
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --prefix=${PYTHON_INSTALL_PREFIX} WORKING_DIRECTORY ${MODULE_PATH})" )
endif()

if( PIP_WHEEL_SWITCH )
    add_custom_target(swig-make-wheel ALL
            COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} bdist_wheel
            WORKING_DIRECTORY ${MODULE_PATH}
            DEPENDS "_${SWIG_MODULE}")
endif()


#------------------------------------------------------------
# These files aren't required to use the bindings but are
# required to make a swig module that depends upon these bindings
#------------------------------------------------------------

file( GLOB_RECURSE swig_includes
  ${CMAKE_CURRENT_SOURCE_DIR}/*.i )
install( FILES ${swig_includes} ${CMAKE_CURRENT_SOURCE_DIR}/gnsstk_swig.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" )


add_subdirectory(tests)
