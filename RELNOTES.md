GPSTk 11.0.0 Release Notes
========================

 * This version addresses the deprecation of `SVNumXRef.cpp` class.
 * Additionally, it includes minor bug fixes and an update of year 2021 to copyright header.
 
Updates since v10.0.0
---------------------

**Library Changes**
  * Removal of SVNumXRef class
 
Fixes since v10.0.0
--------------------
  * Fix StringUtils::words() for cases of empty strings or single words.
  * Fix PNBGPSCNav2DataFactory comment
  * Update source code files copyright header for year 2021
  
Removed Code due to Deprecation
-------------------------------
```
core/lib/GNSSCore/SVNumXRef.cpp
core/lib/GNSSCore/SVNumXRef.hpp
core/tests/GNSSCore/SVNumXRef_T.cpp
ext/apps/misc/CMakeLists.txt
ext/apps/misc/SVNumXRefDUMP.cpp
```