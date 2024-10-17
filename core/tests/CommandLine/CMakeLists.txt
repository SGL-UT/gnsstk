#Tests for CommandLine Classes

add_executable(CommandOption_T CommandOption_T.cpp)
target_link_libraries(CommandOption_T gnsstk)
add_test(NAME CommandLine_CommandOption COMMAND $<TARGET_FILE:CommandOption_T>)

add_executable(CommandOptionParser_T CommandOptionParser_T.cpp)
target_link_libraries(CommandOptionParser_T gnsstk)
add_test(NAME CommandLine_CommandOptionParser COMMAND $<TARGET_FILE:CommandOptionParser_T>)

add_executable(CommandOptionWithCommonTimeArg_T CommandOptionWithCommonTimeArg_T.cpp)
target_link_libraries(CommandOptionWithCommonTimeArg_T gnsstk)
add_test(NAME CommandLine_CommandOptionWithCommonTimeArg COMMAND $<TARGET_FILE:CommandOptionWithCommonTimeArg_T>)

add_executable(CommandLine_T CommandLine_T.cpp)
target_link_libraries(CommandLine_T gnsstk)
add_test(NAME CommandLine_CommandLine COMMAND $<TARGET_FILE:CommandLine_T>)

