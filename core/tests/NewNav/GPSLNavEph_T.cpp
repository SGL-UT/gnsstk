#include "GPSLNavEph.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::GPSLNavEph::L2Codes e)
   {
      s << static_cast<long>(e);
      return s;
   }
}

class GPSLNavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
};


unsigned GPSLNavEph_T ::
constructorTest()
{
   TUDEF("GPSLNavEph", "GPSLNavEph");
   gpstk::GPSLNavEph obj;
   TUASSERTE(uint32_t, 0, obj.pre2);
   TUASSERTE(uint32_t, 0, obj.pre3);
   TUASSERTE(uint32_t, 0, obj.tlm2);
   TUASSERTE(uint32_t, 0, obj.tlm3);
   TUASSERTE(uint16_t, 0, obj.iodc);
   TUASSERTE(uint16_t, 0, obj.iode);
   TUASSERTE(uint8_t,  0, obj.fitIntFlag);
   TUASSERTE(uint8_t, 0xff, obj.healthBits);
   TUASSERTE(uint8_t, 0, obj.uraIndex);
   TUASSERTFE(0.0, obj.tgd);
   TUASSERTE(bool, false, obj.asFlag2);
   TUASSERTE(bool, false, obj.asFlag3);
   TUASSERTE(bool, false, obj.alert2);
   TUASSERTE(bool, false, obj.alert3);
   TUASSERTE(gpstk::GPSLNavEph::L2Codes, gpstk::GPSLNavEph::L2Codes::Invalid1,
             obj.codesL2);
   TUASSERTE(bool, false, obj.L2Pdata);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavEph_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavEph", "getUserTime");
   gpstk::GPSLNavEph obj;
   obj.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // ephemeris = 3 subframes * 6 seconds = 18 seconds.
   exp = exp + 18.0;
   TUASSERTE(gpstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavEph_T ::
fixFitTest()
{
   TUDEF("GPSLNavEph", "fixFit");
   gpstk::GPSLNavEph obj;
      /** @todo When using navdmp, it indicated the begin valid time
       * was 597600 SOW.  The fixFit() method, which was taken from
       * OrbElemRinex::computeBeginValid, does not make this 2-hour
       * boundary adjustment when the Toe isn't already aligned on
       * 2-hour boundaries.  So for now we use 603360 SOW for the
       * test, since that matches expected behavior for this code. */
   gpstk::GPSWeekSecond beginExpWS2(2121, 603360), endExpWS2(2122, 7200);
   gpstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   obj.iodc = 0xd;
   obj.fitIntFlag = 0;
   obj.Toe = obj.Toc = gpstk::GPSWeekSecond(2122, 0);
   obj.xmitTime = gpstk::GPSWeekSecond(2121,603360);
   TUCATCH(obj.fixFit());
   TUASSERTE(gpstk::CommonTime, beginExp2, obj.beginFit);
   TUASSERTE(gpstk::CommonTime, endExp2, obj.endFit);
      //obj.dump(std::cerr, gpstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GPSLNavEph_T ::
validateTest()
{
   TUDEF("GPSLNavData", "validate");
   gpstk::GPSLNavEph obj;
   TUASSERTE(bool, true, obj.validate());
   obj.pre = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre = 0x8b; // this is valid
   obj.pre2 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre2 = 0x8b; // this is valid
   obj.pre3 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre3 = 0x8b; // this is valid (all 3 preambles == 8b)
   TUASSERTE(bool, true, obj.validate());
   TURETURN();
}


int main()
{
   GPSLNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
