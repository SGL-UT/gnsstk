add_executable(PRSolution_T PRSolution_T.cpp)
target_link_libraries(PRSolution_T gnsstk)
add_test(NAME PRSolution_T COMMAND $<TARGET_FILE:PRSolution_T>)
