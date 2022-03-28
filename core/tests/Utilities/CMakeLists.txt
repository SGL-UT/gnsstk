#Tests for Utilities Classes

add_executable(BinUtils_T BinUtils_T.cpp)
target_link_libraries(BinUtils_T gnsstk)
add_test(NAME Utilities_BinUtils COMMAND $<TARGET_FILE:BinUtils_T>)

add_executable(Exception_T Exception_T.cpp)
target_link_libraries(Exception_T gnsstk)
add_test(NAME Utilities_Exception COMMAND $<TARGET_FILE:Exception_T>)

add_executable(StringUtils_T StringUtils_T.cpp)
target_link_libraries(StringUtils_T gnsstk)
add_test(NAME Utilities_StringUtils COMMAND $<TARGET_FILE:StringUtils_T>)

add_executable(Stl_helpers_T Stl_helpers_T.cpp)
target_link_libraries(Stl_helpers_T gnsstk)
add_test(NAME Utilities_Stl_helpers COMMAND $<TARGET_FILE:Stl_helpers_T>)

add_executable(ValidType_T ValidType_T.cpp)
target_link_libraries(ValidType_T gnsstk)
add_test(NAME Utilities_ValidType COMMAND $<TARGET_FILE:ValidType_T>)

add_executable(EnumIterator_T EnumIterator_T.cpp)
target_link_libraries(EnumIterator_T gnsstk)
add_test(NAME Utilities_EnumIterator COMMAND $<TARGET_FILE:EnumIterator_T>)

add_executable(FormattedDouble_T FormattedDouble_T.cpp)
target_link_libraries(FormattedDouble_T gnsstk)
add_test(NAME Utilities_FormattedDouble COMMAND $<TARGET_FILE:FormattedDouble_T>)

add_executable(DebugTrace_T DebugTrace_T.cpp)
target_link_libraries(DebugTrace_T gnsstk)
add_test(NAME Utilities_DebugTrace COMMAND $<TARGET_FILE:DebugTrace_T>)
