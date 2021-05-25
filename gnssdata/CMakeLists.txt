# This builds a sub-library separate from gpstk in order to avoid
# loading a bunch of data into memory that isn't being used (currently
# only the Galileo ionospheric model NeQuick G data).

cmake_minimum_required(VERSION 2.8.5)

project(gnssdata DESCRIPTION "Delay loading of gpstk data tables")

add_library(gnssdata SHARED
  modip.cpp
  )
set_target_properties(gnssdata PROPERTIES VERSION "${GPSTK_VERSION_MAJOR}.${GPSTK_VERSION_MINOR}.${GPSTK_VERSION_PATCH}"
                                          SOVERSION "${GPSTK_VERSION_MAJOR}.${GPSTK_VERSION_MINOR}")

install( TARGETS gnssdata DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT "${EXPORT_TARGETS_FILENAME}" )
