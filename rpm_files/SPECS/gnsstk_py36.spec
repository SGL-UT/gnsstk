%define name python3-gnsstk
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
