GNSSTk 13.7.0 Release Notes
========================

 * This release includes the following:
   * Adding the ability to look up satellites by signal(s) to SatMetaDataStore.
   * Cleaning up HelmertTransform: Comply with style, fix/add tests and documentation.
   * Fixing Rinex3NavHeader to support the source specification for TIME SYSTEM CORR (see New Modules below).
 * Additionally, it contains bug fixes, build and CI updates.

Updates since v13.6.0
---------------------


**Build System and Test Suite**
  * Add SonarQube analysis

**Gitlab CI**
  * Replaced sgl_ci docker images with sgl equivalent

**Library Changes**
  * Add NewNavToRinex conversion utility class
  * Update SatMetaDataStore to properly process SIG records and look-up data via signals.

Fixes since v13.6.0
--------------------
  * Fix cmake issue with ext/swig
  * Fix Rinex3NavHeader to support the source specification for TIME SYSTEM CORR

New Modules
-------------------------------
     core/lib/NewNav/NewNavToRinex.cpp
     core/lib/NewNav/NewNavToRinex.hpp
     core/tests/NewNav/NewNavToRinex_T.cpp