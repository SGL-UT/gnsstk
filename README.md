\page README README

README
------

Github - 
[![Build Status](https://travis-ci.org/SGL-UT/GNSSTk.svg?branch=master)](https://travis-ci.org/SGL-UT/GNSSTk)

NOTICE:
-------

On November 30, 2020 there was a push of many pending changes to the GNSSTk 
repository.  We apologize for the long period without updates, the year 2020
has been a trying time in many respects.

The most obvious change has been a transition to a semantic versioning scheme
(semver.org).  This is intended to be an informal contract regarding the API
that is presented by our tools.  It should always be safe to update to the next 
version, so long as the major version number doesn't change.  
 
Releases containing API changes, however small, will result in a major version 
number increment. Releases containing new features will result in a minor 
version number increment. Releases containing only bug fixes will have a patch
number increment. 


Contents:
---------

* Introduction
* Description: GNSSTk C++ Library
* Description: GNSSTk C++ Applications
* Description: GNSSTk Python Bindings
* Installation
* Testing
* Help & Docs
* Contribution guidelines
* Contributor list
* Credits & Lineage
* License


Introduction:
-------------

The GNSS Toolkit (GNSSTk) is an open-source (LGPL) project sponsored by
the Space and Geophysics Laboratory (SGL), part of the Applied Research 
Laboratories (ARL) at The University of Texas at Austin.

The primary goals of the GNSSTk project are to:

* provide applications for use by the GNSS and satellite navigation community.
* provide a core library to facilitate the development of GNSS applications.


Description: GNSSTk C++ Library:
-------------------------------

The GNSSTk core library provides a number of models and algorithms found in GNSS
textbooks and classic papers, such as solving for the user position or estimating
atmospheric refraction. Common data formats such as RINEX are supported as well. 

There are several categories of functions in the GNSSTk library:

   1. GPS time. Conversion among time representations such as MJD, GPS week 
      and seconds of week, and many others.

   2. Ephemeris calculations. Position and clock interpolation for both
      broadcast and precise ephemerides.

   3. Atmospheric delay models. Includes ionosphere and troposphere models.

   4. Position solution. Includes an implementation of a Receiver Autonomous 
      Integrity Monitoring algorithm.

   5. Mathematics. Includes Matrix and Vector implementations, as well as
      interpolation and numerical integration.

   6. GNSS data structures. Data structures that contain observations mapped
      according to epochs, satellites, sources and types of observations.
      Appropriate processing classes are also provided, including a complete
      'Precise Point Positioning' (PPP) processing chain.

   7. Application framework. Includes processing command lines options,
      providing interactive help and working with file systems.

A more detailed description of the functionality provided by the GNSSTk library 
can be found in the Doxygen documentation on the GNSSTk website.

    http://www.gnsstk.org/bin/view/Documentation/WebHome

The GNSSTk Core Library and its associated test programs can be built 
independently of building the GNSSTk Applications or Auxiliary Libraries.
The GNSSTk Core Library source code contains no dependencies outside of the
GNSSTk Core Library and Standard C++ and will build cleanly on all 
supported platforms.


Description: GNSSTk C++ Applications:
------------------------------------

The GNSSTk libraries are the foundation for the GNSSTk applications suite.
The applications support greater depth of functionality to support research 
and development. The applications are almost entirely console based (i.e., 
without a graphical user interface). They can be grouped functionally into 
the following categories:

   1. RINEX utilities - The RINEX utilities provide a set of applications 
      that can be used to examine, manipulate, and plot RINEX observation 
      files.

   2. Positioning - The positioning applications include two different 
      applications that perform standard pseudorange-based positioning and 
      two that implement differential phase-based solutions. 

   3. Residual analysis - A residual analysis application computes two types 
      of measurement residuals using a single receiver or two receivers in 
      a zero baseline configuration. 

   4. Ionospheric modeling - The ionospheric modeling applications utilize 
      the two frequency TEC estimate from the RINEX utilities and compute 
      a model of the ionosphere. 

   5. Signal Tracking Simulation - These utilities simulate the tracking 
      of GPS C/A and P-code.

   6. Basic transformations -  Conversions of time and coordinate systems.

   7. Observation data collection and conversion -  Translating receiver 
      specific data formats to RINEX.

   8. File comparison and validation - Differing observations files against 
     a truth source.

   9. Data editing - Simple editing like systematic removal of observations 
      by satellite, type or time and more advanced editing like cycle slip 
      detection and correction.

   10.Autonomous and relative positioning - Navigation and surveying 
      applications.


The GNSSTk applications are dependent on the GNSSTk libraries.  However, the 
GNSSTk applications may also contain external dependencies.  Some applications 
may not build or run successfully on all platforms. 


Description: GNSSTk Python Bindings:
-----------------------------------

The GNSSTk python extension package provides access to the GNSSTk C++ library 
from within python. It is built using SWIG and CMake, and installed with a 
standard setup.py script using the python distutils module.

For more details, see $GNSSTK/python/bindings/swig/install_package/README.txt


Installation:
-------------

See the INSTALL.txt for details.

The most recent version of the GNSSTk source code can be found here:

* http://www.gnsstk.org
* https://github.com/SGL-UT/GNSSTk

The tools used for the build and install frameworks are cross-platform.
These include CMake, SWIG, and python (distutils).

Automated build and install is supported on POSIX platforms with the 
supplied Bash script called build.sh. For help on script usage, 
run the script with the help flag:

    $ build.sh -h

For other platforms, such as Windows, please refer to comments and commands
in the same script, which document how we are using CMake, SWIG, and 
distutils for various steps in the build and install process.

To build and install the C++ library and applications on POSIX platforms:

* automated: run build.sh.
* manual: see the contents of build.sh for command examples on how you might run cmake and make to build and install the library.

To build and install the python bindings, you have two options:

* automated: python bindings automatically build with the build.sh. Use -P for install.
* manual build: see build.sh for examples on calling cmake and swig
* manual install: 


Testing:
--------

See the TESTING.txt for details.

	
Help & Docs:
------------

See the DOCUMENTATION.txt for details.

Additional documentation and resources can be found at http://www.gnsstk.org/, 
including:

* Source code and compiled binaries
* Coding examples
* Doxygen documentation
* System requirements and build instructions
* A users guide
* Publications
* Email lists
* Support question/answer
* Development process (including feature suggestions, bug tracking, schedule, testing, and developer documentation)
* Source code repository information
* GNSSTk IRC channel
* Success stories

Credits & Lineage:
------------------

GNSSTk is the by-product of GPS research conducted at ARL:UT since before the first 
satellite launched in 1978; it is the combined effort of many software 
engineers and scientists. In 2003 the research staff at ARL:UT decided to 
open source much of their basic GPS processing software as the GNSSTk.

The development history is documented by a series of related publications: 

	http://www.gnsstk.org/bin/view/Documentation/GNSSTkPublications

See the AUTHORS.txt file for additional detail.

License
-------

The source code provided by the GNSSTk is distributed under the GNU LGPL, Version 3.

* This license gives all users the right to use and redistribute the code.
* Users of the GNSSTk are not required to open their source, according to the LGPL.
* This makes the GNSSTk a practical choice for commercial projects.
* Full text copies of the GPL (COPYING.txt) and the LGPL (COPYING.LESSER.txt) are included with the GNSSTk distribution package. 

For more information about the GPL or LGP, please refer to the following:

* http://www.gnu.org/copyleft/lesser.html
* http://www.gnu.org/licenses/gpl-howto.html

