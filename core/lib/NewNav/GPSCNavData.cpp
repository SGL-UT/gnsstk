#include "GPSCNavData.hpp"

namespace gpstk
{
   const double GPSCNavData::refAGPS = 26559710;
   const double GPSCNavData::refOMEGAdotGPS = -2.6e-9 * PI;
   const double GPSCNavData::refioffsetGPS = 0.3 * PI;
   const double GPSCNavData::refdeltaiGPS = 0.0056 * PI;
   const double GPSCNavData::refwGPS = 0;
   const double GPSCNavData::refEccGPS = 0;

   const double GPSCNavData::refAQZSS = 42164200;
      // I haven't yet found any indication that OMEGA dot is different for QZSS
   const double GPSCNavData::refOMEGAdotQZSS = -2.6e-9 * PI;
   const double GPSCNavData::refioffsetQZSS = 0.25 * PI;
   const double GPSCNavData::refdeltaiQZSS = -0.0111 * PI;
   const double GPSCNavData::refwQZSS = 2.0 * PI * 270.0 / 360.0;
   const double GPSCNavData::refEccQZSS = 0.075;

   GPSCNavData ::
   GPSCNavData()
         : pre(0),
           alert(false)
   {}


   bool GPSCNavData ::
   validate() const
   {
      return ((pre == 0) || (pre == 0x8b));
   }
}
