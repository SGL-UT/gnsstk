#Tests for FileHandling Classes

add_executable(Binex_Types_T Binex_Types_T.cpp)
target_link_libraries(Binex_Types_T gnsstk)
add_test(NAME FileHandling_Binex_Types COMMAND $<TARGET_FILE:Binex_Types_T>)
set_property(TEST FileHandling_Binex_Types PROPERTY LABELS FileHandling)

add_executable(Binex_Attrs_T Binex_Attrs_T.cpp)
target_link_libraries(Binex_Attrs_T gnsstk)
add_test(NAME FileHandling_Binex_Attrs COMMAND $<TARGET_FILE:Binex_Attrs_T>)
set_property(TEST FileHandling_Binex_Attrs PROPERTY LABELS FileHandling)

add_executable(Binex_UpdateExtract_T Binex_UpdateExtract_T.cpp)
target_link_libraries(Binex_UpdateExtract_T gnsstk)
add_test(NAME FileHandling_Binex_UpdateExtract COMMAND $<TARGET_FILE:Binex_UpdateExtract_T>)
set_property(TEST FileHandling_Binex_UpdateExtract PROPERTY LABELS FileHandling)

add_executable(Binex_ReadWrite_T Binex_ReadWrite_T.cpp)
target_link_libraries(Binex_ReadWrite_T gnsstk)
add_test(NAME FileHandling_Binex_ReadWrite COMMAND $<TARGET_FILE:Binex_ReadWrite_T>)
set_property(TEST FileHandling_Binex_ReadWrite PROPERTY LABELS FileHandling)

add_executable(Rinex_T Rinex_T.cpp)
target_link_libraries(Rinex_T gnsstk)
add_test(NAME FileHandling_Rinex_T COMMAND $<TARGET_FILE:Rinex_T>)
set_property(TEST FileHandling_Rinex_T PROPERTY LABELS FileHandling)

add_executable(RinexClock_T RinexClock_T.cpp)
target_link_libraries(RinexClock_T gnsstk)
add_test(NAME FileHandling_RinexClock_T COMMAND $<TARGET_FILE:RinexClock_T>)
set_property(TEST FileHandling_RinexClock_T PROPERTY LABELS FileHandling)

add_executable(RinexMet_T RinexMet_T.cpp)
target_link_libraries(RinexMet_T gnsstk)
add_test(NAME FileHandling_RinexMet_T COMMAND $<TARGET_FILE:RinexMet_T>)
set_property(TEST FileHandling_RinexMet_T PROPERTY LABELS FileHandling)

add_executable(Rinex3Obs_FromScratch_T Rinex3Obs_FromScratch_T.cpp)
target_link_libraries(Rinex3Obs_FromScratch_T gnsstk)
add_test(NAME FileHandling_Rinex3Obs_FromScratch_T COMMAND $<TARGET_FILE:Rinex3Obs_FromScratch_T>)
set_property(TEST FileHandling_Rinex3Obs_FromScratch_T PROPERTY LABELS FileHandling)

add_executable(Rinex3ObsOther_T Rinex3ObsOther_T.cpp)
target_link_libraries(Rinex3ObsOther_T gnsstk)
add_test(NAME FileHandling_Rinex3ObsOther_T COMMAND $<TARGET_FILE:Rinex3ObsOther_T>)
set_property(TEST FileHandling_Rinex3ObsOther_T PROPERTY LABELS FileHandling)

add_executable(RinexNav_T RinexNav_T.cpp)
target_link_libraries(RinexNav_T gnsstk)
add_test(NAME FileHandling_RinexNav_T COMMAND $<TARGET_FILE:RinexNav_T>)
set_property(TEST FileHandling_RinexNav_T PROPERTY LABELS FileHandling)

add_executable(RinexObs_T RinexObs_T.cpp)
target_link_libraries(RinexObs_T gnsstk)
add_test(NAME FileHandling_RinexObs_T COMMAND $<TARGET_FILE:RinexObs_T>)
set_property(TEST FileHandling_RinexObs_T PROPERTY LABELS FileHandling)

add_executable(Rinex3Obs_T Rinex3Obs_T.cpp)
target_link_libraries(Rinex3Obs_T gnsstk)
add_test(NAME FileHandling_Rinex3Obs_T COMMAND $<TARGET_FILE:Rinex3Obs_T>)
set_property(TEST FileHandling_Rinex3Obs_T PROPERTY LABELS FileHandling)

add_executable(Rinex3Nav_T Rinex3Nav_T.cpp)
target_link_libraries(Rinex3Nav_T gnsstk)
add_test(NAME FileHandling_Rinex3Nav_T COMMAND $<TARGET_FILE:Rinex3Nav_T>)
set_property(TEST FileHandling_Rinex3Nav_T PROPERTY LABELS FileHandling)

add_executable(Rinex3Clock_T Rinex3Clock_T.cpp)
target_link_libraries(Rinex3Clock_T gnsstk)
add_test(NAME FileHandling_Rinex3Clock COMMAND $<TARGET_FILE:Rinex3Clock_T>)
set_property(TEST FileHandling_Rinex3Clock PROPERTY LABELS FileHandling)

add_executable(SP3_T SP3_T.cpp)
target_link_libraries(SP3_T gnsstk)
add_test(NAME FileHandling_SP3 COMMAND $<TARGET_FILE:SP3_T>)
set_property(TEST FileHandling_SP3 PROPERTY LABELS FileHandling)

add_executable(FFBinaryStream_T FFBinaryStream_T.cpp)
target_link_libraries(FFBinaryStream_T gnsstk)
add_test(NAME FileHandling_FFBinaryStream COMMAND $<TARGET_FILE:FFBinaryStream_T>)
set_property(TEST FileHandling_FFBinaryStream PROPERTY LABELS FileHandling)

add_executable(Ionex_T Ionex_T.cpp)
target_link_libraries(Ionex_T gnsstk)
add_test(NAME FileHandling_Ionex COMMAND $<TARGET_FILE:Ionex_T>)
set_property(TEST FileHandling_Ionex PROPERTY LABELS FileHandling)

add_executable(IonexStoreStrategy_T IonexStoreStrategy_T.cpp)
target_link_libraries(IonexStoreStrategy_T gnsstk)
add_test(NAME FileHandling_IonexStoreStrategy COMMAND $<TARGET_FILE:IonexStoreStrategy_T>)
set_property(TEST FileHandling_IonexStoreStrategy PROPERTY LABELS FileHandling)

add_executable(Yuma_T Yuma_T.cpp)
target_link_libraries(Yuma_T gnsstk)
add_test(NAME FileHandling_Yuma COMMAND $<TARGET_FILE:Yuma_T>)
add_test(NAME Yuma_Diff
   COMMAND $<TARGET_FILE:df_diff> -1 ${GNSSTK_TEST_DATA_DIR}/test_input_yuma377.txt -2 ${GNSSTK_TEST_OUTPUT_DIR}/test_output_yuma377.out)
set_tests_properties(Yuma_Diff PROPERTIES DEPENDS "FileHandling_Yuma")
set_property(TEST FileHandling_Yuma PROPERTY LABELS FileHandling)
set_property(TEST Yuma_Diff PROPERTY LABELS FileHandling)

add_executable(SEM_T SEM_T.cpp)
target_link_libraries(SEM_T gnsstk)
add_test(NAME FileHandling_SEM COMMAND $<TARGET_FILE:SEM_T>)
set_property(TEST FileHandling_SEM PROPERTY LABELS FileHandling)
