#============================================================
#
# Name    = PythonSetup.cmake
# Purpose = Determine locations of Python library for use by SWIG
# Usage   = add "include( PythonSetup.cmake )" to the appropriate CMakeLists.txt
#
# Scheme  = Top-Level CMakeLists.txt includes this present file
#           First step herein is to look for a CustomPythonSetup.cmake
#           If that fails, this file then tries the "normal" method to find PythonLibs
#           If CMake cannot figure out which Python library to use by that method
#           then modify the template CustomPythonSetup.cmake file to explicitly
#           define the paths to the python library and headers you wish SWIG to use.
#
# Reason  = On systems where the user may have many installations
#           of python, e.g. RedHat or OSX where the system version of Python
#           is an old version not really intended for daily user use, so the user often
#           installs an additional python environment from source or with some package manager.
#           CMake find_package( PythonLibs ) will often stumble on pieces of multiple Python installs
#           in an order that results in mismatching version numbers for the python library
#           and the python include files, and thus cannot build the
#           typemaps for wrapping the C++ code.
#
#============================================================


#------------------------------------------------------------
# If the user provides a custom Python configuration, use it
#------------------------------------------------------------

include( CustomPythonSetup.cmake
         OPTIONAL
         RESULT_VARIABLE PYTHON_CUSTOM_CONFIG )

#------------------------------------------------------------
# If a user-specified python configuration is not found, let CMake try to find the system python
#------------------------------------------------------------
if( ${PYTHON_CUSTOM_CONFIG} MATCHES "NOTFOUND" )
  find_package( PythonInterp )

  # It looks like the find for PythonLibs gets the 'first' Python.h it can find,
  # which does not necessiarly match what the executable found by PythonInterp
  # will be copacetic with. So, we set CMAKE_INCLUDE_PATH to what is returned
  # by the found python-config.

  # For python2, we only need to se tthe include path specifically.
  # Python3 requires more paths from python-config.
  if( ${PYTHON_VERSION_MAJOR} EQUAL 3 )

    # Find the python-config file.  First check for one next to the python
    # interpreter, then look anywhere in the system path.
    get_filename_component(PYTHON_BIN_BASE ${PYTHON_EXECUTABLE} DIRECTORY )

    find_program(PYTHON_CONFIG_EXE "python3-config" PATHS "${PYTHON_BIN_BASE}" NO_DEFAULT_PATH )
    find_program(PYTHON_CONFIG_EXE "python3-config" )
    # If we can't find the python3 specific one, try looking for a version without the three.
    find_program(PYTHON_CONFIG_EXE "python-config" PATHS "${PYTHON_BIN_BASE}" NO_DEFAULT_PATH )
    find_program(PYTHON_CONFIG_EXE "python-config" )
    if(NOT PYTHON_CONFIG_EXE)
      message( FATAL_ERROR "Cannot find any python3-config or python-config. Cannot proceed. Exiting now!" )
      return()
    endif()
    message( STATUS "PYTHON_CONFIG_EXE        = ${PYTHON_CONFIG_EXE}." )

    execute_process( COMMAND "${PYTHON_CONFIG_EXE}" "--includes" OUTPUT_VARIABLE PYTHON_INCLUDES)
    execute_process( COMMAND "${PYTHON_CONFIG_EXE}" "--prefix" OUTPUT_VARIABLE PYTHON_PREFIX)
    execute_process( COMMAND "${PYTHON_CONFIG_EXE}" "--ldflags" OUTPUT_VARIABLE PYTHON_LDFLAGS)

    # String parsing to get the include path
    string(REGEX MATCH "-I(.*) " _python_include ${PYTHON_INCLUDES})
    set(_python_include ${CMAKE_MATCH_1})
    set(CMAKE_INCLUDE_PATH ${_python_include})

    # String parsing to get the library path and libarary name
    string(REGEX MATCH "-L([^ ]*) -l([^ ]*) " _python_libdir ${PYTHON_LDFLAGS})
    set(_python_libdir ${CMAKE_MATCH_1})
    set(_python_libname ${CMAKE_MATCH_2})

    # Python 3 isn't well supported for earlier versions of CMAKE.  So we roll our own.
    string(STRIP ${PYTHON_PREFIX} PYTHON_PREFIX)
    set(PYTHON_LIBRARIES "${_python_libdir}/lib${_python_libname}.so")
    set(PYTHON_INCLUDE_DIR ${_python_include})
    set(PYTHON_INCLUDE_DIRS ${_python_include})
    set(PYTHONLIBS_VERSION_STRING ${PYTHON_VERSION_STRING})
    set(PYTHONLIBS_FOUND TRUE)

  else()
    execute_process( COMMAND "${PYTHON_EXECUTABLE}-config" "--includes" OUTPUT_VARIABLE PYTHON_INCLUDES)
    string(REGEX MATCH "-I(.*) " _python_include ${PYTHON_INCLUDES})
    set(_python_include ${CMAKE_MATCH_1})
    set(CMAKE_INCLUDE_PATH ${_python_include})

    find_package( PythonLibs ${PYTHON_VERSION_STRING} REQUIRED )
  endif()

endif()


#------------------------------------------------------------
# Debug messaging
#------------------------------------------------------------
if( DEBUG_SWITCH OR NOT PYTHONLIBS_FOUND)
  message( STATUS "PYTHON_BIN_BASE          = ${PYTHON_BIN_BASE}" )
  message( STATUS "PYTHONINTERP_FOUND        = ${PYTHONINTERP_FOUND}" )
  message( STATUS "PYTHON_EXECUTABLE         = ${PYTHON_EXECUTABLE}" )
  message( STATUS "PYTHON_VERSION_STRING     = ${PYTHON_VERSION_STRING}" )
  message( STATUS "PYTHONLIBS_FOUND          = ${PYTHONLIBS_FOUND}" )
  message( STATUS "PYTHON_LIBRARIES          = ${PYTHON_LIBRARIES}" )
  message( STATUS "PYTHON_INCLUDE_DIR        = ${PYTHON_INCLUDE_DIRS}" )
  message( STATUS "PYTHON_INCLUDE_DIRS       = ${PYTHON_INCLUDE_DIRS}" )
  message( STATUS "PYTHONLIBS_VERSION_STRING = ${PYTHONLIBS_VERSION_STRING}" )
  message( STATUS "PYTHON_INSTALL_PREFIX     = ${PYTHON_INSTALL_PREFIX}" )
endif()

#------------------------------------------------------------
# Consistent python library and headers could not be found
#------------------------------------------------------------
if( NOT PYTHONLIBS_FOUND )
  message( STATUS "Cannot find requested version of PYTHONLIBS on your system." )
  message( STATUS "Cannot build swig bindings without the right python libraries." )
  message( STATUS "PYTHON_LIBRARY and PYTHON_INCLUDE_DIR versions must match PYTHON_EXECUTABLE." )
  message( FATAL_ERROR "Cannot find PYTHONLIBS. Cannot proceed. Exiting now!" )
  return()
endif()
