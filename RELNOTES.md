GNSSTk 14.3.0 Release Notes
========================

 * This release includes the following:
   * Adding the ability for SP3NavDataFactory to initialize OrbitDataSP3 fields to NaN or whatever.
   * Adding Galileo I/NAV page pair parsing to PNB factory.
   * Adding addBitVec method to PackedNavBits to allow easy packing of an array of 0/1 ints.
 * Additionally, it contains several bug fixes and build system updates.

Updates since v14.2.0
---------------------

**Build System and Test Suite**
  * Add tests for sem and al3 files (See New Modules below).
  * Update gnsstk_enable.sh to be POSIX compliant for wider support.
  * Update Removing special c++17 packages as they are no longer needed.

**Gitlab CI**
  * Update codeowners file to replace contributors that left

**Library Changes**
  * Add the ability for SP3NavDataFactory to initialize OrbitDataSP3 fields to NaN or whatever
  * Add Galileo I/NAV page pair parsing to PNB factory.
  * Add addBitVec method to PackedNavBits to allow easy packing of an array of 0/1 ints.

Fixes since v14.2.0
--------------------
  * Fix bug in PackedNavBits.addDataVec that would drop the last byte of data.
  * Fix PNBGPSCNavDataFactory.cpp Fix weekrollover problem.
  * Fix PackedNavBits segfaulting when add more bits beyond its initial capacity.
  * Removed c++17 features due to segfaults.

New Modules
-------------------------------
     core/tests/NewNav/NavLibrarySEM_T.cpp
     core/tests/NewNav/NavLibraryYuma_T.cpp