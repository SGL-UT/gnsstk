# apps/misc/CMakeLists.txt

add_executable(SVNumXRefDUMP  SVNumXRefDUMP.cpp)
target_link_libraries(SVNumXRefDUMP gnsstk)
install (TARGETS SVNumXRefDUMP DESTINATION "${CMAKE_INSTALL_BINDIR}")
