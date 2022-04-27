%include "RinexNavData.hpp"

   // Allows cast from RinexNavData and Rinex3NavData to EngEphemeris
%extend gnsstk::EngEphemeris
{
   EngEphemeris(const gnsstk::RinexNavData& d)
   {
      return new gnsstk::EngEphemeris(d);
   }
}
