#Tests for ClockModel Classes

add_executable(ObsClockModel_T ObsClockModel_T.cpp)
target_link_libraries(ObsClockModel_T gnsstk)
add_test(NAME ClockModel_ObsClockModel COMMAND $<TARGET_FILE:ObsClockModel_T>)

# add_executable(EpochClockModel_T EpochClockModel_T.cpp)
# target_link_libraries(EpochClockModel_T gnsstk)
# add_test(NAME ClockModel_EpochClockModel COMMAND $<TARGET_FILE:EpochClockModel_T>)

# add_executable(ObsEpochMap_T ObsEpochMap_T.cpp)
# target_link_libraries(ObsEpochMap_T gnsstk)
# add_test(NAME ClockModel_ObsEpochMap COMMAND $<TARGET_FILE:ObsEpochMap_T>)

add_executable(ObsRngDev_T ObsRngDev_T.cpp)
target_link_libraries(ObsRngDev_T gnsstk)
add_test(NAME ClockModel_ObsRngDev COMMAND $<TARGET_FILE:ObsRngDev_T>)

# add_executable(ORDEpoch_T ORDEpoch_T.cpp)
# target_link_libraries(ORDEpoch_T gnsstk)
# add_test(NAME ClockModel_ORDEpoch COMMAND $<TARGET_FILE:ORDEpoch_T>)
