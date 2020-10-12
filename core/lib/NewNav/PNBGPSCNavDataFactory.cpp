#include <math.h>
#include "PNBGPSCNavDataFactory.hpp"
//#include "GPSCNavAlm.hpp"
#include "GPSCNavEph.hpp"
//#include "GPSCNavHealth.hpp"
//#include "GPSCNavTimeOffset.hpp"
#include "TimeCorrection.hpp"

using namespace std;

// This enum is only used here and it's being used to avoid confusion
// on vector indices.  Using enums instead of constants because it
// doesn't use any memory that way.  Also not using strongly typed
// enums because we actually want these values to be implicitly cast
// to integer values.
enum SFIndex
{
   ephM10 = 0,  ///< Message type 10
   ephM11 = 1,  ///< Message type 11
   ephMClk = 2  ///< Clock message (any of type 30-37)
};

/// More CNAV-related constants.
static const double Aref = 26559710;
static const double OMEGAdotREF = -2.6e-9 * gpstk::PI;

/** Subframe index, start bits, bit counts and scale factor (*n for
 * integer quantities, *2^n for floating point quantities) for each of
 * the ephemeris fields.
 * Bit positions/sizes from IS-GPS-200 Figures 30-1 and 30-2.
 * Scale factors taken from Tables 20-I and 20-III.
 * @note When the comment for an esc* enum says "scale factor", the
 *   bits are multiplied by that scalar.  When the comment says "power
 *   factor", the bits are multiplied by 2^n where n is the esc*
 *   value.
 */
enum CNavBitInfo
{
      // every subframe has a preamble so no subframe index here.
      /// @todo Make sure all the scale factors are correct.
   esbPre = 0, ///< Preamble start bit
   enbPre = 8, ///< Preamble number of bits
   escPre = 1, ///< Preamble scale factor

   esbPRN = 8, ///< PRN start bit
   enbPRN = 6, ///< PRN number of bits
   escPRN = 1, ///< PRN scale factor

   esbMsgType = 14, ///< Message type start bit
   enbMsgType = 6, ///< Message type number of bits
   escMsgType = 1, ///< Message type scale factor

   esbTOW = 20, ///< TOW start bit
   enbTOW = 17, ///< TOW number of bits
   escTOW = 6,  ///< TOW scale factor

   esbAlert = 37, ///< Alert flag start bit
   enbAlert = 1,  ///< Alert flag number of bits
   escAlert = 1,  ///< Alert flag scale factor

   esiWN = ephM10, ///< WN subframe index
   esbWN = 38,    ///< WN start bit
   enbWN = 13,    ///< WN number of bits
   escWN = 1,     ///< WN scale factor

   esiHea   = ephM10, ///< Signal health subframe index
   esbHeaL1 = 51,     ///< L1 Signal health bit
   esbHeaL2 = 52,     ///< L2 Signal health bit
   esbHeaL5 = 53,     ///< L5 Signal health bit

   esitop = ephM10, ///< top subframe index
   esbtop = 54,    ///< top start bit
   enbtop = 11,    ///< top number of bits
   esctop = 300,   ///< top scale factor

   esiURA = ephM10, ///< URA index subframe index
   esbURA = 65,    ///< URA index start bit
   enbURA = 5,     ///< URA index number of bits
   escURA = 1,     ///< URA index scale factor

   esitoe1 = ephM10, ///< toe (ephemeris 1) subframe index
   esbtoe1 = 70,    ///< toe (ephemeris 1) start bit
   enbtoe1 = 11,    ///< toe (ephemeris 1) number of bits
   esctoe1 = 300,   ///< toe (ephemeris 1) scale factor

   esidA = ephM10, ///< Delta A subframe index
   esbdA = 81,    ///< Delta A start bit
   enbdA = 26,    ///< Delta A number of bits
   escdA = -9,    ///< Delta A power factor

   esiAdot = ephM10, ///< Adot subframe index
   esbAdot = 107,   ///< Adot start bit
   enbAdot = 25,    ///< Adot number of bits
   escAdot = -21,   ///< Adot power factor

   esidn0 = ephM10, ///< Delta n0 subframe index
   esbdn0 = 132,   ///< Delta n0 start bit
   enbdn0 = 17,    ///< Delta n0 number of bits
   escdn0 = -44, ///< Delta n0 power factor

   esidn0dot = ephM10, ///< Delta n0 dot subframe index
   esbdn0dot = 149,   ///< Delta n0 dot start bit
   enbdn0dot = 23,    ///< Delta n0 dot number of bits
   escdn0dot = -57,   ///< Delta n0 dot power factor

   esiM0 = ephM10, ///< M0-n subframe index
   esbM0 = 172,   ///< M0-n start bit
   enbM0 = 33,    ///< M0-n number of bits
   escM0 = -32,   ///< M0-n power factor

   esiEcc = ephM10, ///< Ecc n subframe index
   esbEcc = 205,   ///< Ecc n start bit
   enbEcc = 33,    ///< Ecc n number of bits
   escEcc = -34,   ///< Ecc n power factor

   esiw = ephM10, ///< w subframe index
   esbw = 238,   ///< w start bit
   enbw = 33,    ///< w number of bits
   escw = -32,   ///< w power factor

      // stored as bool, no scale/bit count needed
   esiInt = ephM10, ///< Integrity status flag subframe index
   esbInt = 271,   ///< Integrity status flag start bit

   esiPL2C = ephM10, ///< L2C Phasing subframe index
   esbPL2C = 272,   ///< L2C Phasing start bit

   esitoe2 = ephM11, ///< toe (ephemeris 2) subframe index
   esbtoe2 = 38,    ///< toe (ephemeris 2) start bit
   enbtoe2 = 11,    ///< toe (ephemeris 2) number of bits
   esctoe2 = 300,   ///< toe (ephemeris 2) scale factor

   esiOMEGA0 = ephM11, ///< OMEGA0-n subframe index
   esbOMEGA0 = 49,    ///< OMEGA0-n MSBs start bit
   enbOMEGA0 = 33,    ///< OMEGA0-n MSBs number of bits
   escOMEGA0 = -32,   ///< OMEGA0-n power factor

   esii0 = ephM11, ///< i0-n subframe index
   esbi0 = 82,    ///< i0-n MSBs start bit
   enbi0 = 33,    ///< i0-n MSBs number of bits
   esci0 = -32,   ///< i0-n power factor

   esidOMEGAdot = ephM11, ///< Delta OMEGAdot subframe index
   esbdOMEGAdot = 115,   ///< Delta OMEGAdot start bit
   enbdOMEGAdot = 17,    ///< Delta OMEGAdot number of bits
   escdOMEGAdot = -44,   ///< Delta OMEGAdot power factor

   esiidot = ephM11, ///< i0-n - DOT subframe index
   esbidot = 132,   ///< i0-n - DOT start bit
   enbidot = 15,    ///< i0-n - DOT number of bits
   escidot = -44,   ///< i0-n - DOT power factor

   esiCis = ephM11, ///< Cis-n subframe index
   esbCis = 147,   ///< Cis-n start bit
   enbCis = 16,    ///< Cis-n number of bits
   escCis = -30,   ///< Cis-n power factor

   esiCic = ephM11, ///< Cic-n subframe index
   esbCic = 163,   ///< Cic-n start bit
   enbCic = 16,    ///< Cic-n number of bits
   escCic = -30,   ///< Cic-n power factor

   esiCrs = ephM11, ///< Crs-n subframe index
   esbCrs = 179,   ///< Crs-n start bit
   enbCrs = 24,    ///< Crs-n number of bits
   escCrs = -8,    ///< Crs-n power factor

   esiCrc = ephM11, ///< Crc-n subframe index
   esbCrc = 203,   ///< Crc-n start bit
   enbCrc = 24,    ///< Crc-n number of bits
   escCrc = -8,    ///< Crc-n power factor

   esiCus = ephM11, ///< Cus-n subframe index
   esbCus = 227,   ///< Cus-n start bit
   enbCus = 21,    ///< Cus-n number of bits
   escCus = -30,   ///< Cus-n power factor

   esiCuc = ephM11, ///< Cuc-n subframe index
   esbCuc = 248,   ///< Cuc-n start bit
   enbCuc = 21,    ///< Cuc-n number of bits
   escCuc = -30,   ///< Cuc-n power factor

      /// @note these apply to all clock messages, type 30-37
   csitop = ephMClk, ///< top subframe index
   csbtop = 38,      ///< top start bit
   cnbtop = 11,      ///< top number of bits
   csctop = 300,     ///< top scale factor

   csiURAned0 = ephMClk, ///< URA_NED0 subframe index
   csbURAned0 = 49,      ///< URA_NED0 start bit
   cnbURAned0 = 5,       ///< URA_NED0 number of bits
   cscURAned0 = 1,       ///< URA_NED0 scale factor

   csiURAned1 = ephMClk, ///< URA_NED1 subframe index
   csbURAned1 = 54,      ///< URA_NED1 start bit
   cnbURAned1 = 3,       ///< URA_NED1 number of bits
   cscURAned1 = 1,       ///< URA_NED1 scale factor

   csiURAned2 = ephMClk, ///< URA_NED2 subframe index
   csbURAned2 = 57,      ///< URA_NED2 start bit
   cnbURAned2 = 3,       ///< URA_NED2 number of bits
   cscURAned2 = 1,       ///< URA_NED2 scale factor

   csitoc = ephMClk, ///< toc subframe index
   csbtoc = 60,      ///< toc start bit
   cnbtoc = 11,      ///< toc number of bits
   csctoc = 300,     ///< toc scale factor

   csiaf0 = ephMClk, ///< af0 subframe index
   csbaf0 = 71,      ///< af0 start bit
   cnbaf0 = 26,      ///< af0 number of bits
   cscaf0 = -35,     ///< af0 power factor

   csiaf1 = ephMClk, ///< af1 subframe index
   csbaf1 = 97,      ///< af1 start bit
   cnbaf1 = 20,      ///< af1 number of bits
   cscaf1 = -48,     ///< af1 power factor

   csiaf2 = ephMClk, ///< af2 subframe index
   csbaf2 = 117,     ///< af2 start bit
   cnbaf2 = 10,      ///< af2 number of bits
   cscaf2 = -60,     ///< af2 power factor
};


namespace gpstk
{
   bool PNBGPSCNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (navIn->getNavID().navType != NavType::GPSCNAVL2)
      {
            // This class only processes GPS CNav.
         return false;
      }
      bool rv = true;
      try
      {
            /*
         cerr << "preamble:  " << hex << navIn->asUnsignedLong(0,8,1) << dec
              << endl
              << "  tlm msg: " << hex << navIn->asUnsignedLong(8,14,1) << dec
              << endl
              << "  integ:   " << navIn->asUnsignedLong(22,1,1) << endl
              << "  reserved:" << navIn->asUnsignedLong(23,1,1) << endl
              << "  parity:  " << hex << navIn->asUnsignedLong(24,6,1) << endl
              << "  tow:     " << navIn->asUnsignedLong(30,17,1) << endl
              << "  alert:   " << navIn->asUnsignedLong(47,1,1) << endl
              << "  A/S:     " << navIn->asUnsignedLong(48,1,1) << endl
              << "  sfid:    " << navIn->asUnsignedLong(49,3,1) << endl;
            */
         unsigned long msgType = navIn->asUnsignedLong(esbMsgType,enbMsgType,
                                                       escMsgType);
         cerr << "msgType = " << msgType << endl;
         unsigned long svid = 0;
         switch (msgType)
         {
            case 10:
            case 11:
            case 30:
            case 31:
            case 32:
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
                  //cerr << "sfid " << sfid << " = ephemeris" << endl;
               rv = processEph(msgType, navIn, navOut);
               break;
/*
            case 4:
            case 5:
               svid = navIn->asUnsignedLong(62,6,1);
                  //cerr << "sfid " << sfid << " = almanac, svid " << svid << endl;
               if ((svid <= gpstk::MAX_PRN_GPS) && (svid >= 1))
               {
                     // process orbit and health
                  rv = processAlmOrb(svid, navIn, navOut);
               }
               else if (svid == 51)
               {
                     // process health
                  rv = processSVID51(navIn, navOut);
               }
               else if (svid == 63)
               {
                     // process health
                  rv = processSVID63(navIn, navOut);
               }
               else if (svid == 56)
               {
                     // process time offset
                  rv = processSVID56(navIn, navOut);
               }
               break;
*/
            default:
                  //cerr << "invalid sfid " << sfid << endl;
               rv = false;
               break;
         }
         // cerr << "  results: " << navOut.size() << endl;
         // for (const auto& i : navOut)
         //    i->dump(cerr,gpstk::NavData::Detail::Full);
      }
      catch (gpstk::Exception& exc)
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


   bool PNBGPSCNavDataFactory ::
   processEph(unsigned msgType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
         /** @todo Should we ignore the PNB satellite ID and just use
          * the PRN in the nav message, or maybe compare the two as an
          * validity check? */
      unsigned prn = navIn->getsatSys().id;
      unsigned vecIdx;
      switch (msgType)
      {
         case 10:
            vecIdx = ephM10;
            break;
         case 11:
            vecIdx = ephM11;
            break;
         case 30:
         case 31:
         case 32:
         case 33:
         case 34:
         case 35:
         case 36:
         case 37:
            vecIdx = ephMClk;
            break;
         default:
            return false;
      }
#if 0
      if ((msgType == 10) && processHea)
      {
            // Add ephemeris health bits from subframe 1.
         gpstk::NavDataPtr p1 = std::make_shared<gpstk::GPSCNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                           navIn->getNavID()),
            NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(esbHea,enbHea,escHea);
            //cerr << "add eph health" << endl;
         navOut.push_back(p1);
      }
#endif
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      if (ephAcc.find(prn) == ephAcc.end())
      {
            // set up a new ephemeris
         ephAcc[prn].resize(3);
         ephAcc[prn][vecIdx] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three subframes at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephSF(ephAcc[prn]);
      ephSF[vecIdx] = navIn;
         // stop processing if we don't have three full subframes
      if (!ephSF[ephM10] || !ephSF[ephM11] || !ephSF[ephMClk] ||
          (ephSF[ephM10]->getNumBits() != 300) ||
          (ephSF[ephM11]->getNumBits() != 300) ||
          (ephSF[ephMClk]->getNumBits() != 300))
      {
         cerr << "Not ready for full eph processing" << endl;
         return true;
      }
         // Stop processing if we don't have matching toe/toc in
         // each of the three message types.
         /** @note Some data elements, e.g. toe, are stored internally
          * as floating point, but are treated as integers in the
          * encoded message.  This is not a mistake.  It's mostly due
          * to how the scaling is handled for the quantity,
          * i.e. linear scaling vs fractional (ldexp). */
      double toe10 = ephSF[esitoe1]->asUnsignedLong(esbtoe1,enbtoe1,esctoe1);
      double toe11 = ephSF[esitoe2]->asUnsignedLong(esbtoe2,enbtoe2,esctoe2);
      double toc = ephSF[csitoc]->asUnsignedLong(csbtoc,cnbtoc,csctoc);
      if ((toe10 != toe11) || (toe10 != toc))
      {
         cerr << "IODC/IODE mismatch, not processing" << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
      cerr << "Ready for full eph processing" << endl;
      gpstk::NavDataPtr p0 = std::make_shared<gpstk::GPSCNavEph>();
      GPSCNavEph *eph = dynamic_cast<gpstk::GPSCNavEph*>(p0.get());
         // NavData
      eph->timeStamp = ephSF[ephM10]->getTransmitTime();
      eph->signal = NavMessageID(
         NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Ephemeris);
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      unsigned wn = ephSF[esiWN]->asUnsignedLong(esbWN,enbWN,escWN);
         // Use the transmit time to get a full week for toe/toc
         //GPSWeekSecond refTime(eph->xmitTime);
         //long refWeek = refTime.week;
         //wn = timeAdjustWeekRollover(wn, refWeek);
         // Now we can set the Toe/Toc properly.  Note that IS-GPS-200
         // defines the toc and toe to be the same for a consistent
         // set of data, and we've already enforced they're the same
         // above.
      eph->Toe = eph->Toc = GPSWeekSecond(wn,toe10);
         // healthy is set below
      eph->Cuc = ephSF[esiCuc]->asSignedDouble(esbCuc,enbCuc,escCuc);
      eph->Cus = ephSF[esiCus]->asSignedDouble(esbCus,enbCus,escCus);
      eph->Crc = ephSF[esiCrc]->asSignedDouble(esbCrc,enbCrc,escCrc);
      eph->Crs = ephSF[esiCrs]->asSignedDouble(esbCrs,enbCrs,escCrs);
      eph->Cic = ephSF[esiCic]->asSignedDouble(esbCic,enbCic,escCic);
      eph->Cis = ephSF[esiCis]->asSignedDouble(esbCis,enbCis,escCis);
      eph->M0  = ephSF[esiM0]->asDoubleSemiCircles(esbM0,enbM0,escM0);
      eph->dn  = ephSF[esidn0]->asDoubleSemiCircles(esbdn0,enbdn0,escdn0);
      eph->dndot = ephSF[esidn0dot]->asDoubleSemiCircles(esbdn0dot,enbdn0dot,
                                                         escdn0dot);
      eph->ecc = ephSF[esiEcc]->asUnsignedDouble(esbEcc,enbEcc,escEcc);
      eph->deltaA = ephSF[esidA]->asSignedDouble(esbdA,enbdA,escdA);
      eph->A = eph->deltaA + Aref;
      eph->Ahalf = ::sqrt(eph->A);
      eph->Adot = ephSF[esiAdot]->asSignedDouble(esbAdot,enbAdot,escAdot);
      eph->OMEGA0 = ephSF[esiOMEGA0]->asDoubleSemiCircles(esbOMEGA0,enbOMEGA0,
                                                          escOMEGA0);
      eph->i0 = ephSF[esii0]->asDoubleSemiCircles(esbi0,enbi0,esci0);
      eph->w = ephSF[esiw]->asDoubleSemiCircles(esbw,enbw,escw);
      eph->dOMEGAdot = ephSF[esidOMEGAdot]->asDoubleSemiCircles(
         esbdOMEGAdot,enbdOMEGAdot,escdOMEGAdot);
      eph->OMEGAdot = eph->dOMEGAdot + OMEGAdotREF;
      eph->idot = ephSF[esiidot]->asDoubleSemiCircles(esbidot,enbidot,escidot);
      eph->af0 = ephSF[csiaf0]->asSignedDouble(csbaf0,cnbaf0,cscaf0);
      eph->af1 = ephSF[csiaf1]->asSignedDouble(csbaf1,cnbaf1,cscaf1);
      eph->af2 = ephSF[csiaf2]->asSignedDouble(csbaf2,cnbaf2,cscaf2);
         // GPSCNavData
      eph->pre = ephSF[ephM10]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->alert = ephSF[ephM10]->asBool(esbAlert);
         // GPSCNavEph
      eph->pre2 = ephSF[ephM11]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->healthL1 = ephSF[esiHea]->asBool(esbHeaL1);
      eph->healthL2 = ephSF[esiHea]->asBool(esbHeaL2);
      eph->healthL5 = ephSF[esiHea]->asBool(esbHeaL5);
         /** @todo should this be limited to only if the L2 health bit
          * is set?  Definitely not if this code ends up being used
          * for L5 CNAV. */
      eph->healthy = (eph->healthL2 == 0); // actually in OrbitDataKepler
      eph->uraED = ephSF[esiURA]->asLong(esbURA,enbURA,escURA);
      eph->alert2 = ephSF[ephM11]->asBool(esbAlert);
      double top = ephSF[esitop]->asUnsignedLong(esbtop,enbtop,esctop);
      eph->top = GPSWeekSecond(wn,top);
      eph->xmit11 = ephSF[ephM11]->getTransmitTime();
      eph->xmitClk = ephSF[ephMClk]->getTransmitTime();
      eph->uraNED0= ephSF[csiURAned0]->asLong(csbURAned0,cnbURAned0,cscURAned0);
      eph->uraNED1= ephSF[csiURAned1]->asUnsignedLong(csbURAned1,cnbURAned1,
                                                      cscURAned1);
      eph->uraNED2= ephSF[csiURAned2]->asUnsignedLong(csbURAned2,cnbURAned2,
                                                      cscURAned2);
      eph->fixFit();
         //cerr << "add eph" << endl;
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(prn);
      return true;
   }


#if 0
   bool PNBGPSCNavDataFactory ::
   processAlmOrb(unsigned long prn, const PackedNavBitsPtr& navIn,
                 NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      if (processHea)
      {
            // Add almanac orbit page health bits.
         gpstk::NavDataPtr p1 = std::make_shared<gpstk::GPSCNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                           navIn->getNavID()),
            NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(136,8,1);
            //cerr << "add alm health" << endl;
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
         // SVID 1-32 contain the almanac orbital elements as well as
         // health information (Figure 20-1 sheet 4), so we'll end up
         // returning two items in navOut.
      gpstk::SatID xmitSat(navIn->getsatSys());
      gpstk::NavDataPtr p0 = std::make_shared<gpstk::GPSCNavAlm>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = gpstk::NavMessageID(
         gpstk::NavSatelliteID(prn, xmitSat, navIn->getobsID(),
                               navIn->getNavID()),
         gpstk::NavMessageType::Almanac);
      GPSCNavAlm *alm = dynamic_cast<gpstk::GPSCNavAlm*>(p0.get());
         // Bit positions taken from IS-GPS-200 figure 20-1 sheet 4,
         // and figure 20-2, but subtracting 1 since PNB is 0-based.
         // Scales taken from Table 20-VI
         // Sinusoidal corrections and other unset parameters are not
         // used by almanac data and are initialized to 0 by
         // constructor
      alm->pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      alm->tlm = navIn->asUnsignedLong(esbTLM,enbTLM,escTLM);
      alm->alert = navIn->asBool(esbAlert);
      alm->asFlag = navIn->asBool(esbAS);
      alm->xmitTime = navIn->getTransmitTime();
      alm->ecc = navIn->asSignedDouble(68,16,-21);
      alm->toa = navIn->asUnsignedDouble(90,8,12);
      gpstk::GPSWeekSecond ws(alm->xmitTime);
      // cerr << "page " << prn << " WNa = ??  toa = " << alm->toa
      //      << "  WNx = " << (ws.week & 0x0ff) << "  tox = " << ws.sow << endl;
      alm->deltai = navIn->asDoubleSemiCircles(98,16,-19);
      alm->i0 = (0.3 * PI) + alm->deltai;
      alm->OMEGAdot = navIn->asDoubleSemiCircles(120,16,-38);
      alm->healthBits = navIn->asUnsignedLong(136,8,1);
      alm->healthy = alm->healthBits == 0;
      alm->Ahalf = navIn->asUnsignedDouble(150,24,-11);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asDoubleSemiCircles(180,24,-23);
      alm->w = navIn->asDoubleSemiCircles(210,24,-23);
      alm->M0 = navIn->asDoubleSemiCircles(240,24,-23);
      const unsigned af0start[] = {270,289};
      const unsigned af0num[] = {8,3};
         // 2 is the size of the af0start/af0num arrays
      alm->af0 = navIn->asSignedDouble(af0start,af0num,2,-20);
      alm->af1 = navIn->asSignedDouble(278,11,-38);
         // If we have a wna for this transmitting PRN, use it to set
         // the toa (identified as Toe/Toc in OrbitDataKepler).
         // Otherwise, stash the data until we do have a wna.
      if ((fullWNaMap.find(xmitSat.id) != fullWNaMap.end()) &&
          (fullWNaMap[xmitSat.id].sow == alm->toa))
      {
         alm->Toe = alm->Toc = fullWNaMap[xmitSat.id];
         alm->fixFit();
            //cerr << "add alm" << endl;
         navOut.push_back(p0);
      }
      else
      {
            // no WNa available yet, accumulate the data until we get a page 51
         almAcc[xmitSat.id].push_back(p0);
      }
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   processSVID51(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      gpstk::SatID xmitSat(navIn->getsatSys());
      if (PNBNavDataFactory::processAlm)
      {
            // Set the fullWNa now that we have something to go on,
            // but only if we're processing almanac data, which is the
            // only situation where it's used.
         double toa = navIn->asUnsignedDouble(68,8,12);
         unsigned shortWNa = navIn->asUnsignedLong(76, 8, 1);
         gpstk::GPSWeekSecond ws(navIn->getTransmitTime());
         long refWeek = ws.week;
         unsigned fullWNa = timeAdjust8BitWeekRollover(shortWNa, refWeek);
         fullWNaMap[xmitSat.id] = GPSWeekSecond(fullWNa, toa);
         // cerr << "page 51 WNa = " << shortWNa << "  toa = " << toa
         //      << "  WNx = " << (ws.week & 0x0ff) << "  tox = " << ws.sow
         //      << "  fullWNa = " << fullWNa << endl;
         NavDataPtrList& ndpl(almAcc[xmitSat.id]);
         for (auto i = ndpl.begin(); i != ndpl.end();)
         {
            GPSCNavAlm *alm = dynamic_cast<GPSCNavAlm*>(i->get());
            if (alm->toa == toa)
            {
               alm->Toe = alm->Toc = fullWNaMap[xmitSat.id];
               alm->fixFit();
                  //cerr << "add alm" << endl;
               navOut.push_back(*i);
               i = ndpl.erase(i);
            }
            else
            {
               ++i;
            }
         }
      }
         // svid 51 = sf 5 page 25. The format has 24 SVs in it,
         // grouped 4 per word.
      if (!PNBNavDataFactory::processHea)
      {
            // User doesn't want health so don't do any processing.
         return true;
      }
      gpstk::ObsID oid(navIn->getobsID());
      gpstk::NavID navid(navIn->getNavID());
      for (unsigned prn = 1, bit = 90; prn <= 24; prn += 4, bit += 30)
      {
         gpstk::NavDataPtr p1 = std::make_shared<gpstk::GPSCNavHealth>();
         gpstk::NavDataPtr p2 = std::make_shared<gpstk::GPSCNavHealth>();
         gpstk::NavDataPtr p3 = std::make_shared<gpstk::GPSCNavHealth>();
         gpstk::NavDataPtr p4 = std::make_shared<gpstk::GPSCNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+0, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(bit+0, 6, 1);
            //cerr << "add page 51 health" << endl;
         navOut.push_back(p1);
         p2->timeStamp = navIn->getTransmitTime();
         p2->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+1, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p2.get())->svHealth =
            navIn->asUnsignedLong(bit+6, 6, 1);
            //cerr << "add page 51 health" << endl;
         navOut.push_back(p2);
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+2, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p3.get())->svHealth =
            navIn->asUnsignedLong(bit+12, 6, 1);
            //cerr << "add page 51 health" << endl;
         navOut.push_back(p3);
         p4->timeStamp = navIn->getTransmitTime();
         p4->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+3, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p4.get())->svHealth =
            navIn->asUnsignedLong(bit+18, 6, 1);
            //cerr << "add page 51 health" << endl;
         navOut.push_back(p4);
      }
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   processSVID63(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
         // svid 63 = sf 4 page 25. The format has SVs 25-32 in it.
         // SV 25 is kind of off on its own so do it first, outside the loop
      if (!PNBNavDataFactory::processHea)
      {
            // User doesn't want health so don't do any processing.
         return true;
      }
      gpstk::SatID xmitSat(navIn->getsatSys());
      gpstk::ObsID oid(navIn->getobsID());
      gpstk::NavID navid(navIn->getNavID());
      gpstk::NavDataPtr p0 = std::make_shared<gpstk::GPSCNavHealth>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = gpstk::NavMessageID(
         gpstk::NavSatelliteID(25, xmitSat, oid, navid),
         gpstk::NavMessageType::Health);
         // prn 25 health starts at bit 229 (1-based), so we use 228 (0-based)
      dynamic_cast<gpstk::GPSCNavHealth*>(p0.get())->svHealth =
         navIn->asUnsignedLong(228, 6, 1);
         //cerr << "add page 63 health" << endl;
      navOut.push_back(p0);      
      for (unsigned prn = 26, bit = 240; prn <= 32; prn += 4, bit += 30)
      {
         gpstk::NavDataPtr p1 = std::make_shared<gpstk::GPSCNavHealth>();
         gpstk::NavDataPtr p2 = std::make_shared<gpstk::GPSCNavHealth>();
         gpstk::NavDataPtr p3 = std::make_shared<gpstk::GPSCNavHealth>();
         gpstk::NavDataPtr p4;
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+0, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(bit+0, 6, 1);
            //cerr << "add page 63 health" << endl;
         navOut.push_back(p1);
         p2->timeStamp = navIn->getTransmitTime();
         p2->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+1, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p2.get())->svHealth =
            navIn->asUnsignedLong(bit+6, 6, 1);
            //cerr << "add page 63 health" << endl;
         navOut.push_back(p2);
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = gpstk::NavMessageID(
            gpstk::NavSatelliteID(prn+2, xmitSat, oid, navid),
            gpstk::NavMessageType::Health);
         dynamic_cast<gpstk::GPSCNavHealth*>(p3.get())->svHealth =
            navIn->asUnsignedLong(bit+12, 6, 1);
            //cerr << "add page 63 health" << endl;
         navOut.push_back(p3);
            // Word 9 has 4 PRNs, word 10 only has 3, so we have to do
            // this check.
         if (prn < 30)
         {
            p4 = std::make_shared<gpstk::GPSCNavHealth>();
            p4->timeStamp = navIn->getTransmitTime();
            p4->signal = gpstk::NavMessageID(
               gpstk::NavSatelliteID(prn+3, xmitSat, oid, navid),
               gpstk::NavMessageType::Health);
            dynamic_cast<gpstk::GPSCNavHealth*>(p4.get())->svHealth =
               navIn->asUnsignedLong(bit+18, 6, 1);
               //cerr << "add page 63 health" << endl;
            navOut.push_back(p4);
         }
      }
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   processSVID56(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
         // svid 56 = sf 4 page 18.
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      gpstk::NavDataPtr p0 = std::make_shared<gpstk::GPSCNavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = gpstk::NavMessageID(
         gpstk::NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         gpstk::NavMessageType::TimeOffset);
      GPSCNavTimeOffset *to = dynamic_cast<gpstk::GPSCNavTimeOffset*>(p0.get());
         // Bit positions taken from IS-GPS-200 figure 20-1 sheet 8,
         // but subtracting 1 since PNB is 0-based.
         // Scales taken from Table 20-IX
         /** @note while some of the values are stored as doubles,
          * they may be extracted as long (e.g. deltatLS) because
          * there is no scaling for the encoded value. */
      const unsigned a0start[] = { 180,210 };
      const unsigned a0num[] = { 24,8 };
      to->deltatLS = navIn->asLong(240,8,1);
         // 2 is the size of the start/num arrays, while -30 is the
         // scale factor i.e. x*2^-30
      to->a0 = navIn->asSignedDouble(a0start,a0num,2,-30);
      to->a1 = navIn->asSignedDouble(150,24,-50);
      to->tot = navIn->asUnsignedDouble(218,8,12);
      to->wnt = navIn->asUnsignedLong(226,8,1);
      to->wnLSF = navIn->asUnsignedLong(248,8,1);
      to->dn = navIn->asUnsignedLong(256,8,1);
      to->deltatLSF = navIn->asLong(270,8,1);
         // adjust week numbers to full week
      gpstk::GPSWeekSecond ws(p0->timeStamp);
      long refWeek = ws.week;
      to->wnt = timeAdjust8BitWeekRollover(to->wnt, refWeek);
      to->wnLSF = timeAdjust8BitWeekRollover(to->wnLSF, refWeek);
         // return results.
         //cerr << "add page 56 time offset" << endl;
      navOut.push_back(p0);
      return true;
   }
#endif

   void PNBGPSCNavDataFactory ::
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

} // namespace gpstk
