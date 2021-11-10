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
#include "GLOFNavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GLOFNavAlm ::
   GLOFNavAlm()
         : healthBits(false),
           tau(std::numeric_limits<double>::quiet_NaN()),
           lambda(std::numeric_limits<double>::quiet_NaN()),
           deltai(std::numeric_limits<double>::quiet_NaN()),
           ecc(std::numeric_limits<double>::quiet_NaN()),
           omega(std::numeric_limits<double>::quiet_NaN()),
           tEpoch(std::numeric_limits<double>::quiet_NaN()),
           deltaT(std::numeric_limits<double>::quiet_NaN()),
           deltaTdot(std::numeric_limits<double>::quiet_NaN()),
           freq(-1)
   {
      signal.messageType = NavMessageType::Almanac;
      msgLenSec = 4.0;
   }


   bool GLOFNavAlm ::
   validate() const
   {
         /// @todo implement some checking.
      return true;
   }


   bool GLOFNavAlm ::
   getXvt(const CommonTime& when, Xvt& xvt)
   {
      return false;
   }


   CommonTime GLOFNavAlm ::
   getUserTime() const
   {
      CommonTime mr = std::max({timeStamp, xmit2});
      return mr + 2.0;
   }


   void GLOFNavAlm ::
   fixFit()
   {
         // There is no stated fit interval or period of validity
         // for almanac data.  However, it is generally safe to
         // assume that the data should not be used before
         // the transmit time. 
      beginFit = timeStamp;
         /// @todo get a better end fit interval than this.
      endFit = CommonTime::END_OF_TIME;
      endFit.setTimeSystem(beginFit.getTimeSystem());
   }


   void GLOFNavAlm ::
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

      // s << "****************************************************************"
      //   << "************" << endl
      //   << "GLONASS ORB/CLK (IMMEDIATE) PARAMETERS" << endl << endl
      //   << "SAT : " << signal.sat << endl << endl;
      s << "**************************************************************"
        << endl
        << " GLONASS ORB/CLK (NON-IMMEDIATE) PARAMETERS for GLONASS Slot "
        << signal.sat.id << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
         return;

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

         /** @bug this shouldn't be here, but I'm putting it here
          * temporarily to make it easier to compare with legacy code
          * output. */
      CommonTime xmit(timeStamp);
      xmit.setTimeSystem(TimeSystem::GLO);
         /* ^^ */
      string tform("%02m/%02d/%Y %03j %02H:%02M:%02S  %7.0s  %P");
      s << endl
        << "              MM/DD/YYYY DOY HH:MM:SS      SOD\n"
        << "Transmit   :  "
        << printTime(xmit,tform) << endl
        << "Orbit Epoch:  "
        << printTime(Toa,tform) << endl
        << endl
        << "Parameter              Value" << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(6);
      s.fill(' ');

      s << "tau         " << setw(16) << tau  << " sec" << endl
        << "lambda      " << setw(16) << lambda << " rad" << endl
        << "di          " << setw(16) << deltai << " rad" << endl
        << "e           " << setw(16) << ecc << " dimensionless" << endl
        << "omega       " << setw(16) << omega << " rad" << endl
        << "tEpoch      " << setw(16) << tEpoch << " seconds" << endl
        << "dT          " << setw(16) << deltaT << " sec/orbit" << endl
        << "dTd         " << setw(16) << deltaTdot << " sec/orbit**2" << endl;

      s.setf(ios::fixed, ios::floatfield);
      s.precision(0);
      s << "C           " << setw(16) <<  healthBits << " encoded:";
      if (healthBits == false)
      {
         s << " NON-operational";
      }
      else
      {
         s << " operational";
      }
      s << endl
        << "M           " << setw(16) << static_cast<int>(satType)
        << " encoded: " << StringUtils::asString(satType) << endl
        << "H           " << setw(16) << freq << " freq. offset" << endl
        << "Transmit SV " << setw(16) << signal.xmitSat << endl
        << "l           " << setw(16) << lhealth
        << " Health of transmitting SV" << endl;

      s.flags(oldFlags);
   }


   void GLOFNavAlm ::
   dumpTerse(std::ostream& s) const
   {
      string tform2("%02m/%02d/%4Y %03j %02H:%02M:%02S");
      stringstream ss;
      ss << "  " << setw(2) << signal.sat.id << "  ";
      ss << printTime(beginFit,tform2);
      ss << " ! ";
      ss << printTime(Toa,tform2) << " ! ";
      if (healthBits == true)
      {
         ss << " op";
      }
      else
      {
         ss << "bad";
      }
      ss << "    " << setw(2) << freq;
      ss << "    " << setw(2) << signal.sat.id;
      s << ss.str();
   }
}
