GNSSTk 13.1.0 Release Notes
========================
 * This release introduces a new operator `!=` for gnsstk:ValidType.
 * Additionally, it contains bug fixes, a test fix,  and a CI update.

Updates since v13.0.0
---------------------

**Build System and Test Suite**
  * Update SystemTime test so it doesn't fail so often and so it meets style rules

**Gitlab CI**
  * Update moved windows cleanup script to ci_control repo.

**Library Changes**
  * Add gnsstk::ValidType::operator!=()

Fixes since v13.0.0
--------------------
  * Fix python docstring generator from Doxygen output
  * Fix Rinex3NavData to right-align satellite IDs when writing RINEX 2.
  * Fix Rinex3NavDataOperatorLessThanFull's broken sorting.
  * Fix TestUtil::fileCompTest to properly detect EOF under Windows.
  * Fix Rinex3ObsStream so the object can be reused.
  * Fix SP3NavDataFactory clock linear interpolation half-order
  * Update BDS TimeOffset validate() so any one term can be 0 but not both