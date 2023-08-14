GNSSTk 14.2.0 Release Notes
========================

 * This release includes the following:
   * Changes in support of CSMS. See New Modules below.
   * Add GPS SV config message to NewNav
   * Add support for norad IDs in SatID for those compilers that support c++17 or later
   * Co-installation of major versions of debian packages.
 * Additionally, it contains several bug fixes and build system updates.

**Build System and Test Suite**
  * Fix gnsstk data submodule reference
  * Update build.sh to opportunistically use Ninja
  * Update create debian/ubuntu major version coinstallable package

**Library Changes**
  * Add support for specifying which c++ standard to use when compiling with g++
  * Add GPS SV config message to NewNav
  * Add support for norad IDs in SatID for those compilers that support c++17 or later
  * Update BDSD?NavEph.cpp Modified end fit determination
  * Changes in support of CSMS

Fixes since v14.1.0
--------------------
  * Fix SEM NavData system value
  * Fix SEMNavDataFactory orbital inclination rate of change
  * Fix time offset sign convention to be consistent between BasicTimeSystemConverter and the NavTimeSystemConverter.
  * Fix a few sign convention changes

New Modules
-------------------------------
     core/lib/GNSSCore/SatTimeSystem.cpp
     core/lib/GNSSCore/SatTimeSystem.hpp
     core/lib/NewNav/GPSNavConfig.cpp
     core/lib/NewNav/GPSNavConfig.hpp
     core/lib/NewNav/GPSSVConfig.cpp
     core/lib/NewNav/GPSSVConfig.hpp
     core/lib/NewNav/SystemNavData.hpp
     core/tests/GNSSCore/SatelliteSystem_T.cpp
     core/tests/NewNav/GPSNavConfig_T.cpp
     core/tests/NewNav/GPSSVConfig_T.cpp
     swig/tests/test_GPSNavConfig.py