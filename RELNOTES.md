GNSSTk 14.6.0 Release Notes
========================

 * This minor release includes the following:
    * Adding more test coverage to reach 80% linear line coverage for SonarQube which includes adding new or updating unit tests.  See New Modules below.
    * Making format input of QZSWeekSecond consistent with other classes
    * Updating Math/PowerSum : This is a refactor of this module.
    * Adding NewNav/README.md to capture what has been implemented
    * Adding NewNav support to parse GPS LNav subframe 4 page 13 NMCT message.
    * Adding HelmertTransformation.cpp/hpp and update unit tests for code coverage
    * Adding DEBUGTRACE to several classes
    * Adding CNav IAURA implementation.
 * It also includes bug fixes and CI/CD updates.

Updates since v14.5.0
---------------------

**Build System and Test Suite**
  * Update Sinex_ReadWrite_T.cpp to run with unit tests, added more extensive tests for Rinex3ObsHeader
  * Update codeowners file to use cpp reviewers group.  Exclude geomatics from sonarqube code coverage
  * Update add fortify results to toolkits dashboard.
  * Update  GLOFNavEph Test : added test for dump.
  * Update NewNav  Updated Unit test GLOFNavAlm_T, GLOFNavData_T, GLOFNavHealth_T, GLOFNavISC_T, GLOFNavUT1TimeOffset_T
  * Update CMakeLists.txt  Added GLOCNavData Unit Test to CMakeLists
  * Update GLOCNavAlm_T.cpp  Added Dump related unit tests.
  * Update GLOCNavEph_T.cpp Added Dump related unit tests.
  * Update GLOCNavHealth_T.cpp Added Dump related unit tests.
  * Update GLOCNavLTDMP_T.cpp Added Dump related unit tests.
  * Update GLOCNavUT1TimeOffset_T.cpp Added Dump related unit tests.
  * Update Gnsstk/FileHandling YumaData.cpp Added Dump unit test.
  * Update GNSSEph EngAlmanac Added dump Test
  * Update turn on pipeline sonar scan quality check
  * Update FileDirProc Filespec.cpp Added Unit test for dump and sort.
  * Update SP3_T.cpp with dump tests for increased coverage
  * Add unit tests for functions in core/lib/AntennaStore.cpp
  * Add ObsClockModel Test Coverage
  * Add ObsRngDev test coverage
  * Add unit tests for AntexData.cpp
  * Add | core/tests/ClockModel/ORDEpoch_T.cpp | Added testing for ORDEpoch
  * Add a few basic tests of PRSolution.
  * Add a few simple tests for the gdc class
  * Add core/tests/ClockModel/SvObvsEpoch_T.cpp | Added unit test file for SvObsEpoch
  * Add unit tests for core/Geomatics/EOPPrediction.cpp
  * Add ObsEpochMap test coverage
  * Add LinearClockModel test coverage
  * Add unit tests for uncovered operations in core/lib/Math for Matrix, Vector, and Stats
  * Add Debug to ORD test
  * Add Geomatics/SunEarthSatGeometry_T unittests
  * Add Coverage to BasicTimeSystemConverter
  * Add CommandLine_T.cpp for unit testing CommandLine.cpp
  * Add full coverage to ReferenceFrame.cpp
  * Add | ci_Combinations | Added unit tests
  * Add full coverage to TimeSystem.cpp
  * Add a few simple tests for the DiscCorr class
  * Add Additional unit test to compliment existing PosSol testing
  * Added unit tests for Geomatics/SpecialFuncs
  * Add Test coverage for YDSTime class
  * Add full coverage to CommonTime.hpp and CommonTime.cpp
  * Add test coverage to Week class
  * Add code coverage to nav time system converter
  * Add test coverage to GPSWeek.hpp/cpp
  * Add all 12 months to test testF2LayerCoeff CCIR.cpp
  * Add full coverage to CNavCrossSourceFilter.hpp and CNavCrossSourceFilter.cpp
  * Add full coverage to TimeSystemCorr.cpp
  * Add test coverage TimeCorrection
  * Add test coverage to GSPWeekZCount
  * Add test coverage to GPSZCount
  * Add test coverage to GPSWeekSecond
  * Add coverage to MJD.hpp
  * Add Coverage to GALWeekSeconds
  * Add coverage to IRNWeekSecond.hpp
  * Add full coverage to UnixTime.hpp and UnixTime.cpp
  * Add coverage to QZSWeekSecond.hpp
  * Add IonexData test for dump method.
  * Add GLOCNavData_T Added unit test file focusing on Dump.
  * Add test coverage to JulianDate
  * Add NewNav/BDSD1NavHealth.cpp test coverage
  * Add test coverage for NewNav/BDSD1NavEph.cpp and NewNav/BDSD1NavAlm.cpp
  * Add test coverage for NewNav BDSD2 classes
  * Add test coverage for NewNav/IonoNav dump functions
  * Add Coverage to Sinex Types dump debug
  * Add testing to PosixTime
  * Add full coverage to GLONASSTime.hpp and GLONASSTime.cpp
  * Add Coverage to Sinex Header
  * Add dump and isSameData tests to GPS/GPSC/GPSL
  * Add Dump testing to BinexData
  * Add test coverage to LNavFilterData.cpp
  * Add test coverage to StdNavTimeOffset.hpp and StdNavTimeOffset.cpp
  * Add test coverage to InterSigCorr.hpp and InterSigCorr.cpp
  * Add unit tests to improve code coverage for FileHandling/RINEX
  * Add unit tests for Epoch.cpp
  * Add testing for `PackedNavBits` dump method
  * Add coverage tests to Gal*NavHealth files.

**Gitlab CI**
  * Update gitlab pipeline windows jobs to use new gitlab runner windows instance on sgl-glrunner1/2 with AMD processor on windows.
  * Update gitlab pipeline job to exclude issues that are not relevant.
  * Update CODEOWNERS with new NewNav reviewers.
  * Update SonarQube pipeline job
  * Update CI pipelines to make use of new Gitlab Runner tags
  * Update gitlab job pipeline job retries.
  * Update fortify job retry to 2
  * Add Fortify Scan pipeline job.
  * Add new windows gitlab runners
  * Fix pipeline yaml identation issue for job windows_build
  * Fix swig tests running in windows DLL builds using python3.8+

**Library Changes**

  * Update by making format input of QZSWeekSecond consistent with other classes
  * Update Math/PowerSum : This is a refactor of this module.
  * Add NewNav/README.md to capture what has been implemented
  * Add NewNav support to parse GPS LNav subframe 4 page 13 NMCT message.
  * Add HelmertTransformation.cpp/hpp and update unit tests for code coverage
  * Add DEBUGTRACE to several classes
  * Add CNav IAURA implementation.

Fixes since v14.5.0
--------------------
  * Update doxygen documentation to show undocumented members such as enum values
  * Fix initialization of GLOCNavUT1TimeOffset members.
  * Fix LNav fit interval computation for QZSS and GPS short/long term extended modes
  * Fix QZSS almanac NavData objects to use the QZS time system in their timestamps.
  * Fix newnav/GLOCNewEph.cpp Fixing introduced bug in factorToSigma().
  * Fix NewNav findAll python interface to provide objects as their dynamic type

New Modules
-------------------------------
     core/lib/NewNav/GPSLNavNMCT.cpp
     core/lib/NewNav/GPSLNavNMCT.hpp
     core/lib/NewNav/GPSNMCTAI.cpp
     core/lib/NewNav/GPSNMCTAI.hpp
     core/tests/ClockModel/LinearClockModel_T.cpp
     core/tests/ClockModel/SvObsEpoch_T.cpp
     core/tests/CommandLine/CommandLine_T.cpp
     core/tests/FileHandling/Rinex3ObsHeader_T.cpp
     core/tests/FileHandling/Rinex_Util_T.cpp
     core/tests/FileHandling/SinexHeader_T.cpp
     core/tests/FileHandling/SinexTypes_T.cpp
     core/tests/Geomatics/AntennaStore_T.cpp
     core/tests/Geomatics/AntexData_T.cpp
     core/tests/Geomatics/DiscCorr_T.cpp
     core/tests/Geomatics/EOPPrediction_T.cpp
     core/tests/Geomatics/SpecialFuncs_T.cpp
     core/tests/Geomatics/SunEarthSatGeometry_T.cpp
     core/tests/Geomatics/gdc_T.cpp
     core/tests/Math/Matrix_Base_T.cpp
     core/tests/Math/Matrix_Slice_T.cpp
     core/tests/NavFilter/CNavCrossSourceFilter_T.cpp
     core/tests/NewNav/GLOCNavData_T.cpp
     core/tests/NewNav/GPSLNavNMCT_T.cpp
     core/tests/PosSol/Combinations_T.cpp
     core/tests/PosSol/PRSolution_T.cpp
     core/tests/TimeHandling/Epoch_T.cpp
     core/tests/TimeHandling/GALWeekSecond_T.cpp
     core/tests/TimeHandling/GPSWeek_T.cpp
     core/tests/TimeHandling/QZSWeekSecond_T.cpp
     core/tests/TimeHandling/Week_T.cpp
