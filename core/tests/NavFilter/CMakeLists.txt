add_executable(NavFilterMgr_T NavFilterMgr_T.cpp)
target_link_libraries(NavFilterMgr_T gnsstk)
add_test(NAME NavFilter_NavFilterMgr COMMAND $<TARGET_FILE:NavFilterMgr_T>)

add_executable(CNavFilter_T CNavFilter_T.cpp)
target_link_libraries(CNavFilter_T gnsstk)
add_test(NAME NavFilter_CNavFilter COMMAND $<TARGET_FILE:CNavFilter_T>)

add_executable(CNav2Filter_T CNav2Filter_T.cpp)
target_link_libraries(CNav2Filter_T gnsstk)
add_test(NAME NavFilter_CNav2Filter COMMAND $<TARGET_FILE:CNav2Filter_T>)
