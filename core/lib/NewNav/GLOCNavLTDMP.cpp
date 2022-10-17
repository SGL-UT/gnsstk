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

#include "GLOCNavLTDMP.hpp"
#include "DebugTrace.hpp"

using namespace std;

namespace gnsstk
{
   GLOCNavLTDMP ::
   GLOCNavLTDMP()
         : tb31(-1), tb32(-1),
           dax0(std::numeric_limits<double>::quiet_NaN()),
           day0(std::numeric_limits<double>::quiet_NaN()),
           daz0(std::numeric_limits<double>::quiet_NaN()),
           ax1(std::numeric_limits<double>::quiet_NaN()),
           ay1(std::numeric_limits<double>::quiet_NaN()),
           az1(std::numeric_limits<double>::quiet_NaN()),
           ax2(std::numeric_limits<double>::quiet_NaN()),
           ay2(std::numeric_limits<double>::quiet_NaN()),
           az2(std::numeric_limits<double>::quiet_NaN()),
           ax3(std::numeric_limits<double>::quiet_NaN()),
           ay3(std::numeric_limits<double>::quiet_NaN()),
           az3(std::numeric_limits<double>::quiet_NaN()),
           ax4(std::numeric_limits<double>::quiet_NaN()),
           ay4(std::numeric_limits<double>::quiet_NaN()),
           az4(std::numeric_limits<double>::quiet_NaN())
   {
   }


   bool GLOCNavLTDMP ::
   validate() const
   {
      return ((tb31 == tb32) &&
              (std::isnan(dax0) == 0) &&
              (std::isnan(day0) == 0) &&
              (std::isnan(daz0) == 0) &&
              (std::isnan(ax1) == 0) &&
              (std::isnan(ay1) == 0) &&
              (std::isnan(az1) == 0) &&
              (std::isnan(ax2) == 0) &&
              (std::isnan(ay2) == 0) &&
              (std::isnan(az2) == 0) &&
              (std::isnan(ax3) == 0) &&
              (std::isnan(ay3) == 0) &&
              (std::isnan(az3) == 0) &&
              (std::isnan(ax4) == 0) &&
              (std::isnan(ay4) == 0) &&
              (std::isnan(az4) == 0) &&
              ((dax0 != 0) ||
               (day0 != 0) ||
               (daz0 != 0) ||
               (ax1 != 0) ||
               (ay1 != 0) ||
               (az1 != 0) ||
               (ax2 != 0) ||
               (ay2 != 0) ||
               (az2 != 0) ||
               (ax3 != 0) ||
               (ay3 != 0) ||
               (az3 != 0) ||
               (ax4 != 0) ||
               (ay4 != 0) ||
               (az4 != 0)));
   }


   Vector<double> GLOCNavLTDMP ::
   geta(double deltat) const
   {
         // I'm not positive but I think this is basically trading
         // optimal speed for optimal precision (by avoiding using
         // really big numbers in conjunction with really small
         // numbers).
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("dax0 = " << scientific << dax0);
      DEBUGTRACE("ax1 = " << scientific << ax1);
      DEBUGTRACE("ax2 = " << scientific << ax2);
      DEBUGTRACE("ax3 = " << scientific << ax3);
      DEBUGTRACE("ax4 = " << scientific << ax4);
      Vector<double> rv({
            dax0 + ax1*deltat + ax2*deltat*deltat + ax3*deltat*deltat*deltat +
            ax4*deltat*deltat*deltat*deltat,
            day0 + ay1*deltat + ay2*deltat*deltat + ay3*deltat*deltat*deltat +
            ay4*deltat*deltat*deltat*deltat,
            daz0 + az1*deltat + az2*deltat*deltat + az3*deltat*deltat*deltat +
            az4*deltat*deltat*deltat*deltat });
      DEBUGTRACE("dt=" << fixed << deltat << setprecision(12) << scientific
                 << "  rv={" << rv(0) << ", " << rv(1) << ", " << rv(2) << "}");
      return rv;
   }


   void GLOCNavLTDMP ::
   dump(std::ostream& s) const
   {
      ios::fmtflags oldFlags = s.flags();
      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.fill(' ');
      s << scientific << setprecision(6)
        << "           LONG-TERM ORBIT PARAMETERS" << endl << endl
        << "                           "
        << "X                  Y                  Z"
        << endl
        << "da0:        " << setw(16) << dax0 << ",  " << setw(16) << day0
        << ",  " << setw(16) << daz0 << " km/s**2" << endl
        << "a1:         " << setw(16) << ax1 << ",  " << setw(16) << ay1
        << ",  " << setw(16) << az1 << " km/s**3" << endl
        << "a2:         " << setw(16) << ax2 << ",  " << setw(16) << ay2
        << ",  " << setw(16) << az2 << " km/s**4" << endl
        << "a3:         " << setw(16) << ax3 << ",  " << setw(16) << ay3
        << ",  " << setw(16) << az3 << " km/s**5" << endl
        << "a4:         " << setw(16) << ax4 << ",  " << setw(16) << ay4
        << ",  " << setw(16) << az4 << " km/s**6" << endl;
      s.flags(oldFlags);
   }

} // namespace gnsstk
