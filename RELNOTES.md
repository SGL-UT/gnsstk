GNSSTk 12.1.0 Release Notes
========================
 * This release introduces minor updates to the toolkit.
 * It includes some of the following:
   * Additional constants for BeiDou and FREQ_STEP_GLONASS_* constants to FreqConsts.hpp
   * Updated SolarPosition SWig bindings to map output data
   * Updated copyright header
 * Additionally, it contains bug fixes and CI updates.

Updates since v12.0.0
---------------------

**Build System and Test Suite**
  * Update conda recipe to use local src already checked out.

**Gitlab CI**
  * Update pipeline retry job setting
  * Update pipeline to use new windows VM instance

**Library Changes**
  * Add some constants for BeiDou.
  * Add PNBNavDataFactory code snippet to newnav docs
  * Add explicit debian dependency fom python-gnsstk to python-enum34.
  * Add "Mixed" satellite system to RinexSatID translation to strings.
  * Add FREQ_STEP_GLONASS_* constants to FreqConsts.hpp
  * Update copyright statement.
  * Update SolarPosition SWIG bindings to map output data.

Fixes since v12.0.0
--------------------
  * Fix RINEX 3 nav headers to preserve leading spaces in comments
  * Fix ClockSatStore where it was interpolating clock drift instead of drift rate.
  * Fix SEMNavDataFactory so it properly sets a timestamp of the data
  * Fix PNBMultiGNSSNavDataFactory so it knows about BeiDou D2
  * Fix PNBBDSD2NavDataFactory so it doesn't dump core if subframe 1 page 2 is missing
  * Update Remove embedded libgnsstk.so from python

New Modules
-------------------------------
     core/lib/NewNav/BDSD1Bits.hpp
     core/lib/NewNav/BDSD2Bits.hpp
     swig/Geomatics/SolarPosition.i
