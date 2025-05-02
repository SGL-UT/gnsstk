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
#include "GPSLNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp"
#include "GPS_URA.hpp"
#include "TimeConstants.hpp"

using namespace std;

namespace gnsstk
{
   GPSLNavEph ::
   GPSLNavEph()
         : pre2(0),
           pre3(0),
           tlm2(0),
           tlm3(0),
           isf2(false),
           isf3(false),
           iodc(0),
           iode(0),
           fitIntFlag(0),
           healthBits(0xff),
           uraIndex(0),
           tgd(0.0),
           asFlag2(false),
           asFlag3(false),
           alert2(false),
           alert3(false),
           codesL2(GPSLNavL2Codes::Invalid1),
           aodo(-1),
           L2Pdata(false)
   {
      signal.messageType = NavMessageType::Ephemeris;
      msgLenSec = 18.0;
   }


   bool GPSLNavEph ::
   validate() const
   {
      return GPSLNavData::validate() && ((pre2 == 0) || (pre2 == 0x8b)) &&
         ((pre3 == 0) || (pre3 == 0x8b));
   }


   CommonTime GPSLNavEph ::
   getUserTime() const
   {
      CommonTime mr = std::max({xmitTime, xmit2, xmit3});
      return mr + 6.0;
   }


   void GPSLNavEph ::
   fixFit()
   {
      // GPS fit interval is based on the fit interval flag and IODC.
      // Nominally, GPS fit interval is 4 hours. When the fit interval flag is 1
      // then the fit interval will be greater than 4 hours. See getLegacyFitInterval
      // for more info.
      uint32_t halfFitInterval = SEC_PER_HOUR * getLegacyFitInterval(iodc, fitIntFlag) / 2;
      
      // QZSS has a fixed fit interval of 2 hours instead
      // of GPS's usual 4 hours. Also the fit interval flag
      // is defined to be always zero (IS-QZSS-PNT-004 Table 4.1.1-2, Section 4.1.2.4)
      if (signal.system == SatelliteSystem::QZSS)
      {
         halfFitInterval = SEC_PER_HOUR;
      }
      
      // By default, the Toe is assumed to be the midpoint of the curve fit interval.
      // (IS-GPS-200N 20.3.4.4, 20.3.4.5)
      beginFit = Toe - halfFitInterval;
      endFit = Toe + halfFitInterval;
      
      // If the Toe is non-nominal then it indicates an upload cutover and Toe is not
      // the midpoint of the curve fit interval. The begin and end fit times must be adjusted.
      //    * A nominal Toe always lies on an hour boundary (IS-GPS-200N 20.3.4.4). This covers
      //      both GPS normal operations, GPS extended operations mode, and QZSS.
      //    * The start of the CEI dataset transmission interval corresponds to the beginning of the
      //      curve fit interval for the CEI. (IS-GPS-200N 20.3.4.4) 
      //      With a nominal Toe, the start of transmission can be inferred. For a non-nominal Toe, 
      //      the start of the transmission interval, and thus the beginning of the curve fit, 
      //      cannot be assumed. For non-nominal Toe, all we can assume is that we (hopefully) capture 
      //      the earliest transmission and set the begin fit time to that value.
      //    * For a non-nominal Toe, the end of the curve fit interval must be adjusted since the Toe
      //      is offset from the assumed curve fit interval midpoint. The Toe is offset
      //      by a small negative deviation (IS-GPS-200N 30.3.4.5). Since the midpoint of the fit interval,
      //      must be a multiple of 5 minutes (IS-GPS-200N 30.3.4.4), and the Toe is offset from the midpoint
      //      by a small negative deviation (IS-GPS-200N 30.3.4.5), then the end fit rounds up to a 5 minute multiple.
      //    * QZSS does not specify the relationship of fit interval to upload cutovers. So we do not adjust them.
      uint32_t toeSOW = static_cast<uint32_t>(GPSWeekSecond(Toe).sow);
      bool isNominalToe = toeSOW % SEC_PER_HOUR == 0;
      if (signal.system == SatelliteSystem::GPS && !isNominalToe)
      {
         beginFit = xmitTime;
         endFit += SEC_PER_5_MIN - (toeSOW % SEC_PER_5_MIN);
      }
   }


      
   std::string GPSLNavEph ::
   getTerseHeader() const
   {
      return "SVN  PRN     Begin Fit        Toe          End Fit       URA     IODC      Health";
   }

   void GPSLNavEph ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (dl != DumpDetail::Terse)
      {
            // standard dump routine is fine for anything other than Terse
         GPSLNavData::dump(s,dl);
         return;
      }
      ios::fmtflags oldFlags = s.flags();

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

      std::string svn("0");
      if (getSVN(signal.sat, Toe, svn))
      {
         s << setw(2) << " " << svn << "  ";
      }
      else
      {
         s << "  XX  ";
      }

      s << setw(2) << signal.sat.id << " ! ";

      string tform = "%3j %02H:%02M:%02S";

      s << printTime( beginFit, tform ) << " ! ";
      s << printTime( Toe, tform ) << " ! ";
      s << printTime( endFit, tform ) << " !  ";

      if (uraIndex < SV_ACCURACY_GPS_MAX_INDEX_VALUE)
      {
         s << setw(4) << setprecision(1) << ura2accuracy(uraIndex) << "  ! ";
      }
      else
      {
         s << setw(4) << "N/A" << "  ! ";
      }
      s << "0x"    << setfill('0')    << hex << setw(3) << iodc << " ! ";
      s << "0x"    << setfill('0')    << setw(2) << (unsigned)healthBits;
      s << setfill(' ') << dec;
      s << "   "   << setw(2) << (unsigned)healthBits << " ! ";

      s << endl;
      s.flags(oldFlags);
   }


   void GPSLNavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
         /** @note we add 6 seconds to xmitTime because we're printing
          * the HOW time not the transmit time.  Also we're really
          * just guessing about the SF2 and SF3 HOW times. */
      unsigned health = healthBits;
      s << "           SUBFRAME OVERHEAD" << endl << endl
        << "               SOW    DOW:HH:MM:SS     IOD    ALERT   A-S   ISF"
        << endl
        << "SF1 HOW:   "
        << gnsstk::printTime(xmitTime+6, "%7.0g  %3a-%1w:%02H:%02M:%02S")
        << "   0x" << hex << setw(3) << internal << setfill('0')
        << nouppercase << iodc << dec << setfill(' ') << "      "
        << noboolalpha << alert << "     " << (asFlag ? " on   " : "off   ")
        << (isf ? "enhanced" : "legacy") << endl
        << "SF2 HOW:   "
        << gnsstk::printTime(xmit2+6, "%7.0g  %3a-%1w:%02H:%02M:%02S")
        << "    0x" << hex << setw(2) << internal << setfill('0')
        << nouppercase << iode << dec << setfill(' ') << "      "
        << alert2 << "     " << (asFlag2 ? " on   " : "off   ")
        << (isf2 ? "enhanced" : "legacy") << endl
        << "SF3 HOW:   "
        << gnsstk::printTime(xmit3+6, "%7.0g  %3a-%1w:%02H:%02M:%02S")
        << "    0x" << hex << setw(2) << internal << setfill('0')
        << nouppercase << iode << dec << setfill(' ') << "      "
        << alert3 << "     " << (asFlag3 ? " on   " : "off   ")
        << (isf3 ? "enhanced" : "legacy") << endl
        << endl
        << "           SV STATUS" << endl << endl
        << "Health bits         :      0x" << setw(2)
        << internal << setfill('0') << hex << health << dec
        << ", " << setfill(' ') << health << endl
        << "Fit interval flag   :" << setw(10) << (unsigned)fitIntFlag << endl
        << "URA index           :" << setw(10) << (unsigned)uraIndex << endl
        << "Code on L2          :" << setw(10) << StringUtils::asString(codesL2)
        << endl
        << "L2 P Nav data       :" << "       " << (L2Pdata ? "off" : " on")
        << endl << right
        << "Tgd                 :" << setw(14) << setprecision(6) << scientific
        << uppercase << tgd << " sec" << endl;
      if (aodo != -1)
      {
         s << "AODO                :" << setw(10) << aodo << " sec" << endl;
      }
      s << endl;
      s.flags(oldFlags);
   }


   bool GPSLNavEphIODCComp ::
   operator()(const std::shared_ptr<GPSLNavEph> lhs,
              const std::shared_ptr<GPSLNavEph> rhs) const
   {
      if (lhs->signal < rhs->signal) return true;
      if (rhs->signal < lhs->signal) return false;
      GPSWeekSecond lws(lhs->Toe), rws(rhs->Toe);
      if (lws.week < rws.week) return true;
      if (rws.week < lws.week) return false;
      if (lhs->iodc < rhs->iodc) return true;
      return false;
   }

   bool GPSLNavEph::
   isSameData(const NavDataPtr& right, bool ignore_timestamp) const
   {
      const std::shared_ptr<GPSLNavEph> eph = std::dynamic_pointer_cast<GPSLNavEph>(right);
      
      if (!eph)
      {
         return false;
      }
      return (GPSLNavData::isSameData(right, ignore_timestamp) &&
         (isf2 == eph->isf2) &&
         (isf3 == eph->isf3) &&
         (fitIntFlag == eph->fitIntFlag) &&
         (healthBits == eph->healthBits) && 
         (uraIndex == eph->uraIndex) &&
         (tgd == eph->tgd) &&
         (alert2 == eph->alert2) &&
         (iodc == eph->iodc) &&
         (iode == eph->iode) &&
         (alert3 == eph->alert3));

         // Checked 6/6
   }

   bool GPSLNavEphCEIComp ::
   operator()(const std::shared_ptr<GPSLNavEph> lhs,
              const std::shared_ptr<GPSLNavEph> rhs) const
   {
         // We still need to do signal and Toe, so do those first,
         // since they're the most likely to change.
      if (lhs->signal < rhs->signal) return true;
      if (rhs->signal < lhs->signal) return false;
      GPSWeekSecond lws(lhs->Toe), rws(rhs->Toe);
      if (lws.week < rws.week) return true;
      if (rws.week < lws.week) return false;
      if (lhs->iodc < rhs->iodc) return true;
      if (rhs->iodc < lhs->iodc) return false;
         // Everything else is in the order it appears in Table 6-I-1,
         // for convenience (i.e. if there's a compelling reason to
         // change it, this order has no specific meaning)
      if (lhs->healthBits < rhs->healthBits) return true;
      if (rhs->healthBits < lhs->healthBits) return false;
      if (lhs->uraIndex < rhs->uraIndex) return true;
      if (rhs->uraIndex < lhs->uraIndex) return false;
      if (lhs->tgd < rhs->tgd) return true;
      if (rhs->tgd < lhs->tgd) return false;
      if (lhs->af0 < rhs->af0) return true;
      if (rhs->af0 < lhs->af0) return false;
      if (lhs->af1 < rhs->af1) return true;
      if (rhs->af1 < lhs->af1) return false;
      if (lhs->af2 < rhs->af2) return true;
      if (rhs->af2 < lhs->af2) return false;
      if (lhs->Toc < rhs->Toc) return true;
      if (rhs->Toc < lhs->Toc) return false;
      if (lhs->Ahalf < rhs->Ahalf) return true;
      if (rhs->Ahalf < lhs->Ahalf) return false;
      if (lhs->dn < rhs->dn) return true;
      if (rhs->dn < lhs->dn) return false;
      if (lhs->fitIntFlag < rhs->fitIntFlag) return true;
      if (rhs->fitIntFlag < lhs->fitIntFlag) return false;
      if (lhs->ecc < rhs->ecc) return true;
      if (rhs->ecc < lhs->ecc) return false;
      if (lhs->M0 < rhs->M0) return true;
      if (rhs->M0 < lhs->M0) return false;
      if (lhs->Toe < rhs->Toe) return true;
      if (rhs->Toe < lhs->Toe) return false;
      if (lhs->Crs < rhs->Crs) return true;
      if (rhs->Crs < lhs->Crs) return false;
      if (lhs->Cuc < rhs->Cuc) return true;
      if (rhs->Cuc < lhs->Cuc) return false;
      if (lhs->Cus < rhs->Cus) return true;
      if (rhs->Cus < lhs->Cus) return false;
      if (lhs->iode < rhs->iode) return true;
      if (rhs->iode < lhs->iode) return false;
      if (lhs->isf < rhs->isf) return true;
      if (rhs->isf < lhs->isf) return false;
      if (lhs->isf2 < rhs->isf2) return true;
      if (rhs->isf2 < lhs->isf2) return false;
      if (lhs->isf3 < rhs->isf3) return true;
      if (rhs->isf3 < lhs->isf3) return false;
      if (lhs->w < rhs->w) return true;
      if (rhs->w < lhs->w) return false;
      if (lhs->OMEGAdot < rhs->OMEGAdot) return true;
      if (rhs->OMEGAdot < lhs->OMEGAdot) return false;
      if (lhs->OMEGA0 < rhs->OMEGA0) return true;
      if (rhs->OMEGA0 < lhs->OMEGA0) return false;
      if (lhs->i0 < rhs->i0) return true;
      if (rhs->i0 < lhs->i0) return false;
      if (lhs->idot < rhs->idot) return true;
      if (rhs->idot < lhs->idot) return false;
      if (lhs->Cic < rhs->Cic) return true;
      if (rhs->Cic < lhs->Cic) return false;
      if (lhs->Cis < rhs->Cis) return true;
      if (rhs->Cis < lhs->Cis) return false;
      if (lhs->Crc < rhs->Crc) return true;
      if (rhs->Crc < lhs->Crc) return false;
      if (lhs->alert < rhs->alert) return true;
      if (rhs->alert < lhs->alert) return false;
      if (lhs->alert2 < rhs->alert2) return true;
      if (rhs->alert2 < lhs->alert2) return false;
      if (lhs->alert3 < rhs->alert3) return true;
      if (rhs->alert3 < lhs->alert3) return false;
      return false;
   }
}
