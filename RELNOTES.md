GNSSTk 13.6.0 Release Notes
========================

 * This release includes the following:
   * Adding callback capability to NavDataFactory classes (See New Modules below).
   * Deprecating old incompatible (nonfunctional) CNAV code (See Removed Code to Deprecation).
   * Updating PRSolution (incl. PRSolve) output covariance in the case of data from only a single epoch.
   * Updating WGS84 ref frame enums.
 * Additionally, it contains bug fixes, build and CI updates.

Updates since v13.5.1
---------------------

**Build System and Test Suite**
  * Remove usage of `nose` in testing of swig generated python.

**Gitlab CI**
  * Update CODEOWNERS file.

**Library Changes**
  * Add callback capability to NavDataFactory classes
  * Update PRSolution (incl. PRSolve) output covariance in the case of data from only a single epoch.
  * Update WGS84 ref frame enums.
  * Deprecate old incompatible (nonfunctional) CNAV code.

Fixes since v13.5.1
--------------------
  * Fix Rinex3ObsData handling of channel zero
  * Fix SWIG warnings for new OrbitData* classes

Removed Code due to Deprecation
-------------------------------
     ext/lib/GNSSEph/CNavDataElement.cpp
     ext/lib/GNSSEph/CNavDataElement.hpp
     ext/lib/GNSSEph/CNavDataElementStore.cpp
     ext/lib/GNSSEph/CNavDataElementStore.hpp
     ext/lib/GNSSEph/CNavEOP.cpp
     ext/lib/GNSSEph/CNavEOP.hpp
     ext/lib/GNSSEph/CNavGGTO.cpp
     ext/lib/GNSSEph/CNavGGTO.hpp
     ext/lib/GNSSEph/CNavISC.cpp
     ext/lib/GNSSEph/CNavISC.hpp
     ext/lib/GNSSEph/CNavReducedAlm.cpp
     ext/lib/GNSSEph/CNavReducedAlm.hpp
     ext/lib/GNSSEph/CNavText.cpp
     ext/lib/GNSSEph/CNavUTC.cpp
     ext/lib/GNSSEph/CNavUTC.hpp
     ext/lib/GNSSEph/DiffCorrBase.cpp
     ext/lib/GNSSEph/DiffCorrBase.hpp
     ext/lib/GNSSEph/DiffCorrClk.cpp
     ext/lib/GNSSEph/DiffCorrEph.cpp
     ext/lib/GNSSEph/DiffCorrEph.hpp

New Modules
-------------------------------
     core/lib/NewNav/NavDataFactoryStoreCallback.hpp
     swig/tests/test_NavDataFactoryCallback.py