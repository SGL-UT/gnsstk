GNSSTk 14.4.0 Release Notes
========================

 * This release includes the following:
   * Adding reference epoch support in constructor of `MultiFormatNavDataFactory` and `SEMNavDataFactory`
   * Adding C++ exception support to Geomatics.
 * Additionally, it contains several bug fixes and build system updates.

Updates since v14.3.0
---------------------

**Build System and Test Suite**
  * Fix intermittent test failure when using BasicTimeSystemConverter::getOffset

**Gitlab CI**
  * Update allow windows dll pipeline test job to fail so it doesn't short circuit the entire toolkit build chain.

**Library Changes**
  * Add C++ exception support to Geomatics.
  * Add reference epoch support in constructor of `MultiFormatNavDataFactory` and `SEMNavDataFactory`

Fixes since v14.3.0
--------------------
  * Fix NBGLOFNavDataFactory Correct Reference Times
  * Add STAKEHOLDERS document with stakeholder information

New Modules
-------------------------------
     swig/tests/test_exceptions.py
