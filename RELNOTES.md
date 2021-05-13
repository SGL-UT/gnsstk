GPSTk 11.1.0 Release Notes
========================

 * This version address bug fixes for the new nav store.
 * Additionally, it adds MDHTRACE debugging feature. 

Updates since v11.0.1
---------------------

**Library Changes**
  * Add DebugTrace code for execution tracing.

Fixes since v11.0.1
--------------------
  * Fix MultiFormatNavDataFactory time system handling in getInitialTime() and getFinalTime().
  * Fix NavDataFactoryWithStore static initialization problem.
  * Fix NavDataFactoryWithStore::addNavData to handle multiple time systems.
  * Fix OrbitDataKepler to properly handle Adot and dndot terms.
  * Fix PNBGPSLNavDataFactory to use unsigned eccentricity in almanac.
  * Fix CNavMidiAlm to use unsigned sqrtA.
  * Remove extraneous space in data/test_input_rinex3_nav_gal.20n  

New Modules
-------------------------------
```
core/lib/Utilities/DebugTrace.cpp
core/lib/Utilities/DebugTrace.hpp
core/tests/Utilities/DebugTrace_T.cpp
```
Known Issues since v11.0.1
-------------------------

 * Due to code fixes for the new nav store in this release, downstream dependencies
 will need to be updated to latest versions to resolve errors in unittests that compare computational results. 
 These tests may fail due to a change in reference data and not a breaking API change.