%define name python3-gnsstk
%define version 14.6.0
%define release 1

Summary:        GNSS Toolkit
Name:           %{name}
Version:        %{version}
Release:        %{release}%{?dist}
License:        LGPL
Source:         %{name}-master.tar.gz
URL:            https://github.com/SGL-UT/GNSSTk
Group:          Development/Libraries
Requires:       gnsstk >= %{version}
Requires:       python3-pip
BuildRequires:  cmake
BuildRequires:  swig
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  ncurses-devel

%description
The GNSS Toolkit (GNSSTk) is an open-source (LGPL) project sponsored by
the Space and Geophysics Laboratory (SGL), part of the Applied Research
Laboratories (ARL) at The University of Texas at Austin.
The primary goals of the GNSSTk project are to:
* provide applications for use by the GNSS and satellite navigation community.
* provide a core library to facilitate the development of GNSS applications.

%prep
%setup -n %{name}-master

# Setup and build GNSSTk utilizing CMake
%build
mkdir build
doxygen Doxyfile
mkdir -p build/swig/doc
python3 swig/docstring_generator.py ./docs/ $(pwd)/build/swig/doc
cd build
cmake -DPYTHON_INSTALL_PREFIX=$RPM_BUILD_ROOT/ -DCMAKE_INSTALL_PREFIX=$RPM_BUILD_ROOT/usr -DBUILD_EXT=ON -DBUILD_PYTHON=ON -DBUILD_FOR_PACKAGE_SWITCH=ON -DVERSIONED_HEADER_INSTALL=ON -DPYTHON_EXECUTABLE=/usr/bin/python3.6 ../
make all -j 4

# Install bin/lib/include folders in RPM BUILDROOT for packaging
%install
cd build
make install -j 4
# Currently the CMAKE installer cannot install python only, so we need to delete the non-python files.
rm -rf $RPM_BUILD_ROOT/usr/README.md
find $RPM_BUILD_ROOT/usr/include/gnsstk*/gnsstk ! -name "*.i" ! -name "gnsstk_swig.hpp" -type f -exec rm {} +
rm -rf $RPM_BUILD_ROOT/usr/bin/*
rm -rf $RPM_BUILD_ROOT/usr/lib64/*
rm -rf $RPM_BUILD_ROOT/usr/share/cmake/GNSSTK
mkdir -p $RPM_BUILD_ROOT/usr/include/gnsstk
cp $RPM_BUILD_ROOT/usr/include/gnsstk*/gnsstk/*.i $RPM_BUILD_ROOT/usr/include/gnsstk
cp $RPM_BUILD_ROOT/usr/include/gnsstk*/gnsstk/gnsstk_swig.hpp $RPM_BUILD_ROOT/usr/include/gnsstk

%clean
rm -rf $RPM_BUILD_ROOT

# Specify files and folders to be packaged
%files
%defattr(-,root,root)
%doc RELNOTES.md PYTHON.md
/usr/include/gnsstk*/gnsstk
/usr/include/gnsstk
/usr/lib/python3.6/site-packages/gnsstk
/usr/lib/python3.6/site-packages/gnsstk-%{version}-py3.6.egg-info


%changelog
* Mon Jan 06 2025 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.6.0 release
- Update turn on pipeline sonar scan quality check
- Add coverage tests to Gal*NavHealth files.
- Add testing for `PackedNavBits` dump method
- Add unit tests for Epoch.cpp
- Update SP3_T.cpp with dump tests for increased coverage
- Add unit tests to improve code coverage for FileHandling/RINEX
- Update Math/PowerSum : This is a refactor of this module.
- Fix NewNav findAll python interface to provide objects as their dynamic type
- Add test coverage to InterSigCorr.hpp and InterSigCorr.cpp
- Add test coverage to StdNavTimeOffset.hpp and StdNavTimeOffset.cpp
- Update fortify job retry to 2
- Update gitlab job pipeline job retries.
- Fix pipeline yaml identation issue for job windows_build
- Add new windows gitlab runners
- Update CI pipelines to make use of new Gitlab Runner tags
- Update SonarQube pipeline job
- Add test coverage to LNavFilterData.cpp
- Add Dump testing to BinexData
- Add dump and isSameData tests to GPS/GPSC/GPSL
- Add Coverage to Sinex Header
- Add full coverage to GLONASSTime.hpp and GLONASSTime.cpp
- Add testing to PosixTime
- Add Coverage to Sinex Types dump debug
- Update FileDirProc Filespec.cpp Added Unit test for dump and sort.
- Add CNav IAURA implementation.
- Update GNSSEph EngAlmanac Added dump Test
- Update Gnsstk/FileHandling YumaData.cpp Added Dump unit test.
- Add test coverage for NewNav/IonoNav dump functions
- Add test coverage for NewNav BDSD2 classes
- Add test coverage for NewNav/BDSD1NavEph.cpp and NewNav/BDSD1NavAlm.cpp
- Add NewNav/BDSD1NavHealth.cpp test coverage
- Fix newnav/GLOCNewEph.cpp Fixing introduced bug in factorToSigma().
- Add test coverage to JulianDate
- Add GLOCNavData_T Added unit test file focusing on Dump.
- Update GLOCNavUT1TimeOffset_T.cpp Added Dump related unit tests.
- Update GLOCNavLTDMP_T.cpp Added Dump related unit tests.
- Update GLOCNavHealth_T.cpp Added Dump related unit tests.
- Update GLOCNavEph_T.cpp Added Dump related unit tests.
- Update GLOCNavAlm_T.cpp  Added Dump related unit tests.
- Update CMakeLists.txt  Added GLOCNavData Unit Test to CMakeLists
- Update by making format input of QZSWeekSecond consistent with other classes
- Update NewNav  Updated Unit test GLOFNavAlm_T, GLOFNavData_T, GLOFNavHealth_T, GLOFNavISC_T, GLOFNavUT1TimeOffset_T
- Update  GLOFNavEph Test : added test for dump.
- Add IonexData test for dump method.
- Add DEBUGTRACE to several classes
- Add coverage to QZSWeekSecond.hpp
- Add full coverage to UnixTime.hpp and UnixTime.cpp
- Add coverage to IRNWeekSecond.hpp
- Add Coverage to GALWeekSeconds
- Add coverage to MJD.hpp
- Add test coverage to GPSWeekSecond
- Add test coverage to GPSZCount
- Add test coverage to GSPWeekZCount
- Add test coverage TimeCorrection
- Add full coverage to TimeSystemCorr.cpp
- Add full coverage to CNavCrossSourceFilter.hpp and CNavCrossSourceFilter.cpp
- Update doxygen documentation to show undocumented members such as enum values
- Add all 12 months to test testF2LayerCoeff CCIR.cpp
- Update add fortify results to toolkits dashboard.
- Update CODEOWNERS with new NewNav reviewers.
- Update codeowners file to use cpp reviewers group.  Exclude geomatics from sonarqube code coverage.
- Add test coverage to GPSWeek.hpp/cpp
- Add code coverage to nav time system converter
- Add test coverage to Week class
- Add full coverage to CommonTime.hpp and CommonTime.cpp
- Add Test coverage for YDSTime class
- Added unit tests for Geomatics/SpecialFuncs
- Add HelmertTransformation.cpp/hpp and update unit tests for code coverage
- Update gitlab pipeline job to exclude issues that are not relevant.
- Fix QZSS almanac NavData objects to use the QZS time system in their timestamps.
- Add Fortify Scan pipeline job.
- Add Additional unit test to compliment existing PosSol testing
- Add a few simple tests for the DiscCorr class
- Add full coverage to TimeSystem.cpp
- Add | ci_Combinations | Added unit tests
- Add full coverage to ReferenceFrame.cpp
- Add CommandLine_T.cpp for unit testing CommandLine.cpp
- Add Coverage to BasicTimeSystemConverter
- Add Geomatics/SunEarthSatGeometry_T unittests
- Add Debug to ORD test
- Add NewNav support to parse GPS LNav subframe 4 page 13 NMCT message.
- Update Sinex_ReadWrite_T.cpp to run with unit tests, added more extensive tests for Rinex3ObsHeader
- Add unit tests for uncovered operations in core/lib/Math for Matrix, Vector, and Stats
- Add LinearClockModel test coverage
- Add ObsEpochMap test coverage
- Add unit tests for core/Geomatics/EOPPrediction.cpp
- Add core/tests/ClockModel/SvObvsEpoch_T.cpp | Added unit test file for SvObsEpoch
- Add a few simple tests for the gdc class
- Add a few basic tests of PRSolution.
- Fix LNav fit interval computation for QZSS and GPS short/long term extended modes
- Add | core/tests/ClockModel/ORDEpoch_T.cpp | Added testing for ORDEpoch
- Fix swig tests running in windows DLL builds using python3.8+
- Add unit tests for AntexData.cpp
- Add ObsRngDev test coverage
- Add ObsClockModel Test Coverage
- Update gitlab pipeline windows jobs to use new gitlab runner windows instance on sgl-glrunner1/2 with AMD processor on windows.
- Add unit tests for functions in core/lib/AntennaStore.cpp
- Add NewNav/README.md to capture what has been implemented
- Fix initialization of GLOCNavUT1TimeOffset members.
* Thu Sep 05 2024 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.5.0 release
- Add test coverage to Epoch clock model
- Add missing include to ObsID.hpp
- Add findAll MultiFormatNavDataFactory Support
- Update gnsstk-data submodule refs
- Add Geomatics/testSSEph.cpp to test suite to improve code coverage
- Add findAll functionality to NavDataFactoryWithStore
- Add isSameData for GPSCNav2 subclasses
- Add isSameData for GLOFNav subclasses
- Add isSameData for GLOCNav subclasses
- Add isSameData for GALINav subclasses
- Add isSameData for GPSLNav subclasses
- Add isSameData for GPSLNav subclasses
- Add isSameData for BDSD1 subclasses
- Add isSameData for GPSCNav subclasses
- Add isSameData for GPSLNav subclasses
- Add isSameData functionality to the root and branches of the NavData
* Fri Jun 07 2024 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.4.1 release
- Update pipeline to use rhel 8 ubi image instead of deprecated centos 8 appstream
- Update navdatafactorywithstore_t find tests non breaking refactor
- Update to handle already disambiguated GPS Week in SEM Almanacs
- Fix SWIG wrapping of satTimeSystem()
* Thu May 09 2024 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.4.0 release
- Add reference epoch support in constructor of `MultiFormatNavDataFactory` and `SEMNavDataFactory`
- Fix intermittent test failure when using BasicTimeSystemConverter::getOffset
- Add C++ exception support to Geomatics.
- Update allow windows dll pipeline test job to fail so it doesn't short circuit the entire toolkit build chain.
- Fix _NBGLOFNavDataFactory Correct Reference Times
- Add STAKEHOLDERS document with stakeholder information
* Mon Dec 18 2023 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.3.0 release
- Add tests for sem and al3 files.
- Add addBitVec method to PackedNavBits to allow easy packing of an array of 0/1 ints.
- Fix PackedNavBits segfaulting when add more bits beyond its initial capacity.
- Update Removing special c++17 packages as they are no longer needed
- Update removed c++17 features due to segfaults.
- Update codeowners file to replace contributors that left
- Fix PNBGPSCNavDataFactory.cpp Fix weekrollover problem_
- Add Galileo I/NAV page pair parsing to PNB factory.
- Fix bug in PackedNavBits.addDataVec that would drop the last byte of data.
- Update gnsstk_enable.sh to be POSIX compliant for wider support.
- Add the ability for SP3NavDataFactory to initialize OrbitDataSP3 fields to NaN or whatever
* Fri Aug 11 2023 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.2.0 release
- Update create debian/ubuntu major version coinstallable package
- Add support for norad IDs in SatID for those compilers that support c++17 or later
- Fix gnsstk data submodule reference
- Changes in support of CSMS
- Update BDSD?NavEph.cpp Modified end fit determination
- Fix a few sign convention changes
- Fix time offset sign convention to be consistent between BasicTimeSystemConverter and the NavTimeSystemConverter.
- Add GPS SV config message to NewNav
- Fix SEMNavDataFactory orbital inclination rate of change
- Fix SEM NavData system value
- Update build.sh to opportunistically use Ninja
- Add support for specifying which c++ standard to use when compiling with g++
* Mon Feb 27 2023 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.1.0 release
- Update refactor PRSolution to update style and decompose large functions.
- Add SWIG bindings for MOPS and GCAT TropModel correctors.
- Fix swig4.0 compile time issues.
- Fix sphinx docs hidden navigation bar
* Tue Dec 20 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v14.0.0 release
* Mon Oct 31 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.8.0 release
* Fri Aug 26 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.7.0 release
- Fix Rinex3NavHeader to support the source specification for TIME SYSTEM CORR
- Add NewNavToRinex conversion utility class
- Add SonarQube analysis
- Clean up HelmertTransform: Comply with style, fix/add tests and documentation
- Replaced sgl_ci docker images with sgl equivalent
- Update SatMetaDataStore to properly process SIG records and look-up data via signals.
- Fix cmake issue with ext/swig
* Thu Jul 28 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.6.0 release
- Update CODEOWNERS file.
- Fix SWIG warnings for new OrbitData* classes
- Add callback capability to NavDataFactory classes
- Deprecate old incompatible (nonfunctional) CNAV code.
- Update WGS84 ref frame enums.
- Fix Rinex3ObsData handling of channel zero
- Update PRSolution (incl. PRSolve) output a covariance in the case of data from only a single epoch.
- Remove usage of `nose` in testing of swig generated python.
* Tue Jul 05 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.5.1 release
- Fix exception handling bug in PRSolution.
- Add conda packages for Python 3.7-3.9
* Thu Jun 23 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.5.0 release
* Fri May 20 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.4.0 release
* Thu Apr 21 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.3.0 release
* Mon Mar 28 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.2.0 release
* Thu Feb 24 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.1.0 release
* Fri Jan 21 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.0.0 release
* Thu Oct 28 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v12.1.0 release
* Wed Sep 15 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v12.0.0 release
- Add GPSCNavISC class for inter-signal corrections
- Add GPSCNav2ISC class for inter-signal corrections
- Add GalINavISC and related classes.
* Thu Jul 22 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.2.1 release
* Thu Jun 17 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.2.0 release
* Thu May 13 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.1.0 release
* Wed Apr 21 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.0.1 release
* Thu Mar 18 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.0.0 release
* Thu Feb 11 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v10.0.0 release
* Thu Jan 14 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v9.1.0 release
* Thu Dec 10 2020 David Barber <dbarber@arlut.utexas.edu>
- Updated for v9.0.0 release
* Thu Nov 19 2020 David Barber <dbarber@arlut.utexas.edu>
- Updated for v8.0.0 release
* Fri Sep 18 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v7.0.0 release
* Thu Jul 30 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v6.0.0 release
* Wed Jun 17 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v5.0.1 release
* Fri Jun 12 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v5.0.0 release
* Mon Mar 30 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v4.0.0 release
* Tue Mar 03 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Split single RPM into core and python packages for v3.1.0 release 2
* Tue Mar 03 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v3.1.0 release
* Tue Feb 11 2020 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v3.0.0 release
* Wed Jan 15 2020 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.12.2 release
* Wed Oct 9 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.12.1 release
* Tue Sep 17 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Update to build from master on CI
* Fri Aug 23 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Update to add /usr/share directory to package
* Thu Aug 15 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.12 release
* Tue Aug 13 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for gitlab ci rpm building
* Fri Apr 5 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.11.2 release
* Mon Feb 4 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.11.1 release
* Thu Jan 3 2019 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.11.0 release
* Tue Dec 11 2018 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.10.7 release
* Fri Oct 12 2018 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.10.6 release - second release
* Mon Sep 24 2018 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.10.5 release
* Mon Aug 27 2018 Bryan Parsons <bparsons@arlut.utexas.edu>
- Updated for v2.10.5 release
* Tue May 8 2018 Bryan Parsons <bparsons@arlut.utexas.edu>
- adding initial RPM configuration files

[ Some changelog entries trimmed for brevity.  -Editor. ]
