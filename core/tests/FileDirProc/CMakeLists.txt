#Tests for FileDirProc Classes

add_executable(FileFilter_T FileFilter_T.cpp)
target_link_libraries(FileFilter_T gpstk)
add_test(FileDirProc_FileFilter FileFilter_T)

add_executable(FileSpecFind_T FileSpecFind_T.cpp)
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  target_link_libraries(FileSpecFind_T gpstk shlwapi)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(FileSpecFind_T gpstk)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_test(NAME FileDirProc_FileSpecFind
  COMMAND FileSpecFind_T
  WORKING_DIRECTORY ${GPSTK_TEST_DATA_DIR})

add_executable(FileSpec_T FileSpec_T.cpp)
target_link_libraries(FileSpec_T gpstk)
add_test(FileDirProc_FileSpec FileSpec_T)

add_executable(FileStore_T FileStore_T.cpp)
target_link_libraries(FileStore_T gpstk)
add_test(FileDirProc_FileStore FileStore_T)

if ( $ENV{EUID} MATCHES "0" )
  message(WARNING "Will not execute FileSpecFind_T test as root")
else ( $ENV{EUID} MATCHES "0" )
  add_executable(FileUtils_T FileUtils_T.cpp)
  target_link_libraries(FileUtils_T gpstk)
  add_test(FileDirProc_FileUtils FileUtils_T)
endif ( $ENV{EUID} MATCHES "0" )

#add_executable(RTFileFrame_T RTFileFrame_T.cpp)
#target_link_libraries(RTFileFrame_T gpstk)
#add_test(FileDirProc_RTFileFrame RTFileFrame_T)
