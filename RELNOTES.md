GNSSTk 13.8.0 Release Notes
========================

 * This release includes the following:
   * Updating documentation of TropModel classes.
   * Spliting test data into gnsstk-data repo with submodules configured.
   * Updating sphinx documentation of SWIG bindings.
   * Adding support for GLONASS CDMA nav data processing (See New Modules below)
 * Additionally, it contains bug fixes, build and CI updates.

Updates since v13.7.0
---------------------

**Build System and Test Suite**
  * Update Changed the minimum cmake version
  * Fix Remove SystemTime tests that randomly fail
  * Fix ORD tests to use the GNSSTk testing framework

**Gitlab CI**
  * Update CODEOWNERS.
  * Fix pipeline push artifacts

**Library Changes**
  * Update change string name in StringUtils to avoid conflict with curses library macro
  * Update exception specifications from throw() to noexcept
  * Update documentation of TropModel classes
  * Update move nav message bit definition enums out of PNBNavDataFactory.cpp files and into their own files.
  * Add documentation on raw range (geometric range) computations
  * Add Split test data into gnsstk-data repo with submodules configured
  * Add support for GLONASS CDMA nav data processing
  * Update sphinx documentation of SWIG bindings
  * Update Deprecate extraneous swig binding for function that no longer exists.

Fixes since v13.7.0
--------------------
  * Fix week rollover issues in GPS LNav, Galileo F/Nav and Galileo I/Nav decoders.
  * Fix bugs in GLONASS CDMA nav data processing

Removed Code due to Deprecation
-------------------------------
     core/tests/ORD/OrdMockClasses.hpp
     swig/sphinx/sphinxsetup.py

New Modules
-------------------------------
     core/lib/NewNav/GLOCBits.hpp
     core/lib/NewNav/GLOCNavAlm.cpp
     core/lib/NewNav/GLOCNavAlm.hpp
     core/lib/NewNav/GLOCNavAlmCorrected.hpp
     core/lib/NewNav/GLOCNavAlmDeltas.hpp
     core/lib/NewNav/GLOCNavAlmNumberCruncher.hpp
     core/lib/NewNav/GLOCNavAlmUncorrected.hpp
     core/lib/NewNav/GLOCNavData.cpp
     core/lib/NewNav/GLOCNavEph.cpp
     core/lib/NewNav/GLOCNavEph.hpp
     core/lib/NewNav/GLOCNavHeader.cpp
     core/lib/NewNav/GLOCNavHeader.hpp
     core/lib/NewNav/GLOCNavHealth.cpp
     core/lib/NewNav/GLOCNavHealth.hpp
     core/lib/NewNav/GLOCNavIono.cpp
     core/lib/NewNav/GLOCNavIono.hpp
     core/lib/NewNav/GLOCNavLTDMP.cpp
     core/lib/NewNav/GLOCNavLTDMP.hpp
     core/lib/NewNav/GLOCNavUT1TimeOffset.cpp
     core/lib/NewNav/GLOCNavUT1TimeOffset.hpp
     core/lib/NewNav/GLOCOrbitType.cpp
     core/lib/NewNav/GLOCOrbitType.hpp
     core/lib/NewNav/GLOCRegime.cpp
     core/lib/NewNav/GLOCRegime.hpp
     core/lib/NewNav/GLOCSatType.cpp
     core/lib/NewNav/GLOCSatType.hpp
     core/lib/NewNav/GPSC2Bits.hpp
     core/lib/NewNav/GPSCBits.hpp
     core/lib/NewNav/GPSLBits.hpp
     core/lib/NewNav/GalFBits.hpp
     core/lib/NewNav/GalIBits.hpp
     core/lib/NewNav/NavFit.hpp
     core/lib/NewNav/PNBGLOCNavDataFactory.cpp
     core/lib/NewNav/PNBGLOCNavDataFactory.hpp
     core/lib/TimeHandling/GLONASSTime.cpp
     core/lib/TimeHandling/GLONASSTime.hpp
     core/tests/NewNav/GLOCNavAlm_T.cpp
     core/tests/NewNav/GLOCNavEph_T.cpp
     core/tests/NewNav/GLOCNavHealth_T.cpp
     core/tests/NewNav/GLOCNavLTDMP_T.cpp
     core/tests/NewNav/GLOCNavTestDataDecl.hpp
     core/tests/NewNav/GLOCNavTestDataDef.hpp
     core/tests/NewNav/GLOCNavUT1TimeOffset_T.cpp
     core/tests/NewNav/PNBGLOCNavDataFactory_T.cpp
     core/tests/TimeHandling/GLONASSTime_T.cpp