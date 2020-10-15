#include "GPSCNavRedAlm.hpp"

using namespace std;

namespace gpstk
{
   GPSCNavRedAlm ::
   GPSCNavRedAlm()
         : deltaA(0),
           phi0(0)
   {
   }


   bool GPSCNavRedAlm ::
   validate() const
   {
      return GPSCNavAlm::validate();
   }


   void GPSCNavRedAlm ::
   fixValues()
   {
      if (signal.sat.system == SatelliteSystem::QZSS)
      {
         M0 = phi0;
         w = refwQZSS;
         ecc = refEccQZSS;
         A = refAQZSS + deltaA;
         Ahalf = ::sqrt(A);
         deltai = refdeltaiQZSS;
         i0 = refioffsetQZSS + deltai;
         OMEGAdot = refOMEGAdotQZSS;
      }
      else
      {
            // GPS
            // IS-GPS-200 gives the PHI0 value as being the sum of
            // M0+w, which is probably fine given the equations in
            // that interface spec, but I'm not convinced this is the
            // right way to handle that in the context of the
            // OrbitDataKepler implementation.  That said, table
            // 5.5.2-9 in IS-QZSS suggests that using a 0 argument of
            // perigee (w) is what you do for GPS.
         M0 = phi0;
         w = refwGPS;
         ecc = refEccGPS;
         A = refAGPS + deltaA;
         Ahalf = ::sqrt(A);
         deltai = refdeltaiGPS;
         i0 = refioffsetGPS + deltai;
         OMEGAdot = refOMEGAdotGPS;
      }
   }
}
