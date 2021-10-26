//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
#include <math.h>
#include "InterSigCorr.hpp"
#include "FreqConv.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   InterSigCorr ::
   InterSigCorr()
         : isc(std::numeric_limits<double>::quiet_NaN()),
           iscLabel("ISC")
   {
      signal.messageType = NavMessageType::ISC;
   }


   void InterSigCorr ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (dl == DumpDetail::OneLine)
      {
         NavData::dump(s,dl);
         return;
      }
         // "header"
      s << "****************************************************************"
        << "************" << endl
        << "Inter-signal Corrections" << endl << endl
        << "PRN : " << setw(2) << signal.sat << " / "
        << "SVN : " << setw(2);
      std::string svn;
      if (getSVN(signal.sat, timeStamp, svn))
      {
         s << svn;
      }
      s << endl << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
         return;

      const ios::fmtflags oldFlags = s.flags();

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

      s << "           TIMES OF INTEREST" << endl << endl
        << "              " << getDumpTimeHdr(dl) << endl
        << "Transmit:     " << getDumpTime(dl, timeStamp) << endl;

      s << endl
        << "           SIGNALS"
        << endl << endl
        << "               BAND          CODE" << endl;
      for (const auto& oid : refOids)
      {
         s << "Reference: " << setw(8) << ObsID::cbDesc[oid.band]
           << "  " << setw(12) << ObsID::tcDesc[oid.code] << endl;
      }
      for (const auto& oid : validOids)
      {
         s << "Valid:     " << setw(8) << ObsID::cbDesc[oid.band]
           << "  " << setw(12) << ObsID::tcDesc[oid.code] << endl;
      }
      s << endl
        << "           CORRECTION"
        << endl << endl
        << setw(20) << left << (iscLabel+":");
      s.setf(ios::scientific, ios::floatfield);
      s.precision(8);
      s.fill(' ');
      s << isc << endl;

      s.flags(oldFlags);
   }


   bool InterSigCorr ::
   getISC(const ObsID& oid, double& corrOut)
      const
   {
      ObsID oidU(oid);
      oidU.type = ObservationType::Unknown;
         // it's possible to have an empty validOids and still be
         // useful, but it is not possible to have an empty refOids or
         // isc=NaN and still be useful.
      if (isnan(isc) || refOids.empty())
         return false;
      if (refOids.count(oidU))
      {
         corrOut = -isc;
      }
      else if (validOids.count(oidU))
      {
         corrOut = -(getGamma(refOids.begin()->band, oidU.band) * isc);
      }
      else
      {
         return false;
      }
      return true;
   }


   bool InterSigCorr ::
   getISC(const ObsID& oid1, const ObsID& oid2, double& corrOut)
      const
   {
      corrOut = 0;
      return true;
   }


   double InterSigCorr ::
   getGPSISC(const PackedNavBitsPtr& navIn, unsigned startBit)
   {
      double rv = 0;
         // GPS ISC/Tgd are always 13 bits x 2^-35
         // but we scale tgdBits by 1 to match the bit pattern.
      unsigned long tgdBits = navIn->asUnsignedLong(startBit, 13, 1);
      if (tgdBits == 0x1000)
      {
            // set to NaN because tgd is not available
         rv = std::numeric_limits<float>::quiet_NaN();
      }
      else
      {
         rv = navIn->asSignedDouble(startBit, 13, -35);
      }
      return rv;
   }
}
