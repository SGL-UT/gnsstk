add_executable(PRSolution_T PRSolution_T.cpp)
target_link_libraries(PRSolution_T gnsstk)
add_test(NAME PRSolution_T COMMAND $<TARGET_FILE:PRSolution_T>)

add_executable(PRSolution2_T PRSolution2_T.cpp)
target_link_libraries(PRSolution2_T gnsstk)
add_test(NAME PRSolution2_T COMMAND $<TARGET_FILE:PRSolution2_T>)

add_executable(Combinations_T Combinations_T.cpp)
target_link_libraries(Combinations_T gnsstk)
add_test(NAME Combinations_T COMMAND $<TARGET_FILE:Combinations_T>)
