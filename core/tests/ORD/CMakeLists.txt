add_executable(OrdUnitTests_T OrdUnitTests_T.cpp)
target_link_libraries(OrdUnitTests_T gnsstk)
add_test(NAME OrdUnitTests_T COMMAND $<TARGET_FILE:OrdUnitTests_T>)

add_executable(OrdRegressionChecks_T OrdRegressionChecks_T.cpp)
target_link_libraries(OrdRegressionChecks_T gnsstk)
add_test(NAME OrdRegressionChecks_T COMMAND $<TARGET_FILE:OrdRegressionChecks_T>)
