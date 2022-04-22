GNSSTk 13.3.0 Release Notes
========================

 * This release includes the following:
   * Update to the Geomatics library including significant style guide changes. 
   * Update to NavDataFactoryWithStore to store TimeOffsetData in the primary store.
 * Additionally, it contains a bug fix and cmake build fix.

Updates since v13.2.0
---------------------

**Build System and Test Suite**
  * Update CMake package to be compatible with same major version.

**Library Changes**
  * Update NavDataFactoryWithStore to store TimeOffsetData in the primary store so find() can be used on it.
  * Update Geomatics libs, including some style

Fixes since v13.2.0
--------------------
  * Fix RPATH to be platform independent