GNSSTk 14.5.0 Release Notes
========================

 * This release includes the following:
   * Adding `isSameData` method for several subclasses.
   * Adding test coverage to Epoch clock model.
   * Adding missing include to `ObsID.hpp`.
   * Adding findAll MultiFormatNavDataFactory Support.

Updates since v14.4.1
---------------------

**Build System and Test Suite**
  * Add test coverage to Epoch clock model

**Gitlab CI**
  * Update gnsstk-data submodule refs

**Library Changes**
  * Add isSameData functionality to the root and branches of the NavData
  * Add isSameData for GPSLNav subclasses
  * Add isSameData for GPSCNav subclasses
  * Add isSameData for BDSD1 subclasses
  * Add isSameData for GPSLNav subclasses
  * Add isSameData for GPSLNav subclasses
  * Add isSameData for GALINav subclasses
  * Add isSameData for GLOCNav subclasses
  * Add isSameData for GLOFNav subclasses
  * Add isSameData for GPSCNav2 subclasses
  * Add findAll functionality to NavDataFactoryWithStore
  * Add Geomatics/testSSEph.cpp to test suite to improve code coverage
  * Add findAll MultiFormatNavDataFactory Support
  * Add missing include to ObsID.hpp

New Modules
-------------------------------
     core/tests/Geomatics/convertSSEph.cpp
