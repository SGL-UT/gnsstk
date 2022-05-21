%include "Rinex3NavData.hpp"

   // Allows cast from Rinex3NavData to EngEphemeris
%extend gnsstk::EngEphemeris
{
   EngEphemeris(const gnsstk::Rinex3NavData& d)
   {
      return new gnsstk::EngEphemeris(d);
   }
}
