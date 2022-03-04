//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================

#include "logstream.hpp"

namespace gnsstk
{

      // This is a terrible kludge that nevertheless is necessary
      // under Windows 2003 and 2005 because global optimization
      // (compiler switch /O1 or /O2 or /Og) causes the reportingLevel
      // to be defined ONLY in the module in which ReportingLevel() is
      // assigned. Rather than give up optimization or
      // ReportingLevel(), I came up with this kludge, which is to
      // remove reportingLevel from the ReportingLevel() function and
      // make it a static member of the class, then initialize it
      // outside the class.
      // Similarly for dumpTimeTags and dumpLevels.
#ifdef WIN32
//#ifndef LOGSTREAM_INITIALIZE_REPORTING_LEVEL
//#define LOGSTREAM_INITIALIZE_REPORTING_LEVEL
   template<> LogLevel Log<ConfigureLOGstream>::reportingLevel = INFO;
   template<> bool Log<ConfigureLOGstream>::dumpTimeTags = false;
   template<> bool Log<ConfigureLOGstream>::dumpLevels = false;
//#endif
#endif
}
