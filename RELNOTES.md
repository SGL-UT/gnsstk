GNSSTk 15.3.0 Release Notes
========================

 * This release introduces PRSolution2, a distinct positioning-solution module with an experimental fix for APV calculation, alongside BeiDou nav-message test coverage and build/CI improvements.

Updates since v15.2.0
---------------------

**Build System and Test Suite**
  * Add BeiDou D1 Invalid Navigation Message Data Test
  * Update build script to clean install directory separately from build directory

**Gitlab CI**
  * Use reduced pkgbuild image

**Library Changes**
  * Add PackedNavBit class to SWIG
  * Add an experimental namespace with changes to fix PRSolution module APV calculation
  * Update PRSolution2 classes with names distinct from PRSolution classes
  * Add documentation for PRSolution2 migration

New Modules
-------------------------------
     core/lib/PosSol/PRSolution2.cpp
     core/lib/PosSol/PRSolution2.hpp
     core/tests/PosSol/PRSolution2_T.cpp
