# apps/CMakeLists.txt

add_subdirectory (filetools)
add_subdirectory (geomatics)

# If on UNIX, do these
if (UNIX)
    add_subdirectory (rfw)
endif (UNIX)

add_subdirectory (Rinextools)


