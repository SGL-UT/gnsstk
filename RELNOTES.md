Known Issues
------------

SWIG 3.0.10 is not supported for generating Python bindings, due to
errors in code generation in that version.  This is the version of
SWIG that ships with Debian 9 (stretch).  Compilation will fail when
using SWIG 3.0.10.  If you need Python wrappers for this code, please
use a different version of SWIG (2.0.12 and 3.0.12 have been tested
successfully).

GPSTk 10.0.0 Release Notes
========================

 * This version addresses "Ephemeris Store Redesign" by implementing for a new set of classes `NewNav` for storing parsed navigation messages.
 * Additionally, it contains minor library updates including "CommonTime/TimeSystem Redesign `RefTime`" and bug fixes.

Updates since v9.1.0
---------------------

**Library Changes**
  * Add ObsID support for GLONASS frequency offset and mcode metadata.
  * Add SatID wildcard support for system and ID.
  * Add NavType wildcard value.
  * Add PackedNavBits support for decoding more types.
  * Add NewNav code tree to replace existing nav store classes (in core/lib/NewNav and ext/lib/NewNav).
  * Add TimeCorrection support for 8-bit week rollover correction.
  * Add NavTimeSystemConverter that uses a NavLibrary object to get nav-sourced time offset data.
  * Add CommonTime changeTimeSystem method that uses a static TimeSystemConverter.
  * Add TimeTag changeTimeSystem methods for both static and specified TimeSystemConverter objects.
  * Add RinexNavDataFactory support of time offset data in RINEX NAV header.
  * Update Removed obsolete files Bancroft.*
  * Update TimeTag_T.cpp to follow proper style
  * Update Doxyfile to generate graphs by default.
  * Update CommandOption to allow changing the description post-constructor.
  * Update TestUtil to print invalid boolean values as 'true' or 'false' and increased precision when printing invalid floating point values.
  * Update GPSWeekZcount to support printing day of week by name.
  * Update SWIG to remove pseudo-mapping that conflicts with SatID constructor changes.
  * Update bc2sp3 to use Frameworks for better command line option help.
  * Update sp3version to use Frameworks for better command line option help.
  * Update TimeSystemCorr_T to use test macros and such for improved readability.
  * Update TimeSystemCorrection (RINEX3 NAV header) to store reference time in CommonTime.
  * Update Removed obsolete files
  * Update MultiFormatNavDataFactory to implement getOffset(), getInitialTime() and getFinalTime()
  * Update NavDataFactory::getFactoryFormats() methods to prevent printing formats that don't support the desired message types.
  * Update RINEX NAV test data files that had improperly formatted DELTA-UTC records.

Fixes since v9.1.0
--------------------
  * Fix OrbAlmGen to decode A**1/2 as unsigned rather than signed.
  * Fix NavDataFactoryWithStore infinite loop bug in find.
  * Fix bug in WindowFilter due to uninitialized boolean.
  * Fix NavDataFactory classes to enforce proper initialization and destruction.
  * Fix "implement proper command line option help where needed"
  * Fix test binaries to be excluded from bin
  * Fix NavDataFactoryWithStore::getOffset() so it doesn't fail when reversing the offset look-up (e.g. GPS->UTC vs UTC->GPS).
  * Fix NavDataFactoryWithStore::edit() and clear() to process time offset data.
  
Removed Code due to Deprecation
```
ext/lib/PosSol/Bancroft.cpp
ext/lib/PosSol/Bancroft.hpp
```

New Modules
-------------------------------
```
core/lib/NewNav/*.cpp
core/lib/NewNav/*.hpp
ext/lib/NewNav/SEMNavDataFactory.cpp
ext/lib/NewNav/SEMNavDataFactory.hpp
ext/lib/NewNav/YumaNavDataFactory.cpp
ext/lib/NewNav/YumaNavDataFactory.hpp
core/lib/RefTime/*.cpp
core/lib/RefTime/*.hpp
```
