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
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
#include "NavSatelliteID.hpp"

namespace gnsstk
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
   NavSatelliteID(unsigned long subj, SatelliteSystem sys, CarrierBand car,
                  TrackingCode track, XmitAnt ant, int freqOffs,
                  bool freqOffsWild, NavType nmt)
         : NavSignalID(sys,ObsID(ObservationType::NavMsg,car,track,freqOffs,
                                 ant,freqOffsWild),nmt),
           sat(subj,sys)
   {
      xmitSat.makeWild();
   }


   NavSatelliteID ::
   NavSatelliteID(unsigned long subj, SatelliteSystem sys, const ObsID& oid,
                  NavType nmt)
         : NavSignalID(sys,oid,nmt),
           sat(subj,sys)
   {
      xmitSat.makeWild();
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
         : NavSignalID(xmit.system, oid, navid.navType),
           sat(subj, xmit.system),
           xmitSat(xmit)
   {
   }


   NavSatelliteID ::
   NavSatelliteID(const SatID& subj, const SatID& xmit, const ObsID& oid,
                  const NavID& navid)
         : NavSignalID(xmit.system, oid, navid.navType),
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
         : NavSignalID(subj.system, gnsstk::CarrierBand::Any,
                       gnsstk::TrackingCode::Any, gnsstk::NavType::Any),
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
         // std::cerr << __PRETTY_FUNCTION__ << std::endl;
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
