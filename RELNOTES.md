GNSSTk 15.1.0 Release Notes
========================

 * This minor release includes the following:
     * Adding PreciseRange and PhaseWindup to SWIG
     * Adding nav type enums supporting BeiDou B-CNav1, B-CNav2
     * Adding B-CNav3, and regional boolean variable to GPSCNavIono class
     * Updating CommandOptionParser to use memory safe data structure
     * Updating `NewNav/OrbitDataKepler.cpp` to use the eccentric anomaly rate `dek` directly in the true anomaly rate `dlk` calculation
     * Reducing impact of ABI incompatibility
 * It also includes bug fixes, pipeline fixes, and repository CODEOWNER changes. 

Updates since v15.0.0
---------------------
  * Update/refactor swig CMakeLists.txt
  * Update CommandOptionParser to use memory safe data structure instead of C-style array.
  * Update `NewNav/OrbitDataKepler.cpp` to use the eccentric anomaly rate `dek` directly in the true anomaly rate `dlk` calculation.  This directly propagates the `dek` term for anticipated future changes to `dek` per the Jira Issue, but currently does not have any effect upon calculations.
  * Update CODEOWNERS
  * Update reducing impact of ABI incompatibility


**Library Changes**
  * Add nav type enums supporting BeiDou B-CNav1, B-CNav2, and B-CNav3
  * Add PreciseRange and PhaseWindup to SWIG
  * Add regional boolean variable to GPSCNavIono class.

Fixes since v15.0.0
--------------------
  * Fix PRSolution to not fail with GlobalTropModel edge case.
  * Fix cpp static analysis pipeline job
  * Fix pipeline job package_redhat_8_py36 to use larger gitlab runner size
  * Fix to temporarily ignore bad TimeHandling test case.
  * Fix NavID to correctly inference NavType of BeiDou's Phase III GEO SVs.

New Modules
-------------------------------
     core/lib/Geomatics/PhaseWindup.cpp
     core/lib/Geomatics/PhaseWindup.hpp
     core/tests/Geomatics/PhaseWindup_T.cpp
