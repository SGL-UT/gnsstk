# Testing the swig/python bindings
#
# Since this requires that there be a valid python runtime, these tests are
# be quite different from the C++ library and application tests

add_test(NAME swig_test_import
  COMMAND ${PYTHON_EXECUTABLE} -c "import gpstk; print('gpstk imported successfully')"
  WORKING_DIRECTORY ${MODULE_PATH}
  )
set_property(TEST swig_test_import PROPERTY LABELS swig)


set(td  ${MODULE_PATH}/tests)

add_test(NAME swig_test_constants
  COMMAND ${PYTHON_EXECUTABLE} test_constants.py
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_constants PROPERTY LABELS swig constants)

add_test(NAME swig_test_rinex3
  COMMAND ${PYTHON_EXECUTABLE} test_rinex3.py -i ${GPSTK_TEST_DATA_DIR} -o ${GPSTK_TEST_OUTPUT_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_rinex3 PROPERTY LABELS swig RINEX3)

add_test(NAME swig_test_time
  COMMAND ${PYTHON_EXECUTABLE} test_time.py -i ${GPSTK_TEST_DATA_DIR} -o ${GPSTK_TEST_OUTPUT_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_time PROPERTY LABELS swig Time CommonTime)

add_test(NAME swig_test_misc
  COMMAND ${PYTHON_EXECUTABLE} test_misc.py
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_misc PROPERTY LABELS swig ObsID SatID)

#add_test(NAME swig_test_store
#  COMMAND ${PYTHON_EXECUTABLE} test_store.py
#  WORKING_DIRECTORY ${td}
#  )
#set_property(TEST swig_test_store PROPERTY LABELS swig )

add_test(NAME swig_test_NewNav
  COMMAND ${PYTHON_EXECUTABLE} test_NewNav.py
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_NewNav PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_NavLibrary
  COMMAND ${PYTHON_EXECUTABLE} test_NavLibrary.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_NavLibrary PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_RinexNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} test_RinexNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_RinexNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_SP3NavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} test_SP3NavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_SP3NavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_SEMNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} test_SEMNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_SEMNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_YumaNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} test_YumaNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_YumaNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_MultiFormatNavDataFactory
  COMMAND ${PYTHON_EXECUTABLE} test_MultiFormatNavDataFactory.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_MultiFormatNavDataFactory PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavAlm
  COMMAND ${PYTHON_EXECUTABLE} test_GPSLNavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavEph
  COMMAND ${PYTHON_EXECUTABLE} test_GPSLNavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavHealth
  COMMAND ${PYTHON_EXECUTABLE} test_GPSLNavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSLNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} test_GPSLNavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSLNavTimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavAlm
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavEph
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavHealth
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNavTimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2Alm
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNav2Alm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2Alm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2Eph
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNav2Eph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2Eph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2Health
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNav2Health.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2Health PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GPSCNav2TimeOffset
  COMMAND ${PYTHON_EXECUTABLE} test_GPSCNav2TimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GPSCNav2TimeOffset PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavAlm
  COMMAND ${PYTHON_EXECUTABLE} test_GalINavAlm.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavAlm PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavEph
  COMMAND ${PYTHON_EXECUTABLE} test_GalINavEph.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavEph PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavHealth
  COMMAND ${PYTHON_EXECUTABLE} test_GalINavHealth.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavHealth PROPERTY LABELS swig NewNav)

add_test(NAME swig_test_GalINavTimeOffset
  COMMAND ${PYTHON_EXECUTABLE} test_GalINavTimeOffset.py -i ${GPSTK_TEST_DATA_DIR}
  WORKING_DIRECTORY ${td}
  )
set_property(TEST swig_test_GalINavTimeOffset PROPERTY LABELS swig NewNav)
