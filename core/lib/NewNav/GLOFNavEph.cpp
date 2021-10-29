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

#include "GLOFNavEph.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GLOFNavEph ::
   GLOFNavEph()
         : clkBias(std::numeric_limits<double>::quiet_NaN()),
           freqBias(std::numeric_limits<double>::quiet_NaN()),
           healthBits(-1),
           tb(-1),
           P1(-1), P2(-1), P3(-1), P4(-1),
           interval(-1),
           opStatus(PCode::Unknown),
           tauDelta(std::numeric_limits<double>::quiet_NaN()),
           aod(-1),
           accIndex(-1)
   {
      signal.messageType = NavMessageType::Ephemeris;
      msgLenSec = 8.0;
   }


   bool GLOFNavEph ::
   validate() const
   {
         /// @todo implement some checking.
      return true;
   }


   bool GLOFNavEph ::
   getXvt(const CommonTime& when, Xvt& xvt)
   {
      return false;
   }


   CommonTime GLOFNavEph ::
   getUserTime() const
   {
      CommonTime mr = std::max({timeStamp, xmit2, xmit3, xmit4});
      return mr + 2.0;
   }


   void GLOFNavEph ::
   fixFit()
   {
      beginFit = timeStamp;
         // See PNBGLOFNavDataFactory::processEph for more info
      unsigned kludge = (interval > 0 ? interval : 30);
         // half the interval in seconds = interval*60/2 = interval*30
      endFit = Toe + (kludge*30.0 + 30.0);
   }


   void GLOFNavEph ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (dl == DumpDetail::Terse)
      {
         dumpTerse(s);
         return;
      }
      ios::fmtflags oldFlags = s.flags();

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

      s << "****************************************************************"
        << "************" << endl
        << "GLONASS ORB/CLK (IMMEDIATE) PARAMETERS" << endl << endl
        << "SAT : " << signal.sat << endl << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
         return;

      string tform("%02m/%02d/%Y %03j %02H:%02M:%02S  %7.0s  %P");
      s << "           TIMES OF INTEREST" << endl
        << "              MM/DD/YYYY DOY HH:MM:SS      SOD" << endl
        << "Begin Valid:  " << printTime(beginFit,tform) << endl
        << "Orbit Epoch:  " << printTime(Toe,tform) << endl
        << "End Valid:    " << printTime(endFit,tform) << endl
        << endl
        << "Parameter              Value" << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(6);
      s.fill(' ');

      s << "X, X', X''  "
        << setw(16) << pos[0] << " m, "
        << setw(16) << vel[0] << " m/s, "
        << setw(16) << acc[0] << " m/s**2"
        << endl
        << "Y, Y', Y''  "
        << setw(16) << pos[1] << " m, "
        << setw(16) << vel[1] << " m/s, "
        << setw(16) << acc[1] << " m/s**2"
        << endl
        << "Z, Z', Z''  "
        << setw(16) << pos[2] << " m, "
        << setw(16) << vel[2] << " m/s, "
        << setw(16) << acc[2] << " m/s**2"
        << endl
        << "tau         " << setw(16) << clkBias  << " sec" << endl
        << "gamma       " << setw(16) << freqBias << " sec/sec" << endl
        << "tauDelta    " << setw(16) << tauDelta << " sec (interfreq bias)"
        << endl;

      s.setf(ios::fixed, ios::floatfield);
      s.precision(0);
      s << "t_b         " << setw(16) << tb << " increments" << endl
        << "P           " << setw(16) << static_cast<int>(opStatus)
        << " encoded: " << StringUtils::asString(opStatus) << endl
        << "P1          " << setw(16) <<  P1 << " encoded:"
        << " Time interval = " << interval << " minutes"
        << endl
        << "P2          " << setw(16) <<  P2 << " encoded:";
      switch (P2)
      {
         case 0:
            s << " even epoch time";
            break;
         case 1:
            s << " odd epoch time";
            break;
         default:
            s << " ?????";
            break;
      }
      s << endl
        << "P3          " << setw(16) <<  P3 << " encoded: ";
      switch (P3)
      {
         case 0:
            s << "4 satellites in almanac strings";
            break;
         case 1:
            s << "5 satellites in almanac strings";
            break;
         default:
            s << "?????";
            break;
      }
      s << endl
        << "P4          " << setw(16) << P4 << " data set change flag" << endl
        << "slot        " << setw(16) << slot << endl
        << "F_T         " << setw(16) << accIndex << " encoded, "
        << setw(5) << getAccuracy() << " m" << endl
        << "N_t         " << setw(16) << dayCount << " day of quadrennial"
        << endl
        << "E           " << setw(16) << aod << " age of info in days" << endl
        << "M           " << setw(16) << static_cast<int>(satType)
        << " encoded: " << StringUtils::asString(satType) << endl
        << "B           " << setw(16) << (unsigned)healthBits << " encoded: ";
      if (healthBits & 0x0004)
      {
         s << " MALFUNCTION";
      }
      else
      {
         s << " Functional";
      }
      s << endl
        << "l (String3) " << setw(16) << lhealth << " encoded: ";
      if (lhealth == 0)
      {
         s << " healthy";
      }
      else
      {
         s << " unhealthy";
      }
      s << endl
        << "Health (B&l)" << setw(16) << StringUtils::asString(health) << endl;
      if (!signal.obs.freqOffsWild)
      {
         s << "H           " << setw(16) << signal.obs.freqOffs
           <<" frequency number";
      }
      else
      {
         s << "H           " << "         unknown"
           <<" frequency number";
      }
      s << endl;

      s.flags(oldFlags);
   }


   void GLOFNavEph ::
   dumpTerse(std::ostream& s) const
   {
      string tform("%02H:%02M:%02S");
      string tform2("%02m/%02d/%4Y %03j %02H:%02M:%02S");
      s << "  " << setw(2) << signal.sat.id << "  "
        << printTime(beginFit,tform) << " ! "
        << printTime(Toe,tform2) << " ! "
        << printTime(endFit,tform) << "       "
        << lhealth << "     "
        << healthBits << "  "
        << endl;
   }


   double GLOFNavEph ::
   getAccuracy() const
   {
      double retVal = 0.0;
      switch (accIndex)
      {
        case  0: retVal =   1.0; break;
        case  1: retVal =   2.0; break;
        case  2: retVal =   2.5; break;
        case  3: retVal =   4.0; break;
        case  4: retVal =   5.0; break;
        case  5: retVal =   7.0; break;
        case  6: retVal =  10.0; break;
        case  7: retVal =  12.0; break;
        case  8: retVal =  14.0; break;
        case  9: retVal =  16.0; break;
        case 10: retVal =  32.0; break;
        case 11: retVal =  64.0; break;
        case 12: retVal = 128.0; break;
        case 13: retVal = 256.0; break;
        case 14: retVal = 512.0; break;
        case 15: 
        default:
           retVal = 0.0; 
      }
      return retVal; 
   }
}
