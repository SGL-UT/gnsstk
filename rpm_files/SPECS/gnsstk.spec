%define name gnsstk
%define version 14.3.0
%define release 1

Summary:        GNSS Toolkit
Name:           %{name}
Version:        %{version}
Release:        %{release}%{?dist}
License:        LGPL
Source:         %{name}-master.tar.gz
URL:            https://github.com/SGL-UT/GNSSTk
Group:          Development/Libraries
BuildRequires: cmake
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: ncurses-devel

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
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_EXT=ON -DUSE_RPATH=OFF -DVERSIONED_HEADER_INSTALL=ON ../
make all -j 4

# Install bin/lib/include folders in RPM BUILDROOT for packaging
%install
cd build
make install -j 4 DESTDIR=$RPM_BUILD_ROOT
rm -rf $RPM_BUILD_ROOT/usr/README.md
mkdir -p $RPM_BUILD_ROOT/usr/include/gnsstk
cp $RPM_BUILD_ROOT/usr/include/gnsstk*/gnsstk/*.h $RPM_BUILD_ROOT/usr/include/gnsstk
cp $RPM_BUILD_ROOT/usr/include/gnsstk*/gnsstk/*.hpp $RPM_BUILD_ROOT/usr/include/gnsstk

%clean
rm -rf $RPM_BUILD_ROOT

# Specify files and folders to be packaged
%files
%defattr(-,root,root)
%doc README.md COPYING.md RELNOTES.md
/usr/include/gnsstk*/gnsstk
/usr/include/gnsstk
/usr/bin/*
/usr/lib64/*
/usr/share/cmake/GNSSTK


%changelog
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
- Update NewNav docs
- Update Move NewNav enums to the namespace level for consistency
- Update group path corrector navLib from shared_ptr to reference to work around swig/python problem
- Update Refactor duplicate raw range implementations into a single class.
- Add Ubuntu 20.04 pipeline jobs
- Add a CorrectorType that was missed
- Fix swig build error
- Update the nonsensical ISC interface in NavLibrary with one that does make sense.
- Add group path delay calculator (GroupPathCorr and related classes).
- Update Refactor reference frames into RefFrame
- Update Refactor HelmertTransform into HelmertTransformer
- Update compiler to Visual Studio 2019 in the Windows build scripts.
- Add signal details to nav dump methods
- Update Deprecated Debian 9 build
- Update SWIG cmake rules now that we're no longer supporting cmake version 2
- Add NavData::clone method
- Updated the clean build parameter
* Mon Oct 31 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.8.0 release
- Fix Remove SystemTime tests that randomly fail
- Update Deprecate extraneous swig binding for function that no longer exists.
- Update CODEOWNERS.
- Update sphinx documentation of SWIG bindings
- Update Changed the minimum cmake version
- Update documentation of TropModel classes
- Fix bugs in GLONASS CDMA nav data processing
- Add support for GLONASS CDMA nav data processing
- Add Split test data into gnsstk-data repo with submodules configured
- Update exception specifications from throw() to noexcept
- Fix ORD tests to use the GNSSTk testing framework
- Update change string name in StringUtils to avoid conflict with curses library macro
- Fix week rollover issues in GPS LNav, Galileo F/Nav and Galileo I/Nav decoders.
- Update move nav message bit definition enums out of PNBNavDataFactory.cpp files and into their own files.
- Add documentation on raw range (geometric range) computations
- Fix pipeline push artifacts
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
- Revert "clone" method additions for now.
- Fix Doxygen project title & index top label.
- Add FactoryControl class for configuring aspects of NavDataFactory classes (initially, adding a filter to throw out empty BDS time offset data, and another for filtering time offset data in general).
- Fix error in decoding BDS D2 ephemeris A1 parameter.
- Fix NewNav OrbitDataKepler children to use appropriate ellipsoids.
- Fix SolarSystemEphemeris potential string termination issue.
- Fix NBTropModel potential null dereference issue.
- Fix BinexData potential buffer overflow issues.
- Add NavData::clone method
- Clean up example code
- Update SWIG math tests to use unittest framework
- Updated the linux build scripts to specify a release build type.
- Update CODEOWNERS to eliminate superfluous required merge approvals.
- Replaced cloning repo_utils with pip installed tks_ci_tools
- Update the debian-11 to store less history.
- Update CODEOWNERS to use package reviewer groups.
* Fri May 20 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.4.0 release
- Update conda recipe and CI pipeline to release from stable branch
- Update DOCKER_REGISTRY variable.
- Add support for deploying to Debian 11
- Move SEM and Yuma file support from ext to core.
- Move Geomatics code from ext into core
- Fix PNBGPSLNavDataFactory missing half-week test.
- Adding pipeline job for sonar
- Fix swig wrapper missing class/template issue
- Update gitlab pipeline for job artifacts.
- Refactor SWIG bindings into separate modules
* Thu Apr 21 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.3.0 release
- Fix RPATH to be platform independent
- Update Geomatics libs, including some style
- Update NavDataFactoryWithStore to store TimeOffsetData in the primary store so find() can be used on it.
- Update CMake package to be compatible with same major version.
* Mon Mar 28 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.2.0 release
- Update to SatPass.hpp to expand GNSSTK_EXPORT macro for outFormat and longfmt.
- Update per style guide
- Fix for non-windows builds broken by the fixes for windows builds.
- Update exports and cmake files to allow SWIG to build under Visual Studio 2019
- Minor comment fix-up for OMEGA0
- Migrate IONEX file processing code into core, updating to meet style.
- Update NewNav getXvt() method to support optionally specifying a different phase center than the default.
- Add clearTypeFilter and addTypeFilter methods to NewNav for C++ and Python use
- Add missing NewNav tests.
- Add missing PNBGLOFNavDataFactory documentation.
- Fix PNBGLOFNavDataFactory to properly include the correct subject satellite for almanac health.
- Fix PNBGLOFNavDataFactory to use the correct string for decoding ISC.
- Update CCIR documentation to explain error conditions.
- Add Visual Studio export qualifier to DebugTrace::enabled
- Add CMAKE as a special class of reviewers.
- Fix gitlab CI Lint warning
- Add CommandOptionNavEnumHelp class
- Add tabularize() to StringUtils
- Add NavLibrary debug tracing
- Update GalINavAlm/Eph dump() to show interpreted health status.
- Fix test issues arising from merging.
- Add the ability to build gnsstk as a DLL under Windows
- Update NewNav getXvt methods to set the reference frame.
* Thu Feb 24 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.1.0 release
- Update SystemTime test so it doesn't fail so often and so it meets style rules
- Fix SP3NavDataFactory clock linear interpolation half-order
- Add gnsstk::ValidType::operator!=()
- Update moved windows cleanup script to ci_control repo.
- Fix Rinex3ObsStream so the object can be reused.
- Update BDS TimeOffset validate() so any one term can be 0 but not both
- Fix TestUtil::fileCompTest to properly detect EOF under Windows.
- Fix Rinex3NavDataOperatorLessThanFull's broken sorting.
- Fix Rinex3NavData to right-align satellite IDs when writing RINEX 2.
- Fix python docstring generator from Doxygen output
* Fri Jan 21 2022 David Barber <dbarber@arlut.utexas.edu>
- Updated for v13.0.0 release
-   Add windows packaging job
-   Update copyright header to year 2022
-   Fix KlobucharIonoNavData day rollover check
-   Fix CommonTime/TimeTag/etc. time offset to subtract instead of add.
-   Update Rename IonoData::getCorrection to getIonoCorr
-   Update Rename NeQuickIonoData to NeQuickIonoNavData
-   Update Rename KlobucharIonoData to KlobucharIonoNavData
-   Update Rename IonoData to IonoNavData
-   Add GLONASS NewNav classes
-   Update Refactor SunEarthSatGeometry from ext to core, comply with style guide.
-   Update Remove numerous deprecated classes
-   Fix Reset times when NavDataFactory is cleared
-   Update SWIG does not support c++11 type aliasing 'using' until 3.0.11. Debian 9 has swig 3.0.10
-   Fix cmake when installing gnsstk swig bindings
-   Update debian package python3-gnsstk to explicitly conflict with python-gnsstk
-   Add method for determining if a BeiDou satellite should be GEO
-   Add support for Terse mode dumps to NavDataFactoryWithStore
-   Add getIndexSet, getNavMap methods to NavDataFactoryWithStore
-   Fix loadIntoMap methods of the NavDataFactory tree to properly fill in the desired data maps
-   Add minimal (mostly unimplemented except for select classes) isSameData and compare methods to the NavData tree
-   Update wavelength constants to be computed via c/freq
-   Fix FileSpec text handling
-   Fix (remove) redundant/misleading docstring paragraph.
-   Add Compare classes for unique-ifying/sorting GPSLNavEph
-   Add option to df_diff to ignore specific columns of matching lines
-   Update build.sh to use python3 by default instead of python2
-   Add profiler support to build process.
-   Update SP3NavDataFactory for performance improvements.
-   Add ObsID constructor that avoids wildcards.
-   Fix ObsID incorrect comment in constructor docs.
-   Update debian control files to build python3 bindings, instead of python2.
-   Update NewNav debug output to use DEBUGTRACE instead of cerr
-   Update test_GPSLNavEph.py to reflect changes in GPSLNavEph getUserTime implementation
-   Update SWIG stuff due to changes in #includes
-   Update python3 detection for swig bindings.
-   NewNav dump method updates for consistency.
-   Add GPSLNavEph xmit2/xmit3 fields for transmit time of SF2/3
-   Add a "Terse" dump type for NewNav
-   Fix YumaNavDataFactory problem with reading Yuma nav files
-   Add option for regular expression exclusion in df_diff.
-   Update how initial/finalTime are set in NavDataFactoryWithStore.
-   Add NavLibrary::getIndexSet() methods.
-   Add NavDataFactory::getIndexSet() methods.
-   Add MultiFormatNavDataFactory::getFactory() method.
-   Add Store CI artifacts in artifactory.
-   Update Refactor ORDs code to use newnav
* Thu Oct 28 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v12.1.0 release
- Update SolarPosition SWIG bindings to map output data.
- Add FREQ_STEP_GLONASS_* constants to FreqConsts.hpp
- Update copyright statement.
- Fix PNBBDSD2NavDataFactory so it doesn't dump core if subframe 1 page 2 is missing
- Fix PNBMultiGNSSNavDataFactory so it knows about BeiDou D2
- Fix SEMNavDataFactory so it properly sets a timestamp of the data
- Remove references to latex documentation that no longer exists
- Fix ClockSatStore where it was interpolating clock drift instead of drift rate.
- Add "Mixed" satellite system to RinexSatID translation to strings.
- Fix RINEX 3 nav headers to preserve leading spaces in comments
- Update conda recipe to use local src already checked out.
- Add explicit debian dependency fom python-gnsstk to python-enum34.
- Add PNBNavDataFactory code snippet do newnav docs
- Update pipeline to use new windows VM instance
- Add some constants for BeiDou.
- Update Remove embedded libgnsstk.so from python
- Update pipeline retry job setting
* Wed Sep 15 2021 David Barber <dbarber@arlut.utexas.edu>
- Use Pip to instal python wheel.
- Rename gpstk to gnsstk  
- fix conda recipe for py36 to make sure enum34 is optional dependency. 
- Allow windows and system install jobs to fail until we fix the concurrent-id.
- Deprecate conda package support for numpy=1.8  
- Resolve "Major updates for new nav store"
- TKS Split Out: Ci updates for issue_479_feature branch 
- Integrate SWIG changes into feature branch, 3rd try
- CI Updates for feature branch issue_479_feature (TKS SPLIT OUT)
- Expand IonoModel and IonoModelStore to improve usability
- Change implementation of StdNavTimeOffset to make sense for gpstk
- Add BDS D2 support to new nav
- Add BDS D1 support to new nav
- Add support for Galileo I/NAV ISC (BGD)
- Implement GPS CNAV-2 ISC support
- Implement CNAV ISC processing
* Thu Jul 22 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.2.1 release
- Updated git url ref to new sgl-git
* Thu Jun 17 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.2.0 release
- Reformat Position tests (Position_T.cpp) to follow style.
- Fix typo bugs in two trop models.
- Add fortify pipeline yaml file
- Fix VectorOperators.hpp to put math functions in std namespace.
- Change header-file installation for debian to use versioned path names.
- Fix pipeline yaml for protected jobs.
- Fix library SONAME: use only major version
- Fix CNavMidiAlm to use unsigned sqrtA.
- Remove extraneous space in data/test_input_rinex3_nav_gal.20n
- Fix PNBGPSLNavDataFactory to use unsigned eccentricity in almanac.
- Fix OrbitDataKepler to properly handle Adot and dndot terms.
- Fix NavDataFactoryWithStore::addNavData to handle multiple time systems.
- Fix NavDataFactoryWithStore static initialization problem.
- Fix MultiFormatNavDataFactory time system handling in getInitialTime() and getFinalTime().
- Add DebugTrace code for execution tracing.
* Thu May 13 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.1.0 release
- Fix CNavMidiAlm to use unsigned sqrtA.
- Remove extraneous space in data/test_input_rinex3_nav_gal.20n
- Fix PNBGPSLNavDataFactory to use unsigned eccentricity in almanac.
- Fix OrbitDataKepler to properly handle Adot and dndot terms.
- Fix NavDataFactoryWithStore::addNavData to handle multiple time systems.
- Fix NavDataFactoryWithStore static initialization problem.
- Fix MultiFormatNavDataFactory time system handling in getInitialTime() and getFinalTime().
- Add DebugTrace code for execution tracing.
* Wed Apr 21 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.0.1 release
- Fix ext/lib/FileHandling/SEM/SEMData.cpp: correct faulty unit conversion.
* Thu Mar 18 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v11.0.0 release
- Deprecate external users contributed code and references.
- Update source code files copyright header for year 2021
- Fix PNBGPSCNav2DataFactory comment
- Remove SVNumXref (deprecated code).
- Fix StringUtils::words() for cases of empty strings or single words.
* Thu Feb 11 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v10.0.0 release
- Update RINEX NAV test data files that had improperly formatted DELTA-UTC records.
- Update NavDataFactory::getFactoryFormats() methods to prevent printing formats that don't support the desired message types.
- Add RinexNavDataFactory support of time offset data in RINEX NAV header.
- Fix NavDataFactoryWithStore::edit() and clear() to process time offset data.
- Fix NavDataFactoryWithStore::getOffset() so it doesn't fail when reversing the offset look-up (e.g. GPS->UTC vs UTC->GPS).
- Update MultiFormatNavDataFactory to implement getOffset(), getInitialTime() and getFinalTime()
- Update Removed obsolete files
- Fix test binaries to be excluded from bin
- Update TimeSystemCorrection (RINEX3 NAV header) to store reference time in CommonTime.
- Update TimeSystemCorr_T to use test macros and such for improved readability.
- Fix "implement proper command line option help where needed"
- Update sp3version to use Frameworks for better command line option help.
- Update bc2sp3 to use Frameworks for better command line option help.
- Fix NavDataFactory classes to enforce proper initialization and destruction.
- Fix bug in WindowFilter due to uninitialized boolean.
- Add TimeTag changeTimeSystem methods for both static and specified TimeSystemConverter objects.
- Add CommonTime changeTimeSystem method that uses a static TimeSystemConverter.
- Add NavTimeSystemConverter that uses a NavLibrary object to get nav-sourced time offset data.
- Fix NavDataFactoryWithStore infinite loop bug in find.
- Update SWIG to remove pseudo-mapping that conflicts with SatID constructor changes.
- Fix OrbAlmGen to decode A**1/2 as unsigned rather than signed.
- Add TimeCorrection support for 8-bit week rollover correction.
- Update GPSWeekZcount to support printing day of week by name.
- Update TestUtil to print invalid boolean values as 'true' or 'false' and increased precision when printing invalid floating point values.
- Add NewNav code tree to replace existing nav store classes (in core/lib/NewNav and ext/lib/NewNav).
- Add PackedNavBits support for decoding more types.
- Add NavType wildcard value.
- Add SatID wildcard support for system and ID.
- Add ObsID support for GLONASS frequency offset and mcode metadata.
- Update CommandOption to allow changing the description post-constructor.
- Update Doxyfile to generate graphs by default.
- Update TimeTag_T.cpp to follow proper style
* Thu Jan 14 2021 David Barber <dbarber@arlut.utexas.edu>
- Updated for v9.1.0 release
- Update various source files to add namespace context where it was missing.
- Update CommonTime/TimeSystem Redesign
- Update CommonTime to implement changeTimeSystem method.
- Add BasicTimeSystemConversion class to wrap existing second-precision TimeSystem conversion.
- Add TimeSystemConversion class to support generalized time system conversion.
- Update TimeSystem to support conversion to/from QZSS
- Update buil.sh Allow better error reporting
- Update EngNav decoding internals to use C++ instead of C storage
- Update SatMetaDataStore to improve documentation.
- Update EngNav documentation for clarity.
- Add to CommandOptionParser the ability to show program usage in a format that can be used in doxygen.
- Remove latex documentation in favor of doxygen.
- Update across the library to improve documentation.
- Add accessors to the ObsID description maps for SWIG.
- Update CI pipelines to make protected word searches advisory only.
- Fix conda recipe to specify correct default branch for GPSTk.
* Thu Dec 10 2020 David Barber <dbarber@arlut.utexas.edu>
- Updated for v9.0.0 release
- Update File_Hunter related files were deprecated and moved to tks_legacy_archive repo
- Update readme.
- Modify IonoModel to accommodate Klo params in different units.
- Update the CI pipeline to reflect that the new default branch name.
- Add CI stage to check files and history for sensitive words.
* Thu Nov 19 2020 David Barber <dbarber@arlut.utexas.edu>
- Updated for v8.0.0 release
- Update IonoModel to include all modernized GPS bands.
- Update SVNumXRef Add SVN77/PRN14
- Fix OrbSysGpsL_56 Correct iono parameter units
- Update CI to verify proper copyright and license header.
- Refactor pipeline to use git https protocol instead of ssh
- Update copyright language to reflect year 2020
- Fix EL8 RPM Generation to avoid including build-id files.
- Add Centos8 build/test/package/deploy jobs to the CI pieline.
- Refactor `PRSolution2` to `PRSolutionLegacy`.
- Update CODEOWNERS file.
- Fix SP3EphemerisStore to properly handle correlation data from SP3c files.
- Refactor calculate_ord method definition out of header file.
- Update python 2.7 conda recipe to avoid using preprocessig-selector for enum34.
- Update Nav reader code to properly assign being/end validity values for non-GPS GNSS data.
* Fri Sep 18 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v7.0.0 release
- Fix GalEphemeris Corrected behavior of isHealthy()
- Update SWIG Bindings to wrap enumerations more correctly.
- Update OceanLoadTides.hpp Add reference to SPOTL in doxygen comments
- Refactor gpstk pipeline downstream jobs
- Refactor debian dpkg-buildpackage Update control files to remove python bindings
- Refactor debian8 remove python bindings
- Dropping Python Swig binding support for RHEL7/DEB7
- Refactor ObsID and RinexObsID initialization of containers (tcDesc, etc.) to use C++11 syntax and eliminate the use of a singleton initializer.
- Move RINEX-isms (such as the string constructor for decoding RINEX obs IDs) in ObsID into RinexObsID where they belong.
- Refactor swig bindings for enums to use similar naming conventions between C++ and python (e.g. gpstk::TrackingCode::CA in C++ and gpstk.TrackingCode.CA in python)
- Add EnumIterator class to provide the ability to iterate over the above enum classes.
- Rename TrackingCode enumerations to better support codes that RINEX does not by using names based on the ICDs rather than what RINEX uses.
- Refactor enumerations in TimeSystem, ReferenceFrame, IERSConvention, SatID, NavID and ObsID (SatelliteSystem, NavType, ObservationType, CarrierBand, TrackingCode) to use strongly typed enumerations and move them outside the scope of those classes.
- Add SatMetaDataStore findSatByFdmaSlot
- Add Documentation Artifact to CI Pipeline
- Refactor Yuma/SEM file support back into ext
- Fix core/lib include statements to search GPSTk include directory.
- Fix OrbSysGpsL_55 Restore output of text message
- Fix various pieces of code to resolve warning messages on various platforms.
- Add CODEOWNERS file.
* Mon Sep 14 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v7.0.0 release
- Update OceanLoadTides.hpp Add reference to SPOTL in doxygen comments
- Refactor gpstk pipeline downstream jobs
- Refactor debian dpkg-buildpackage Update control files to remove python bindings
- Refactor debian8 remove python bindings
- Dropping Python Swig binding support for RHEL7/DEB7
- Refactor ObsID and RinexObsID initialization of containers (tcDesc, etc.) to use C++11 syntax and eliminate the use of a singleton initializer.
- Move RINEX-isms (such as the string constructor for decoding RINEX obs IDs) in ObsID into RinexObsID where they belong.
- Refactor swig bindings for enums to use similar naming conventions between C++ and python (e.g. gpstk::TrackingCode::CA in C++ and gpstk.TrackingCode.CA in python)
- Add EnumIterator class to provide the ability to iterate over the above enum classes.
- Rename TrackingCode enumerations to better support codes that RINEX does not by using names based on the ICDs rather than what RINEX uses.
- Refactor enumerations in TimeSystem, ReferenceFrame, IERSConvention, SatID, NavID and ObsID (SatelliteSystem, NavType, ObservationType, CarrierBand, TrackingCode) to use strongly typed enumerations and move them outside the scope of those classes.
- Add SatMetaDataStore findSatByFdmaSlot
- Add Documentation Artifact to CI Pipeline
- Refactor Yuma/SEM file support back into ext
- Fix core/lib include statements to search GPSTk include directory.
- Fix OrbSysGpsL_55 Restore output of text message
- Fix various pieces of code to resolve warning messages on various platforms.
- Add CODEOWNERS file.
* Thu Jul 30 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v6.0.0 release
- Update SWIG bindings to fix memory leaks for Matrix.
- Fix YDSTime to eliminate use of uninitialized variables.
- Fix FFBinaryStraem to eliminate use of uninitialized variables.
- Fix TimeRange_T to eliminate dereferencing of unallocated memory pointers.
- Fix OrbElem to eliminate use of uninitialized variables.
- Fix BDSEphemeris to eliminate use of uninitialized variables.
- Fix GlobalTropModel to eliminate use of uninitialized variables.
- Fix RinexObsHeader to eliminate use of uninitialized variables.
- Fix gdc::getArcStats to eliminate a memory leak.
- Fix WindowFilter to eliminate use of uninitialized variables.
- Fix OrbSysStore to eliminate a memory leak.
- Fix OrbAlmStore to eliminate a memory leak.
- Fix rstats to eliminate a memory leak.
- Fix dfix to eliminate use of uninitialized variables.
- Fix CNavFilter_T to eliminate a memory leak.
- Fix CNav2Filter_T to eliminate a memory leak.
- Fix Rinex3Obs_FromScratch_t.cpp to eliminate a memory leak.
- Fix CommandOption_T to eliminate a memory leak.
- Fix BasicFrameworkHelp_T to eliminate a memory leak.
- Fix RationalizeRinexNav to eliminate a memory leak.
- Fix OrbitEph to initialize all data members.
- Fix RinDump to eliminate a memory leak.
- Fix FFTextStream to eliminate an invalid memory read.
- Fix PRSolve to eliminate a memory leak.
- Fix rinheaddiff to eliminate a memory leak.
- Fix RinDump to eliminate a memory leak.
- Fix AntennaStore BeiDou phase center
- Update CI Pipeline to be more adept at producing downloadable packages.
- Update and Adding swig support for IonoModel and Nav Reader
- Update test data to use fortran format numbers in RINEX nav data.
- Refactor text stream classes to use stream operations instead of string construction.
- Update existing uses of doub2for etc. to use FormattedDouble.
- Deprecate doub2sci, doubleToScientific, sci2for, doub2for, and for2doub in StringUtils.
- Add FormattedDouble as a more versatile replacement for doub2for, for2doub, etc.
- Update gitlab-ci.yml to declare a registry associated with each image.
- Update SVNumXRef Add SVN76/PRN23
- Refactor SatMetaDataStore to split records to reduce redundancy of data, particularly of data that does not change over the life of the satellite.
- Update SVNumXRef Deassign PRN23 from SVN60
- Deprecate FileHunter class.
- Update AntexData Add BeiDou-3, QZSS, NAVIC satellite types
- Fix FileSpecFind to support both forward slash and backslash file separators under windows.
- Add Filter class to FileSpecFind to support matching of sets of values.
- Fix FileSpecFind to handle more use cases under windows.
- Add FileSpec methods hasTimeField and hasNonTimeField.
- Update FileSpec for a more optimal implementation of hasField
- Update FileFilterFrameWithHeader to use FileSpecFind instead of FileHunter.
- Update FileFilterFrame to use FileSpecFind instead of FileHunter.
- Add CI testing on Redhat 7 with SWIG 3 installed.
- Update CommonTime::setTimeSystem to return a reference to itself.
- Update swig CMakeLists.txt to preclude setting RUNPATH in _gpstk.so thus preventing it from pointing to the wrong libgpstk.so
- Fix SWIG vector constructor to take a list of integers
- Update Ashtech and other classes to prevent SWIG from generating bad code.
- Add Rinex3ObsHeader accessors for wavelengthFactor in support of python.
* Wed Jun 17 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v5.0.1 release
- Fix Swig wrapper of PRSolution to allow passing list of GNSS system.
* Fri Jun 12 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v5.0.0 release
- Note: Rinex 3.04 support is not fully in compliance with "9.1 Phase Cycle Shifts", although that does not affect any of our use cases.
- Update PreciseRange to input frequencies as well as GNSS.
- Update PRSolution for multi-GNSS solution, including one Rx clock per system.
- Update OrbitEph::adjustValidity to subtract two hours only for GPS.
- Add RinexObsID support for decoding channel number and ionospheric delay pseudo-observables.
- Add ObsID support for numerous previously unsupported codes.
- Add FreqConsts.hpp to replace the deprecated constants with new names based on ICD naming instead of RINEX.
- Remove frequency/wavelength constants from GNSSconstants.hpp and put them in DeprecatedConsts.hpp
- Update Rinex3ObsData to support channel number and ionospheric delay data.
- Fix QZSEphemeris to use a sane begin time for QZSS nav.
- Fix Rinex3NavData to use a sane begin time for QZSS nav.
- Fix Rinex3ClockData/Rinex3ClockHeader to decode systems using existing classes.
- Update PRSolve to input multiple GNSS options, and for multi-GNSS solution, including one Rx clock per system.
- Update RinSum to improve support aux headers over simple comment support.
- Update RinDump to support all current (as of RINEX 3.04) codes
- Add FileUtils/FileUtils_T comments explaining debian 7 test failures.
- Move a collection of tests and code from ext to core.
- Fix SWIG bindings to use Python-list to C-array typemaps in gpstk_typemaps.i
- Update RinSum to have better diagnostics when there is an invalid Rinex header
- Add BasicFramework documentation.
- Update CommandOption classes to return a const reference instead of a copy of a vector.
- Update Position::transformTo,asGeodetic,asECEF to return a reference.
- Fix Rinex3ObsHeader::Fields SWIG bindings.
- Fix Rinex3ObsHeader::Fields SWIG bindings.
- Add SWIG bindings to support the nested classes within Rinex3ObsHeader.
- Fix RINEX reference data for tests containing the "Signal Strenth" typo, or remove it for RINEX 2 tests.
- Update Rinex3ObsHeader to use a nested class called "Flags" for header field management, replacing the bit field.
- Modify installation script to create site-packages if it doesn't exist.
- Update Yuma_T test to use the most modern cmake test script and df_diff to account for minor differences in floating point least significant digits.
- Update rstats testing to use portable ctest mechanisms instead of the python script, and split the truth data accordingly.
- Remove FileHunter_T failing test from Windows build in preparation for future deprecation of the FileHunter class.
- Update FileSpec to use WIN32 precompiler macro instead of _WIN32 to get the correct file separator.
- Update CommandOptionParser to remove the .exe file extension when printing help under Windows.
- Fix RinexSatID/SP3SatID to properly handle QZSS and GEO satellite IDs over 100
- Add FileSpecFind as an eventual replacement for FileHunter
- Deprecate SVNumXRef in favor of SatMetaDataStore
- Update MJD/JD timeconvert tests to be able to pass under windows.
- Update CI to retain, as CI artifacts, output from failed tests.
- Fix decimation bug Rinex3ObsLoader corrected decimation logic
- Update OrbitEphStore to have more information when a potentially corrupt ephemeris is added.
- Add NavID Add methods to convert between enumeration and string and test them.
- Add ObsID Add methods to convert between enumeration and string and test them.
- Update SWIG files so that gpstk::Exception is treated as a Python Exception
- Add tests for computeXvt and getSVHealth with unhealthy satellites
* Mon Mar 30 2020 Andrew Kuck <kuck@arlut.utexas.edu>
- Updated for v4.0.0 release
- Update CMAKE to optimize at level O3 on linux by default.
- Updated call signatures in OrbElemRinex to support downstream callers.
- Add string conversions to-from SatID.SatelliteSystem
- Remove deprecated dynamic exception specifications.
- Update SVNumXRef to move PRN 18 from SVN 34 to SVN 75
- Add CI checks to ensure continued compatibility with Debian 7.
- Fix test failures observed running on Debian 10.
- Fix misleading comment on CommonTime.m_day
- Change OrbitEph to use CGCS2000 ellipsoid for BeiDou when computing relativity correction
- Fix BDSEphemeris to use GEO equations only above 7 degrees.
- Fix AlmOrbit to add missing final value of G.
- Fix BrcKeplerOrbit to use relativity correction and initial URE value of 0.
- Update svXvt to set Xvt.health when available.
- Fix slight errors in svXvt velocity computation.
- Modify RPM packages to seperate python installation from C++ apps/libs.
- Fix bug in timeconvert app breaking after year 2038
- Fix incorrect interval in output header from RinEdit if thinning option is used.
- Fix memory leak during OrbElmStore deallocation.
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
