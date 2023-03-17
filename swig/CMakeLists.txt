#----------------------------------------
# gnsstk/swig/CMakeLists.txt
#----------------------------------------

cmake_minimum_required( VERSION 3.7.2  )
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13.0")
  # UseSWIG target_name_policy
  cmake_policy(SET CMP0078 OLD)
endif()
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14.0")
  # UseSWIG module_name_policy
  cmake_policy(SET CMP0086 NEW)
endif()
include( make_copy_targets.cmake )

find_package( SWIG REQUIRED )
include( ${SWIG_USE_FILE} )

if( NOT DEFINED CPPSTD )
  set( CPPSTD "c++11" )
endif()

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
#----------------------------------------
# -Wno-write-strings disables warnings that g++ generates by the
# truckload because the Python C API uses char* instead of
# const char*
if( ${CMAKE_SYSTEM_NAME} MATCHES "SunOS" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CPPSTD}" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=${CPPSTD}" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CPPSTD} -Wno-write-strings" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj" )
    get_filename_component(PYTHON_LIBRARY_PATH "${PYTHON_LIBRARY}" DIRECTORY)
    link_directories(${PYTHON_LIBRARY_PATH})
endif()

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

include_directories( ${PYTHON_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FileDirProc )
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
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/PosSol )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/SWIGHelpers )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/RefTime )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/TimeHandling )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Utilities )
include_directories(AFTER SYSTEM ${PROJECT_BINARY_DIR})

set(TK_EXTRA_FLAGS -DGNSSTK_EXPORT -DSWIG_TYPE_TABLE=gnsstk)

set( CLOCKMODEL_MODULE "ClockModel" )
set(SWIG_MODULE_${CLOCKMODEL_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(ClockModel/ClockModel.i PROPERTIES CPLUSPLUS ON
                                         USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${CLOCKMODEL_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES ClockModel/ClockModel.i )
swig_link_libraries( ${CLOCKMODEL_MODULE} gnsstk )

set( CODEGEN_MODULE "CodeGen" )
set(SWIG_MODULE_${CODEGEN_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(CodeGen/CodeGen.i PROPERTIES CPLUSPLUS ON
                                         USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${CODEGEN_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES CodeGen/CodeGen.i )
swig_link_libraries( ${CODEGEN_MODULE} gnsstk )

set( FILEDIRPROC_MODULE "FileDirProc" )
set(SWIG_MODULE_${FILEDIRPROC_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(FileDirProc/FileDirProc.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${FILEDIRPROC_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES FileDirProc/FileDirProc.i )
swig_link_libraries( ${FILEDIRPROC_MODULE} gnsstk )

set( FILEHANDLING_MODULE "FileHandling" )
set(SWIG_MODULE_${FILEHANDLING_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(FileHandling/FileHandling.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${FILEHANDLING_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES FileHandling/FileHandling.i )
swig_link_libraries( ${FILEHANDLING_MODULE} gnsstk )

set( GEOMATICS_MODULE "Geomatics" )
set(SWIG_MODULE_${GEOMATICS_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(Geomatics/Geomatics.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${GEOMATICS_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES Geomatics/Geomatics.i )
swig_link_libraries( ${GEOMATICS_MODULE} gnsstk )

set( GNSSCORE_MODULE "GNSSCore" )
set(SWIG_MODULE_${GNSSCORE_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(GNSSCore/GNSSCore.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${GNSSCORE_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES GNSSCore/GNSSCore.i )
swig_link_libraries( ${GNSSCORE_MODULE} gnsstk )

set( GNSSEPH_MODULE "GNSSEph" )
set(SWIG_MODULE_${GNSSEPH_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(GNSSEph/GNSSEph.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${GNSSEPH_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES GNSSEph/GNSSEph.i )
swig_link_libraries( ${GNSSEPH_MODULE} gnsstk )

set( MATH_MODULE "Math" )
set(SWIG_MODULE_${MATH_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(Math/Math.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${MATH_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES Math/Math.i )
swig_link_libraries( ${MATH_MODULE} gnsstk )

set( NAVFILTER_MODULE "NavFilter" )
set(SWIG_MODULE_${NAVFILTER_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(NavFilter/NavFilter.i PROPERTIES CPLUSPLUS ON
                                            USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${NAVFILTER_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES NavFilter/NavFilter.i )
swig_link_libraries( ${NAVFILTER_MODULE} gnsstk )

set( NEWNAV_MODULE "NewNav" )
set(SWIG_MODULE_${NEWNAV_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(NewNav/NewNav.i PROPERTIES CPLUSPLUS ON
                                            USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${NEWNAV_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES NewNav/NewNav.i )
swig_link_libraries( ${NEWNAV_MODULE} gnsstk )

set( ORD_MODULE "ORD" )
set(SWIG_MODULE_${ORD_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(ORD/ORD.i PROPERTIES CPLUSPLUS ON
                                            USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${ORD_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES ORD/ORD.i )
swig_link_libraries( ${ORD_MODULE} gnsstk )

set( POSSOL_MODULE "PosSol" )
set(SWIG_MODULE_${POSSOL_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(PosSol/PosSol.i PROPERTIES CPLUSPLUS ON
                                            USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${POSSOL_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES PosSol/PosSol.i )
swig_link_libraries( ${POSSOL_MODULE} gnsstk )

set( REFTIME_MODULE "RefTime" )
set(SWIG_MODULE_${REFTIME_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(RefTime/RefTime.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${REFTIME_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES RefTime/RefTime.i )
swig_link_libraries( ${REFTIME_MODULE} gnsstk )

set( RXIO_MODULE "Rxio" )
set(SWIG_MODULE_${RXIO_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(Rxio/Rxio.i PROPERTIES CPLUSPLUS ON
                                            USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${RXIO_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES Rxio/Rxio.i )
swig_link_libraries( ${RXIO_MODULE} gnsstk )

set( TIMEHANDLING_MODULE "TimeHandling" )
set(SWIG_MODULE_${TIMEHANDLING_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(TimeHandling/TimeHandling.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${TIMEHANDLING_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES TimeHandling/TimeHandling.i )
swig_link_libraries( ${TIMEHANDLING_MODULE} gnsstk )

set( UTILITIES_MODULE "Utilities" )
set(SWIG_MODULE_${UTILITIES_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(Utilities/Utilities.i PROPERTIES CPLUSPLUS ON
                                                USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${UTILITIES_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES Utilities/Utilities.i )
swig_link_libraries( ${UTILITIES_MODULE} gnsstk )

set( SWIG_MODULE "gnsstk" )
set(SWIG_MODULE_${SWIG_MODULE}_EXTRA_FLAGS ${TK_EXTRA_FLAGS})
set_source_files_properties(gnsstk.i PROPERTIES CPLUSPLUS ON
                                     USE_SWIG_DEPENDENCIES TRUE)
swig_add_library( ${SWIG_MODULE}
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES gnsstk.i )
swig_link_libraries( ${SWIG_MODULE} gnsstk )

# Install the gnsstk module into the package file tree
set( MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR}/module )
set( CMAKE_SWIG_OUTDIR ${MODULE_PATH}/${SWIG_MODULE} )


#----------------------------------------
# Add custom commands to the _gnsstk.so build target
#----------------------------------------
#    ...to relocate the output from the swig build target to
#    the correct directory within the python package file tree
#        CMake documentation claims that all SWIG target output
#        will be written to $CMAKE_SWIG_OUTDIR, but
#        testing shows that this ONLY works for the gnsstk.py
#        and does NOT work for the _gnsstk.so
#----------------------------------------
add_custom_target(swig-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MODULE_PATH}/gnsstk)
add_dependencies("${SWIG_MODULE}" swig-make-directory)

add_custom_command( TARGET "_${CLOCKMODEL_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${CLOCKMODEL_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${CLOCKMODEL_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CLOCKMODEL_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${CODEGEN_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${CODEGEN_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${CODEGEN_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CODEGEN_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${FILEDIRPROC_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${FILEDIRPROC_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${FILEDIRPROC_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${FILEDIRPROC_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${FILEHANDLING_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${FILEHANDLING_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${FILEHANDLING_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${FILEHANDLING_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${GEOMATICS_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${GEOMATICS_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${GEOMATICS_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${GEOMATICS_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${GNSSCORE_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${GNSSCORE_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${GNSSCORE_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${GNSSCORE_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${GNSSEPH_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${GNSSEPH_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${GNSSEPH_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${GNSSEPH_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${MATH_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${MATH_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${MATH_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${MATH_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${NAVFILTER_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${NAVFILTER_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${NAVFILTER_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${NAVFILTER_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${NEWNAV_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${NEWNAV_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${NEWNAV_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${NEWNAV_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${ORD_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${ORD_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${ORD_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${ORD_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${POSSOL_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${POSSOL_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${POSSOL_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${POSSOL_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${REFTIME_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${REFTIME_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${REFTIME_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${REFTIME_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${RXIO_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${RXIO_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${RXIO_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${RXIO_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${TIMEHANDLING_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${TIMEHANDLING_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${TIMEHANDLING_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${TIMEHANDLING_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${UTILITIES_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${UTILITIES_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${UTILITIES_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${UTILITIES_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${SWIG_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_${SWIG_MODULE}>" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${SWIG_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${SWIG_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

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
list(APPEND meta_files AUTHORS.md COPYING.md COPYING.LESSER.md ChangeLog.md INSTALL.md README.md TESTING.md )
make_copy_targets(meta_files ${CMAKE_SOURCE_DIR} ${MODULE_PATH})


#--------------------------------------------
# Initialize a working space to build the doxygen/sphinx docs
#--------------------------------------------

set( SWIG_DOC_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc )
include_directories( ${SWIG_DOC_DIR} )
file( MAKE_DIRECTORY ${SWIG_DOC_DIR} )
# file needs to exist regardless if docstring_generator.py has run or not
if( NOT EXISTS ${SWIG_DOC_DIR}/doc.i )
    file( WRITE ${SWIG_DOC_DIR}/doc.i "// dummy file. Is written if docs are not generated for python bindings.")
endif()

# Provide ability to use CMake variables to define some values within
# the setup.py script
set( SWIG_MODULE_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX} )
# just because, apparently.
if (WIN32)
    set( SWIG_MODULE_SUFFIX ".pyd" )
endif(WIN32)
set( SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" )
set( SETUP_PY    "${MODULE_PATH}/setup.py" )
configure_file( ${SETUP_PY_IN} ${SETUP_PY} )

set( GNSSTK_ROOT_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/gnsstk/__init__.py.in" )
set( GNSSTK_ROOT_PY    "${MODULE_PATH}/gnsstk/__init__.py" )
configure_file( ${GNSSTK_ROOT_PY_IN} ${GNSSTK_ROOT_PY} )

# fix paths for windows
if (WIN32)
    string(REPLACE "/" "\\\\" PYTHON_INSTALL_PREFIX "${PYTHON_INSTALL_PREFIX}")
    string(REPLACE "/" "\\\\" MODULE_PATH "${MODULE_PATH}")
endif()

# Note: For setup.py install options, see https://docs.python.org/2/install/
if( BUILD_FOR_PACKAGE_SWITCH )
    # If building for a package, use the "root" switch to imitate the
    # system package path
    install( DIRECTORY DESTINATION ${PYTHON_INSTALL_PREFIX}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages )
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --root=${PYTHON_INSTALL_PREFIX} WORKING_DIRECTORY ${MODULE_PATH})" )
elseif( PYTHON_USER_INSTALL )
    # If performing a user-install, use the `--user` flag, otherwise
    # it may not create the correct directores.
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --user WORKING_DIRECTORY ${MODULE_PATH})" )
else()
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pip install --prefix=${PYTHON_INSTALL_PREFIX} ${MODULE_PATH} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})" )
endif()

if( PIP_WHEEL_SWITCH )
    add_custom_target(swig-make-wheel ALL
            COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} bdist_wheel
            WORKING_DIRECTORY ${MODULE_PATH}
            DEPENDS "${SWIG_MODULE}")
endif()


#------------------------------------------------------------
# These files aren't required to use the bindings but are
# required to make a swig module that depends upon these bindings
#------------------------------------------------------------

file( GLOB_RECURSE swig_includes
  ${CMAKE_CURRENT_SOURCE_DIR}/*.i )
install( FILES ${swig_includes} ${CMAKE_CURRENT_SOURCE_DIR}/gnsstk_swig.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" )


add_subdirectory(tests)
