GPSTk 11.2.0 Release Notes
========================

 * This release introduces Debian versioned path headers to allow multi-version support.
 * Additionally, it contains minor bug fixes and a CI change for fortify scans.

Updates since v11.1.0
---------------------

**Gitlab CI**
  * Add fortify pipeline yaml file

**Library Changes**
  * Add DebugTrace code for execution tracing.
  * Change header-file installation for debian to use versioned path names.  

Fixes since v11.1.0
--------------------
  * Fix MultiFormatNavDataFactory time system handling in getInitialTime() and getFinalTime().
  * Fix NavDataFactoryWithStore static initialization problem.
  * Fix NavDataFactoryWithStore::addNavData to handle multiple time systems.
  * Fix OrbitDataKepler to properly handle Adot and dndot terms.
  * Fix PNBGPSLNavDataFactory to use unsigned eccentricity in almanac.
  * Fix CNavMidiAlm to use unsigned sqrtA.
  * Fix pipeline yaml for protected jobs.
  * Fix VectorOperators.hpp to put math functions in std namespace.
  * Fix typo bugs in two trop models.
  * Reformat Position tests (Position_T.cpp) to follow style.
  * Remove extraneous space in data/test_input_rinex3_nav_gal.20n
  
Known Issues since v11.1.0
-------------------------
 * Due to code fixes for the new nav store in v11.1.0 release, downstream dependencies
 will need to be updated to latest versions to resolve errors in unittests that compare computational results. 
 These tests may fail due to a change in reference data and not a breaking API change.