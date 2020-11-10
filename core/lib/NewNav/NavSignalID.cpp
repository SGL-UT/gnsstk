#include "NavSignalID.hpp"

namespace gpstk
{
   NavSignalID ::
   NavSignalID()
         : system(SatelliteSystem::Unknown),
           carrier(CarrierBand::Unknown),
           code(TrackingCode::Unknown),
           nav(NavType::Unknown)
   {
   }


   NavSignalID ::
   NavSignalID(SatelliteSystem sys, CarrierBand car, TrackingCode track,
               NavType nmt)
         : system(sys),
           carrier(car),
           code(track),
           nav(nmt)
   {
   }


   int NavSignalID ::
   order(const NavSignalID& right) const
   {
      if (system < right.system) return -1;
      if (system > right.system) return 1;
      if ((carrier != CarrierBand::Any) && (right.carrier != CarrierBand::Any))
      {
         if (carrier < right.carrier) return -1;
         if (carrier > right.carrier) return 1;
      }
      if ((code != TrackingCode::Any) && (right.code != TrackingCode::Any))
      {
         if (code < right.code) return -1;
         if (code > right.code) return 1;
      }
      if ((nav != NavType::Any) && (right.nav != NavType::Any))
      {
         if (nav < right.nav) return -1;
         if (nav > right.nav) return 1;
      }
      return 0;
   }


   bool NavSignalID ::
   isWild() const
   {
      return ((carrier == CarrierBand::Any) ||
              (code == TrackingCode::Any) ||
              (nav == NavType::Any));
   }
}
