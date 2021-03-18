GPSTk 11.0.0 Release Notes
========================

 * This version addresses the deprecation of `SVNumXRef.cpp` class.
 * It also includes deprecation of external user's code. 
 * Additionally, it includes minor bug fixes and an update of year 2021 to copyright header.
 
Updates since v10.0.0
---------------------

**Library Changes**
  * Removal of SVNumXRef class
  * Removal of external user's classes.
 
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

Removed External User's Code due to Deprecation
-----------------------------------------------
```
core/lib/AppFrame/MainAdapter.hpp
core/lib/TimeHandling/EpochDataStore.cpp
core/lib/TimeHandling/EpochDataStore.hpp
ext/lib/AstroEph/AstronomicalFunctions.cpp
ext/lib/AstroEph/AstronomicalFunctions.hpp
ext/lib/AstroEph/MoonPosition.cpp
ext/lib/AstroEph/MoonPosition.hpp
ext/lib/AstroEph/PlanetEphemeris.cpp
ext/lib/AstroEph/PlanetEphemeris.hpp
ext/lib/AstroEph/SunPosition.cpp
ext/lib/AstroEph/SunPosition.hpp
ext/lib/GNSSCore/SolidTides.cpp
ext/lib/GNSSCore/SolidTides.hpp
ext/lib/GNSSEph/EOPDataStore.cpp
ext/lib/GNSSEph/EOPDataStore.hpp
ext/lib/Math/BaseDistribution.hpp
ext/lib/Math/Chi2Distribution.cpp
ext/lib/Math/Chi2Distribution.hpp
ext/lib/Math/FIRDifferentiator5thOrder.cpp
ext/lib/Math/FIRDifferentiator5thOrder.hpp
ext/lib/Math/FilterBase.hpp
ext/lib/Math/GaussianDistribution.cpp
ext/lib/Math/GaussianDistribution.hpp
ext/lib/Math/SpecialFunctions.cpp
ext/lib/Math/SpecialFunctions.hpp
ext/lib/Math/StudentDistribution.cpp
ext/lib/Math/StudentDistribution.hpp
```