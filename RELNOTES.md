GNSSTk 13.0.0 Release Notes
========================

 * This release introduces a major update to the toolkit.
 * It includes the following:
   * Final deprecation of old C++ Nav classes (see list below)
   * Addition of GLONASS NewNav classes (see list below)
   * Addition of NewNav support for BeiDou data from RINEX
   * Fixing windows' destructor order by using a shared_ptr
   * Python2 is no longer supported for our platform packages.
 * Additionally, it contains minor library updates and bug fixes

Updates since v12.1.0
---------------------

**Build System and Test Suite**
   *  Add Store CI artifacts in artifactory.
   *  Update debian control files to build python3 bindings, instead of python2.
   *  Update test_GPSLNavEph.py to reflect changes in GPSLNavEph getUserTime implementation
   *  Update python3 detection for swig bindings.
   *  Add profiler support to build process.
   *  Update build.sh to use python3 by default instead of python2
   *  Update debian package python3-gnsstk to explicitly conflict with python-gnsstk
   *  Fix cmake when installing gnsstk swig bindings

**Gitlab CI**
   *  Add windows packaging job

**Library Changes**
   *  Update Refactor ORDs code to use newnav
   *  Add MultiFormatNavDataFactory::getFactory() method.
   *  Add NavDataFactory::getIndexSet() methods.
   *  Add NavLibrary::getIndexSet() methods.
   *  Update how initial/finalTime are set in NavDataFactoryWithStore.
   *  Add option for regular expression exclusion in df_diff.
   *  Add a "Terse" dump type for NewNav
   *  Add GPSLNavEph xmit2/xmit3 fields for transmit time of SF2/3
   *  Update NewNav dump method updates for consistency.
   *  Update NewNav debug output to use DEBUGTRACE instead of cerr
   *  Update SWIG stuff due to changes in #includes
   *  Add ObsID constructor that avoids wildcards.
   *  Update SP3NavDataFactory for performance improvements.
   *  Add option to df_diff to ignore specific columns of matching lines
   *  Add Compare classes for unique-ifying/sorting GPSLNavEph
   *  Update wavelength constants to be computed via c/freq
   *  Add minimal (mostly unimplemented except for select classes) isSameData and compare methods to the NavData tree
   *  Add getIndexSet, getNavMap methods to NavDataFactoryWithStore
   *  Add support for Terse mode dumps to NavDataFactoryWithStore
   *  Add method for determining if a BeiDou satellite should be GEO
   *  Update SWIG does not support c++11 type aliasing 'using' until 3.0.11. Debian 9 has swig 3.0.10
   *  Add GLONASS NewNav classes
   *  Update Rename IonoData to IonoNavData
   *  Update Rename KlobucharIonoData to KlobucharIonoNavData
   *  Update Rename NeQuickIonoData to NeQuickIonoNavData
   *  Update Rename IonoData::getCorrection to getIonoCorr
   *  Update Remove numerous deprecated classes
   *  Update Refactor SunEarthSatGeometry from ext to core, comply with style guide.
   *  Fixing windows' destructor order by using a shared_ptr

Fixes since v12.1.0
--------------------
   *  Fix YumaNavDataFactory problem with reading Yuma nav files
   *  Fix ObsID incorrect comment in constructor docs.
   *  Fix (remove) redundant/misleading docstring paragraph.
   *  Fix FileSpec text handling
   *  Fix loadIntoMap methods of the NavDataFactory tree to properly fill in the desired data maps
   *  Fix Reset times when NavDataFactory is cleared
   *  Fix CommonTime/TimeTag/etc. time offset to subtract instead of add.
   *  Fix KlobucharIonoNavData day rollover check
   *  Update copyright header to year 2022

New Modules
-------------------------------
     core/lib/Geomatics/SunEarthSatGeometry.cpp
     core/lib/Geomatics/SunEarthSatGeometry.hpp
     core/lib/NewNav/GLOFBits.hpp
     core/lib/NewNav/GLOFNavAlm.cpp
     core/lib/NewNav/GLOFNavAlm.hpp
     core/lib/NewNav/GLOFNavData.cpp
     core/lib/NewNav/GLOFNavData.hpp
     core/lib/NewNav/GLOFNavEph.cpp
     core/lib/NewNav/GLOFNavEph.hpp
     core/lib/NewNav/GLOFNavHealth.cpp
     core/lib/NewNav/GLOFNavISC.hpp
     core/lib/NewNav/GLOFNavUT1TimeOffset.cpp
     core/lib/NewNav/GLOFNavUT1TimeOffset.hpp
     core/lib/NewNav/PNBGLOFNavDataFactory.cpp
     core/lib/NewNav/PNBGLOFNavDataFactory.hpp
     core/tests/NewNav/GLOFNavAlm_T.cpp
     core/tests/NewNav/GLOFNavEph_T.cpp
     core/tests/NewNav/GLOFNavHealth_T.cpp
     core/tests/NewNav/GLOFNavISC_T.cpp
     core/tests/NewNav/GLOFNavTimeOffset_T.cpp
     core/tests/NewNav/GLOFNavUT1TimeOffset_T.cpp

Removed Code due to Deprecation
-------------------------------
     core/lib/GNSSEph/BDSEphemeris.cpp
     core/lib/GNSSEph/BDSEphemeris.hpp
     core/lib/GNSSEph/ClockSatStore.cpp
     core/lib/GNSSEph/ClockSatStore.hpp
     core/lib/GNSSEph/GPSEphemeris.cpp
     core/lib/GNSSEph/GPSEphemeris.hpp
     core/lib/GNSSEph/GPSEphemerisStore.cpp
     core/lib/GNSSEph/GPSEphemerisStore.hpp
     core/lib/GNSSEph/GalEphemeris.cpp
     core/lib/GNSSEph/GalEphemeris.hpp
     core/lib/GNSSEph/GloEphemerisStore.cpp
     core/lib/GNSSEph/GloEphemerisStore.hpp
     core/lib/GNSSEph/OrbElem.cpp
     core/lib/GNSSEph/OrbElem.hpp
     core/lib/GNSSEph/OrbElemBase.cpp
     core/lib/GNSSEph/OrbElemBase.hpp
     core/lib/GNSSEph/OrbElemRinex.cpp
     core/lib/GNSSEph/OrbElemRinex.hpp
     core/lib/GNSSEph/OrbElemStore.cpp
     core/lib/GNSSEph/OrbElemStore.hpp
     core/lib/GNSSEph/OrbitEph.cpp
     core/lib/GNSSEph/OrbitEph.hpp
     core/lib/GNSSEph/OrbitEphStore.cpp
     core/lib/GNSSEph/OrbitEphStore.hpp
     core/lib/GNSSEph/PositionRecord.cpp
     core/lib/GNSSEph/PositionSatStore.cpp
     core/lib/GNSSEph/PositionSatStore.hpp
     core/lib/GNSSEph/QZSEphemeris.cpp
     core/lib/GNSSEph/QZSEphemeris.hpp
     core/lib/GNSSEph/RationalizeRinexNav.cpp
     core/lib/GNSSEph/RationalizeRinexNav.hpp
     core/lib/GNSSEph/Rinex3EphemerisStore.cpp
     core/lib/GNSSEph/Rinex3EphemerisStore.hpp
     core/lib/GNSSEph/RinexEphemerisStore.cpp
     core/lib/GNSSEph/SP3EphemerisStore.cpp
     core/lib/GNSSEph/SP3EphemerisStore.hpp
     core/lib/GNSSEph/TabularSatStore.hpp
     core/lib/GNSSEph/XvtStore.hpp
     core/tests/GNSSEph/BDSEphemeris_T.cpp
     core/tests/GNSSEph/GPSEphemerisStore_T.cpp
     core/tests/GNSSEph/GalEphemeris_T.cpp
     core/tests/GNSSEph/GloEphemerisStore_T.cpp
     core/tests/GNSSEph/OrbElemLNav_valid_T.cpp
     core/tests/GNSSEph/OrbElemStore_T.cpp
     core/tests/GNSSEph/OrbElem_T.cpp
     core/tests/GNSSEph/OrbitEphStore_T.cpp
     core/tests/GNSSEph/OrbitEph_T.cpp
     core/tests/GNSSEph/RinexEphemerisStore_T.cpp
     core/tests/GNSSEph/SP3EphemerisStore_T.cpp
     ext/lib/GNSSEph/BDSEphemerisStore.cpp
     ext/lib/GNSSEph/BDSEphemerisStore.hpp
     ext/lib/GNSSEph/CNavMidiAlm.cpp
     ext/lib/GNSSEph/CNavMidiAlm.hpp
     ext/lib/GNSSEph/GPSAlmanacStore.cpp
     ext/lib/GNSSEph/GPSAlmanacStore.hpp
     ext/lib/GNSSEph/GPSOrbElemStore.cpp
     ext/lib/GNSSEph/GalEphemerisStore.cpp
     ext/lib/GNSSEph/GalEphemerisStore.hpp
     ext/lib/GNSSEph/OrbAlm.hpp
     ext/lib/GNSSEph/OrbAlmFactory.cpp
     ext/lib/GNSSEph/OrbAlmFactory.hpp
     ext/lib/GNSSEph/OrbAlmGen.cpp
     ext/lib/GNSSEph/OrbAlmGen.hpp
     ext/lib/GNSSEph/OrbAlmStore.cpp
     ext/lib/GNSSEph/OrbAlmStore.hpp
     ext/lib/GNSSEph/OrbData.cpp
     ext/lib/GNSSEph/OrbData.hpp
     ext/lib/GNSSEph/OrbDataSys.hpp
     ext/lib/GNSSEph/OrbDataSysFactory.cpp
     ext/lib/GNSSEph/OrbDataSysFactory.hpp
     ext/lib/GNSSEph/OrbDataUTC.hpp
     ext/lib/GNSSEph/OrbSysGpsC_30.cpp
     ext/lib/GNSSEph/OrbSysGpsC_30.hpp
     ext/lib/GNSSEph/OrbSysGpsC_32.cpp
     ext/lib/GNSSEph/OrbSysGpsC_32.hpp
     ext/lib/GNSSEph/OrbSysGpsC_33.cpp
     ext/lib/GNSSEph/OrbSysGpsC_33.hpp
     ext/lib/GNSSEph/OrbSysGpsL.cpp
     ext/lib/GNSSEph/OrbSysGpsL.hpp
     ext/lib/GNSSEph/OrbSysGpsL_51.cpp
     ext/lib/GNSSEph/OrbSysGpsL_51.hpp
     ext/lib/GNSSEph/OrbSysGpsL_52.cpp
     ext/lib/GNSSEph/OrbSysGpsL_52.hpp
     ext/lib/GNSSEph/OrbSysGpsL_55.cpp
     ext/lib/GNSSEph/OrbSysGpsL_55.hpp
     ext/lib/GNSSEph/OrbSysGpsL_56.cpp
     ext/lib/GNSSEph/OrbSysGpsL_56.hpp
     ext/lib/GNSSEph/OrbSysGpsL_63.cpp
     ext/lib/GNSSEph/OrbSysGpsL_63.hpp
     ext/lib/GNSSEph/OrbSysGpsL_Reserved.cpp
     ext/lib/GNSSEph/OrbSysGpsL_Reserved.hpp
     ext/lib/GNSSEph/OrbSysStore.cpp
     ext/lib/GNSSEph/OrbSysStore.hpp
     ext/lib/GNSSEph/QZSEphemerisStore.cpp
     ext/lib/GNSSEph/QZSEphemerisStore.hpp
     ext/lib/GNSSEph/RinexClockStore.hpp
     ext/lib/GNSSEph/SEMAlmanacStore.cpp
     ext/lib/GNSSEph/YumaAlmanacStore.cpp
     ext/lib/Geomatics/SunEarthSatGeometry.cpp
     ext/lib/Geomatics/SunEarthSatGeometry.hpp
     ext/lib/PosSol/PRSolutionLegacy.cpp
     ext/lib/PosSol/PRSolutionLegacy.hpp
     ext/tests/GNSSEph/CNavPackets_T.cpp
     ext/tests/GNSSEph/OrbAlmGen_T.cpp
     ext/tests/GNSSEph/OrbAlmStore_T.cpp
     ext/tests/GNSSEph/OrbAlm_T.cpp
     ext/tests/GNSSEph/OrbDataSys_T.cpp
     ext/tests/GNSSEph/OrbSysGpsC_T.cpp
     ext/tests/GNSSEph/OrbSysStore_T.cpp
     swig/src/gnsstk_templates.i