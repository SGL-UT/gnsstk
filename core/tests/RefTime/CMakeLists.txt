#Tests for RefTime Classes

add_executable(HelmertTransform_T HelmertTransform_T.cpp)
target_link_libraries(HelmertTransform_T gnsstk)
add_test(NAME RefTime_HelmertTransform COMMAND $<TARGET_FILE:HelmertTransform_T>)

add_executable(ReferenceFrame_T ReferenceFrame_T.cpp)
target_link_libraries(ReferenceFrame_T gnsstk)
add_test(NAME RefTime_ReferenceFrame COMMAND $<TARGET_FILE:ReferenceFrame_T>)

add_executable(TimeSystem_T TimeSystem_T.cpp)
target_link_libraries(TimeSystem_T gnsstk)
add_test(NAME RefTime_TimeSystem COMMAND $<TARGET_FILE:TimeSystem_T>)

add_executable(TimeSystemCorr_T TimeSystemCorr_T.cpp)
target_link_libraries(TimeSystemCorr_T gnsstk)
add_test(NAME RefTime_TimeSystemCorr COMMAND $<TARGET_FILE:TimeSystemCorr_T>)

add_executable(BasicTimeSystemConverter_T BasicTimeSystemConverter_T.cpp)
target_link_libraries(BasicTimeSystemConverter_T gnsstk)
add_test(NAME RefTime_BasicTimeSystemConverter COMMAND $<TARGET_FILE:BasicTimeSystemConverter_T>)

add_executable(NavTimeSystemConverter_T NavTimeSystemConverter_T.cpp)
target_link_libraries(NavTimeSystemConverter_T gnsstk)
add_test(NAME RefTime_NavTimeSystemConverter COMMAND $<TARGET_FILE:NavTimeSystemConverter_T>)
