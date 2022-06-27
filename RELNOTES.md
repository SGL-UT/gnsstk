GNSSTk 13.5.0 Release Notes
========================

 * This release includes the following:
   * Reorganizing Kepler-based orbit classes in NewNav (See new modules below).
     * Adding FactoryControl class for configuring aspects of NavDataFactory classes
     * Fixing NewNav OrbitDataKepler children to use appropriate ellipsoids.
     * Fixing error in decoding BDS D2 ephemeris A1 parameter.
   * Cleaning up C++ example code.
   * Updating SWIG math tests to use unittest framework.
 * Additionally, it contains bug fixes, build and CI updates.

Updates since v13.4.0
---------------------

**Build System and Test Suite**
  * Update the debian-11 to store less history.
  * Updated the linux build scripts to specify a release build type.
  * Update SWIG math tests to use unittest framework
  * Replaced cloning repo_utils with pip installed tks_ci_tools

**Gitlab CI**
  * Update CODEOWNERS to eliminate superfluous required merge approvals.
  * Update CODEOWNERS to use package reviewer groups.  

**Library Changes**
  * Add FactoryControl class for configuring aspects of NavDataFactory classes (initially, 
    adding a filter to throw out empty BDS time offset data, and another for filtering time offset data in general).

Fixes since v13.4.0
--------------------
  * Fix BinexData potential buffer overflow issues.
  * Fix NBTropModel potential null dereference issue.
  * Fix SolarSystemEphemeris potential string termination issue.
  * Fix NewNav OrbitDataKepler children to use appropriate ellipsoids.
  * Fix error in decoding BDS D2 ephemeris A1 parameter.
  * Fix Doxygen project title & index top label.
  * Clean up example code

New Modules
-------------------------------
     core/lib/NewNav/FactoryControl.hpp
     core/lib/NewNav/OrbitDataBDS.hpp
     core/lib/NewNav/OrbitDataGPS.hpp
     core/lib/NewNav/OrbitDataGal.hpp
     core/lib/NewNav/TimeOffsetFilter.cpp
     core/lib/NewNav/TimeOffsetFilter.hpp