GNSSTk 14.0.0 Release Notes
========================

 * This release introduces a major update to the toolkit.
 * It includes the following:
   * Refactoring HelmertTransform into HelmertTransformer and reference frames into RefFrame.
   * Adding signal details to nav dump methods.
   * Adding group path delay calculator GroupPathCorr and related classes. (See New Modules below).
   * Refactoring duplicate raw range implementations into a single class.
   * Moving NewNav enums to the namespace level for consistency.
   * Deprecating support for Debian 9.
   * Adding support for Ubuntu20.04 (focal)
 * Additionally, it contains minor library updates and bug fixes

Updates since v13.8.0
---------------------

**Build System and Test Suite**
  * Update the clean build parameter
  * Update SWIG cmake rules now that we're no longer supporting cmake version 2
  * Update compiler to Visual Studio 2019 in the Windows build scripts.

**Gitlab CI**
  * Update Deprecated Debian 9 build
  * Add Ubuntu 20.04 pipeline jobs

**Library Changes**
  * Add group path delay calculator (GroupPathCorr and related classes).
  * Update Refactor HelmertTransform into HelmertTransformer
  * Update Refactor reference frames into RefFrame
  * Update the nonsensical ISC interface in NavLibrary with one that does make sense.
  * Update Refactor duplicate raw range implementations into a single class.
  * Update group path corrector navLib from shared_ptr to reference to work around swig/python problem
  * Update Move NewNav enums to the namespace level for consistency
  * Update NewNav docs
  * Add NavData::clone method
  * Add signal details to nav dump methods
  * Add group path delay calculator (GroupPathCorr and related classes).
  * Add a CorrectorType that was missed

Fixes since v13.8.0
--------------------
  * Fix swig build error

Removed Code due to Deprecation
-------------------------------
     core/lib/GNSSEph/GloEphemeris.cpp
     core/lib/GNSSEph/GloEphemeris.hpp

New Modules
-------------------------------
     core/lib/FileHandling/MetReader.hpp
     core/lib/GNSSCore/BCISCorrector.cpp
     core/lib/GNSSCore/BCISCorrector.hpp
     core/lib/GNSSCore/BCIonoCorrector.cpp
     core/lib/GNSSCore/BCIonoCorrector.hpp
     core/lib/GNSSCore/CorrDupHandling.cpp
     core/lib/GNSSCore/CorrDupHandling.hpp
     core/lib/GNSSCore/CorrectionResult.hpp
     core/lib/GNSSCore/CorrectionResults.cpp
     core/lib/GNSSCore/CorrectionResults.hpp
     core/lib/GNSSCore/CorrectorType.cpp
     core/lib/GNSSCore/CorrectorType.hpp
     core/lib/GNSSCore/GroupPathCorr.cpp
     core/lib/GNSSCore/GroupPathCorr.hpp
     core/lib/GNSSCore/GroupPathCorrector.hpp
     core/lib/GNSSCore/HelmertTransformer.cpp
     core/lib/GNSSCore/HelmertTransformer.hpp
     core/lib/GNSSCore/RefFrame.cpp
     core/lib/GNSSCore/RefFrame.hpp
     core/lib/GNSSCore/RefFrameRlz.cpp
     core/lib/GNSSCore/RefFrameRlz.hpp
     core/lib/GNSSCore/RefFrameSys.cpp
     core/lib/GNSSCore/RefFrameSys.hpp
     core/lib/GNSSCore/TransformLibrary.cpp
     core/lib/GNSSCore/TransformLibrary.hpp
     core/lib/GNSSCore/Transformer.hpp
     core/lib/GNSSCore/TropCorrector.hpp
     core/lib/GNSSEph/RawRange.cpp
     core/lib/GNSSEph/RawRange.hpp
     core/lib/NewNav/GLOFNavPCode.cpp
     core/lib/NewNav/GLOFNavPCode.hpp
     core/lib/NewNav/GLOFNavSatType.hpp
     core/lib/NewNav/GPSLNavL2Codes.cpp
     core/lib/NewNav/GPSLNavL2Codes.hpp
     core/tests/FileHandling/MetReader_T.cpp
     core/tests/GNSSCore/BCISCorrector_T.cpp
     core/tests/GNSSCore/BCIonoCorrector_T.cpp
     core/tests/GNSSCore/CorrDupHandling_T.cpp
     core/tests/GNSSCore/CorrectionResults_T.cpp
     core/tests/GNSSCore/CorrectorType_T.cpp
     core/tests/GNSSCore/GroupPathCorr_T.cpp
     core/tests/GNSSCore/GroupPathCorrector_T.cpp
     core/tests/GNSSCore/HelmertTransformer_T.cpp
     core/tests/GNSSCore/RawRange_T.cpp
     core/tests/GNSSCore/RefFrameRlz_T.cpp
     core/tests/GNSSCore/RefFrameSys_T.cpp
     core/tests/GNSSCore/TransformLibrary_T.cpp
     core/tests/GNSSCore/TropCorrector_T.cpp
     core/tests/Geomatics/PreciseRange_T.cpp
     swig/SWIGHelpers/std_tuple.i
     swig/tests/__init__.py