GPSTk 9.1.0 Release Notes
========================

 * This version addresses minor library updates including improved documentation
 * Additionally, it contains a CI update for advisory only protected word searches and a conda recipe fix.

Updates since v9.0.0
---------------------
**Build System and Test Suite**
  * Update build.sh Allow better error reporting

**Gitlab CI**
  * Update CI pipelines to make protected word searches advisory only.

**Library Changes**
  * Add accessors to the ObsID description maps for SWIG.
  * Add to CommandOptionParser the ability to show program usage in a format that can be used in doxygen.
  * Add TimeSystemConversion class to support generalized time system conversion.
  * Add BasicTimeSystemConversion class to wrap existing second-precision TimeSystem conversion.
  * Update across the library to improve documentation.
  * Update SatMetaDataStore to improve documentation.
  * Update EngNav decoding internals to use C++ instead of C storage
  * Update TimeSystem to support conversion to/from QZSS
  * Update CommonTime to implement changeTimeSystem method.
  * Update CommonTime/TimeSystem Redesign
  * Update various source files to add namespace context where it was missing  

Fixes since v9.0.0
--------------------
  * Fix conda recipe to specify correct default branch for GPSTk.