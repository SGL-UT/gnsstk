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
#include "GLOCNavAlm.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "GLOCNavEph.hpp"
#include "DebugTrace.hpp"

using namespace std;

namespace gnsstk
{
   const PZ90Ellipsoid GLOCNavAlm::ell;
   const double GLOCNavAlm::GM = ell.gm_km();
   const double GLOCNavAlm::ae = ell.a_km();
      /** @todo These constants are defined in the individual ICDs for
       * each signal.  They're currently taken from the L3 ICD as I
       * don't have any L1 or L2 data.  As such, they may need to
       * change to signal-dependent in the future. */
   const double GLOCNavAlm::iav = 64.8;
   const double GLOCNavAlm::Tav = 40544.0;
   const double GLOCNavAlm::J20 = 1082625.75e-9;


   GLOCNavAlm ::
   GLOCNavAlm()
         : orbitType(GLOCOrbitType::Unknown),
           numSVs(0),
           aoa(0),
           NA(0),
           statusReg(0),
           satType(GLOCSatType::Unknown),
           tau(std::numeric_limits<double>::quiet_NaN()),
           lambda(std::numeric_limits<double>::quiet_NaN()),
           tLambda(std::numeric_limits<double>::quiet_NaN()),
           deltai(std::numeric_limits<double>::quiet_NaN()),
           ecc(std::numeric_limits<double>::quiet_NaN()),
           omega(std::numeric_limits<double>::quiet_NaN()),
           deltaT(std::numeric_limits<double>::quiet_NaN()),
           deltaTdot(std::numeric_limits<double>::quiet_NaN())
   {
      signal.messageType = NavMessageType::Almanac;
      msgLenSec = 3.0;
   }


   bool GLOCNavAlm ::
   validate() const
   {
         /// @todo implement some checking.
      return true;
   }


   bool GLOCNavAlm ::
   getXvt(const CommonTime& when, Xvt& xvt, const ObsID& oid)
   {
      xvt = math.getXvt(*this, when);
      xvt.health = (header.svUnhealthy | header.dataInvalid
                    ? Xvt::Unhealthy : Xvt::Healthy);
         /// @todo Add some sanity checks later.
      return true;
   }


   CommonTime GLOCNavAlm ::
   getUserTime() const
   {
      return timeStamp + 3.0;
   }


   void GLOCNavAlm ::
   fixFit()
   {
      DEBUGTRACE_FUNCTION();
         // There is no stated fit interval or period of validity
         // for almanac data.  However, it is generally safe to
         // assume that the data should not be used before
         // the transmit time. 
      beginFit = timeStamp;
         /// @todo get a better end fit interval than this.
      endFit = CommonTime::END_OF_TIME;
      endFit.setTimeSystem(beginFit.getTimeSystem());
   }


   void GLOCNavAlm ::
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

      s << "**************************************************************"
        << endl
        << " GLONASS ORB/CLK (NON-IMMEDIATE) PARAMETERS" << endl << endl
        << "SAT : " << signal.sat << endl << "      "
        << StringUtils::asString(satType) << endl << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
      {
         return;
      }

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

      header.dumpOverHeader(s);
      header.dumpStrOverhead("STR20:", s);

      string tform("%02m/%02d/%Y %03j %02H:%02M:%02S  %7.0s  %P");
      s << endl
        << "           TIMES OF INTEREST" << endl << endl
        << "              MM/DD/YYYY DOY HH:MM:SS      SOD\n"
        << "Begin Valid:  " << printTime(beginFit,tform) << endl
        << "Orbit Epoch:  "
        << printTime(Toa,tform) << endl
        << "End Valid:    " << printTime(endFit,tform) << endl
        << endl
        << "           ORBIT PARAMETERS" << endl << endl
        << "Parameter              Value" << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(6);
      s.fill(' ');

      s << "orbit       " << setw(16) << (int)orbitType << " "
        << StringUtils::asString(orbitType) << endl
        << "alm SVs     " << setw(16) << numSVs << endl
        << "alm age     " << setw(16) << aoa << " days" << endl
        << "status      " << setw(16) << hex << showbase << nouppercase
        << statusReg << dec << noshowbase;
      if (statusReg & 0x10)
      {
         s << " L1";
      }
      if (statusReg & 0x08)
      {
         s << " L2";
      }
      if (statusReg & 0x04)
      {
         s << " L3";
      }
      s << endl
        << "tau         " << setw(16) << tau  << " sec" << endl
        << "lambda      " << setw(16) << lambda << " rad" << endl
        << "di          " << setw(16) << deltai << " rad" << endl
        << "e           " << setw(16) << ecc << " dimensionless" << endl
        << "omega       " << setw(16) << omega << " rad" << endl
        << "tLambda     " << setw(16) << tLambda << " seconds" << endl
        << "dT          " << setw(16) << deltaT << " sec/orbit" << endl
        << "dTd         " << setw(16) << deltaTdot << " sec/orbit**2" << endl;

      s.setf(ios::fixed, ios::floatfield);
      s.precision(0);
      s << "Transmit SV " << setw(16) << signal.xmitSat << endl;

      s.flags(oldFlags);
   }


   void GLOCNavAlm ::
   dumpTerse(std::ostream& s) const
   {
      string tform2("%02m/%02d/%4Y %03j %02H:%02M:%02S");
      stringstream ss;
      ss << "  " << setw(2) << signal.sat.id << "  ";
      ss << printTime(beginFit,tform2);
      ss << " ! ";
      ss << printTime(Toa,tform2) << " ! ";
      if (!header.svUnhealthy)
      {
         ss << " op";
      }
      else
      {
         ss << "bad";
      }
      ss << "    " << setw(2) << signal.sat.id;
      s << ss.str();
   }
   

   GLOCNavAlm::Perturbations ::
   Perturbations()
         : a(std::numeric_limits<double>::quiet_NaN()),
           h(std::numeric_limits<double>::quiet_NaN()),
           l(std::numeric_limits<double>::quiet_NaN()),
           lambda(std::numeric_limits<double>::quiet_NaN()),
           omega(std::numeric_limits<double>::quiet_NaN()),
           i(std::numeric_limits<double>::quiet_NaN()),
           Lk(std::numeric_limits<double>::quiet_NaN())
   {
   }


   GLOCNavAlm::Perturbations GLOCNavAlm::Perturbations ::
   operator-(const Perturbations& right) const
   {
      Perturbations rv;
      rv.a = a-right.a;
      rv.h = h-right.h;
      rv.l = l-right.l;
      rv.lambda = lambda-right.lambda;
      rv.omega = omega-right.omega;
      rv.i = i-right.i;
      rv.Lk = Lk-right.Lk;
      return rv;
   }


   GLOCNavAlm::NumberCruncher ::
   NumberCruncher()
         : Deltatpr(std::numeric_limits<double>::quiet_NaN()),
           W(-1),
           Tdr(std::numeric_limits<double>::quiet_NaN()),
           n(std::numeric_limits<double>::quiet_NaN()),
           E0(std::numeric_limits<double>::quiet_NaN()),
           L1(std::numeric_limits<double>::quiet_NaN()),
           L(std::numeric_limits<double>::quiet_NaN()),
           B(std::numeric_limits<double>::quiet_NaN())
   {
   }


   GLOCNavAlm::Uncorrected ::
   Uncorrected()
         : p(std::numeric_limits<double>::quiet_NaN())
   {
   }


   GLOCNavAlm::Corrected ::
   Corrected()
         : p(std::numeric_limits<double>::quiet_NaN()),
           E(std::numeric_limits<double>::quiet_NaN()),
           epsilon(std::numeric_limits<double>::quiet_NaN()),
           nu(std::numeric_limits<double>::quiet_NaN()),
           u(std::numeric_limits<double>::quiet_NaN()),
           r(std::numeric_limits<double>::quiet_NaN()),
           vu(std::numeric_limits<double>::quiet_NaN()),
           vr(std::numeric_limits<double>::quiet_NaN())
   {
   }
}
