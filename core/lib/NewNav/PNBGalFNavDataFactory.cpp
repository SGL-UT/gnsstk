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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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
#include "PNBGalFNavDataFactory.hpp"
#include "GalFNavEph.hpp"
#include "GalFNavTimeOffset.hpp"
#include "GalFNavIono.hpp"
#include "GalFNavISC.hpp"
#include "GalINavHealth.hpp"
#include "GALWeekSecond.hpp"
#include "TimeCorrection.hpp"
#include "EngNav.hpp"
#include "TimeString.hpp"
#include "GalFBits.hpp"

using namespace std;
using namespace gnsstk::galfnav;

namespace gnsstk
{
   bool PNBGalFNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      if (navIn->getNavID().navType != NavType::GalFNAV)
      {
            // This class only processes Galileo F/NAV.
         return false;
      }
      bool rv = true;
      try
      {
         unsigned long pageType = navIn->asUnsignedLong(
            fsbType,fnbType,fscType);
            /// @todo implement validity checks if there are any.
         switch (pageType)
         {
               /** @note While the ICD doesn't label page type 1 as
                * being part of the ephemeris, there is data in page
                * type 1 that is required to construct the
                * ephemeris. */
            case 1:
            case 2:
            case 3:
            case 4:
               // cerr << "pageType " << pageType << " = ephemeris" << endl;
               rv = processEph(pageType, navIn, navOut);
               break;
            case 5:
            case 6:
               // cerr << "pageType " << pageType << " = almanac" << endl;
               rv = processAlm(pageType, navIn, navOut);
               break;
            default:
               // cerr << "pageType " << pageType << " = unhandled" << endl;
               rv = true;
               break;
         }
         // cerr << "  results: " << navOut.size() << endl;
         // for (const auto& i : navOut)
         //    i->dump(cerr,DumpDetail::Full);
      }
      catch (Exception& exc)
      {
         rv = false;
         cerr << exc << endl;
      }
      catch (std::exception& exc)
      {
         rv = false;
         cerr << exc.what() << endl;
      }
      catch (...)
      {
         rv = false;
         cerr << "Unknown exception" << endl;
      }
      return rv;
   }


   bool PNBGalFNavDataFactory ::
   processEph(unsigned pageType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (!PNBNavDataFactory::processEph && !PNBNavDataFactory::processHea &&
          !PNBNavDataFactory::processTim)
      {
            // User doesn't want ephemerides or health so don't do any
            // processing.
         return true;
      }
      if (ephAcc.find(key) == ephAcc.end())
      {
         // cerr << "ephAcc is empty for signal " << key << endl;
            // set up a new ephemeris
         ephAcc[key].resize(numEphPTs);
         ephAcc[key][pageType-1] = navIn;
      }
      std::vector<PackedNavBitsPtr> &ephPage(ephAcc[key]);
      ephPage[pageType-1] = navIn;
         // Health and ionospheric correction information is in page type 1.
      if (pageType == 1)
      {
         if (PNBNavDataFactory::processIono)
         {
               // Add iono data from word type 5
            NavDataPtr p2 = std::make_shared<GalFNavIono>();
            GalFNavIono *ip2 = dynamic_cast<GalFNavIono*>(p2.get());
            ip2->timeStamp = navIn->getTransmitTime();
            ip2->signal = NavMessageID(key, NavMessageType::Iono);
            ip2->ai[0] = navIn->asUnsignedDouble(esbai0,enbai0,escai0);
            ip2->ai[1] = navIn->asSignedDouble(esbai1,enbai1,escai1);
            ip2->ai[2] = navIn->asSignedDouble(esbai2,enbai2,escai2);
            ip2->idf[0] = navIn->asBool(esbIDFR1);
            ip2->idf[1] = navIn->asBool(esbIDFR2);
            ip2->idf[2] = navIn->asBool(esbIDFR3);
            ip2->idf[3] = navIn->asBool(esbIDFR4);
            ip2->idf[4] = navIn->asBool(esbIDFR5);
            navOut.push_back(p2);
         }
         if (PNBNavDataFactory::processHea)
         {
               // Add health bits from page type 1.
            NavDataPtr p1 = std::make_shared<GalFNavHealth>();
            GalFNavHealth *hp1 = dynamic_cast<GalFNavHealth*>(p1.get());
            hp1->timeStamp = navIn->getTransmitTime();
            hp1->signal = NavMessageID(key, NavMessageType::Health);
            hp1->signal.obs.band = CarrierBand::L5;
            hp1->signal.obs.code = TrackingCode::E5aI;
            hp1->sigHealthStatus = static_cast<GalHealthStatus>(
               ephPage[esiE5ahs]->asUnsignedLong(esbE5ahs,enbE5ahs,escE5ahs));
            hp1->dataValidityStatus = static_cast<GalDataValid>(
               ephPage[esiE5advs]->asUnsignedLong(
                  esbE5advs,enbE5advs,escE5advs));
            hp1->sisaIndex = ephPage[esiSISA]->asUnsignedLong(esbSISA,enbSISA,
                                                              escSISA);
            navOut.push_back(p1);
         }
         if (PNBNavDataFactory::processISC)
         {
               // Add ISC data from page type 1.
            NavDataPtr p4 = std::make_shared<GalFNavISC>();
            p4->timeStamp = navIn->getTransmitTime();
            p4->signal = NavMessageID(
               NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                              navIn->getobsID(), navIn->getNavID()),
               NavMessageType::ISC);
            GalFNavISC *isc = dynamic_cast<GalFNavISC*>(p4.get());
            isc->isc = navIn->asSignedDouble(esbBGDa,enbBGDa,escBGDa);
            navOut.push_back(p4);
         }
      }
      else if ((pageType == 4) && PNBNavDataFactory::processTim)
      {
            // GST-UTC offset
         NavDataPtr p3 = std::make_shared<GalFNavTimeOffset>();
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         GalFNavTimeOffset *to = dynamic_cast<GalFNavTimeOffset*>(p3.get());
         to->tgt = TimeSystem::UTC;
         to->a0 = navIn->asSignedDouble(esbA0,enbA0,escA0);
         to->a1 = navIn->asSignedDouble(esbA1,enbA1,escA1);
         to->deltatLS = navIn->asLong(esbdtLS, enbdtLS, escdtLS);
         to->tot = navIn->asUnsignedLong(esbtot,enbtot,esctot);
         to->wnot = navIn->asUnsignedLong(esbWNot,enbWNot,escWNot);
         to->wnLSF = navIn->asUnsignedLong(esbWNlsf, enbWNlsf, escWNlsf);
         to->dn = navIn->asUnsignedLong(esbDN, enbDN, escDN);
         to->deltatLSF = navIn->asLong(esbdtLSF, enbdtLSF, escdtLSF);
         to->tow = navIn->asUnsignedLong(esbTOW_4, enbTOW_4, escTOW_4);
         GALWeekSecond gws(to->timeStamp);
         long refWeek = gws.week;
         to->wnot = timeAdjust8BitWeekRollover(to->wnot, refWeek);
         to->wnLSF = timeAdjust8BitWeekRollover(to->wnLSF, refWeek);
         to->refTime = GALWeekSecond(to->wnot, to->tot);
         to->effTime = GALWeekSecond(to->wnLSF, (to->dn-1)*86400);
         // cerr << "gws.week=" << gws.week << "  gws.sow=" << gws.sow << "  refTime=" << to->refTime << endl;
         navOut.push_back(p3);
            // GST-GPS offset
         p3 = std::make_shared<GalFNavTimeOffset>();
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         to = dynamic_cast<GalFNavTimeOffset*>(p3.get());
         to->tgt = TimeSystem::GPS;
         to->a0 = navIn->asSignedDouble(esbA0G,enbA0G,escA0G);
         to->a1 = navIn->asSignedDouble(esbA1G,enbA1G,escA1G);
         to->tot = navIn->asUnsignedLong(esbt0G,enbt0G,esct0G);
         to->wnot = navIn->asUnsignedLong(esbWN0G,enbWN0G,escWN0G);
         to->tow = navIn->asUnsignedLong(esbTOW_4, enbTOW_4, escTOW_4);
            // WN0G is 6 bits...
         to->wnot = (gws.week & ~0x3f) | to->wnot;
         to->refTime = GALWeekSecond(to->wnot, to->tot);
            // Do not attempt to set the effectivity time for
            // converting from Galileo to GPS time.  effTime only
            // applies to UTC.
         // cerr << "gws.week=" << gws.week << "  gws.sow=" << gws.sow << "  refTime=" << to->refTime << endl;
         navOut.push_back(p3);
      }
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any further
            // processing.
         return true;
      }
         // stop processing if we don't have three full subframes
      if (!ephPage[pt1] || !ephPage[pt2] || !ephPage[pt3] || !ephPage[pt4] ||
          (ephPage[pt1]->getNumBits() != 244) ||
          (ephPage[pt2]->getNumBits() != 244) ||
          (ephPage[pt3]->getNumBits() != 244) ||
          (ephPage[pt4]->getNumBits() != 244))
      {
         // cerr << "Not ready for full F/NAV eph processing" << endl
         //      << "  pt1: " << (bool)ephPage[pt1] << " "
         //      << (ephPage[pt1] ? ephPage[pt1]->getNumBits() : -1) << endl
         //      << "  pt2: " << (bool)ephPage[pt2] << " "
         //      << (ephPage[pt2] ? ephPage[pt2]->getNumBits() : -1) << endl
         //      << "  pt3: " << (bool)ephPage[pt3] << " "
         //      << (ephPage[pt3] ? ephPage[pt3]->getNumBits() : -1) << endl
         //      << "  pt4: " << (bool)ephPage[pt4] << " "
         //      << (ephPage[pt4] ? ephPage[pt4]->getNumBits() : -1) << endl;
         return true;
      }
         // Stop processing if we don't have matching IODnav in each
         // of the four page types.
      unsigned long iod1, iod2, iod3, iod4;
      iod1 = ephPage[esiIOD_1]->asUnsignedLong(esbIOD_1,enbIOD_1,escIOD_1);
      iod2 = ephPage[esiIOD_2]->asUnsignedLong(esbIOD_2,enbIOD_2,escIOD_2);
      iod3 = ephPage[esiIOD_3]->asUnsignedLong(esbIOD_3,enbIOD_3,escIOD_3);
      iod4 = ephPage[esiIOD_4]->asUnsignedLong(esbIOD_4,enbIOD_4,escIOD_4);
      if ((iod1 != iod2) || (iod1 != iod3) || (iod1 != iod4))
      {
         // cerr << "IODnav mismatch, not processing" << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GalFNavEph>();
      GalFNavEph *eph = dynamic_cast<GalFNavEph*>(p0.get());
         // NavData
      eph->timeStamp = ephPage[pt1]->getTransmitTime();
      eph->signal = NavMessageID(key, NavMessageType::Ephemeris);
      // cerr << "Ready for full F/NAV eph processing for " << (NavSignalID)key << endl;
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
      double t0e = ephPage[esit0e]->asUnsignedLong(esbt0e,enbt0e,esct0e);
      double t0c = ephPage[esit0c]->asUnsignedLong(esbt0c,enbt0c,esct0c);
      unsigned wn_1 = ephPage[esiWN_1]->asUnsignedLong(esbWN_1,enbWN_1,escWN_1);
      unsigned tow_1 = ephPage[esiTOW_1]->asUnsignedLong(esbTOW_1,enbTOW_1,
                                                         escTOW_1);
      GALWeekSecond xmit1(wn_1, tow_1);
      unsigned wn_3 = ephPage[esiWN_3]->asUnsignedLong(esbWN_3,enbWN_3,escWN_3);
      unsigned tow_3 = ephPage[esiTOW_3]->asUnsignedLong(esbTOW_3,enbTOW_3,
                                                         escTOW_3);
      GALWeekSecond xmit3(wn_3, tow_3);
      // cerr << "  wn_1=" << wn_1 << "  tow_1=" << tow_1 << "  wn_3=" << wn_3 << "  tow_3=" << tow_3 << "  t0e=" << t0e << "  t0c=" << t0c << endl;
      eph->Toe = GALWeekSecond(wn_3,t0e).weekRolloverAdj(xmit3);
      eph->Toc = GALWeekSecond(wn_1,t0c).weekRolloverAdj(xmit1);
         // health is set below
      eph->Cuc = ephPage[esiCuc]->asSignedDouble(esbCuc,enbCuc,escCuc);
      eph->Cus = ephPage[esiCus]->asSignedDouble(esbCus,enbCus,escCus);
      eph->Crc = ephPage[esiCrc]->asSignedDouble(esbCrc,enbCrc,escCrc);
      eph->Crs = ephPage[esiCrs]->asSignedDouble(esbCrs,enbCrs,escCrs);
      eph->Cic = ephPage[esiCic]->asSignedDouble(esbCic,enbCic,escCic);
      eph->Cis = ephPage[esiCis]->asSignedDouble(esbCis,enbCis,escCis);
      eph->M0  = ephPage[esiM0]->asDoubleSemiCircles(esbM0,enbM0,escM0);
      eph->dn  = ephPage[esidn]->asDoubleSemiCircles(esbdn,enbdn,escdn);
         // no dndot in F/NAV
      eph->ecc = ephPage[esiEcc]->asUnsignedDouble(esbEcc,enbEcc,escEcc);
      eph->Ahalf = ephPage[esiAhalf]->asUnsignedDouble(esbAhalf,enbAhalf,
                                                       escAhalf);
      eph->A = eph->Ahalf * eph->Ahalf;
         // no Adot in F/NAV
      eph->OMEGA0 = ephPage[esiOMEGA0]->asDoubleSemiCircles(esbOMEGA0,enbOMEGA0,
                                                            escOMEGA0);
      eph->i0 = ephPage[esii0]->asDoubleSemiCircles(esbi0,enbi0,esci0);
      eph->w = ephPage[esiw]->asDoubleSemiCircles(esbw,enbw,escw);
      eph->OMEGAdot = ephPage[esiOMEGAdot]->asDoubleSemiCircles(
         esbOMEGAdot,enbOMEGAdot,escOMEGAdot);
      eph->idot = ephPage[esiidot]->asDoubleSemiCircles(esbidot,enbidot,
                                                        escidot);
      eph->af0 = ephPage[esiaf0]->asSignedDouble(esbaf0,enbaf0,escaf0);
      eph->af1 = ephPage[esiaf1]->asSignedDouble(esbaf1,enbaf1,escaf1);
      eph->af2 = ephPage[esiaf2]->asSignedDouble(esbaf2,enbaf2,escaf2);
         // GalFNavEph
      eph->bgdE5aE1 = ephPage[esiBGDa]->asSignedDouble(esbBGDa,enbBGDa,escBGDa);
      eph->sisaIndex = ephPage[esiSISA]->asUnsignedLong(esbSISA,enbSISA,
                                                        escSISA);
      eph->svid = ephPage[esiSVID]->asUnsignedLong(esbSVID,enbSVID,escSVID);
      eph->xmit2 = ephPage[pt2]->getTransmitTime();
      eph->xmit3 = ephPage[pt3]->getTransmitTime();
      eph->xmit4 = ephPage[pt4]->getTransmitTime();
      eph->iodnav1 = iod1;
      eph->iodnav2 = iod2;
      eph->iodnav3 = iod3;
      eph->iodnav4 = iod4;
      eph->hsE5a = static_cast<GalHealthStatus>(
         ephPage[esiE5ahs]->asUnsignedLong(esbE5ahs,enbE5ahs,escE5ahs));
      eph->dvsE5a = static_cast<GalDataValid>(
         ephPage[esiE5advs]->asUnsignedLong(esbE5advs,enbE5advs,escE5advs));
         // set health using the Galileo algorithms.
      eph->health = GalINavHealth::galHealth(eph->hsE5a,eph->dvsE5a,
                                             eph->sisaIndex);
      eph->wn1 = wn_1;
      eph->tow1 = tow_1;
      eph->wn2 = ephPage[esiWN_2]->asUnsignedLong(esbWN_2,enbWN_2,escWN_2);
      eph->tow2 = ephPage[esiTOW_2]->asUnsignedLong(esbTOW_2,enbTOW_2,escTOW_2);
      eph->wn3 = wn_3;
      eph->tow3 = tow_3;
      eph->tow4 = ephPage[esiTOW_4]->asUnsignedLong(esbTOW_4,enbTOW_4,escTOW_4);
      eph->fixFit();
      // cerr << "add F/NAV eph" << endl;
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(key);
      return true;
   }


   bool PNBGalFNavDataFactory ::
   processAlm(unsigned pageType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (!PNBNavDataFactory::processAlm && !PNBNavDataFactory::processHea)
      {
            // User doesn't want almanacs or health so don't do any
            // processing.
         return true;
      }
      if (almAcc.find(key) == almAcc.end())
      {
         // cerr << "almAcc is empty for signal " << key << endl;
            // set up a new almanac
         almAcc[key].resize(numAlmPTs);
         almAcc[key][pageType-5] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three subframes at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &almPage(almAcc[key]);
      almPage[pageType-5] = navIn;
         // stop processing if we don't have three full subframes
      if (!almPage[pt5] || !almPage[pt6] ||
          (almPage[pt5]->getNumBits() != 244) ||
          (almPage[pt6]->getNumBits() != 244))
      {
         // cerr << "Not ready for full F/NAV alm processing" << endl
         //      << "  pt5: " << (bool)almPage[pt5] << " "
         //      << (almPage[pt5] ? almPage[pt5]->getNumBits() : -1) << endl
         //      << "  pt6: " << (bool)almPage[pt6] << " "
         //      << (almPage[pt6] ? almPage[pt6]->getNumBits() : -1) << endl;
         return true;
      }
         // Stop processing if we don't have matching IODa in each of
         // the two page types.
      unsigned long ioda5, ioda6;
      ioda5 = almPage[pt5]->asUnsignedLong(asbIODa,anbIODa,ascIODa);
      ioda6 = almPage[pt6]->asUnsignedLong(asbIODa,anbIODa,ascIODa);
      if (ioda5 != ioda6)
      {
         // cerr << "IODa mismatch, not processing: " << ioda5 << " " << ioda6 << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
         // OMEGA0 is split in SV(SVID2) but not in SVID1 or SVID3 so
         // we process it separately.
         // SVID1
      NavDataPtr p0 = std::make_shared<GalFNavAlm>();
      NavDataPtr p1 = std::make_shared<GalFNavHealth>();
      GalFNavAlm *alm = dynamic_cast<GalFNavAlm*>(p0.get());
      GalFNavHealth *hp1 = dynamic_cast<GalFNavHealth*>(p1.get());
      if (processAlmOrb(almPage, alm, hp1,  pt5, pt5, asiSVID_1, asbSVID_1,
                        asidAhalf_1, asbdAhalf_1,
                        asiEcc_1, asbEcc_1, asiw_1, asbw_1, asidi_1, asbdi_1,
                        asiOMEGAdot_1, asbOMEGAdot_1,
                        asiM0_1, asbM0_1, asiaf0_1, asbaf0_1, asiaf1_1,
                        asbaf1_1, asiE5ahs_1, asbE5ahs_1))
      {
         if (PNBNavDataFactory::processAlm)
         {
               // cerr << "add F/NAV alm SVID1" << endl;
            alm->ioda5 = ioda5;
            alm->ioda6 = ioda6;
            alm->OMEGA0 = almPage[asiOMEGA0_1]->asDoubleSemiCircles(
               asbOMEGA0_1, anbOMEGA0, ascOMEGA0);
            navOut.push_back(p0);
         }
         if (PNBNavDataFactory::processHea)
         {
            navOut.push_back(p1);
         }
      }
         // SVID2
      p0 = std::make_shared<GalFNavAlm>();
      p1 = std::make_shared<GalFNavHealth>();
      alm = dynamic_cast<GalFNavAlm*>(p0.get());
      hp1 = dynamic_cast<GalFNavHealth*>(p1.get());
      if (processAlmOrb(almPage, alm, hp1,  pt5, pt6, asiSVID_2, asbSVID_2,
                        asidAhalf_2, asbdAhalf_2,
                        asiEcc_2, asbEcc_2, asiw_2, asbw_2, asidi_2, asbdi_2,
                        asiOMEGAdot_2, asbOMEGAdot_2,
                        asiM0_2, asbM0_2, asiaf0_2, asbaf0_2, asiaf1_2,
                        asbaf1_2, asiE5ahs_2, asbE5ahs_2))
      {
         if (PNBNavDataFactory::processAlm)
         {
               // cerr << "add F/NAV alm SVID1" << endl;
            alm->ioda5 = ioda5;
            alm->ioda6 = ioda6;
               // OMEGA0 for SVID2 is split across page type 5 and 6,
               // but I don't want to add the overhead of combining
               // the two PackedNavBits objects just to decode this
               // one item, so split the decoding across the two
               // objects.  Get the sign from the MSBs of course.
            long msb = almPage[asiOMEGA0m_2]->asLong(
               asbOMEGA0m_2, anbOMEGA0m_2, 1);
            unsigned long lsb = almPage[asiOMEGA0l_2]->asUnsignedLong(
               asbOMEGA0l_2, anbOMEGA0l_2, 1);
               // combine the bits
            long iomega = (msb << anbOMEGA0l_2) | lsb;
               // scale
            alm->OMEGA0 = ldexp((double)iomega, ascOMEGA0) * PI;
            // cerr << "msb=" << hex << msb << "  lsb=" << lsb << "  iomega="
            //      << iomega << "  OMEGA0=" << alm->OMEGA0 << endl;
            navOut.push_back(p0);
         }
         if (PNBNavDataFactory::processHea)
         {
            navOut.push_back(p1);
         }
      }
         // SVID3
      p0 = std::make_shared<GalFNavAlm>();
      p1 = std::make_shared<GalFNavHealth>();
      alm = dynamic_cast<GalFNavAlm*>(p0.get());
      hp1 = dynamic_cast<GalFNavHealth*>(p1.get());
      if (processAlmOrb(almPage, alm, hp1,  pt6, pt6, asiSVID_3, asbSVID_3,
                        asidAhalf_3, asbdAhalf_3,
                        asiEcc_3, asbEcc_3, asiw_3, asbw_3, asidi_3, asbdi_3,
                        asiOMEGAdot_3, asbOMEGAdot_3,
                        asiM0_3, asbM0_3, asiaf0_3, asbaf0_3, asiaf1_3,
                        asbaf1_3, asiE5ahs_3, asbE5ahs_3))
      {
         if (PNBNavDataFactory::processAlm)
         {
               // cerr << "add F/NAV alm SVID1" << endl;
            alm->ioda5 = ioda5;
            alm->ioda6 = ioda6;
            alm->OMEGA0 = almPage[asiOMEGA0_3]->asDoubleSemiCircles(
               asbOMEGA0_3, anbOMEGA0, ascOMEGA0);
            navOut.push_back(p0);
         }
         if (PNBNavDataFactory::processHea)
         {
            navOut.push_back(p1);
         }
      }
         // Clear out the broadcast almanac that's been processed.
      almAcc.erase(key);
      return true;
   }


   bool PNBGalFNavDataFactory ::
   processAlmOrb(const std::vector<PackedNavBitsPtr>& almPage,
                 GalFNavAlm *alm, GalFNavHealth *hp1,
                 int ptA, int ptB,
                 int asiSVID, int asbSVID,
                 int asidAhalf, int asbdAhalf,
                 int asiEcc, int asbEcc,
                 int asiw, int asbw,
                 int asidi, int asbdi,
                 int asiOMEGAdot, int asbOMEGAdot,
                 int asiM0, int asbM0,
                 int asiaf0, int asbaf0,
                 int asiaf1, int asbaf1,
                 int asiE5ahs, int asbE5ahs)
   {
         // NavData
      alm->timeStamp = almPage[ptA]->getTransmitTime();
      unsigned long svid = almPage[asiSVID]->asUnsignedLong(asbSVID,anbSVID,
                                                            ascSVID);
      if (svid == 0)
         return false; // empty almanac, do nothing.
      SatID xmitSat(almPage[ptA]->getsatSys());
      NavSatelliteID sat(svid, xmitSat, almPage[ptA]->getobsID(),
                         almPage[ptA]->getNavID());
      alm->signal = NavMessageID(sat, NavMessageType::Almanac);
         // cerr << "Ready for full F/NAV alm processing for " << (NavSignalID)sat << endl;
         // OrbitData = empty
         // OrbitDataKepler
      alm->xmitTime = alm->timeStamp;
      alm->xmit2 = almPage[ptB]->getTransmitTime();
      alm->t0a = almPage[asit0a]->asUnsignedLong(asbt0a,anbt0a,asct0a);
      alm->wna = almPage[asiWNa]->asUnsignedLong(asbWNa,anbWNa,ascWNa);
         // Not sure if this is appropriate but it beats saving the
         // 12-bit WN from page type 5.
      GALWeekSecond gws(alm->xmitTime);
         // WNa is 2 bits...
      gws.week = (gws.week & ~0x03) | alm->wna;
      gws.sow = alm->t0a;
      alm->Toe = alm->Toc = gws;
         // cerr << "  wna=" << alm->wna << "  tow=" << gws.sow << "  t0a=" << alm->t0a << "  Toe=" << printTime(alm->Toe, "%L %g") << "  xmit=" << printTime(alm->xmitTime, "%L %g") << endl;
      alm->wna = gws.week;
      alm->M0  = almPage[asiM0]->asDoubleSemiCircles(asbM0,anbM0,ascM0);
      alm->ecc = almPage[asiEcc]->asUnsignedDouble(asbEcc,anbEcc,ascEcc);
      alm->dAhalf = almPage[asidAhalf]->asUnsignedDouble(asbdAhalf,anbdAhalf,
                                                         ascdAhalf);
         // slightly annoying that the reference value is for A but
         // the transmitted data is A**1/2.
      alm->Ahalf = ::sqrt(GalFNavAlm::refA) + alm->dAhalf;
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->deltai = almPage[asidi]->asDoubleSemiCircles(asbdi,anbdi,ascdi);
      alm->i0 = GalFNavAlm::refioffset + alm->deltai;
      alm->w = almPage[asiw]->asDoubleSemiCircles(asbw,anbw,ascw);
      alm->OMEGAdot = almPage[asiOMEGAdot]->asDoubleSemiCircles(
         asbOMEGAdot,anbOMEGAdot,ascOMEGAdot);
      alm->af0 = almPage[asiaf0]->asSignedDouble(asbaf0,anbaf0,ascaf0);
      alm->af1 = almPage[asiaf1]->asSignedDouble(asbaf1,anbaf1,ascaf1);
         // GalFNavData
      alm->hsE5a = static_cast<GalHealthStatus>(
         almPage[asiE5ahs]->asUnsignedLong(asbE5ahs,anbE5ahs,ascE5ahs));
      alm->fixFit();
      alm->fixHealth();
      if (PNBNavDataFactory::processHea)
      {
         hp1->timeStamp = almPage[asiE5ahs]->getTransmitTime();
         hp1->signal = NavMessageID(sat, NavMessageType::Health);
         hp1->sigHealthStatus = static_cast<GalHealthStatus>(
            almPage[asiE5ahs]->asUnsignedLong(asbE5ahs,anbE5ahs,ascE5ahs));
            /// @todo figure out what DVS and SISA should be here.
      }
      return true;
   }


#if 0
   void PNBGalFNavDataFactory ::
   dumpState(std::ostream& s)
      const
   {
      s << "fullWNaMap.size() = " << fullWNaMap.size() << " (expect 32)" << endl
        << "almAcc.size() = " << almAcc.size() << " (expect 32)" << endl;
      for (const auto& i : almAcc)
      {
         s << "  almAcc[" << i.first << "].size() = " << i.second.size()
           << endl;
      }
      s << "ephAcc.size() = " << ephAcc.size() << " (expect 32)" << endl;
      for (const auto& i : ephAcc)
      {
         s << "  ephAcc[" << i.first << "].size() = " << i.second.size()
           << endl;
         for (unsigned j = 0; j < i.second.size(); j++)
         {
            s << "    ephAcc[" << i.first << "][" << j << "] = ";
            if (!i.second[j])
               s << "unset";
            else
               s << i.second[j]->getNumBits() << " bits";
            s << endl;
         }
      }
   }
#endif


   void PNBGalFNavDataFactory ::
   resetState()
   {
      almAcc.clear();
      ephAcc.clear();
   }
} // namespace gnsstk
