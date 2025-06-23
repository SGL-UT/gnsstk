GNSSTk 15.0.0 Release Notes
========================

 * This release introduces a major update to the toolkit.
 * It includes the following:
   * Adding std C++17 support while deprecating std C++11 support. (**api breaking**)
   * Updating MultiFormatNavDataFactory and PNBMultiGNSSNavDataFactory to allow multiple independent instances. (**api breaking**)
   * Updating  GNSSconstants Align PI, TWO_PI, SQRT_PI to the exact specification of the IS-GPS-200
 * Additionally, it contains bug fixes updates and build CI/CD updates.

Updates since v14.6.0
---------------------

**Known Issues**
  * Some distributions (such as RHEL 8) may support different versions of gcc (such as gcc 8.x and 9.x but gcc 8.x is the default). A version of gcc (such as gcc 9 on RHEL 8) that supports C++17 and the C++17 ABI is stable must be chosen to build. Note that gcc 8.x on RHEL 8 may support some C++17 but the C++17 ABI is not stable and thus should not be used.
  * Support for MSVC14 (Microsoft Visual Studio 14) and older compilers is removed. Only Microsoft Visual Studio 19 and newer versions are supported.

**Build System and Test Suite**
  * Fix df_diff comparisons, off-by-one error, and cmake test usage of df_diff.

**Gitlab CI**
  * Update Fortify pipeline scan job
  * Update submodule ref to point to latest gnsstk-data merge
  * Fix gitlab pipeline jobs retry for only system failures.
  * Fix Fortify pipeline retry condition

**Library Changes**
  * Add C++17 support for Toolkits
  * Update MultiFormatNavDataFactory and PNBMultiGNSSNavDataFactory to allow multiple independent instances.
  * Update GNSSconstants Align PI, TWO_PI, SQRT_PI to the exact specification of the IS-GPS-200

Fixes since v14.6.0
--------------------
  * Fix EngNav such that subframe pattern ID look-ups disallow unassigned SV ID values.
  * Fix PNBBDSD1NavDataFactory Change SOW cracking from asSignedDouble() to asUnsignedInt()
  * Fix SNAPPER Check ptr for null before calling string constructor. Running SNAPPER with lambda in cloud doesn't have the env variable $HOME by default
  * Fix GPSLNavEph fit interval computation.
  * Fix correct destruction of NavData with virtual destructor

Removed Code due to Deprecation
-------------------------------
  * No longer build and publish packages with std C++11

New Modules
-------------------------------
     core/tests/GNSSCore/GNSSconstants_T.cpp
