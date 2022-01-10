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

/// @file Rinex3NavData.cpp
/// Encapsulates RINEX 3 Navigation data.

#include "Rinex3NavData.hpp"

#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "BDSWeekSecond.hpp"
#include "QZSWeekSecond.hpp"
#include "TimeString.hpp"
#include "GNSSconstants.hpp"
#include "StringUtils.hpp"

namespace gnsstk
{
   using namespace StringUtils;
   using namespace std;

   Rinex3NavData ::
   Rinex3NavData()
         : time(CommonTime::BEGINNING_OF_TIME), PRNID(-1), fitint(4),
           xmitTime(0), weeknum(0), accuracy(0), health(0), codeflgs(0),
           L2Pdata(0), IODC(0), IODE(0), TauN(0), GammaN(0), MFTraw(0),
           MFtime(0), freqNum(0), ageOfInfo(0), datasources(0), IODnav(0),
           accCode(0), IODN(0), Toc(0), af0(0), af1(0), af2(0), Tgd(0), Tgd2(0),
           Cuc(0), Cus(0), Crc(0), Crs(0), Cic(0), Cis(0), Toe(0), M0(0), dn(0),
           ecc(0), Ahalf(0), OMEGA0(0), i0(0), w(0), OMEGAdot(0), idot(0),
           px(0), py(0), pz(0), vx(0), vy(0), vz(0), ax(0), ay(0), az(0)
   {}


   // Create from a RINEX version 2 RinexNavData (for backward compatibility)
   Rinex3NavData::Rinex3NavData(const RinexNavData& rnd)
   {
      // Epoch
      time = rnd.time;
      satSys = "G";
      PRNID = rnd.PRNID;
      sat = RinexSatID(PRNID,SatelliteSystem::GPS);
      xmitTime = rnd.getXmitWS().sow;
      weeknum = rnd.getXmitWS().week;
      accuracy = rnd.accuracy;
      health = rnd.health;

      // flags
      codeflgs = rnd.codeflgs;
      L2Pdata = rnd.L2Pdata;
      IODC = rnd.IODC;
      IODE = rnd.IODE;

      // clock
      Toc = rnd.getTocWS().sow;
      af0 = rnd.af0;
      af1 = rnd.af1;
      af2 = rnd.af2;
      Tgd = rnd.Tgd;
      Tgd2 = 0.0;

      // perturbations
      Cus = rnd.Cus;
      Crc = rnd.Crc;
      Crs = rnd.Crs;
      Cic = rnd.Cic;
      Cis = rnd.Cis;

      // Orbit parameters
      Toe = rnd.Toe;
      M0 = rnd.M0;
      dn = rnd.dn;
      ecc = rnd.ecc;
      Ahalf = rnd.Ahalf;
      OMEGA0 = rnd.OMEGA0;
      i0 = rnd.i0;
      w = rnd.w;
      OMEGAdot = rnd.OMEGAdot;
      idot = rnd.idot;
      fitint = rnd.fitint;
   }


   // Deprecated
   // This routine uses EngEphemeris, so is for GPS data only.
   // The comments about GPS v. Galileo next to each elements are just notes
   // from sorting out the ICDs in the RINEX 3 documentation. Please leave
   // them there until we add a routine for handling GalRecord or similar.
   Rinex3NavData::Rinex3NavData(const EngEphemeris& ee) // GPS only
   {
      // epoch info
      satSys = ee.getSatSys();
      PRNID  = ee.getPRNID();
      sat    = RinexSatID(PRNID,SatelliteSystem::GPS);
      time   = ee.getEpochTime();

      Toc     = ee.getToc();
      xmitTime = fixSF1xmitSOW(ee.getHOWTime(1));
      weeknum = ee.getFullWeek();

      accuracy = ee.getAccuracy();
      health   = ee.getHealth();

      // GPS or Galileo data

      af0 = ee.getAf0(); // GPS and Galileo only
      af1 = ee.getAf1(); // GPS and Galileo only
      af2 = ee.getAf2(); // GPS and Galileo only

      Crs = ee.getCrs(); // GPS and Galileo only
      dn  = ee.getDn();  // GPS and Galileo only
      M0  = ee.getM0();  // GPS and Galileo only

      Cuc   = ee.getCuc();   // GPS and Galileo only
      ecc   = ee.getEcc();   // GPS and Galileo only
      Cus   = ee.getCus();   // GPS and Galileo only
      Ahalf = ee.getAhalf(); // GPS and Galileo only

      Toe    = ee.getToe();    // GPS and Galileo only
      Cic    = ee.getCic();    // GPS and Galileo only
      OMEGA0 = ee.getOmega0(); // GPS and Galileo only
      Cis    = ee.getCis();    // GPS and Galileo only

      i0       = ee.getI0();       // GPS and Galileo only
      Crc      = ee.getCrc();      // GPS and Galileo only
      w        = ee.getW();        // GPS and Galileo only
      OMEGAdot = ee.getOmegaDot(); // GPS and Galileo only

      idot = ee.getIDot(); // GPS and Galileo only

      // GPS-only data

      IODE = ee.getIODE(); // GPS only

      codeflgs = ee.getCodeFlags(); // GPS only
      L2Pdata  = ee.getL2Pdata();   // GPS only

      Tgd  = ee.getTgd();  // GPS only
      IODC = ee.getIODC(); // GPS only

      fitint = ee.getFitInterval(); // GPS only
   }  // End of 'Rinex3NavData::Rinex3NavData(const EngEphemeris& ee)'


      /* This function retrieves a RINEX 3 NAV record from the given
       *  FFStream.
       *  If an error is encountered in reading from the stream, the stream
       *  is returned to its original position and its fail-bit is set.
       *  @throw StringException when a StringUtils function fails.
       *  @throw FFStreamError when exceptions(failbit) is set and a read
       *          or formatting error occurs. This also resets the stream
       *          to its pre-read position.
       */
   void Rinex3NavData::reallyGetRecord(FFStream& ffs)
   {

      try
      {
         Rinex3NavStream& strm = dynamic_cast<Rinex3NavStream&>(ffs);

         // If the header hasn't been read, read it...
         if (!strm.headerRead)
         {
            try
            {
               strm >> strm.header;
            }
            catch(std::exception& e)
            {
               FFStreamError fse(string("std::exception reading header ") +
                                 e.what());
               GNSSTK_THROW(fse);
            }
            catch(FFStreamError& fse)
            {
               GNSSTK_RETHROW(fse);
            }
         }

         // get the first line, the epoch line
         getPRNEpoch(strm);

         // get 3 data records
         for(int i=1; i<=3; i++) getRecord(i, strm);

         // SBAS and GLO only have 3 records
         if (satSys == "S" || satSys == "R") return;

         // GPS GAL QZSS BDS have 7 records, get 4-7
         if (satSys == "G" || satSys == "E" || satSys == "J" || satSys == "C" ||
             satSys == "I")
         {
            for(int i=4; i<=7; i++)
            {
               getRecord(i, strm);
            }
         }
      }
      catch(std::exception& e)
      {
         FFStreamError fse(string("std::exception: ") + e.what());
         GNSSTK_THROW(fse);
      }
      catch(FFStreamError& fse)
      {
         GNSSTK_RETHROW(fse);
      }
      catch(StringException& se)
      {
         GNSSTK_RETHROW(se);
      }

   }  // End of method 'Rinex3NavData::reallyGetRecord(FFStream& ffs)'


      // Outputs the record to the FFStream \a s.
   void Rinex3NavData::reallyPutRecord(FFStream& ffs) const
   {
      try
      {
         Rinex3NavStream& strm = dynamic_cast<Rinex3NavStream&>(ffs);

         putPRNEpoch(strm);

         // put 3 data records
         for(int i=1; i<=3; i++)
         {
            putRecord(i, strm);
         }

         // SBAS and GLO only have 3 records
         if (satSys == "S" || satSys == "R")
         {
            return;
         }

         // GPS QZS BDS and GAL have 7 records, put 4-7
         if (satSys == "G" || satSys == "C" || satSys == "E" || satSys == "J")
         {
            for(int i=4; i<=7; i++)
            {
               putRecord(i, strm);
            }
         }
      }
      catch(std::exception& e)
      {
         FFStreamError fse(string("std::exception: ") + e.what());
         GNSSTK_THROW(fse);
      }
      catch(FFStreamError& fse)
      {
         GNSSTK_RETHROW(fse);
      }
      catch(StringException& se)
      {
         GNSSTK_RETHROW(se);
      }

   }  // End of method 'Rinex3NavData::reallyPutRecord(FFStream& ffs)'

      // A debug output function.
      // Prints the PRN id and the IODC for this record.
   void Rinex3NavData::dump(ostream& s) const
   {
      s << "Rinex3NavData dump: "
         << satSys << setfill('0') << setw(2) << PRNID << setfill(' ')
         << printTime(time, " TOC %Y/%02m/%02d %02H:%02M:%02S")
         << fixed << setprecision(3)
         << " wk " << weeknum << " xmit " << xmitTime << " Toe " << Toe << endl;
      s << " Toc " << Toc << scientific << setprecision(12)
         << " af0 " << af0 << " af1 " << af1 << " af2 " << af2
         << " Tgd " << Tgd << " Tgd2 " << Tgd2 << endl;

      s << " M0 " << M0 << " Ecc " << ecc << " sqrtA " << Ahalf << " OM "
        << OMEGA0 << endl;
      s << " i0 " << i0 << " om " << w << " dOMdt " << OMEGAdot << " didt "
        << idot << endl;
      s << " Cuc " << Cuc << " Cus " << Cus << " Crc " << Crc << " Crs " << Crs
        << " Cic " << Cic << " Cis " << Cis << endl;

      if (satSys == "G" || satSys == "J")
      {
            // GPS QZSS
         s << " health " << health << " acc " << accuracy << " fit " << fitint
           << " IODE " << IODE << " IODC " << IODC
           << " codeflags " << codeflgs << " L2P " << L2Pdata << endl;
      }
      //else if (satSys == "R")     // GLONASS
      else if (satSys == "E")
      {
            // Galileo
         s << " IODnav " << IODnav << " datasources " << datasources << endl;
      }
      //else if (satSys == "C")     // BeiDou
   }

   string Rinex3NavData::dumpString(void) const
   {
      ostringstream s;
      s << "RND " << satSys
         << setfill('0') << setw(2) << PRNID << setfill(' ');
      if (satSys == "G" || satSys == "J")
      {
            // GPS or QZSS
         s << " TOE: " << setw(4) << weeknum
           << " " << fixed << setw(10) << setprecision(3) << Toe.val
           << " TOC: " << printTime(time,"%4Y %02m %02d %02H %02M %06.3f %P")
           << " xmitTime: " << setw(6) << xmitTime
           << " IODE/C: " << int(IODE) << "/" << int(IODC)
           << " hlth: " << health
           << " cflgs: " << codeflgs << " L2P: " << L2Pdata
           << " fit: " << fitint.val;
      }
      else if (satSys == "R")
      {
            // GLONASS
         s << " freq: " << setw(2) << freqNum
           << " hlth: " << setw(2) << health
           << " " << printTime(time,"%4Y %02m %02d %02H %02M %06.3f")
           << " MFtime: " << setw(6) << MFtime
           << " TauN: " << scientific << setw(19) << setprecision(12)
           << TauN.val
           << " GammaN: " << setw(19) << GammaN.val
           << " AOI: " << fixed << setprecision(2) << setw(4) << ageOfInfo.val;
      }
      else if (satSys == "S")
      {
            // Geosync (SBAS)
         s << " URAm: " << setw(2) << freqNum
           << " hlth: " << setw(2) << health
           << " " << printTime(time,"%4Y %02m %02d %02H %02M %06.3f")
           << " MFtime: " << setw(6) << MFtime
           << " aGf0: " << scientific << setw(19) << setprecision(12)
           << TauN.val
           << " aGf1: " << setw(19) << GammaN.val
           << " IODN " << fixed << setprecision(2) << setw(4) << ageOfInfo.val;
      }
      else if (satSys == "E")
      {
            // Galileo
         s << " TOE: " << setw(4) << weeknum
           << " " << fixed << setw(10) << setprecision(3) << Toe.val
           << " TOC: " << printTime(time,"%4Y %02m %02d %02H %02M %06.3f %P")
           << " xmitTime: " << setw(6) << xmitTime
           << " IODnav: " << int(IODnav) << " hlth: " << health
           << " datasources " << datasources;
      }
      else if (satSys == "C")
      {
            // BeiDou
         s << " TOE: " << setw(4) << weeknum
           << " " << fixed << setw(10) << setprecision(3) << Toe.val
           << " TOC: " << printTime(time,"%4Y %02m %02d %02H %02M %06.3f %P")
           << " xmitTime: " << setw(6) << xmitTime
           << " IODE/C: " << int(IODE) << "/" << int(IODC);
      }
      else
      {
         s << " (unknown system: " << satSys << ")";
      }

      return s.str();
   }  // End of method 'Rinex3NavData::asString

      // Deprecated; use GPSEphemeris.
      // Converts this Rinex3NavData to an EngEphemeris object.
   Rinex3NavData::operator EngEphemeris() const throw()
   {
      EngEphemeris ee;

      // There's no TLM word in Rinex3NavData, so it's set to 0.
      // Likewise, there's no AS alert or tracker.
      // Also, in RINEX, the accuracy is in meters, and setSF1 expects
      // the accuracy flag.  We'll give it zero and pass the accuracy
      // separately via the setAccuracy() method.

      ee.tlm_message[0] = 0;
      ee.tlm_message[1] = 0;
      ee.tlm_message[2] = 0;
      ee.HOWtime[0] = xmitTime + 6;       // GPS standard specifies
      ee.HOWtime[1] = ee.HOWtime[0] + 6;  // how the transmit time
      ee.HOWtime[2] = ee.HOWtime[1] + 6;  // relates to HOWtime.
      ee.ASalert[0] = 1;               //AS and alert flags set to 1 (default)
      ee.ASalert[1] = 1;
      ee.ASalert[2] = 1;

      ee.weeknum    = weeknum;
      ee.codeflags  = codeflgs;
      ee.health     = health;
      ee.IODC       = short(IODC);
      ee.L2Pdata    = L2Pdata;
      ee.Tgd        = Tgd;
      ee.tracker    = 0;
      ee.PRNID      = PRNID;
      ee.satSys     = satSys;
      bool healthy = false;
      if (health == 0) healthy = true;
      short accFlag = 0; //will be set later.
          //BrcClockCorrection takes a flag, while EngEphemeris takes a double.
      double toc    = Toc;

      double timeDiff =toc - ee.HOWtime[0];
      short epochWeek = ee.weeknum;
      if (timeDiff < -HALFWEEK) epochWeek++;
      else if (timeDiff > HALFWEEK) epochWeek--;

      CommonTime tocCT = GPSWeekSecond(epochWeek, Toc, TimeSystem::GPS);

         // The observation ID has a type of navigation, but the
         // carrier and code types are undefined.  They could be
         // L1/L2 C/A, P, Y,.....
      ObsID obsID(ObservationType::NavMsg, CarrierBand::Undefined, TrackingCode::Undefined);
      ee.bcClock.loadData( satSys, obsID, PRNID, tocCT,
                        accFlag, healthy, af0, af1, af2);

      ee.IODE    = short(IODE);
      ee.fitint  = (fitint > 4) ? 1 : 0;
      //double toe = Toe; //?????

      //Needed for modernized nav quatities
      double A = Ahalf * Ahalf;
      double dndot = 0.0;
      double Adot = 0.0;

      short fitHours = getLegacyFitInterval(ee.IODC, ee.fitint);
      long beginFitSOW = Toe - (fitHours/2)*3600.0;
      long endFitSOW = Toe + (fitHours/2)*3600.0;
      short beginFitWk = ee.weeknum;
      short endFitWk = ee.weeknum;

      if (beginFitSOW < 0)
      {
         beginFitSOW += FULLWEEK;
         beginFitWk--;
      }
      CommonTime beginFit = GPSWeekSecond(beginFitWk, beginFitSOW,
                                          TimeSystem::GPS);
      if (endFitSOW >= FULLWEEK)
      {
         endFitSOW += FULLWEEK;
         endFitWk++;
      }

      CommonTime endFit = GPSWeekSecond(endFitWk, endFitSOW, TimeSystem::GPS);
      CommonTime toeCT = GPSWeekSecond(epochWeek, Toe, TimeSystem::GPS);

      ee.orbit.loadData( satSys, obsID, PRNID, beginFit, endFit, toeCT,
                      accFlag, healthy, Cuc, Cus, Crc, Crs, Cic, Cis,
                      M0, dn, dndot, ecc, A, Ahalf, Adot, OMEGA0, i0,
                      w, OMEGAdot, idot);

      ee.haveSubframe[0] = true;    // need to be true to perform certain EngEphemeris functions
      ee.haveSubframe[1] = true;    // examples: ee.dump(), ee.setAccuracy()
      ee.haveSubframe[2] = true;

      ee.setAccuracy(accuracy);

     return ee;

   }  // End of 'Rinex3NavData::operator EngEphemeris()'


      // Converts the (non-CommonTime) data to an easy list
      // for comparison operators.
   list<double> Rinex3NavData::toList() const
   {

      list<double> l;

      l.push_back(PRNID);
      l.push_back(xmitTime);
      l.push_back(weeknum);
      l.push_back(codeflgs);
      l.push_back(accuracy.val);
      l.push_back(health);
      l.push_back(L2Pdata);
      l.push_back(IODC.val);
      l.push_back(IODE.val);
      l.push_back(Toe.val);
      l.push_back(af0.val);
      l.push_back(af1.val);
      l.push_back(af2.val);
      l.push_back(Tgd.val);
      l.push_back(Cuc.val);
      l.push_back(Cus.val);
      l.push_back(Crc.val);
      l.push_back(Crs.val);
      l.push_back(Cic.val);
      l.push_back(Cis.val);
      l.push_back(Toc);
      l.push_back(M0.val);
      l.push_back(dn.val);
      l.push_back(ecc.val);
      l.push_back(Ahalf.val);
      l.push_back(OMEGA0.val);
      l.push_back(i0.val);
      l.push_back(w.val);
      l.push_back(OMEGAdot.val);
      l.push_back(idot.val);
      l.push_back(fitint.val);

      return l;

   }  // End of method 'Rinex3NavData::toList()'


      /* Generates the PRN/epoch line and outputs it to strm
       *  @param strm RINEX Nav stream
       */
   void Rinex3NavData::putPRNEpoch(Rinex3NavStream& strm) const
   {
      string line;
      CivilTime civtime(time);

      if (strm.header.version >= 3)
      {
            // version 3
         strm << sat.toString() << ' '
              << printTime(civtime, "%4Y %02m %02d %02H %02M %02S");
      }
      else
      {
            // version 2
         strm << setw(2) << PRNID << ' '
              << printTime(civtime, "%02y %2m %2d %2H %2M %4.1f");
      }

      if (satSys == "R" || satSys == "S")
      {
         strm << TauN << GammaN << RNDouble(MFtime);
      }
      else if (satSys == "G" || satSys == "E" || satSys == "J" || satSys == "C")
      {
         strm << af0 << af1 << af2;
      }

      strm << endl;
      strm.lineNumber++;
   }  // End of 'Rinex3NavData::putPRNEpoch(Rinex3NavStream& strm)'


      // Construct and write the nth record after the epoch record
      //  @param int n                 Record number (1-7), for nth record
      //                               after the epoch line.
      //  @param Rinex3NavStream strm  Stream to read from.
   void Rinex3NavData::putRecord(const int& nline, Rinex3NavStream& strm) const
   {

      if (nline < 1 || nline > 7)
      {
         FFStreamError fse(string("Invalid line number ") + asString(nline));
         GNSSTK_THROW(fse);
      }

      try
      {
         if (strm.header.version < 3)
         {
            strm << "   ";
         }
         else
         {
            strm << "    ";
         }


         // Internally (Rinex3NavData), weeknum=week of HOW
         // In RINEX 3 *files*, weeknum is the week of TOE.
         RNDouble wk(weeknum);
         long xmit = xmitTime;
         if (xmit - Toe > HALFWEEK)
         {
            xmit -= FULLWEEK;
            wk++;
         }
         else if (xmit - Toe < -(HALFWEEK))
         {
            xmit += FULLWEEK;
            wk--;
         }

         if (nline == 1)
         {
            if (satSys == "R" || satSys == "S")
            {
                  // GLO and GEO
               strm << px << vx << ax << RNDouble(health);
            }
            else if (satSys == "G" || satSys == "C" || satSys == "J")
            {
                  // GPS,BDS,QZS
               strm << IODE << Crs << dn << M0;
            }
            else if (satSys == "E")
            {
                  // GAL
               strm << IODnav << Crs << dn << M0;
            }
         }
         else if (nline == 2)
         {
            if (satSys == "R" || satSys == "S")
            {
                  // GLO and GEO
               strm << py << vy << ay;
               if (satSys == "R")
                  strm << RNDouble(freqNum);
               else
                  strm << accCode;
            }
            else
            {
                  // GPS,GAL,BDS,QZS
               strm << Cuc << ecc << Cus << Ahalf;
            }
         }
         else if (nline == 3)
         {
            if (satSys == "R" || satSys == "S")
            {
                  // GLO GEO
               strm << pz << vz << az;
               if (satSys == "R")
                  strm << ageOfInfo;
               else                             // GEO
                  strm << IODN;
            }
            else
            {
                  // GPS,GAL,BDS,QZS
               strm << Toe << Cic << OMEGA0 << Cis;
            }
         }

         // SBAS and GLO end here

         else if (nline == 4)
         {
               // GPS,GAL,BDS,QZS
            strm << i0 << Crc << w << OMEGAdot;
         }

         else if (nline == 5)
         {
            if (satSys == "G" || satSys == "J")
            {
                  // GPS QZS
               strm << idot << RNDouble(codeflgs) << wk << RNDouble(L2Pdata);
            }
            else if (satSys == "E")
            {
                  // GAL
               strm << idot << RNDouble(datasources) << wk << RNDouble(0);
            }
            else if (satSys == "C")
            {
                  // BDS
               strm << idot << RNDouble(0) << wk << RNDouble(0);
            }
         }

         else if (nline == 6)
         {
            strm << accuracy << RNDouble(health);

            if (satSys == "G" || satSys == "J")
            {
                  // GPS, QZS
               strm << Tgd << IODC;
            }
            else if (satSys == "E" || satSys == "C")
            {
                  // GAL, BDS
               strm << Tgd << Tgd2;
            }
         }

         else if (nline == 7)
         {
            strm << RNDouble(xmit);
            if (satSys == "G" || satSys == "J")
            {
               strm << fitint;
            }
            else if (satSys == "E")
            {
               ;
            }
            else if (satSys == "C")
            {
               strm << IODC;
            }
         }

         strm << endl;
         strm.lineNumber++;
      }
      catch (std::exception &e)
      {
         FFStreamError err("std::exception: " + string(e.what()));
         GNSSTK_THROW(err);
      }

   }  // End of method 'Rinex3NavData::putRecord(const int& nline,...'


   void Rinex3NavData::getPRNEpoch(Rinex3NavStream& strm)
   {
      try
      {
         int i;
         short yr,mo,day,hr,min;
         double dsec;

         string line;
         while(line.empty()) // ignore blank lines in place of epoch lines
            strm.formattedGetLine(line, true);

         if (strm.header.version >= 3)
         {
            // check for spaces in the right spots...
            if (line[3] != ' ')
               GNSSTK_THROW(FFStreamError("Badly formatted epoch line"));
            for(i = 8; i <= 20; i += 3)
            {
               if (line[i] != ' ')
               {
                  GNSSTK_THROW(FFStreamError("Badly formatted epoch line"));
               }
            }

            satSys = line.substr(0,1);
            PRNID = asInt(line.substr(1,2));
            sat.fromString(line.substr(0,3));

            yr  = asInt(line.substr( 4,4));
            mo  = asInt(line.substr( 9,2));
            day = asInt(line.substr(12,2));
            hr  = asInt(line.substr(15,2));
            min = asInt(line.substr(18,2));
            dsec = asDouble(line.substr(21,2));
         }
         else
         {
               // RINEX 2
            for(i=2; i <= 17; i+=3)
            {
               if (line[i] != ' ')
               {
                  GNSSTK_THROW(FFStreamError("Badly formatted epoch line"));
               }
            }

            satSys = string(1,strm.header.fileSys[0]);
            PRNID = asInt(line.substr(0,2));
            sat.fromString(satSys + line.substr(0,2));

            yr  = asInt(line.substr( 2,3));
            if (yr < 80)
               yr += 100;     // rollover is at 1980
            yr += 1900;
            mo  = asInt(line.substr( 5,3));
            day = asInt(line.substr( 8,3));
            hr  = asInt(line.substr(11,3));
            min = asInt(line.substr(14,3));
            dsec = asDouble(line.substr(17,5));
         }

         // Fix RINEX epochs of the form 'yy mm dd hr 59 60.0'
         short ds = 0;
         if (dsec >= 60.)
         {
            ds = dsec;
            dsec = 0;
         }
         time = CivilTime(yr,mo,day,hr,min,dsec).convertToCommonTime();
         if (ds != 0)
            time += ds;

         // specify the time system based on satellite system
         time.setTimeSystem(TimeSystem::Any);
         if (satSys == "G") time.setTimeSystem(TimeSystem::GPS);
         if (satSys == "R") time.setTimeSystem(TimeSystem::GLO);
         if (satSys == "E") time.setTimeSystem(TimeSystem::GAL);
         if (satSys == "C") time.setTimeSystem(TimeSystem::BDT);
         if (satSys == "J") time.setTimeSystem(TimeSystem::QZS);
         if (satSys == "S") time.setTimeSystem(TimeSystem::GPS);

         // TOC is the clock time
         GPSWeekSecond gws(time);         // sow is system-independent
         Toc = gws.sow;

         if (strm.header.version < 3)
         {
               // Rinex 2.*
            if (satSys == "G")
            {
               af0 = line.substr(22,19);
               af1 = line.substr(41,19);
               af2 = line.substr(60,19);
            }
            else if (satSys == "R" || satSys == "S")
            {
               TauN   =      line.substr(22,19);
               GammaN =      line.substr(41,19);
               MFtime =      RNDouble(line.substr(60,19));
               if (satSys == "R")
               {
                     // make MFtime consistent with R3.02
                  MFtime += int(Toc/86400) * 86400;
               }
            }
         }
         else if (satSys == "G" || satSys == "E" || satSys == "C" ||
                  satSys == "J")
         {
            af0 = line.substr(23,19);
            af1 = line.substr(42,19);
            af2 = line.substr(61,19);
         }
         else if (satSys == "R" || satSys == "S")
         {
            TauN   =      line.substr(23,19);
            GammaN =      line.substr(42,19);
            MFtime =      RNDouble(line.substr(61,19));
         }
      }
      catch (std::exception &e)
      {
         FFStreamError err("std::exception: " + string(e.what()));
         GNSSTK_THROW(err);
      }
   }


   void Rinex3NavData::getRecord(const int& nline, Rinex3NavStream& strm)
   {
      if (nline < 1 || nline > 7)
      {
         FFStreamError fse(string("Invalid line number ") + asString(nline));
         GNSSTK_THROW(fse);
      }

      try
      {
         int n(strm.header.version < 3 ? 3 : 4);
         string line;
         strm.formattedGetLine(line);

         if (nline == 1)
         {
            if (satSys == "G" || satSys == "J" || satSys == "C")
            {
               IODE = line.substr(n,19); n+=19;
               Crs  = line.substr(n,19); n+=19;
               dn   = line.substr(n,19); n+=19;
               M0   = line.substr(n,19);
            }
            else if (satSys == "E")
            {
               IODnav = line.substr(n,19); n+=19;
               Crs    = line.substr(n,19); n+=19;
               dn     = line.substr(n,19); n+=19;
               M0     = line.substr(n,19);
            }
            else if (satSys == "R" || satSys == "S")
            {
               px     =        line.substr(n,19); n+=19;
               vx     =        line.substr(n,19); n+=19;
               ax     =        line.substr(n,19); n+=19;
               health =        RNDouble(line.substr(n,19));
            }
         }

         else if (nline == 2)
         {
            if (satSys == "G" || satSys == "E" || satSys == "J" ||
                satSys == "C")
            {
               Cuc   = line.substr(n,19); n+=19;
               ecc   = line.substr(n,19); n+=19;
               Cus   = line.substr(n,19); n+=19;
               Ahalf = line.substr(n,19);
            }
            else if (satSys == "R" || satSys == "S")
            {
               py      =        line.substr(n,19); n+=19;
               vy      =        line.substr(n,19); n+=19;
               ay      =        line.substr(n,19); n+=19;
               if (satSys == "R")
               {
                  freqNum = RNDouble(line.substr(n,19));
               }
               else                       // GEO
               {
                  accCode = line.substr(n,19);
               }
            }
         }

         else if (nline == 3)
         {
            if (satSys == "G" || satSys == "E" || satSys == "J" ||
                satSys == "C")
            {
               Toe    = line.substr(n,19); n+=19;
               Cic    = line.substr(n,19); n+=19;
               OMEGA0 = line.substr(n,19); n+=19;
               Cis    = line.substr(n,19);
            }
            else if (satSys == "R" || satSys == "S")
            {
               pz        = line.substr(n,19); n+=19;
               vz        = line.substr(n,19); n+=19;
               az        = line.substr(n,19); n+=19;
               if (satSys == "R")
               {
                  ageOfInfo = line.substr(n,19);
               }
               else                       // GEO
               {
                  IODN = line.substr(n,19);
               }
            }
         }

         else if (nline == 4)
         {
            i0       = line.substr(n,19); n+=19;
            Crc      = line.substr(n,19); n+=19;
            w        = line.substr(n,19); n+=19;
            OMEGAdot = line.substr(n,19);
         }

         else if (nline == 5)
         {
            if (satSys == "G" || satSys == "J" || satSys == "C")
            {
               idot     =        line.substr(n,19); n+=19;
               codeflgs = RNDouble(line.substr(n,19)); n+=19;
               weeknum  = RNDouble(line.substr(n,19)); n+=19;
               L2Pdata  = RNDouble(line.substr(n,19));
            }
            else if (satSys == "E")
            {
               idot        =       line.substr(n,19); n+=19;
               datasources =RNDouble(line.substr(n,19)); n+=19;
               weeknum     =RNDouble(line.substr(n,19)); n+=19;
            }
         }

         else if (nline == 6)
         {
            Tgd2 = 0.0;
            if (satSys == "G" || satSys == "J")
            {
               accuracy =       line.substr(n,19); n+=19;
               health   = RNDouble(line.substr(n,19)); n+=19;
               Tgd      =       line.substr(n,19); n+=19;
               IODC     =       line.substr(n,19);
            }
            else if (satSys == "E")
            {
               accuracy =       line.substr(n,19); n+=19;
               health   = RNDouble(line.substr(n,19)); n+=19;
               Tgd      =       line.substr(n,19); n+=19;
               Tgd2     =       line.substr(n,19);
            }
            else if (satSys == "C")
            {
               accuracy =       line.substr(n,19); n+=19;
               health   = RNDouble(line.substr(n,19)); n+=19;
               Tgd      =       line.substr(n,19); n+=19;
               Tgd2     =       line.substr(n,19);
            }
         }

         else if (nline == 7)
         {
            xmitTime = RNDouble(line.substr(n,19)); n+=19;
            if (satSys == "C")
            {
               IODC    =        line.substr(n,19); n+=19;
            }
            else
            {
               fitint  =        line.substr(n,19); n+=19;
            }

            // Some RINEX files have xmitTime < 0.
            while(xmitTime < 0)
            {
               xmitTime += (long)FULLWEEK;
            }

            // In RINEX *files*, weeknum is the week of TOE.
            // Internally (Rinex3NavData), weeknum is week of transmission
            if (xmitTime - Toe > HALFWEEK)
               weeknum--;
            else if (xmitTime - Toe < -HALFWEEK)
               weeknum++;
         }
      }
      catch (std::exception &e)
      {
         FFStreamError err("std::exception: " + string(e.what()));
         GNSSTK_THROW(err);
      }

   }  // end getRecord()


}  // End of namespace gnsstk
