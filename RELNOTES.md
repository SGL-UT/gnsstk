GNSSTk 15.2.0 Release Notes
========================

 * This minor release includes the following:
     * Adding RefFrameRlz missing G2296 realization of WGS 84
     * Fixing PNBGPSCNavDataFactory Add_MT_assembly_checks
     * Fixing RefFrameRlz incorrect reference frame adoption dates for WGS 84 and ITRF realizations.
     * Updating `NewNav/OrbitDataKepler.cpp` with changes to the eccentric anomaly rate `dek` specified in the Proposed Change Notice to IS-GPS-200 as of 2025-08-27.
 * It also includes the following CI/CD updates:
     * Fixing conda pkg pipeline job by pinning version of conda glibc to 2.28
     * Updating fortify scan results with filtering
     * Fixing build.sh script to remove non-existent ctest `v` flag.

Updates since v15.1.0
---------------------

**Build System and Test Suite**
  * Update fortify scan to exclude ext libraries.
  * Update fortify_filter-issues.txt Adding files.
  * Update fortify-filter-issues.txt Adding verbiage explaining fortify exclusions
  * Fix build.sh to remove non-existent ctest `-v` flag.

**Gitlab CI**
  * Fix conda pkg pipeline job by pinning version of conda glibc to 2.28

**Library Changes**
  * Add RefFrameRlz missing G2296 realization of WGS 84.
  * Update `NewNav/OrbitDataKepler.cpp` with changes to the eccentric anomaly rate `dek` specified in the Proposed Change Notice to IS-GPS-200 as of 2025-08-27.

Fixes since v15.1.0
--------------------
  * Fix PNBGPSCNavDataFactory Add_MT_assembly_checks
  * Fix RefFrameRlz incorrect reference frame adoption dates for WGS 84 and ITRF realizations.

