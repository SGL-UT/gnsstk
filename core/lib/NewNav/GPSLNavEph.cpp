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
           codesL2(L2Codes::Invalid1),
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
      GPSWeekSecond xws(xmitTime), toeWS(Toe);
      int xmitWeek = xws.week;
      long xmitSOW = (long) xws.sow;
      bool isNominalToe = (long)toeWS.sow % 7200 == 0;
      double fitSeconds = 3600.0 * getLegacyFitInterval(iodc, fitIntFlag);
      endFit = Toe + (fitSeconds/2.0);

         // If the toe is NOT offset, then the begin valid time can be set
         // to the beginning of the two hour interval.
         // NOTE: This is only true for GPS.   We can't do this
         // for QZSS, even though it also broadcasts the LNAV message format.
      if (signal.system==SatelliteSystem::GPS && isNominalToe)
      {
         xmitSOW = xmitSOW - (xmitSOW % 7200);
      }

         // If there IS an offset, all we can assume is that we (hopefully)
         // captured the earliest transmission and set the begin valid time
         // to that value.
         //
         // @note Prior to GPS III, the offset was typically applied
         // to BOTH the first and second data sets following a
         // cutover.  So this means the SECOND data set will NOT be
         // coerced to the top of the even hour start time if it
         // wasn't collected at the top of the hour.
      beginFit = GPSWeekSecond(xmitWeek, xmitSOW, xws.getTimeSystem());
         // If an upload cutover, need some adjustment.
         // Calculate the SOW aligned with the mid point and then
         // calculate the number of seconds the toe is SHORT
         // of that value.   That's how far the endValid needs
         // to be adjusted.
      if (!isNominalToe)
      {
         long sow = (long) toeWS.sow;
         long num900secIntervals = sow / 900;
         long midPointSOW = (num900secIntervals+1) * 900;
         double adjustUp = (double) (midPointSOW - sow);
         endFit += adjustUp;
      }
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


   namespace StringUtils
   {
      std::string asString(GPSLNavEph::L2Codes e)
      {
         switch (e)
         {
            case GPSLNavEph::L2Codes::Invalid1: return "Invalid1";
            case GPSLNavEph::L2Codes::Pcode:    return "P only";
            case GPSLNavEph::L2Codes::CAcode:   return "C/A only";
            case GPSLNavEph::L2Codes::Invalid2: return "Invalid2";
            default:                            return "Unknown";
         }
      }
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
