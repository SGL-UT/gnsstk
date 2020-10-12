#include "GPSCNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gpstk
{
   GPSCNavEph ::
   GPSCNavEph()
         : pre2(0),
           healthL1(true),
           healthL2(true),
           healthL5(true),
           uraED(-16),
           alert2(false)
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   bool GPSCNavEph ::
   validate() const
   {
      return GPSCNavData::validate() && ((pre2 == 0) || (pre2 == 0x8b));
   }


   void GPSCNavEph ::
   fixFit()
   {
      CommonTime xmit1st = std::min({xmitTime, xmit11, xmitClk});
#if 0
      GPSWeekSecond xws(xmitTime), toeWS(Toe);
      int xmitWeek = xws.week;
      long xmitSOW = (long) xws.sow;
      bool isNominalToe = (long)toeWS.sow % 7200 == 0;
      double fitSeconds = 3600.0 * getLegacyFitInterval(iodc, fitIntFlag);
      endFit = Toe + (fitSeconds/2.0);

         // If the toe is NOT offset, then the begin valid time can be set
         // to the beginning of the two hour interval. 
         // NOTE: This is only true for GPS.   We can't do this
         // for QZSS, even though it also broadcasts the CNAV message format.
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
      beginFit = GPSWeekSecond(xmitWeek, xmitSOW, TimeSystem::GPS);
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
#endif
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
        << dec << setw(4) << (unsigned) uraNED2 << ", packed  0x"
        << hex << setw(3) << /*URAnedPacked <<*/ dec << endl
        << "Integrity Status Flag          : "
        << (integStat ? "1 (Enhanced)" : "0 (Legacy)")
        << endl << endl << endl
        << "              Week(10bt)     SOW     DOW   UTD     SOD"
        << "   MM/DD/YYYY   HH:MM:SS" << endl
        << "Predict    :  " << printTime(top, dumpTimeFmt) << endl
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
        << "              Week(10bt)     SOW     DOW   UTD     SOD"
        << "   MM/DD/YYYY   HH:MM:SS" << endl
        << "Message 10:   " << printTime(xmitTime, dumpTimeFmt) << endl
        << "Message 11:   " << printTime(xmit11, dumpTimeFmt) << endl
        << "Clock:        " << printTime(xmitClk, dumpTimeFmt) << endl;
      s.flags(oldFlags);
   }
}
