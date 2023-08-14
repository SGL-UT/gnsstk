# Testing the swig/python bindings
#
# Since this requires that there be a valid python runtime, these tests are
# be quite different from the C++ library and application tests

message(STATUS "swig test path ${MODULE_PATH}")
message(STATUS "swig test bin dir ${CMAKE_BINARY_DIR}")
set(tp  ${MODULE_PATH}/tests)
set(td  ${MODULE_PATH}/gnsstk)
if (WIN32)
  # @todo Using CMAKE_BUILD_TYPE isn't really right as it ignores the
  # --config option, effectively, but I haven't figured out a way to
  # expand $<CONFIG> or whatever such that it can be used here to set
  # the PATH environment variable.
  set(TESTPATH "${MODULE_PATH}/gnsstk;${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE};$ENV{PATH}")
  # Windows uses ; to separate paths, cmake uses it to separate list
  # items, so we have to do this replacement to make things behave as
  # they should.
  string(REPLACE ";" "\\;" TESTPATH "${TESTPATH}")
  string(REPLACE "/" "\\" TESTPATH "${TESTPATH}")
endif()

macro(prep_swig_test target labels)
  set_property(TEST ${target} PROPERTY LABELS swig ${labels})
  if (WIN32)
    set_property(TEST ${target} PROPERTY ENVIRONMENT "PATH=${TESTPATH};PYTHONPATH=${td}")
  else()
    set_property(TEST ${target} PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR};PYTHONPATH=${td}")
  endif()
endmacro(prep_swig_test)

add_test(NAME swig_test_import
  COMMAND ${PYTHON_EXECUTABLE} -c "import gnsstk; print('gnsstk imported successfully')"
#  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_import python)

add_test(NAME swig_test_constants
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_constants.py
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_constants constants)

add_test(NAME swig_test_rinex3
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_rinex3.py -i ${GNSSTK_TEST_DATA_DIR} -o ${GNSSTK_TEST_OUTPUT_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_rinex3 RINEX3)

add_test(NAME swig_test_time
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_time.py -i ${GNSSTK_TEST_DATA_DIR} -o ${GNSSTK_TEST_OUTPUT_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_time Time CommonTime)

add_test(NAME swig_test_misc
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_misc.py
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_misc ObsID SatID)

#add_test(NAME swig_test_store
#  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_store.py
#  WORKING_DIRECTORY ${td}
#  )
#prep_swig_test(swig_test_store python)

add_test(NAME swig_test_NewNav
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NewNav.py
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_NewNav NewNav)

add_test(NAME swig_test_NavLibrary
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NavLibrary.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_NavLibrary NewNav)

add_test(NAME swig_test_RinexNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_RinexNavDataFactory.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_RinexNavDataFactory NewNav)

add_test(NAME swig_test_SP3NavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_SP3NavDataFactory.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_SP3NavDataFactory NewNav)

add_test(NAME swig_test_SEMNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_SEMNavDataFactory.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_SEMNavDataFactory NewNav)

add_test(NAME swig_test_YumaNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_YumaNavDataFactory.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_YumaNavDataFactory NewNav)

add_test(NAME swig_test_MultiFormatNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_MultiFormatNavDataFactory.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_MultiFormatNavDataFactory NewNav)

add_test(NAME swig_test_GPSLNavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavAlm.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSLNavAlm NewNav)

add_test(NAME swig_test_GPSLNavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavEph.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSLNavEph NewNav)

add_test(NAME swig_test_GPSLNavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavHealth.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSLNavHealth NewNav)

add_test(NAME swig_test_GPSLNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavTimeOffset.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSLNavTimeOffset NewNav)

add_test(NAME swig_test_GPSNavConfig
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSNavConfig.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSNavConfig NewNav)

add_test(NAME swig_test_GPSCNavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavAlm.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNavAlm NewNav)

add_test(NAME swig_test_GPSCNavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavEph.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNavEph NewNav)

add_test(NAME swig_test_GPSCNavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavHealth.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNavHealth NewNav)

add_test(NAME swig_test_GPSCNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavTimeOffset.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNavTimeOffset NewNav)

add_test(NAME swig_test_GPSCNav2Alm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2Alm.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNav2Alm NewNav)

add_test(NAME swig_test_GPSCNav2Eph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2Eph.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNav2Eph NewNav)

add_test(NAME swig_test_GPSCNav2Health
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2Health.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNav2Health NewNav)

add_test(NAME swig_test_GPSCNav2TimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2TimeOffset.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GPSCNav2TimeOffset NewNav)

add_test(NAME swig_test_GalINavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavAlm.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalINavAlm NewNav)

add_test(NAME swig_test_GalINavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavEph.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalINavEph NewNav)

add_test(NAME swig_test_GalINavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavHealth.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalINavHealth NewNav)

add_test(NAME swig_test_GalINavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavTimeOffset.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalINavTimeOffset NewNav)

add_test(NAME swig_test_GalFNavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavAlm.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalFNavAlm NewNav)

add_test(NAME swig_test_GalFNavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavEph.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalFNavEph NewNav)

add_test(NAME swig_test_GalFNavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavHealth.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalFNavHealth NewNav)

add_test(NAME swig_test_GalFNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavTimeOffset.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GalFNavTimeOffset NewNav)

add_test(NAME swig_test_FileHandling
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_FileHandling.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_FileHandling swig)

add_test(NAME swig_test_GNSSCore
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GNSSCore.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GNSSCore swig)

add_test(NAME swig_test_GNSSEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GNSSEph.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_GNSSEph swig)

add_test(NAME swig_test_NavFilter
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NavFilter.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_NavFilter swig)

add_test(NAME swig_test_math
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_math.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_math swig)

add_test(NAME swig_test_matrix
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_matrix.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_matrix swig)

add_test(NAME swig_test_vector
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_vector.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_vector swig)

add_test(NAME swig_test_NavDataFactoryCallback
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NavDataFactoryCallback.py -i ${GNSSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
prep_swig_test(swig_test_NavDataFactoryCallback swig)
