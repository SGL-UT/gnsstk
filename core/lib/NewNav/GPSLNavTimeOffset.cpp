#include "GPSLNavTimeOffset.hpp"
#include "GPSWeekSecond.hpp"

using namespace std;

static const unsigned maxDN = 7;
static const double maxtot = 602112.0;
static const double mintot = 0;

namespace gpstk
{
   GPSLNavTimeOffset ::
   GPSLNavTimeOffset()
         : deltatLS(0.0), a0(0.0), a1(0.0), tot(0.0), wnt(0), wnLSF(0), dn(0),
           deltatLSF(0.0)
   {
   }


   bool GPSLNavTimeOffset ::
   validate() const
   {
         // see IS-GPS-200 
         // DN is defined in the ICD as being 1-7 but we also include
         // the value 0 as valid because we won't always have anything
         // else.
      return ((dn <= maxDN) && (tot <= maxtot) && (tot >= mintot));
   }


   bool GPSLNavTimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset)
      const
   {
      if (((fromSys == TimeSystem::GPS) && (toSys == TimeSystem::UTC)) ||
          ((fromSys == TimeSystem::UTC) && (toSys == TimeSystem::GPS)))
      {
         GPSWeekSecond ws(when);
         offset = deltatLS + a0 + a1*(ws.sow - tot + 604800.0*(ws.week-wnt));
         if (fromSys == TimeSystem::UTC)
            offset = -offset;
         return true;
      }
      return false;
   }


   void GPSLNavTimeOffset ::
   dump(std::ostream& s, Detail dl) const
   {
      const ios::fmtflags oldFlags = s.flags();
      NavData::dump(s,dl);
      double offset;
      switch (dl)
      {
         case Detail::OneLine:
            break;
         case Detail::Brief:
               // brief just shows the offset as of the reference time.
            getOffset(TimeSystem::GPS, TimeSystem::UTC,
                      gpstk::GPSWeekSecond(wnt,tot), offset);
            s << "GPS-UTC offset = " << offset << endl;
            break;
         case Detail::Full:
            getOffset(TimeSystem::GPS, TimeSystem::UTC,
                      gpstk::GPSWeekSecond(wnt,tot), offset);
            s << setprecision(16)
              << "  A0         = " << a0 << endl
              << "  A1         = " << a1 << endl
              << "  delta tLS  = " << deltatLS << endl
              << "  tot        = " << tot << endl
              << "  WNt        = " << wnt << endl
              << "  WN(LSF)    = " << wnLSF << endl
              << "  DN         = " << dn << endl
              << "  delta tLSF = " << deltatLSF << endl
              << "  offset(ref)= " << offset << endl;
            break;
      }
      s.flags(oldFlags);
   }


   TimeOffsetData::TimeCvtSet GPSLNavTimeOffset ::
   getConversions() const
   {
      TimeCvtKey keyF(TimeSystem::GPS,TimeSystem::UTC);
      TimeCvtKey keyR(TimeSystem::UTC,TimeSystem::GPS);
      return TimeCvtSet({ keyF, keyR });
   }
}

