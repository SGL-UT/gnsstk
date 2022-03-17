#Tests for FileHandling Classes

add_executable(Binex_Types_T Binex_Types_T.cpp)
target_link_libraries(Binex_Types_T gnsstk)
add_test(NAME FileHandling_Binex_Types COMMAND $<TARGET_FILE:Binex_Types_T>)

add_executable(Binex_Attrs_T Binex_Attrs_T.cpp)
target_link_libraries(Binex_Attrs_T gnsstk)
add_test(NAME FileHandling_Binex_Attrs COMMAND $<TARGET_FILE:Binex_Attrs_T>)

add_executable(Binex_UpdateExtract_T Binex_UpdateExtract_T.cpp)
target_link_libraries(Binex_UpdateExtract_T gnsstk)
add_test(NAME FileHandling_Binex_UpdateExtract COMMAND $<TARGET_FILE:Binex_UpdateExtract_T>)

add_executable(Binex_ReadWrite_T Binex_ReadWrite_T.cpp)
target_link_libraries(Binex_ReadWrite_T gnsstk)
add_test(NAME FileHandling_Binex_ReadWrite COMMAND $<TARGET_FILE:Binex_ReadWrite_T>)

add_executable(Rinex_T Rinex_T.cpp)
target_link_libraries(Rinex_T gnsstk)
add_test(NAME FileHandling_Rinex_T COMMAND $<TARGET_FILE:Rinex_T>)

add_executable(RinexClock_T RinexClock_T.cpp)
target_link_libraries(RinexClock_T gnsstk)
add_test(NAME FileHandling_RinexClock_T COMMAND $<TARGET_FILE:RinexClock_T>)

add_executable(RinexMet_T RinexMet_T.cpp)
target_link_libraries(RinexMet_T gnsstk)
add_test(NAME FileHandling_RinexMet_T COMMAND $<TARGET_FILE:RinexMet_T>)

add_executable(Rinex3Obs_FromScratch_T Rinex3Obs_FromScratch_T.cpp)
target_link_libraries(Rinex3Obs_FromScratch_T gnsstk)
add_test(NAME FileHandling_Rinex3Obs_FromScratch_T COMMAND $<TARGET_FILE:Rinex3Obs_FromScratch_T>)

add_executable(Rinex3ObsOther_T Rinex3ObsOther_T.cpp)
target_link_libraries(Rinex3ObsOther_T gnsstk)
add_test(NAME FileHandling_Rinex3ObsOther_T COMMAND $<TARGET_FILE:Rinex3ObsOther_T>)

add_executable(RinexNav_T RinexNav_T.cpp)
target_link_libraries(RinexNav_T gnsstk)
add_test(NAME FileHandling_RinexNav_T COMMAND $<TARGET_FILE:RinexNav_T>)

add_executable(RinexObs_T RinexObs_T.cpp)
target_link_libraries(RinexObs_T gnsstk)
add_test(NAME FileHandling_RinexObs_T COMMAND $<TARGET_FILE:RinexObs_T>)

add_executable(Rinex3Obs_T Rinex3Obs_T.cpp)
target_link_libraries(Rinex3Obs_T gnsstk)
add_test(NAME FileHandling_Rinex3Obs_T COMMAND $<TARGET_FILE:Rinex3Obs_T>)

add_executable(Rinex3Nav_T Rinex3Nav_T.cpp)
target_link_libraries(Rinex3Nav_T gnsstk)
add_test(NAME FileHandling_Rinex3Nav_T COMMAND $<TARGET_FILE:Rinex3Nav_T>)

add_executable(Rinex3Clock_T Rinex3Clock_T.cpp)
target_link_libraries(Rinex3Clock_T gnsstk)
add_test(NAME FileHandling_Rinex3Clock COMMAND $<TARGET_FILE:Rinex3Clock_T>)

add_executable(SP3_T SP3_T.cpp)
target_link_libraries(SP3_T gnsstk)
add_test(NAME FileHandling_SP3 COMMAND $<TARGET_FILE:SP3_T>)

add_executable(FFBinaryStream_T FFBinaryStream_T.cpp)
target_link_libraries(FFBinaryStream_T gnsstk)
add_test(NAME FileHandling_FFBinaryStream COMMAND $<TARGET_FILE:FFBinaryStream_T>)

add_executable(Ionex_T Ionex_T.cpp)
target_link_libraries(Ionex_T gnsstk)
add_test(NAME FileHandling_Ionex COMMAND $<TARGET_FILE:Ionex_T>)

add_executable(IonexStoreStrategy_T IonexStoreStrategy_T.cpp)
target_link_libraries(IonexStoreStrategy_T gnsstk)
add_test(NAME FileHandling_IonexStoreStrategy COMMAND $<TARGET_FILE:IonexStoreStrategy_T>)

set( df_diff ${GNSSTK_BINDIR}/df_diff)
