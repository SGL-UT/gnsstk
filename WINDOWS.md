\page WINDOWS WINDOWS

Windows
-------

This document describes steps that were taken in order to get gnsstk
to work properly under Windows using DLLs.  For information on
installing in a Windows environment, refer to INSTALL.md.

In order to get the toolkit to build and work in a native Windows
environment, using either DLLs or static libraries, a number of
kludges were necessary in both the code and CMake files.

Exports
-------

Producing a Windows DLL requires a list of all exported symbols.
CMake can handle this to some extent, using the
CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS option, but this doesn't include
global variables (including static data members of classes).

In order to make these globals accessible to applications and
libraries linked to gnsstk, we generate (at CMake time) a file that
defines appropriate macros to export the symbols.  This file is named
`gnsstk_export.h`.

The symbols must then be annotated using the GNSSTK_EXPORT
preprocessor macro, e.g.
`GNSSTK_EXPORT static const bool nativeLittleEndian;`

SWIG
----

While gnsstk currently does not support SWIG under Windows, the
exports used to accomodate Windows DLL building conflict with how SWIG
works.  SWIG's code generator does not "drill down" into include
files, so certain include files in the library are forced to manually
`#define GNSSTK_EXPORT` so that SWIG doesn't result in a syntax error.

Tests
-----

Tests created using the CMake add_test function typically have not
been tracked on their location.  In order for Windows to be able to
find the DLL at run time, as well as the test executable, the add_test
functions changed from:

`add_test(ClockModel_ObsClockModel ObsClockModel_T)`

to

`add_test(NAME ClockModel_ObsClockModel COMMAND $<TARGET_FILE:ObsClockModel_T>)`

Linking and NewNav
------------------

The NewNav segment of the GNSSTk library has the ability to be
extended via external add-on libraries.  These libraries should have a
static initializer, akin to what's in NavStatic.cpp, to add support
for additional formats to the library.

However, for the Windows environment when DLLs are being used for the
toolkit, these "optional libraries" that extend GNSSTk must also have
CMake configuration files and be added to the application builds via
CMake.  This is done via the -P option in `build.sh` or by adding a
`find_package(xxxx CONFIG)` function to the file `gnsstk/ExtLinkage.cmake`
(which doesn't normally exist).

The CMake config file must contain additions to three lists, as in the
following example:

```
list( APPEND SUPPORTLIBS mytk )
list( APPEND SUPPORTINCS mytk/MyNavDataFactory.hpp )
list( APPEND SUPPORTCLASS mytk::MyNavDataFactory )
```

   * SUPPORTLIBS adds the name of the library to link to.

   * SUPPORTINCS adds the name of an include file which will be added
     to applications, and it defines the class in SUPPORTCLASS.

   * SUPPORTCLASS names a class to instantiate in main, which prevents
     Visual Studio from optimizing out the library specified in
     SUPPORTLIBS which wouldn't be explicitly used in this situation
     (remember, this describes using extensions where the application
     has no a priori knowledge of the extension in question).

The latter two are used by the `GenerateNewNavIncludes` CMake
function, defined in `SupportFuncs.cmake` (see the gnsstk-apps repo),
to generate a pair of include files that add everything needed to the
code.

The generated `NewNavInc.h` file should be added to your source file
containing your `main()` function where you have all the normal
includes.

The generated `NewNavInit.h` file should be added at the start of your
`main()` function.

Again, refer to gnsstk-apps for more concrete examples.
