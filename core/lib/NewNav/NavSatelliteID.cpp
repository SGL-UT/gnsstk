#include "NavSatelliteID.hpp"

namespace gpstk
{
   NavSatelliteID ::
   NavSatelliteID()
         : sat(0,SatelliteSystem::Unknown), xmitSat(0,SatelliteSystem::Unknown)
   {
   }


   NavSatelliteID ::
   NavSatelliteID(const NavSignalID& right)
         : NavSignalID(right)
   {
      sat.makeWild();
      xmitSat.makeWild();
   }


   NavSatelliteID ::
   NavSatelliteID(unsigned long subj, unsigned long xmit,
                  SatelliteSystem sys, CarrierBand car, TrackingCode track,
                  NavType nmt)
         : NavSignalID(sys,car,track,nmt), sat(subj,sys), xmitSat(xmit,sys)
   {
   }


   NavSatelliteID ::
   NavSatelliteID(unsigned long subj,
                  SatelliteSystem sys, CarrierBand car, TrackingCode track,
                  NavType nmt)
         : NavSignalID(sys,car,track,nmt), sat(subj,sys)
   {
      xmitSat.makeWild();
   }


   NavSatelliteID ::
   NavSatelliteID(unsigned long subj, const SatID& xmit, const ObsID& oid,
                  const NavID& navid)
         : NavSignalID(xmit.system, oid.band, oid.code, navid.navType),
           sat(subj, xmit.system),
           xmitSat(xmit)
   {
   }


   NavSatelliteID ::
   NavSatelliteID(const SatID& subj, const SatID& xmit, const ObsID& oid,
                  const NavID& navid)
         : NavSignalID(xmit.system, oid.band, oid.code, navid.navType),
           sat(subj),
           xmitSat(xmit)
   {
   }


   NavSatelliteID ::
   NavSatelliteID(const SatID& subj, const SatID& xmit, CarrierBand car,
                  TrackingCode track, NavType nmt)
         : NavSignalID(subj.system, car, track, nmt),
           sat(subj), xmitSat(xmit)
   {
   }


   NavSatelliteID ::
   NavSatelliteID(const SatID& subj)
         : NavSignalID(subj.system, gpstk::CarrierBand::Any,
                       gpstk::TrackingCode::Any, gpstk::NavType::Any),
           sat(subj)
   {
      xmitSat.makeWild();
   }


   bool NavSatelliteID ::
   operator<(const NavSatelliteID& right) const
   {
         // Use the order() method so we don't have to compare
         // everything in the parent class twice.
      if (sat < right.sat) return true;
      if (right.sat < sat) return false;
      if (xmitSat < right.xmitSat) return true;
      if (right.xmitSat < xmitSat) return false;
      int o = NavSignalID::order(right);
      if (o < 0) return true;
      return false;
   }


   bool NavSatelliteID ::
   operator==(const NavSatelliteID& right) const
   {
         // Use the order() method so we don't have to compare
         // everything in the parent class twice.
      return ((NavSignalID::order(right) == 0) && (sat == right.sat) &&
              (xmitSat == right.xmitSat));
   }


   bool NavSatelliteID ::
   isWild() const
   {
      return sat.isWild() || xmitSat.isWild() || NavSignalID::isWild();
   }
}
