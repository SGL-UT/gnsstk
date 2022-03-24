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

  # For python2, we only need to set the include path specifically.
  # Python3 requires more paths from python-config.
  if( WIN32 )
    find_package( PythonLibs ${PYTHON_VERSION_STRING} REQUIRED )
  elseif( ${PYTHON_VERSION_MAJOR} EQUAL 3 )

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
    message( STATUS "PYTHON_CONFIG_EXE        = ${PYTHON_CONFIG_EXE}" )

    execute_process( COMMAND "${PYTHON_CONFIG_EXE}" "--includes" OUTPUT_VARIABLE PYTHON_INCLUDES)
    execute_process( COMMAND "${PYTHON_CONFIG_EXE}" "--prefix" OUTPUT_VARIABLE PYTHON_PREFIX)
    execute_process( COMMAND "${PYTHON_CONFIG_EXE}" "--ldflags" OUTPUT_VARIABLE PYTHON_LDFLAGS)

    # String parsing to get the include path
    # message( DEBUG "PYTHON_INCLUDES          = ${PYTHON_INCLUDES}" )
    string(REGEX MATCH "-I(.*) " _python_include ${PYTHON_INCLUDES})
    set(_python_include ${CMAKE_MATCH_1})
    set(CMAKE_INCLUDE_PATH ${_python_include})

    # String parsing PYTHON_LDFLAGS to get the library path and library name
    # Note: Major assumption here that the first library (-l) is the libpythonxx.so
    # We'll try every directory (-L) to locate it.
    # message( DEBUG "PYTHON_LDFLAGS            = ${PYTHON_LDFLAGS}" )
    string(REGEX MATCH " -l([^ ]*) " _python_libname ${PYTHON_LDFLAGS})
    set(_python_libname ${CMAKE_MATCH_1})
    # message( DEBUG "_python_libname           = ${_python_libname}" )

    # Grab all the "-L" directories, remove the "-L" and trailing spaces,
    # then replace the inner spaces with semicolon to make it a list.
    string(REGEX MATCH "(-L[^ ]* )+" _python_libdirargs ${PYTHON_LDFLAGS})
    # message( DEBUG "_python_libdirargs        = ${_python_libdirargs}" )
    string(REPLACE "-L" ""  _python_libdir ${_python_libdirargs})
    string(STRIP ${_python_libdir} _python_libdir)
    string(REPLACE " " ";"  _python_libdirs ${_python_libdir})
    # message( DEBUG "_python_libdirs           = ${_python_libdirs}" )

    foreach(_py_lib_dir ${_python_libdirs})
      find_file(_python_library "lib${_python_libname}.so" PATHS ${_py_lib_dir} NO_DEFAULT_PATH )
      # message( DEBUG "_python_library           = ${_python_library}" )
    endforeach()

    # Python 3 isn't well supported for earlier versions of CMAKE.  So we roll our own.
    string(STRIP ${PYTHON_PREFIX} PYTHON_PREFIX)
    set(PYTHON_LIBRARIES "${_python_lib_file}")
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

# an OR could conceivably be used here, but I'm setting this to match
# the if/else chain in swig/CMakeLists.txt
set(GNSSTK_SWIG_MODULE_DIR "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages")
if (BUILD_FOR_PACKAGE_SWITCH)
  # nothing extra to do here
elseif (PYTHON_USER_INSTALL)
  # nothing extra to do here
else()
  if (WIN32)
    # Is this a peculiarty of Miniconda3 under Windows?  Seems like it.
    set(GNSSTK_SWIG_MODULE_DIR "lib/site-packages")
  endif()
endif()

#------------------------------------------------------------
# Debug messaging
#------------------------------------------------------------
if( DEBUG_SWITCH OR NOT PYTHONLIBS_FOUND)
  message( STATUS "PYTHON_BIN_BASE           = ${PYTHON_BIN_BASE}" )
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
