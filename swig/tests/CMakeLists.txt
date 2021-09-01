# Testing the swig/python bindings
#
# Since this requires that there be a valid python runtime, these tests are
# be quite different from the C++ library and application tests

message(STATUS "swig test path ${MODULE_PATH}")
message(STATUS "swig test bin dir ${CMAKE_BINARY_DIR}")
set(tp  ${MODULE_PATH}/tests)
set(td  ${MODULE_PATH}/gpstk)

macro(add_swig_test target test)
  add_test(${target} ${test})
  set_property(TEST ${target} PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}")
endmacro(add_swig_test)

add_test(NAME swig_test_import
  COMMAND ${PYTHON_EXECUTABLE} -c "import gpstk; print('gpstk imported successfully')"
#  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_import PROPERTY LABELS swig)
set_property(TEST swig_test_import PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR};PYTHONPATH=${td}")

add_test(NAME swig_test_constants
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_constants.py
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_constants PROPERTY LABELS swig constants)

add_test(NAME swig_test_rinex3
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_rinex3.py -i ${GPSTK_TEST_DATA_DIR} -o ${GPSTK_TEST_OUTPUT_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_rinex3 PROPERTY LABELS swig RINEX3)

add_test(NAME swig_test_time
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_time.py -i ${GPSTK_TEST_DATA_DIR} -o ${GPSTK_TEST_OUTPUT_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_time PROPERTY LABELS swig Time CommonTime)

add_test(NAME swig_test_misc
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_misc.py
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_misc PROPERTY LABELS swig ObsID SatID)

#add_test(NAME swig_test_store
#  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_store.py
#  WORKING_DIRECTORY ${td}
#  )
#set_property(TEST swig_test_store PROPERTY LABELS swig )

add_test(NAME swig_test_NewNav
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NewNav.py
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_NewNav PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_NavLibrary
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NavLibrary.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_NavLibrary PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_RinexNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_RinexNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_RinexNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_SP3NavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_SP3NavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_SP3NavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_SEMNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_SEMNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_SEMNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_YumaNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_YumaNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_YumaNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_MultiFormatNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_MultiFormatNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_MultiFormatNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSLNavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavTimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavTimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2Alm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2Alm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2Alm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2Eph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2Eph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2Eph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2Health
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2Health.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2Health PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2TimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GPSCNav2TimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2TimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalINavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavTimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalFNavAlm
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalFNavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalFNavEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalFNavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalFNavHealth
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalFNavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalFNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GalFNavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalFNavTimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_FileHandling
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_FileHandling.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_FileHandling PROPERTY LABELS swig)

add_test(NAME swig_test_GNSSCore
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GNSSCore.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GNSSCore PROPERTY LABELS swig)

add_test(NAME swig_test_GNSSEph
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_GNSSEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GNSSEph PROPERTY LABELS swig)

add_test(NAME swig_test_NavFilter
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_NavFilter.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_NavFilter PROPERTY LABELS swig)

add_test(NAME swig_test_math
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_math.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_math PROPERTY LABELS swig)

add_test(NAME swig_test_matrix
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_matrix.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_matrix PROPERTY LABELS swig)

add_test(NAME swig_test_vector
  COMMAND ${PYTHON_EXECUTABLE} ${tp}/test_vector.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_vector PROPERTY LABELS swig)
