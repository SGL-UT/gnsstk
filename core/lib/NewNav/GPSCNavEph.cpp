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
#include <limits>  // quiet_NaN()

#include "GPS_URA.hpp"
#include "GPSCNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp"
#include "DebugTrace.hpp"
#include "TimeConstants.hpp"

using namespace std;

namespace gnsstk
{
   GPSCNavEph ::
   GPSCNavEph()
         : pre11(0),
           preClk(0),
           healthL1(true),
           healthL2(true),
           healthL5(true),
           uraED(-16),
           uraNED0(-16),
           uraNED1(0),
           uraNED2(0),
           alert11(false),
           alertClk(false),
           integStat(false),
           phasingL2C(false),
           deltaA(0.0),
           dOMEGAdot(0.0),
           top(gnsstk::CommonTime::BEGINNING_OF_TIME),
           xmit11(gnsstk::CommonTime::BEGINNING_OF_TIME),
           xmitClk(gnsstk::CommonTime::BEGINNING_OF_TIME)
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   bool GPSCNavEph ::
   validate() const
   {
      return GPSCNavData::validate() && ((pre11 == 0) || (pre11 == 0x8b)) &&
         ((preClk == 0) || (preClk == 0x8b));
   }


   CommonTime GPSCNavEph ::
   getUserTime() const
   {
      CommonTime mr = std::max({xmitTime, xmit11, xmitClk});
      if (signal.nav == NavType::GPSCNAVL2)
         return mr + 12.0;
      return mr + 6.0;
   }


   void GPSCNavEph ::
   fixFit()
   {
      // GPS CNav fit interval is fixed to three hours 
      // (IS-GPS-200N 30.3.3.1.1, IS-GPS-200N 30.3.4.4)
      uint16_t halfFitInterval = (SEC_PER_HOUR * 3) / 2;

      // By default, the Toe is assumed to be the midpoint of the curve fit interval.
      // (IS-GPS-200N 30.3.4.4)
      beginFit = Toe - halfFitInterval;
      endFit = Toe + halfFitInterval;
      
      // If the Toe is non-nominal then it indicates an upload cutover and Toe is not
      // the midpoint of the curve fit interval. The begin and end fit times must be adjusted.
      //    * A nominal Toe is always 1.5 hours ahead of 2 hour boundaries (IS-GPS-200N 30.3.4.5).
      //    * The start of the CEI dataset transmission interval corresponds to the beginning of the
      //      curve fit interval for the CEI. (IS-GPS-200N 30.3.4.4) 
      //      With a nominal Toe, the start of transmission can be inferred. For a non-nominal Toe, 
      //      the start of the transmission interval, and thus the beginning of the curve fit, 
      //      cannot be assumed. For non-nominal Toe, all we can assume is that we (hopefully) capture 
      //      the earliest transmission and set the begin fit time to that value.
      //    * For a non-nominal Toe, the end of the curve fit interval must be adjusted since the Toe
      //      is offset from the assumed curve fit interval midpoint. The Toe is offset
      //      by a small negative deviation (IS-GPS-200N 30.3.4.5) which would be 5 minutes
      //      for CNav due to the LSB scale factor (IS-GPS-200N Table 30-I). i.e. The midpoint
      //      is 5 minutes ahead of the Toe, so add 5 minutes to the end of the fit time.
      //    * QZSS does not specify the relationship of fit interval to upload cutovers. So we do not adjust them.
      uint32_t toeSOW = static_cast<uint32_t>(GPSWeekSecond(Toe).sow);
      bool isNominalToe = (toeSOW % (2 * SEC_PER_HOUR)) == halfFitInterval;
      if (signal.system == SatelliteSystem::GPS && !isNominalToe)
      {
         beginFit = std::min({xmitTime, xmit11, xmitClk});
         endFit += SEC_PER_5_MIN;
      }
   }


   double GPSCNavEph ::
   compositeIAURAUpperBound(const gnsstk::CommonTime &t, double elevation) const
   {
      try
      {
         return cnavCompositeIAURAUpperBound(t, top, elevation, uraED, uraNED0, uraNED1, uraNED2);
      }
      catch(const InvalidRequest& exc)
      {
         return std::numeric_limits<double>::quiet_NaN();
      }
   }


   void GPSCNavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');
      s << "           ACCURACY PARAMETERS"
        << endl
        << endl
        << "ED accuracy index              :  " << setfill(' ')
        << dec << setw(4) << (int) uraED << endl
        << "NED accuracy indices  0, 1, 2  :  " << setfill(' ')
        << dec << setw(4) << (int) uraNED0 << ", "
        << dec << setw(4) << (unsigned) uraNED1 << ", "
        << dec << setw(4) << (unsigned) uraNED2 << endl
        << "Integrity Status Flag          : "
        << (integStat ? "1 (Enhanced)" : "0 (Legacy)")
        << endl << endl << endl
        << "              " << getDumpTimeHdr(DumpDetail::Full) << endl
        << "Predict    :  " << getDumpTime(DumpDetail::Full, top) << endl
        << endl
        << "           SV STATUS"
        << endl
        << endl
        << "Health bits  L1, L2, L5        :     " << setfill('0') << setw(1)
        << healthL1 << ",  " << healthL2 << ",  " << healthL5
        << endl
        << "L2C Phasing                    :     " << setfill(' ')
        << phasingL2C << " (0=quadrature, 1=in-phase)"
        << endl << endl << endl
        << "           TRANSMIT TIMES" << endl << endl
        << "              " << getDumpTimeHdr(DumpDetail::Full) << endl
        << "Message 10:   " << getDumpTime(DumpDetail::Full, xmitTime) << endl
        << "Message 11:   " << getDumpTime(DumpDetail::Full, xmit11) << endl
        << "Clock:        " << getDumpTime(DumpDetail::Full, xmitClk) << endl;
      s.flags(oldFlags);
   }

   bool GPSCNavEph::
   isSameData(const NavDataPtr& right, bool ignore_timestamp) const
   {
      const std::shared_ptr<GPSCNavEph> eph = std::dynamic_pointer_cast<GPSCNavEph>(right);
      
      if (!eph)
      {
         return false;
      }
      return (GPSCNavData::isSameData(right, true) &&
         (healthL1 == eph->healthL1) &&
         (healthL2 == eph->healthL2) && 
         (healthL5 == eph->healthL5) &&
         (uraED == eph->uraED) &&
         (uraNED0 == eph->uraNED0) &&
         (uraNED1 == eph->uraNED1) &&
         (uraNED2 == eph->uraNED2) &&
         (integStat == eph->integStat) &&
         (deltaA == eph->deltaA) &&
         (dOMEGAdot == eph->dOMEGAdot) &&
         (top == eph->top));

         // Checked 6/11/2024
   }
}
