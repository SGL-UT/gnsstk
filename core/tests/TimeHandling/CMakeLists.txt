#Tests for the TimeHandling Classes

add_executable(ANSITime_T ANSITime_T.cpp)
target_link_libraries(ANSITime_T gnsstk)
add_test(NAME TimeHandling_ANSITime COMMAND $<TARGET_FILE:ANSITime_T>)
set_property(TEST TimeHandling_ANSITime PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(CivilTime_T CivilTime_T.cpp)
target_link_libraries(CivilTime_T gnsstk)
add_test(NAME TimeHandling_CivilTime COMMAND $<TARGET_FILE:CivilTime_T>)
set_property(TEST TimeHandling_CivilTime PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(CommonTime_T CommonTime_T.cpp)
target_link_libraries(CommonTime_T gnsstk)
add_test(NAME TimeHandling_CommonTime COMMAND $<TARGET_FILE:CommonTime_T>)
set_property(TEST TimeHandling_CommonTime PROPERTY LABELS TimeHandling TimeStorage)

add_executable(GPSWeekSecond_T GPSWeekSecond_T.cpp)
target_link_libraries(GPSWeekSecond_T gnsstk)
add_test(NAME TimeHandling_GPSWeekSecond COMMAND $<TARGET_FILE:GPSWeekSecond_T>) 
set_property(TEST TimeHandling_GPSWeekSecond PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(GPSWeekZcount_T GPSWeekZcount_T.cpp)
target_link_libraries(GPSWeekZcount_T gnsstk)
add_test(NAME TimeHandling_GPSWeekZcount COMMAND $<TARGET_FILE:GPSWeekZcount_T>)
set_property(TEST TimeHandling_GPSWeekZcount PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(IRNWeekSecond_T IRNWeekSecond_T.cpp)
target_link_libraries(IRNWeekSecond_T gnsstk)
add_test(NAME TimeHandling_IRNWeekSecond COMMAND $<TARGET_FILE:IRNWeekSecond_T>) 
set_property(TEST TimeHandling_IRNWeekSecond PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(JulianDate_T JulianDate_T.cpp)
target_link_libraries(JulianDate_T gnsstk)
add_test(NAME TimeHandling_JulianDate COMMAND $<TARGET_FILE:JulianDate_T>)
set_property(TEST TimeHandling_JulianDate PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(MJD_T MJD_T.cpp)
target_link_libraries(MJD_T gnsstk)
add_test(NAME TimeHandling_MJD COMMAND $<TARGET_FILE:MJD_T>)
set_property(TEST TimeHandling_MJD PROPERTY LABELS TimeHandling TimeTag TimeStorage)

# Commenting this out because the test for checking that the system
# time is being set correctly fails at random.
#add_executable(SystemTime_T SystemTime_T.cpp)
#target_link_libraries(SystemTime_T gnsstk)
#add_test(NAME TimeHandling_SystemTime COMMAND $<TARGET_FILE:SystemTime_T>)
#set_property(TEST TimeHandling_SystemTime PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(TimeConverters_T TimeConverters_T.cpp)
target_link_libraries(TimeConverters_T gnsstk)
add_test(NAME TimeHandling_TimeConverters COMMAND $<TARGET_FILE:TimeConverters_T>)
set_property(TEST TimeHandling_TimeConverters PROPERTY LABELS TimeHandling)

add_executable(TimeString_T TimeString_T.cpp)
target_link_libraries(TimeString_T gnsstk)
add_test(NAME TimeHandling_TimeString COMMAND $<TARGET_FILE:TimeString_T>)
set_property(TEST TimeHandling_TimeString PROPERTY LABELS TimeHandling)

add_executable(TimeTag_T TimeTag_T.cpp)
target_link_libraries(TimeTag_T gnsstk)
add_test(NAME TimeHandling_TimeTag COMMAND $<TARGET_FILE:TimeTag_T>)
set_property(TEST TimeHandling_TimeTag PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(UnixTime_T UnixTime_T.cpp)
target_link_libraries(UnixTime_T gnsstk)
add_test(NAME TimeHandling_UnixTime COMMAND $<TARGET_FILE:UnixTime_T>)
set_property(TEST TimeHandling_UnixTime PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(PosixTime_T PosixTime_T.cpp)
target_link_libraries(PosixTime_T gnsstk)
add_test(NAME TimeHandling_PosixTime COMMAND $<TARGET_FILE:PosixTime_T>)
set_property(TEST TimeHandling_PosixTime PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(YDSTime_T YDSTime_T.cpp)
target_link_libraries(YDSTime_T gnsstk)
add_test(NAME TimeHandling_YDSTime COMMAND $<TARGET_FILE:YDSTime_T>)
set_property(TEST TimeHandling_YDSTime PROPERTY LABELS TimeHandling TimeTag TimeStorage)

add_executable(TimeRange_T TimeRange_T.cpp)
target_link_libraries(TimeRange_T gnsstk)
add_test(NAME TimeHandling_TimeRange COMMAND $<TARGET_FILE:TimeRange_T>)
set_property(TEST TimeHandling_TimeRange PROPERTY LABELS TimeHandling)

add_executable(GPSZcount_T GPSZcount_T.cpp)
target_link_libraries(GPSZcount_T gnsstk)
add_test(NAME TimeHandling_GPSZcount COMMAND $<TARGET_FILE:GPSZcount_T>)
set_property(TEST TimeHandling_GPSZcount PROPERTY LABELS TimeHandling TimeStorage)

add_executable(TimeCorrection_T TimeCorrection_T.cpp)
target_link_libraries(TimeCorrection_T gnsstk)
add_test(NAME TimeHandling_TimeCorrection COMMAND $<TARGET_FILE:TimeCorrection_T>)
set_property(TEST TimeHandling_TimeCorrection PROPERTY LABELS TimeHandling TimeStorage)

add_executable(GLONASSTime_T GLONASSTime_T.cpp)
target_link_libraries(GLONASSTime_T gnsstk)
add_test(NAME TimeHandling_GLONASSTime COMMAND $<TARGET_FILE:GLONASSTime_T>)
set_property(TEST TimeHandling_GLONASSTime PROPERTY LABELS TimeHandling TimeStorage)
