set( SD ${GNSSTK_TEST_DATA_DIR} )
set( TD ${GNSSTK_TEST_OUTPUT_DIR} )

###############################################################################
# Test EarthOrientation against SOFA example code
###############################################################################
add_executable(test_EO_SOFA test_EO_SOFA.cpp)
target_link_libraries(test_EO_SOFA gnsstk)

add_test(NAME EarthOrientation_SOFA
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:test_EO_SOFA>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=EarthOrientation_SOFA
         -DTESTBASE=EarthOrientation_SOFA
         -DDIFF_ARGS=-l1\ -e1.e-12
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)
set_property(TEST EarthOrientation_SOFA PROPERTY LABELS Geomatics)

###############################################################################
# Test SolidEarth and OceanLoad Tides vs IERS software (cf OceanLoadTides.cpp)
###############################################################################
add_executable(test_tides test_tides.cpp)
target_link_libraries(test_tides gnsstk)

add_test(NAME EarthTides_IERS
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:test_tides>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=EarthTides_IERS
         -DTESTBASE=EarthTides_IERS
         -DARGS=--dt\ 360.\ --prec\ 6\ --solid\ --ocean\ --oceanfile\ ${GNSSTK_TEST_DATA_DIR}/testocean.blq\ --oceansite\ ONSALA,REYKJAVIK\ --atm\ --atmfile\ ${GNSSTK_TEST_DATA_DIR}/testatm.atl\ --atmsite\ reyk,onsa
         -DDIFF_ARGS=-l4\ -e1.e-5
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)
set_property(TEST EarthTides_IERS PROPERTY LABELS Geomatics)

set(JPLDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/geomatics/JPLeph/JPL/)

###############################################################################
# Test StatsFilters filters
###############################################################################
add_executable(StatsFilter_T StatsFilter_T.cpp)
target_link_libraries(StatsFilter_T gnsstk)
add_test(NAME StatsFilter COMMAND $<TARGET_FILE:StatsFilter_T>)
set_property(TEST StatsFilter PROPERTY LABELS Geomatics)

################################################################################
# Test Rinex3ObsFileLoader RINEX3.03 input
###############################################################################
add_executable(Rinex3ObsLoader_T Rinex3ObsLoader_T.cpp)
target_link_libraries(Rinex3ObsLoader_T gnsstk)

add_test(NAME Rinex3ObsLoader_R303
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Rinex3ObsLoader_T>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=Rinex3ObsLoader_R303
         -DTESTBASE=Rinex3ObsLoader_R303
         -DARGS=${GNSSTK_TEST_DATA_DIR}/Rinex3ObsLoader303.obs
         -DDIFF_ARGS=-l1
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)
set_property(TEST Rinex3ObsLoader_R303 PROPERTY LABELS Geomatics)

###############################################################################
# Test Rinex3ObsFileLoader RINEX2.11 input
###############################################################################
add_test(NAME Rinex3ObsLoader_R211
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Rinex3ObsLoader_T>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=Rinex3ObsLoader_R211
         -DTESTBASE=Rinex3ObsLoader_R211
         -DARGS=${GNSSTK_TEST_DATA_DIR}/Rinex3ObsLoader211.obs
         -DDIFF_ARGS=-l1
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)
set_property(TEST Rinex3ObsLoader_R211 PROPERTY LABELS Geomatics)

###############################################################################
# Test Rinex3ObsFileLoader RINEX2.10 input
###############################################################################
add_test(NAME Rinex3ObsLoader_R210
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Rinex3ObsLoader_T>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=Rinex3ObsLoader_R210
         -DTESTBASE=Rinex3ObsLoader_R210
         -DARGS=${GNSSTK_TEST_DATA_DIR}/Rinex3ObsLoader210.obs
         -DDIFF_ARGS=-l1
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)
set_property(TEST Rinex3ObsLoader_R210 PROPERTY LABELS Geomatics)

###############################################################################
add_executable(KalmanFilter_T KalmanFilter_T.cpp)
target_link_libraries(KalmanFilter_T gnsstk)
add_test(NAME KalmanFilter COMMAND $<TARGET_FILE:KalmanFilter_T>)
set_property(TEST KalmanFilter PROPERTY LABELS Geomatics)

################################################################################
add_executable(PreciseRange_T PreciseRange_T.cpp)
target_link_libraries(PreciseRange_T gnsstk)
add_test(NAME PreciseRange COMMAND $<TARGET_FILE:PreciseRange_T>)
set_property(TEST PreciseRange PROPERTY LABELS Geomatics)

################################################################################
function(GenerateNewNavIncludes WHERE)
   message(STATUS "generating in ${WHERE}")
   set(INCFILE "${WHERE}/NewNavInc.h")
   write_file(${INCFILE} "// Auto-generated by cmake")
   foreach (INCFILEINC ${SUPPORTINCS})
      write_file(${INCFILE} "#include <${INCFILEINC}>" APPEND)
   endforeach()
   set(INITFILE "${WHERE}/NewNavInit.h")
   write_file(${INITFILE} "// Auto-generated by cmake")
   foreach (INITFILECLASS ${SUPPORTCLASS})
      write_file(${INITFILE} "{" APPEND)
      write_file(${INITFILE} "   ${INITFILECLASS} obj;" APPEND)
      write_file(${INITFILE} "}" APPEND)
   endforeach()
endfunction()

GenerateNewNavIncludes(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

################################################################################
add_executable(testSSEph testSSEph.cpp)
target_link_libraries(testSSEph gnsstk)

set(JPLDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../data/inputs/JPLeph/JPL)
add_test(NAME JPL_403eph_accuracy
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:testSSEph>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=JPL_403eph_accuracy
         -DTESTBASE=JPL_403eph_accuracy
         -DARGS=--file\ ${JPLDIR}/SolarSystem1975to2050.403.bin\ --test\ ${JPLDIR}/testpo.403\ -d
         -DDIFF_ARGS=-l2\ -z2\ -e1.e-16
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)

set_property(TEST JPL_403eph_accuracy PROPERTY LABELS Geomatics)
################################################################################

add_executable(convertSSEph convertSSEph.cpp)
target_link_libraries(convertSSEph gnsstk)

set(JPLDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../data/inputs/JPLeph/JPL)
add_test(NAME JPL_403eph_conversion
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:convertSSEph>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GNSSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GNSSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=JPL_403eph_conversion
         -DTESTBASE=JPL_403eph_conversion
         -DARGS=--header\ ${JPLDIR}/header.403\ --data\ ${JPLDIR}/ascp1975.403,${JPLDIR}/ascp2000.403,${JPLDIR}/ascp2025.403\ --output\ SolarSystem1975to2050.403.bin\ --verbose
         -DDIFF_ARGS=-l50\ -z1\ -e1.e-15
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_SOURCE_DIR}/core/tests/testsuccexp.cmake)
set_property(TEST JPL_403eph_conversion PROPERTY LABELS Geomatics)

################################################################################
set(TESTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../data/inputs/)
add_executable(AntennaStore_T AntennaStore_T.cpp)
target_link_libraries(AntennaStore_T gnsstk)
target_compile_definitions(AntennaStore_T PRIVATE TESTDIR="${TESTDIR}")
add_test(NAME AntennaStore COMMAND $<TARGET_FILE:AntennaStore_T>)
set_property(TEST AntennaStore PROPERTY LABELS Geomatics)
################################################################################
add_executable(EOPPrediction_T EOPPrediction_T.cpp)
target_link_libraries(EOPPrediction_T gnsstk)
add_test(NAME EOPPrediction COMMAND $<TARGET_FILE:EOPPrediction_T>)
set_property(TEST EOPPrediction PROPERTY LABELS Geomatics)
################################################################################
set(TESTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../data/inputs/)
add_executable(AntexData_T AntexData_T.cpp)
target_link_libraries(AntexData_T gnsstk)
target_compile_definitions(AntexData_T PRIVATE TESTDIR="${TESTDIR}")
add_test(NAME AntexData COMMAND $<TARGET_FILE:AntexData_T>)
set_property(TEST AntexData PROPERTY LABELS Geomatics)

add_executable(gdc_T gdc_T.cpp)
target_link_libraries(gdc_T gnsstk)
add_test(NAME gdc_T COMMAND $<TARGET_FILE:gdc_T>)
set_property(TEST gdc_T PROPERTY LABELS Geomatics)

################################################################################

add_executable(SunEarthSatGeometry_T SunEarthSatGeometry_T.cpp)
target_link_libraries(SunEarthSatGeometry_T gnsstk)
add_test(NAME SunEarthSatGeometry COMMAND $<TARGET_FILE:SunEarthSatGeometry_T>)
set_property(TEST SunEarthSatGeometry PROPERTY LABELS Geomatics)