#include "GPSCNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gpstk
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
           deltaA(false),
           dOMEGAdot(false),
           top(gpstk::CommonTime::BEGINNING_OF_TIME),
           xmit11(gpstk::CommonTime::BEGINNING_OF_TIME),
           xmitClk(gpstk::CommonTime::BEGINNING_OF_TIME)
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
      CommonTime xmit1st = std::min({xmitTime, xmit11, xmitClk});
      GPSWeekSecond xws(xmit1st), toeWS(Toe);
      int xmitWeek = xws.week;
      long xmitSOW = (long) xws.sow;
         /** @todo replace all these magic numbers with named
          * constants or enums, with sensible names, not
          * "NINTY_MINUTES" [sic] */
      bool isNominalToe = (long)toeWS.sow % 7200 == 90*60;
      endFit = Toe + 90*60;

         // If the toe is NOT offset, then the begin valid time can be set
         // to the beginning of the two hour interval. 
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
      if (!isNominalToe)
      {
         endFit += 300;
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
