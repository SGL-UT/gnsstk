GNSSTk 13.4.0 Release Notes
========================

 * This release includes the following:
   * Refactoring SWIG bindings into separate modules for better build performance.
   * Moving Geomatics, SEM and Yuma file support, from ext to core.
   * Adding support for deploying to Debian 11.
   * Adding support for SonarQube code quality analysis.
 * Additionally, it contains bug fixes and CI updates.

Updates since v13.3.0
---------------------

**Build System and Test Suite**
  * Add support for deploying to Debian 11

**Gitlab CI**
  * Adding pipeline job for sonar
  * Update DOCKER_REGISTRY variable.
  * Update conda recipe and CI pipeline to release from stable branch

**Library Changes**
  * Refactor SWIG bindings into separate modules
  * Move Geomatics code from ext into core
  * Move SEM and Yuma file support from ext to core.

Fixes since v13.3.0
--------------------
  * Fix swig wrapper missing class/template issue
  * Fix PNBGPSLNavDataFactory missing half-week test.

Removed Code due to Deprecation
-------------------------------
     ext/tests/FileHandling/SEM_T.cpp
     ext/tests/NewNav/ExtFactoryInitializer_T.cpp
     ext/tests/RinexNav/xRinexNav.cpp
     ext/tests/RinexNav/xRinexTest.cpp
     ext/tests/geomatics/KalmanFilter_T.cpp
     ext/tests/geomatics/StatsFilter_T.cpp
     ext/tests/geomatics/testSSEph.cpp
     ext/tests/geomatics/test_EO_SOFA.cpp
     ext/tests/geomatics/test_tides.cpp
     swig/SWIGHelpers/Enums.i
     swig/SWIGHelpers/pythonfunctions.i

New Modules
-------------------------------
     core/tests/FileHandling/SEM_T.cpp
     core/tests/Geomatics/KalmanFilter_T.cpp
     core/tests/Geomatics/StatsFilter_T.cpp
     core/tests/Geomatics/testSSEph.cpp
     core/tests/Geomatics/test_EO_SOFA.cpp
     core/tests/Geomatics/test_tides.cpp
     swig/ClockModel/ClockModel.i
     swig/CodeGen/CodeGen.i
     swig/FileDirProc/FileDirProc.i
     swig/NewNav/NewNav.i
     swig/ORD/ORD.i
     swig/PosSol/PosSol.i
     swig/RefTime/RefTime.i
     swig/Rxio/Rxio.i
     swig/SWIGHelpers/cleanup.i
     swig/SWIGHelpers/renameEnums.i