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
#include "GLOCNavIono.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "FreqConv.hpp"
//#include "iossmart.hpp"

using namespace std;

namespace gnsstk
{
   GLOCNavIono ::
   GLOCNavIono()
         : peakTECF2(std::numeric_limits<double>::quiet_NaN()),
           solarIndex(std::numeric_limits<double>::quiet_NaN()),
           geoIndex(std::numeric_limits<double>::quiet_NaN())
   {
      msgLenSec = 3.0;
   }


   bool GLOCNavIono ::
   validate() const
   {
         /// @todo Implement some validation.
      return true;
   }


   void GLOCNavIono ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');
      switch (dl)
      {
         case DumpDetail::OneLine:
            NavData::dump(s,dl);
            break;
         case DumpDetail::Brief:
            NavData::dump(s,dl);
            s << "  c_A = " << peakTECF2
              << "  c_F10.7 = " << solarIndex
              << "  c_Ap = " << geoIndex
              << endl;
            break;
         case DumpDetail::Full:
               // "header"
            s << "*************************************************************"
              << "***************" << endl
              << "Ionospheric correction data" << endl << endl
              << "PRN : " << setw(2) << signal.sat << " / "
              << "SVN : " << setw(2);
            std::string svn;
            if (getSVN(signal.sat, timeStamp, svn))
            {
               s << svn;
            }
            s << endl << endl
              << "           TIMES OF INTEREST"
              << endl << endl
              << "              " << getDumpTimeHdr(dl) << endl
              << "Transmit:     " << getDumpTime(dl, timeStamp) << endl
              << endl
              << "           IONO PARAMETERS" << endl << endl;
                  // format chosed based on precision available in data
              // << "c_A                " << scifmt(16,7) << peakTECF2 << endl
              // << "c_F10.7            " << scifmt(16,4) << solarIndex << endl
              // << "c_Ap               " << fixedfmt(16,0) << geoIndex << endl;
            break;
      }
      s.flags(oldFlags);
   }


   double GLOCNavIono ::
   getIonoCorr(const CommonTime& when,
               const Position& rxgeo,
               const Position& svgeo,
               CarrierBand band) const
   {
         /// @todo implement this
      return std::numeric_limits<double>::quiet_NaN();
   }
}
