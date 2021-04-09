set( SD ${GPSTK_TEST_DATA_DIR} )
set( TD ${GPSTK_TEST_OUTPUT_DIR} )

###############################################################################
# Test EarthOrientation against SOFA example code
###############################################################################
add_executable(test_EO_SOFA test_EO_SOFA.cpp)
target_link_libraries(test_EO_SOFA gpstk)

add_test(NAME EarthOrientation_SOFA
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:test_EO_SOFA>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GPSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GPSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=EarthOrientation_SOFA
         -DTESTBASE=EarthOrientation_SOFA
         -DDIFF_ARGS=-l1\ -e1.e-12
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST EarthOrientation_SOFA PROPERTY LABELS Geomatics)

###############################################################################
# Test SolidEarth and OceanLoad Tides vs IERS software (cf OceanLoadTides.cpp)
###############################################################################
add_executable(test_tides test_tides.cpp)
target_link_libraries(test_tides gpstk)

add_test(NAME EarthTides_IERS
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:test_tides>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GPSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GPSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=EarthTides_IERS
         -DTESTBASE=EarthTides_IERS
         -DARGS=--dt\ 360.\ --prec\ 6\ --solid\ --ocean\ --oceanfile\ ${GPSTK_TEST_DATA_DIR}/testocean.blq\ --oceansite\ ONSALA,REYKJAVIK\ --atm\ --atmfile\ ${GPSTK_TEST_DATA_DIR}/testatm.atl\ --atmsite\ reyk,onsa
         -DDIFF_ARGS=-l4\ -e1.e-5
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST EarthTides_IERS PROPERTY LABELS Geomatics)

set(JPLDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/geomatics/JPLeph/JPL/)

###############################################################################
# Test StatsFilters filters
###############################################################################
add_executable(StatsFilter_T StatsFilter_T.cpp)
target_link_libraries(StatsFilter_T gpstk)
add_test(StatsFilter StatsFilter_T)
set_property(TEST StatsFilter PROPERTY LABELS Geomatics)

################################################################################
# Test Rinex3ObsFileLoader RINEX3.03 input
###############################################################################
add_executable(Rinex3ObsLoader_T Rinex3ObsLoader_T.cpp)
target_link_libraries(Rinex3ObsLoader_T gpstk)

add_test(NAME Rinex3ObsLoader_R303
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Rinex3ObsLoader_T>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GPSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GPSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=Rinex3ObsLoader_R303
         -DTESTBASE=Rinex3ObsLoader_R303
         -DARGS=${GPSTK_TEST_DATA_DIR}/Rinex3ObsLoader303.obs
         -DDIFF_ARGS=-l1
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST Rinex3ObsLoader_R303 PROPERTY LABELS Geomatics)

###############################################################################
# Test Rinex3ObsFileLoader RINEX2.11 input
###############################################################################
add_test(NAME Rinex3ObsLoader_R211
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Rinex3ObsLoader_T>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GPSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GPSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=Rinex3ObsLoader_R211
         -DTESTBASE=Rinex3ObsLoader_R211
         -DARGS=${GPSTK_TEST_DATA_DIR}/Rinex3ObsLoader211.obs
         -DDIFF_ARGS=-l1
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST Rinex3ObsLoader_R211 PROPERTY LABELS Geomatics)

###############################################################################
# Test Rinex3ObsFileLoader RINEX2.10 input
###############################################################################
add_test(NAME Rinex3ObsLoader_R210
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Rinex3ObsLoader_T>
         -DDIFF_PROG=$<TARGET_FILE:df_diff>
         -DSOURCEDIR=${GPSTK_TEST_DATA_DIR}
         -DTARGETDIR=${GPSTK_TEST_OUTPUT_DIR}
         -DTESTNAME=Rinex3ObsLoader_R210
         -DTESTBASE=Rinex3ObsLoader_R210
         -DARGS=${GPSTK_TEST_DATA_DIR}/Rinex3ObsLoader210.obs
         -DDIFF_ARGS=-l1
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST Rinex3ObsLoader_R210 PROPERTY LABELS Geomatics)

###############################################################################
add_executable(KalmanFilter_T KalmanFilter_T.cpp)
target_link_libraries(KalmanFilter_T gpstk)
add_test(KalmanFilter KalmanFilter_T)
set_property(TEST KalmanFilter PROPERTY LABELS Geomatics)

################################################################################
