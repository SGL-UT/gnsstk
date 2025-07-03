# Testing the swig/python bindings
#
# Since this requires that there be a valid python runtime, these tests are
# be quite different from the C++ library and application tests

message(STATUS "swig test path ${MODULE_PATH}")
message(STATUS "swig test bin dir ${CMAKE_BINARY_DIR}")
message(STATUS "BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS}")
set(tp ${MODULE_PATH}/tests)
set(td ${MODULE_PATH}/gnsstk)
if (WIN32)
  # @todo Using CMAKE_BUILD_TYPE isn't really right as it ignores the
  # --config option, effectively, but I haven't figured out a way to
  # expand $<CONFIG> or whatever such that it can be used here to set
  # the PATH environment variable.
  set(TESTPATH "${MODULE_PATH}/gnsstk;"
               "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE};"
               "$ENV{PATH}")
  # Windows uses ; to separate paths, cmake uses it to separate list
  # items, so we have to do this replacement to make things behave as
  # they should.
  string(REPLACE ";" "\\;" TESTPATH "${TESTPATH}")
  string(REPLACE "/" "\\" TESTPATH "${TESTPATH}")
  # Starting with python3.8, DLLs will be ignored if added to PATH variable.
  # This is workaround for now until we add gnsstk.dll to same package directory as _gnsstk.pyd
  find_package(Python COMPONENTS Interpreter REQUIRED)
  message(STATUS "Python_VERSION ${Python_VERSION}")
  if (BUILD_SHARED_LIBS AND Python_VERSION VERSION_GREATER_EQUAL "3.8")
   add_custom_target(COPY_GNSSTK_DLL
      COMMENT "add_custom_target COPY_GNSSTK_DLL"
   )
   add_custom_command(TARGET COPY_GNSSTK_DLL
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/gnsstk.dll"
        "${MODULE_PATH}/gnsstk"
   )
  endif()
endif()

macro(prep_swig_test target)
  set_property(TEST ${target}
               PROPERTY LABELS swig)
  if (WIN32)
    set_property(TEST ${target}
                 PROPERTY ENVIRONMENT "PATH=${TESTPATH};"
                 "PYTHONPATH=${td}")
  else()
    set_property(TEST ${target}
                 PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR};"
                 "PYTHONPATH=${td}")
  endif()
endmacro(prep_swig_test)

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test_*.py)
# Remove tests not ready for prime time
list(REMOVE_ITEM TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_store.py")
list(REMOVE_ITEM TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_tropmodel.py")

foreach(TEST_FILE ${TEST_FILES})
  message(STATUS "adding test ${TEST_FILE}")
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(NAME swig_${TEST_NAME}
             COMMAND ${PYTHON_EXECUTABLE}
                     ${tp}/${TEST_NAME}.py
                     -i ${GNSSTK_TEST_DATA_DIR}
                     -o ${GNSSTK_TEST_OUTPUT_DIR}
             WORKING_DIRECTORY ${td}
    )
    prep_swig_test(swig_${TEST_NAME})
endforeach()

add_test(NAME swig_test_import
  COMMAND ${PYTHON_EXECUTABLE}
  -c "import gnsstk; print('gnsstk imported successfully')"
)
prep_swig_test(swig_test_import)
