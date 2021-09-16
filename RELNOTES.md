GNSSTk 12.0.0 Release Notes
========================

 * This release introduces several major updates to the toolkit.
 * It includes the following:
   * Toolkit name change from gpstk to gnsstk including a c++ namespace change to `gnsstk::`
   * New Nav Store library. 
   * Swig improvements including swig wrapper updates for the new Nav store library.
   * Paradigm of separating apps versus libraries code into separate repos.
     * Gnsstk repo contains only libraries while the new gnsstk-apps repo contains only apps.  
 * Additionally, it contains minor bug fixes and a CI updates.

Updates since v11.2.1
---------------------

**Build System and Test Suite**
  * fix conda recipe for py36 to make sure enum34 is optional dependency.
  * Deprecate conda package support for numpy=1.8 and py2.7
  * Use Pip to install python wheel.

**Gitlab CI**
  * Allow windows and system install jobs to fail until we fix the concurrent-id.
  * CI Updates for feature branch issue_479_feature (TKS SPLIT OUT)

**Library Changes**
  * Rename namespace from gpstk to gnsstk
  * Repo only contains libraries as apps were migrated into new repository gnsstk-apps. 
  * Resolve "Major updates for new nav store"
  * Add BDS D2 support to new nav
  * Add BDS D1 support to new nav
  * Add support for Galileo I/NAV ISC (BGD)
  * Integrate SWIG changes into feature branch
  * Implement GPS CNAV-2 ISC support
  * Implement CNAV ISC processing

Fixes since v11.2.1
--------------------
  * Change implementation of StdNavTimeOffset to make sense for gnsstk
  * Expand IonoModel and IonoModelStore to improve usability

Code migrated to gpstk-apps repo
-------------------------------
     core/apps/Rinextools/RinDump.cpp
     core/apps/Rinextools/RinEdit.cpp
     core/apps/Rinextools/RinSum.cpp
     core/apps/Rinextools/scanBrdcFile.cpp
     core/apps/checktools/CheckFrame.hpp
     core/apps/checktools/rmwcheck.cpp
     core/apps/checktools/rnwcheck.cpp
     core/apps/checktools/rowcheck.cpp
     core/apps/difftools/DiffFrame.hpp
     core/apps/difftools/rinheaddiff.cpp
     core/apps/difftools/rmwdiff.cpp
     core/apps/difftools/rnwdiff.cpp
     core/apps/difftools/rowdiff.cpp
     core/apps/filetools/bc2sp3.cpp
     core/apps/filetools/smdscheck.cpp
     core/apps/mergetools/MergeFrame.hpp
     core/apps/mergetools/mergeRinMet.cpp
     core/apps/mergetools/mergeRinNav.cpp
     core/apps/mergetools/mergeRinObs.cpp
     core/apps/positioning/PRSolve.cpp
     core/apps/positioning/poscvt.cpp
     core/apps/time/calgps.cpp
     core/apps/time/timeconvert.cpp
     core/apps/time/timediff.cpp
     ext/apps/Rinextools/RinNav.cpp
     ext/apps/filetools/sp3version.cpp
     ext/apps/geomatics/JPLeph/convertSSEph.cpp
     ext/apps/geomatics/cycleslips/DiscFix.cpp
     ext/apps/geomatics/cycleslips/dfix.cpp
     ext/apps/geomatics/kalman/mergeSRI.cpp
     ext/apps/geomatics/kalman/tkalm.cpp
     ext/apps/geomatics/kalman/tkalm.hpp
     ext/apps/geomatics/relposition/ClockModel.cpp
     ext/apps/geomatics/relposition/CommandInput.cpp
     ext/apps/geomatics/relposition/CommandInput.hpp
     ext/apps/geomatics/relposition/ComputeRAIMSolution.cpp
     ext/apps/geomatics/relposition/Configure.cpp
     ext/apps/geomatics/relposition/DDBase.cpp
     ext/apps/geomatics/relposition/DDBase.hpp
     ext/apps/geomatics/relposition/DDid.cpp
     ext/apps/geomatics/relposition/DDid.hpp
     ext/apps/geomatics/relposition/DataOutput.cpp
     ext/apps/geomatics/relposition/DataStructures.cpp
     ext/apps/geomatics/relposition/DataStructures.hpp
     ext/apps/geomatics/relposition/DoubleDifference.cpp
     ext/apps/geomatics/relposition/EditDDs.cpp
     ext/apps/geomatics/relposition/EditRawDataBuffers.cpp
     ext/apps/geomatics/relposition/ElevationMask.cpp
     ext/apps/geomatics/relposition/EphemerisImprovement.cpp
     ext/apps/geomatics/relposition/Estimation.cpp
     ext/apps/geomatics/relposition/PhaseWindup.cpp
     ext/apps/geomatics/relposition/PhaseWindup.hpp
     ext/apps/geomatics/relposition/ProcessRawData.cpp
     ext/apps/geomatics/relposition/ReadObsFiles.cpp
     ext/apps/geomatics/relposition/ReadRawData.cpp
     ext/apps/geomatics/relposition/StochasticModels.cpp
     ext/apps/geomatics/relposition/Synchronization.cpp
     ext/apps/geomatics/relposition/Timetable.cpp
     ext/apps/geomatics/relposition/constants.hpp
     ext/apps/geomatics/relposition/index.hpp
     ext/apps/geomatics/robust/lsfilt.cpp
     ext/apps/geomatics/robust/rstats.cpp
     ext/apps/rfw/DeviceStream.hpp
     ext/apps/rfw/FDStreamBuff.cpp
     ext/apps/rfw/FDStreamBuff.hpp
     ext/apps/rfw/TCPStreamBuff.cpp
     ext/apps/rfw/TCPStreamBuff.hpp
     ext/apps/rfw/rfw.cpp
     ext/apps/rfw/tcptest.cpp

New Modules
-------------------------------     
     core/lib/ClockModel/SvObsEpoch.cpp
     core/lib/ClockModel/SvObsEpoch.hpp
     core/lib/GNSSCore/Angle.cpp
     core/lib/GNSSCore/Angle.hpp
     core/lib/GNSSCore/AngleReduced.cpp
     core/lib/GNSSCore/AngleReduced.hpp
     core/lib/GNSSCore/FreqConv.cpp
     core/lib/GNSSCore/FreqConv.hpp
     core/lib/GNSSCore/GalileoIonoEllipsoid.hpp
     core/lib/GNSSCore/XmitAnt.cpp
     core/lib/GNSSCore/XmitAnt.hpp
     core/lib/GNSSEph/ClockRecord.cpp
     core/lib/GNSSEph/ClockRecord.hpp
     core/lib/GNSSEph/PositionRecord.cpp
     core/lib/GNSSEph/PositionRecord.hpp
     core/lib/NewNav/BDSD1NavAlm.cpp
     core/lib/NewNav/BDSD1NavAlm.hpp
     core/lib/NewNav/BDSD1NavData.cpp
     core/lib/NewNav/BDSD1NavData.hpp
     core/lib/NewNav/BDSD1NavEph.cpp
     core/lib/NewNav/BDSD1NavEph.hpp
     core/lib/NewNav/BDSD1NavHealth.cpp
     core/lib/NewNav/BDSD1NavHealth.hpp
     core/lib/NewNav/BDSD1NavISC.cpp
     core/lib/NewNav/BDSD1NavISC.hpp
     core/lib/NewNav/BDSD1NavIono.cpp
     core/lib/NewNav/BDSD1NavIono.hpp
     core/lib/NewNav/BDSD1NavTimeOffset.cpp
     core/lib/NewNav/BDSD1NavTimeOffset.hpp
     core/lib/NewNav/BDSD2NavAlm.cpp
     core/lib/NewNav/BDSD2NavAlm.hpp
     core/lib/NewNav/BDSD2NavData.cpp
     core/lib/NewNav/BDSD2NavData.hpp
     core/lib/NewNav/BDSD2NavEph.cpp
     core/lib/NewNav/BDSD2NavEph.hpp
     core/lib/NewNav/BDSD2NavHealth.cpp
     core/lib/NewNav/BDSD2NavHealth.hpp
     core/lib/NewNav/BDSD2NavISC.cpp
     core/lib/NewNav/BDSD2NavISC.hpp
     core/lib/NewNav/BDSD2NavIono.cpp
     core/lib/NewNav/BDSD2NavIono.hpp
     core/lib/NewNav/BDSD2NavTimeOffset.cpp
     core/lib/NewNav/BDSD2NavTimeOffset.hpp
     core/lib/NewNav/BDSconsts.hpp
     core/lib/NewNav/CCIR.cpp
     core/lib/NewNav/CCIR.hpp
     core/lib/NewNav/DumpDetail.hpp
     core/lib/NewNav/GPSCNav2ISC.cpp
     core/lib/NewNav/GPSCNav2ISC.hpp
     core/lib/NewNav/GPSCNav2Iono.hpp
     core/lib/NewNav/GPSCNavISC.cpp
     core/lib/NewNav/GPSCNavISC.hpp
     core/lib/NewNav/GPSCNavIono.cpp
     core/lib/NewNav/GPSCNavIono.hpp
     core/lib/NewNav/GPSLNavISC.cpp
     core/lib/NewNav/GPSLNavISC.hpp
     core/lib/NewNav/GPSLNavIono.cpp
     core/lib/NewNav/GPSLNavIono.hpp
     core/lib/NewNav/GalDataValid.cpp
     core/lib/NewNav/GalDataValid.hpp
     core/lib/NewNav/GalFNavAlm.cpp
     core/lib/NewNav/GalFNavAlm.hpp
     core/lib/NewNav/GalFNavEph.cpp
     core/lib/NewNav/GalFNavEph.hpp
     core/lib/NewNav/GalFNavHealth.cpp
     core/lib/NewNav/GalFNavHealth.hpp
     core/lib/NewNav/GalFNavISC.cpp
     core/lib/NewNav/GalFNavISC.hpp
     core/lib/NewNav/GalFNavIono.cpp
     core/lib/NewNav/GalFNavIono.hpp
     core/lib/NewNav/GalFNavTimeOffset.cpp
     core/lib/NewNav/GalFNavTimeOffset.hpp
     core/lib/NewNav/GalHealthStatus.cpp
     core/lib/NewNav/GalHealthStatus.hpp
     core/lib/NewNav/GalINavAlm.cpp
     core/lib/NewNav/GalINavAlm.hpp
     core/lib/NewNav/GalINavEph.cpp
     core/lib/NewNav/GalINavEph.hpp
     core/lib/NewNav/GalINavHealth.cpp
     core/lib/NewNav/GalINavHealth.hpp
     core/lib/NewNav/GalINavISC.cpp
     core/lib/NewNav/GalINavISC.hpp
     core/lib/NewNav/GalINavIono.cpp
     core/lib/NewNav/GalINavIono.hpp
     core/lib/NewNav/GalINavTimeOffset.cpp
     core/lib/NewNav/GalINavTimeOffset.hpp
     core/lib/NewNav/InterSigCorr.cpp
     core/lib/NewNav/InterSigCorr.hpp
     core/lib/NewNav/IonoData.hpp
     core/lib/NewNav/KlobucharIonoData.cpp
     core/lib/NewNav/KlobucharIonoData.hpp
     core/lib/NewNav/MODIP.cpp
     core/lib/NewNav/MODIP.hpp
     core/lib/NewNav/NDFUniqConstIterator.hpp
     core/lib/NewNav/NDFUniqIterator.hpp
     core/lib/NewNav/NavDataFactory.cpp
     core/lib/NewNav/NeQuickIonoData.cpp
     core/lib/NewNav/NeQuickIonoData.hpp
     core/lib/NewNav/PNBBDSD1NavDataFactory.cpp
     core/lib/NewNav/PNBBDSD1NavDataFactory.hpp
     core/lib/NewNav/PNBBDSD2NavDataFactory.cpp
     core/lib/NewNav/PNBBDSD2NavDataFactory.hpp
     core/lib/NewNav/PNBGalFNavDataFactory.cpp
     core/lib/NewNav/PNBGalFNavDataFactory.hpp
     core/lib/NewNav/PNBGalINavDataFactory.cpp
     core/lib/NewNav/PNBGalINavDataFactory.hpp
     core/lib/NewNav/StdNavTimeOffset.cpp
     core/lib/NewNav/StdNavTimeOffset.hpp
     core/lib/TestFramework/FactoryCounter.hpp
     core/tests/GNSSCore/AngleReduced_T.cpp
     core/tests/GNSSCore/Angle_T.cpp
     core/tests/GNSSCore/XmitAnt_T.cpp
     core/tests/NewNav/BDSD1NavAlm_T.cpp
     core/tests/NewNav/BDSD1NavData_T.cpp
     core/tests/NewNav/BDSD1NavEph_T.cpp
     core/tests/NewNav/BDSD1NavHealth_T.cpp
     core/tests/NewNav/BDSD1NavISC_T.cpp
     core/tests/NewNav/BDSD1NavIono_T.cpp
     core/tests/NewNav/BDSD1NavTimeOffset_T.cpp
     core/tests/NewNav/CCIR_T.cpp
     core/tests/NewNav/D1NavTestDataDecl.hpp
     core/tests/NewNav/D1NavTestDataDef.hpp
     core/tests/NewNav/D2NavTestDataDecl.hpp
     core/tests/NewNav/D2NavTestDataDef.hpp
     core/tests/NewNav/GPSCNav2Iono_T.cpp
     core/tests/NewNav/GPSCNavISC_T.cpp
     core/tests/NewNav/GPSCNavIono_T.cpp
     core/tests/NewNav/GPSLNavISC_T.cpp
     core/tests/NewNav/GPSLNavIono_T.cpp
     core/tests/NewNav/GalDataValid_T.cpp
     core/tests/NewNav/GalFNavAlm_T.cpp
     core/tests/NewNav/GalFNavEph_T.cpp
     core/tests/NewNav/GalFNavHealth_T.cpp
     core/tests/NewNav/GalFNavISC_T.cpp
     core/tests/NewNav/GalFNavTestDataDecl.hpp
     core/tests/NewNav/GalFNavTestDataDef.hpp
     core/tests/NewNav/GalFNavTimeOffset_T.cpp
     core/tests/NewNav/GalHealthStatus_T.cpp
     core/tests/NewNav/GalINavAlm_T.cpp
     core/tests/NewNav/GalINavEph_T.cpp
     core/tests/NewNav/GalINavHealth_T.cpp
     core/tests/NewNav/GalINavISC_T.cpp
     core/tests/NewNav/GalINavTestDataDecl.hpp
     core/tests/NewNav/GalINavTestDataDef.hpp
     core/tests/NewNav/GalINavTimeOffset_T.cpp
     core/tests/NewNav/InterSigCorr_T.cpp
     core/tests/NewNav/KlobucharIonoData_T.cpp
     core/tests/NewNav/MODIP_T.cpp
     core/tests/NewNav/NeQuickIonoData_T.cpp
     core/tests/NewNav/PNBBDSD1NavDataFactory_T.cpp
     core/tests/NewNav/PNBBDSD2NavDataFactory_T.cpp
     core/tests/NewNav/PNBGalFNavDataFactory_T.cpp
     core/tests/NewNav/PNBGalINavDataFactory_T.cpp
     core/tests/NewNav/StdNavTimeOffset_T.cpp
     swig/FileHandling/Binex/BinexFilterOperators.i
     swig/FileHandling/FileHandling.i
     swig/FileHandling/RINEX/RinexMetFilterOperators.i
     swig/FileHandling/RINEX/RinexNavData.i
     swig/FileHandling/RINEX/RinexNavFilterOperators.i
     swig/FileHandling/RINEX/RinexObsFilterOperators.i
     swig/FileHandling/RINEX3/Rinex3NavData.i
     swig/FileHandling/RINEX3/Rinex3NavFilterOperators.i
     swig/FileHandling/RINEX3/Rinex3ObsFilterOperators.i
     swig/FileHandling/Stream.i
     swig/GNSSCore/GNSSCore.i
     swig/GNSSCore/Position.i
     swig/GNSSCore/SatID.i
     swig/GNSSCore/SatMetaDataSort.i
     swig/GNSSEph/GNSSEph.i
     swig/GNSSEph/GPS_URA.i
     swig/GNSSEph/NavID.i
     swig/Geomatics/Geomatics.i
     swig/Geomatics/Namelist.i
     swig/Geomatics/SunEarthSatGeometry.i
     swig/Math/Math.i
     swig/Math/Matrix.i
     swig/Math/Triple.i
     swig/Math/Vector.i
     swig/NavFilter/CNavFilter.i
     swig/NavFilter/LNavEphMaker.i
     swig/NavFilter/LNavFilterData.i
     swig/NavFilter/LNavOrderFilter.i
     swig/NavFilter/NavFilter.i
     swig/NavFilter/NavOrderFilter.i
     swig/NewNav/NewNavSharedPtr.i
     swig/NewNav/NewNavTypemaps.i
     swig/SWIGHelpers/Arrays.i
     swig/SWIGHelpers/Enums.i
     swig/SWIGHelpers/Ignore.i
     swig/SWIGHelpers/Macros.i
     swig/SWIGHelpers/Renames.i
     swig/SWIGHelpers/STLTemplates.i
     swig/SWIGHelpers/StringPrint.i
     swig/SWIGHelpers/TemplateMacros.i
     swig/SWIGHelpers/pythonfunctions.i
     swig/TimeHandling/TimeHandling.i
     swig/TimeHandling/TimeString.i
     swig/Utilities/FormattedDouble.i
     swig/Utilities/Utilities.i
     swig/gnsstk.i
     swig/gnsstk/__init__.py.in
     swig/gnsstk_swig.hpp
     swig/gnsstk_swig.i
     swig/src/gnsstk_templates.i
     swig/tests/test_FileHandling.py
     swig/tests/test_GNSSCore.py
     swig/tests/test_GNSSEph.py
     swig/tests/test_GPSCNav2Alm.py
     swig/tests/test_GPSCNav2Eph.py
     swig/tests/test_GPSCNav2Health.py
     swig/tests/test_GPSCNav2TimeOffset.py
     swig/tests/test_GPSCNavAlm.py
     swig/tests/test_GPSCNavEph.py
     swig/tests/test_GPSCNavHealth.py
     swig/tests/test_GPSCNavTimeOffset.py
     swig/tests/test_GPSLNavAlm.py
     swig/tests/test_GPSLNavEph.py
     swig/tests/test_GPSLNavHealth.py
     swig/tests/test_GPSLNavTimeOffset.py
     swig/tests/test_GalFNavAlm.py
     swig/tests/test_GalFNavEph.py
     swig/tests/test_GalFNavHealth.py
     swig/tests/test_GalFNavTimeOffset.py
     swig/tests/test_GalINavAlm.py
     swig/tests/test_GalINavEph.py
     swig/tests/test_GalINavHealth.py
     swig/tests/test_GalINavTimeOffset.py
     swig/tests/test_MultiFormatNavDataFactory.py
     swig/tests/test_NavFilter.py
     swig/tests/test_NavLibrary.py
     swig/tests/test_NewNav.py
     swig/tests/test_RinexNavDataFactory.py
     swig/tests/test_SEMNavDataFactory.py
     swig/tests/test_SP3NavDataFactory.py
     swig/tests/test_YumaNavDataFactory.py
     swig/tests/test_math.py
     swig/tests/test_matrix.py
     swig/tests/test_vector.py
