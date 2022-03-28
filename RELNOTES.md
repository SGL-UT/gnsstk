GNSSTk 13.2.0 Release Notes
========================

 * This release adds the following:
   * GNSSTk as a windows DLL package.
   * GNSSTk with windows swig python bindings.
   * `CommandOptionNavEnumHelp` class
   * ClearTypeFilter and addTypeFilter methods to NewNav for C++ and Python use
   * Migrate IONEX file processing code into core, updating to meet style
 * Additionally, it contains some bug fixes and test fixes

Updates since v13.1.0
---------------------

**Build System and Test Suite**
  * Fix test issues arising from merging.
  * Fix for non-windows builds broken by the fixes for windows builds.

**Library Changes**
  * Add the ability to build gnsstk as a DLL under Windows
  * Add NavLibrary debug tracing
  * Add tabularize() to StringUtils
  * Add CommandOptionNavEnumHelp class
  * Add CMAKE as a special class of reviewers.
  * Add Visual Studio export qualifier to DebugTrace::enabled
  * Add missing PNBGLOFNavDataFactory documentation.
  * Add missing NewNav tests.
  * Add clearTypeFilter and addTypeFilter methods to NewNav for C++ and Python use
  * Update NewNav getXvt methods to set the reference frame.
  * Update GalINavAlm/Eph dump() to show interpreted health status.
  * Update CCIR documentation to explain error conditions.
  * Update NewNav getXvt() method to support optionally specifying a different phase center than the default.
  * Update exports and cmake files to allow SWIG to build under Visual Studio 2019
  * Update per style guide
  * Update to SatPass.hpp to expand GNSSTK_EXPORT macro for outFormat and longfmt.
  * Migrate IONEX file processing code into core, updating to meet style.
  * Minor comment fix-up for OMEGA0

Fixes since v13.1.0
--------------------
  * Fix PNBGLOFNavDataFactory to use the correct string for decoding ISC.
  * Fix PNBGLOFNavDataFactory to properly include the correct subject satellite for almanac health.

New Modules
-------------------------------
     core/lib/FileHandling/Ionex/IonexStoreStrategy.cpp
     core/lib/FileHandling/Ionex/IonexStoreStrategy.hpp
     core/lib/NewNav/CommandOptionNavEnumHelp.cpp
     core/lib/NewNav/CommandOptionNavEnumHelp.hpp
     core/lib/NewNav/NavSearchOrder.cpp
     core/lib/Utilities/logstream.cpp
     core/tests/FileHandling/IonexStoreStrategy_T.cpp
     core/tests/FileHandling/Ionex_T.cpp
     core/tests/NewNav/BDSD2NavAlm_T.cpp
     core/tests/NewNav/BDSD2NavData_T.cpp
     core/tests/NewNav/BDSD2NavEph_T.cpp
     core/tests/NewNav/BDSD2NavHealth_T.cpp
     core/tests/NewNav/BDSD2NavISC_T.cpp
     core/tests/NewNav/BDSD2NavIono_T.cpp
     core/tests/NewNav/BDSD2NavTimeOffset_T.cpp
     core/tests/NewNav/GLOFNavTestDataDecl.hpp
     core/tests/NewNav/GLOFNavTestDataDef.hpp
     core/tests/NewNav/GPSCNav2ISC_T.cpp
     core/tests/NewNav/GalFNavIono_T.cpp
     core/tests/NewNav/GalINavIono_T.cpp
     core/tests/NewNav/PNBGLOFNavDataFactory_T.cpp