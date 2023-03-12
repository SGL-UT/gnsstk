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

/**
 * @file Rinex3NavHeader.cpp
 * Encapsulate header of RINEX 3 navigation file, including RINEX 2
 * compatibility.
 */

#include "StringUtils.hpp"
#include "GNSSconstants.hpp"
#include "SystemTime.hpp"
#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "BDSWeekSecond.hpp"
#include "TimeString.hpp"
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavStream.hpp"

#include <iostream>
#include <set>
#include <cmath>
#include <unordered_map>
#include <sstream>

using namespace gnsstk::StringUtils;
using namespace std;

namespace gnsstk
{

   static const unordered_map<IonoCorr::CorrType, IonoCorr::CorrType>
   dependent_pairs
   {
      {IonoCorr::CorrType::GPSA, IonoCorr::CorrType::GPSB},
      {IonoCorr::CorrType::QZSA, IonoCorr::CorrType::QZSB},
      {IonoCorr::CorrType::BDSA, IonoCorr::CorrType::BDSB},
      {IonoCorr::CorrType::IRNA, IonoCorr::CorrType::IRNB},
      {IonoCorr::CorrType::GPSB, IonoCorr::CorrType::GPSA},
      {IonoCorr::CorrType::QZSB, IonoCorr::CorrType::QZSA},
      {IonoCorr::CorrType::BDSB, IonoCorr::CorrType::BDSA},
      {IonoCorr::CorrType::IRNB, IonoCorr::CorrType::IRNA}
   };

   const string Rinex3NavHeader::stringVersion     = "RINEX VERSION / TYPE";
   const string Rinex3NavHeader::stringRunBy       = "PGM / RUN BY / DATE";
   const string Rinex3NavHeader::stringComment     = "COMMENT";
   const string Rinex3NavHeader::stringIonoCorr    = "IONOSPHERIC CORR";
   const string Rinex3NavHeader::stringTimeSysCorr = "TIME SYSTEM CORR";
   const string Rinex3NavHeader::stringLeapSeconds = "LEAP SECONDS";
      //R2.10GLO
   const string Rinex3NavHeader::stringCorrSysTime = "CORR TO SYSTEM TIME";
      //R2.11GPS
   const string Rinex3NavHeader::stringDeltaUTC    = "DELTA-UTC: A0,A1,T,W";
      //R2.11GEO
   const string Rinex3NavHeader::stringDUTC        = "D-UTC A0,A1,T,W,S,U";
      //R2.11
   const string Rinex3NavHeader::stringIonAlpha    = "ION ALPHA";
      //R2.11
   const string Rinex3NavHeader::stringIonBeta     = "ION BETA";
   const string Rinex3NavHeader::stringEoH         = "END OF HEADER";


   IonoCorr ::
   IonoCorr()
         : type(Unknown),
           param{ FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                                  'D', StringUtils::FFSign::NegOnly,
                                  StringUtils::FFAlign::Right),
         FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                         'D', StringUtils::FFSign::NegOnly,
                         StringUtils::FFAlign::Right),
         FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                         'D', StringUtils::FFSign::NegOnly,
                         StringUtils::FFAlign::Right),
         FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                         'D', StringUtils::FFSign::NegOnly,
                         StringUtils::FFAlign::Right) }
   {
   }


   IonoCorr ::
   IonoCorr(std::string str)
         : type(Unknown), // just in case someone breaks fromString
           param{ FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                                  'D', StringUtils::FFSign::NegOnly,
                                  StringUtils::FFAlign::Right),
         FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                         'D', StringUtils::FFSign::NegOnly,
                         StringUtils::FFAlign::Right),
         FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                         'D', StringUtils::FFSign::NegOnly,
                         StringUtils::FFAlign::Right),
         FormattedDouble(0, StringUtils::FFLead::Decimal, 4, 2, 12,
                         'D', StringUtils::FFSign::NegOnly,
                         StringUtils::FFAlign::Right) }
   {
      this->fromString(str);
   }


   std::string IonoCorr ::
   asString() const noexcept
   {
      switch(type)
      {
         case GAL:  return std::string("GAL");
         case GPSA: return std::string("GPSA");
         case GPSB: return std::string("GPSB");
         case QZSA: return std::string("QZSA");
         case QZSB: return std::string("QZSB");
         case BDSA: return std::string("BDSA");
         case BDSB: return std::string("BDSB");
         case IRNA: return std::string("IRNA");
         case IRNB: return std::string("IRNB");
         case Unknown:
            break;
      }
      return std::string("ERROR");
   }


   void IonoCorr ::
   fromString(const std::string str)
   {
      static const std::unordered_map<std::string, IonoCorr::CorrType> from_string
      {
         {"GAL", CorrType::GAL},
         {"GPSA", CorrType::GPSA},
         {"GPSB", CorrType::GPSB},
         {"QZSA", CorrType::QZSA},
         {"QZSB", CorrType::QZSB},
         {"BDSA", CorrType::BDSA},
         {"BDSB", CorrType::BDSB},
         {"IRNA", CorrType::IRNA},
         {"IRNB", CorrType::IRNB}
      };
      std::string key = gnsstk::StringUtils::upperCase(str);

      if (!from_string.contains(key))
         GNSSTK_THROW(Exception("Unknown ionospheric correction parameter: " + str));

      type = from_string.at(key);
   }


   bool IonoCorr ::
   operator==(const IonoCorr& right)
      const
   {
         // Epsilon is chosen based on data format in RINEX which is
         // %12.4f.  Rounding errors can be expected up to .0001
      return ((type == right.type) &&
              (fabs(param[0] - right.param[0]) < 0.0001) &&
              (fabs(param[1] - right.param[1]) < 0.0001) &&
              (fabs(param[2] - right.param[2]) < 0.0001) &&
              (fabs(param[3] - right.param[3]) < 0.0001));
   }


   bool IonoCorr ::
   operator<(const IonoCorr& right)
      const
   {
      if (type < right.type) return true;
      if (type > right.type) return false;
      if (param[0] < right.param[0]) return true;
      if (param[0] > right.param[0]) return false;
      if (param[1] < right.param[1]) return true;
      if (param[1] > right.param[1]) return false;
      if (param[2] < right.param[2]) return true;
      if (param[2] > right.param[2]) return false;
      if (param[3] < right.param[3]) return true;
      return false;
   }


   void Rinex3NavHeader::reallyGetRecord(FFStream& ffs)
   {
      Rinex3NavStream& strm = dynamic_cast<Rinex3NavStream&>(ffs);

         // if already read, just return
      if(strm.headerRead == true) return;

      int i;
      valid = 0;

         // clear out anything that was unsuccessfully read first
      commentList.clear();

      while (!(valid & validEoH))
      {
         string line;
         strm.formattedGetLine(line);
         stripTrailing(line);
         if(line.length() == 0) continue;
         else if(line.length() < 60 || line.length() > 80)
         {
            FFStreamError e("Invalid line length");
            GNSSTK_THROW(e);
         }

         string thisLabel(line, 60, 20);

            // following is huge if else else ... endif for each record type
         if(thisLabel == stringVersion)
         {
               // "RINEX VERSION / TYPE"
            version = asDouble(line.substr( 0,20));

            fileType = strip(line.substr(20,20));
            if(version >= 3)
            {                        // ver 3
               if(fileType[0] != 'N' && fileType[0] != 'n')
               {
                  FFStreamError e("File type is not NAVIGATION: " + fileType);
                  GNSSTK_THROW(e);
               }
               fileSys = strip(line.substr(40,20));   // not in ver 2
               setFileSystem(fileSys);
               fileType = "N: GNSS NAV DATA";
            }
            else
            {                                    // ver 2
               if(fileType[0] == 'N' || fileType[0] == 'n')
                  setFileSystem("G");
               else if(fileType[0] == 'G' || fileType[0] == 'g')
                  setFileSystem("R");
               else if(fileType[0] == 'H' || fileType[0] == 'h')
                  setFileSystem("S");
               else
               {
                  FFStreamError e("Version 2 file type is invalid: " +
                                  fileType);
                  GNSSTK_THROW(e);
               }
               fileType = "N: GPS NAV DATA";
            }
            valid |= validVersion;
         }
         else if(thisLabel == stringRunBy)
         {
               // "PGM / RUN BY / DATE"
            fileProgram = strip(line.substr( 0,20));
            fileAgency = strip(line.substr(20,20));
               // R2 may not have 'UTC' at end
            date = strip(line.substr(40,20));
            valid |= validRunBy;
         }
         else if(thisLabel == stringComment)
         {
               // "COMMENT"
            commentList.push_back(stripTrailing(line.substr(0,60)));
            valid |= validComment;
         }
         else if(thisLabel == stringIonAlpha)
         {
               // GPS alpha "ION ALPHA"  R2.11
            IonoCorr ic("GPSA");
            for(i = 0; i < 4; i++)
               ic.param[i] = line.substr(2 + 12*i, 12);
            mapIonoCorr[ic.type] = ic;
            if (mapIonoCorr.contains(IonoCorr::CorrType::GPSB))
               valid |= validIonoCorr;
         }
         else if(thisLabel == stringIonBeta)
         {
               // GPS beta "ION BETA"  R2.11
            IonoCorr ic("GPSB");
            for(i = 0; i < 4; i++)
               ic.param[i] = line.substr(2 + 12*i, 12);
            mapIonoCorr[ic.type] = ic;
            if (mapIonoCorr.contains(IonoCorr::CorrType::GPSA))
               valid |= validIonoCorr;
         }
         else if(thisLabel == stringIonoCorr)
         {
               // "IONOSPHERIC CORR"
            IonoCorr ic;
            try
            {
               ic.fromString(strip(line.substr(0,4)));
            }
            catch(Exception& e)
            {
               FFStreamError fse(e.what());
               GNSSTK_THROW(e);
            }
            for(i = 0; i < 4; i++)
               ic.param[i] = line.substr(5 + 12*i, 12);
            mapIonoCorr[ic.type] = ic;
            if (!dependent_pairs.contains(ic.type)
                || mapIonoCorr.contains(dependent_pairs.at(ic.type)))
               valid |= validIonoCorr;
         }
         else if(thisLabel == stringDeltaUTC)
         {
               // "DELTA-UTC: A0,A1,T,W" R2.11 GPS
            TimeSystemCorrection tc("GPUT");
            tc.A0 = RNDouble(line.substr(3,19));
            tc.A1 = RNDouble(line.substr(22,19));
            tc.refTime = GPSWeekSecond(asInt(line.substr(50,9)),
                                       asDouble(line.substr(41,9)),
                                       TimeSystem::GPS);
            tc.geoProvider = string("    ");
            tc.geoUTCid = 0;

            mapTimeCorr[tc.asString4()] = tc;
            valid |= validTimeSysCorr;
         }
            // R2.11 but Javad uses it in 3.01
         else if(thisLabel == stringCorrSysTime)
         {
               // "CORR TO SYSTEM TIME"  R2.10 GLO
            TimeSystemCorrection tc("GLUT");
            tc.refTime = CivilTime(asInt(line.substr(0,6)),
                                   asInt(line.substr(6,6)),
                                   asInt(line.substr(12,6)), 0, 0, 0.0,
                                   TimeSystem::GLO);
            tc.A0 = -RNDouble(line.substr(21,19));    // -TauC
            tc.A1 = 0.0;
            tc.geoProvider = string("    ");
            tc.geoUTCid = 3;                          // UTC(SU)

            mapTimeCorr[tc.asString4()] = tc;
            valid |= validTimeSysCorr;
         }
         else if(thisLabel == stringDUTC)
         {
               // "D-UTC A0,A1,T,W,S,U"  // R2.11 GEO
            TimeSystemCorrection tc("SBUT");
            tc.A0 = RNDouble(line.substr(0,19));
            tc.A1 = RNDouble(line.substr(19,19));
            tc.refTime = GPSWeekSecond(asInt(line.substr(45,5)),
                                       asDouble(line.substr(38,7)),
                                       TimeSystem::GPS);
            tc.geoProvider = line.substr(51,5);
            tc.geoUTCid = asInt(line.substr(57,2));

            mapTimeCorr[tc.asString4()] = tc;
            valid |= validTimeSysCorr;
         }
         else if(thisLabel == stringTimeSysCorr)
         {
               // R3 only // "TIME SYSTEM CORR"
            TimeSystemCorrection tc;
            try
            {
               tc.fromString(strip(line.substr(0,4)));
            }
            catch(Exception& e)
            {
               FFStreamError fse(e.what());
               GNSSTK_THROW(e);
            }

            tc.A0 = RNDouble(line.substr(5,17));
            tc.A1 = RNDouble(line.substr(22,16));
            if ((tc.type == TimeSystemCorrection::BDUT) ||
                (tc.type == TimeSystemCorrection::BDGP))
            {
                  // BDT time stamps are actually referenced to BDT week epoch
               tc.refTime = BDSWeekSecond(asInt(line.substr(45,5)),
                                          asDouble(line.substr(38,7)));
            }
            else
            {
                  // Everything else (including Galileo, oddly) is
                  // referenced to the GPS epoch.
               tc.refTime = GPSWeekSecond(asInt(line.substr(45,5)),
                                          asDouble(line.substr(38,7)));
            }
            tc.geoProvider = strip(line.substr(51,6));
            tc.geoUTCid = asInt(line.substr(57,2));
            tc.fixTimeSystem();

            mapTimeCorr[tc.asString4()] = tc;
            valid |= validTimeSysCorr;
         }
         else if(thisLabel == stringLeapSeconds)
         {
               // "LEAP SECONDS"
            leapSeconds = asInt(line.substr(0,6));
            leapDelta = asInt(line.substr(6,6));      // R3 only
            leapWeek = asInt(line.substr(12,6));      // R3 only
            leapDay = asInt(line.substr(18,6));       // R3 only
            valid |= validLeapSeconds;
         }
         else if(thisLabel == stringEoH)
         {
               // "END OF HEADER"
            valid |= validEoH;
         }
         else
         {
            GNSSTK_THROW(FFStreamError("Unknown header label >" + thisLabel +
                                      "< at line " +
                                      asString<size_t>(strm.lineNumber)));
         }
      }

      unsigned long allValid;
      if(version >= 3.0)
         allValid = allValid3;
      else if(version >= 2 && version < 3)
         allValid = allValid2;
      else
      {
         FFStreamError e("Unknown or unsupported RINEX version "+
                         asString(version,2));
         GNSSTK_THROW(e);
      }

      if((allValid & valid) != allValid)
      {
         FFStreamError e("Incomplete or invalid header");
         GNSSTK_THROW(e);
      }

      strm.header = *this;
      strm.headerRead = true;

   } // end of reallyGetRecord

      //-----------------------------------------------------------------------
   void Rinex3NavHeader::reallyPutRecord(FFStream& ffs) const
   {
      Rinex3NavStream& strm = dynamic_cast<Rinex3NavStream&>(ffs);

      strm.header = (*this);

      int j;
      unsigned long allValid;
      if(version >= 3.0)
         allValid = allValid3;
      else if(version >= 2 && version < 3)
         allValid = allValid2;
      else
      {
         FFStreamError err("Unknown RINEX version: " + asString(version,4));
         GNSSTK_THROW(err);
      }

      if((valid & allValid) != allValid)
      {
         FFStreamError err("Incomplete or invalid header.");
         GNSSTK_THROW(err);
      }

      if(valid & validVersion)
      {
            // "RINEX VERSION / TYPE"
         ffs << right << setw(10) << setprecision(2) << fixed << version
             << setw(10) << ' ' << left << setw(20) << fileType;

         if (version >= 3)
            ffs << left << setw(20) << fileSys;
         else
            ffs << setw(20) << ' ';
         ffs << stringVersion << endlpp;
      }

      if(valid & validRunBy)
      {
            // "PGM / RUN BY / DATE"
         strm << left << setw(20) << fileProgram
              << left << setw(20) << fileAgency;
         SystemTime sysTime;
         if(version < 3)
         {
            strm << left << setw(20)
                 << printTime(sysTime,"%02m/%02d/%04Y %02H:%02M:%02S");
         }
         else
         {
            strm << left << setw(20)
                 << printTime(sysTime,"%04Y%02m%02d %02H%02M%02S UTC");
         }
         strm << stringRunBy << endlpp;
      }

      if(valid & validComment)
      {
            // "COMMENT"
         for (unsigned i = 0; i < commentList.size(); i++)
         {
            strm << left << setw(60) << commentList[i] << stringComment
                 << endlpp;
         }
      }

      if (valid & validIonoCorr)
      {
            // "IONOSPHERIC CORR"
         for (auto const& [type, value] : mapIonoCorr)
         {
            std::stringstream data;

            data << std::left << std::setw(5) << value.asString();
            for (auto const& parameter : value.param)
               data << parameter;
            strm << std::setw(60) << data.str() << stringIonoCorr << endlpp;
         }
      }

      if(valid & validTimeSysCorr)
      {
            // "TIME SYSTEM CORR"
         map<string,TimeSystemCorrection>::const_iterator it;
         for(it=mapTimeCorr.begin(); it != mapTimeCorr.end(); ++it)
         {
            const TimeSystemCorrection& tc(it->second);
            if(version >= 3)
            {
               strm << tc.asString4() << " "
                    << FormattedDouble(tc.A0, StringUtils::FFLead::Decimal, 10,
                                       2, 17, 'D', StringUtils::FFSign::NegOnly,
                                       StringUtils::FFAlign::Right)
                    << FormattedDouble(tc.A1, StringUtils::FFLead::Decimal, 9,
                                       2, 16, 'D', StringUtils::FFSign::NegOnly,
                                       StringUtils::FFAlign::Right);
               if ((tc.type == TimeSystemCorrection::BDUT) ||
                   (tc.type == TimeSystemCorrection::BDGP))
               {
                     // BDT time stamps are actually referenced to BDT
                     // week epoch
                  BDSWeekSecond ws(tc.refTime);
                  strm << right << setw(7) << (unsigned)ws.sow
                       << right << setw(5) << ws.week;
               }
               else
               {
                     // Everything else (including Galileo, oddly) is
                     // referenced to the GPS epoch.
                  GPSWeekSecond ws(tc.refTime);
                  strm << right << setw(7) << (unsigned)ws.sow
                       << right << setw(5) << ws.week;
               }
                  // For SBAS this should be one of the three
                  // providers (EGNOS, WAAS or MSAS).  For others it
                  // should be system and number Snn of GNSS satellite
                  // broadcasting the time system difference.
               if (!tc.geoProvider.empty())
               {
                  strm << ' ' << right << setw(5) << tc.geoProvider << ' ';
               }
               else
               {
                  strm << setw(7) << ' ';
               }
               strm << right << setw(2) << tc.geoUTCid << ' '
                    << stringTimeSysCorr;
            }
            else
            {
               if(tc.asString4() == "GPUT")
               {
                     // "DELTA-UTC: A0,A1,T,W" R2.11 GPS
                  GPSWeekSecond ws(tc.refTime);
                  strm << "   " << RNDouble(tc.A0) << RNDouble(tc.A1)
                       << right << setw(9) << (unsigned)ws.sow
                       << right << setw(9) << ws.week << " "
                       << stringDeltaUTC;
               }
               else if(tc.asString4() == "GLGP")
               {
                     // "CORR TO SYSTEM TIME" R2.10 GLO
                  CivilTime civ(tc.refTime);
                  strm << right << setw(6) << civ.year
                       << right << setw(6) << civ.month
                       << right << setw(6) << civ.day
                       << RNDouble(tc.A0) << setw(23) << ' '
                       << stringCorrSysTime;
               }
               else if(tc.asString4() == "SBUT")
               {
                     // "D-UTC A0,A1,T,W,S,U" R2.11 GEO
                  GPSWeekSecond ws(tc.refTime);
                  strm << RNDouble(tc.A0) << RNDouble(tc.A1)
                       << right << setw(7) << (unsigned)ws.sow
                       << right << setw(5) << ws.week
                       << right << setw(6) << tc.geoProvider << " "
                       << right << setw(2) << tc.geoUTCid << " "
                       << stringDUTC;
               }
            }

            strm << endlpp;
         }
      }

      if(valid & validLeapSeconds)
      {
            // "LEAP SECONDS"
         strm << right << setw(6) << leapSeconds;
         if(version >= 3)
         {                                    // ver 3
            strm << right << setw(6) << leapDelta
                 << right << setw(6) << leapWeek
                 << right << setw(6) << leapDay
                 << setw(36) << ' ';
         }
         else // ver 2
         {
            strm << setw(54) << ' ';
         }
         strm << stringLeapSeconds << endlpp;
      }

      if(valid & validEoH)
      {
            // "END OF HEADER"
         strm << setw(60) << ' ' << stringEoH << endlpp;
      }

   } // end of reallyPutRecord

      //-----------------------------------------------------------------------

   void Rinex3NavHeader::dump(ostream& s) const
   {

      s << "---------------------------------- REQUIRED "
        << "----------------------------------\n";

      s << "Rinex Version " << fixed << setw(5) << setprecision(2) << version
        << ",  File type " << fileType << ", System " << fileSys << ".\n";
      s << "Prgm: " << fileProgram << ",  Run: " << date << ",  By: "
        << fileAgency << endl;

      s << "(This header is ";
      if(version >= 3 && (valid & allValid3) == allValid3)
         s << "VALID RINEX version 3";
      else if(version < 3 && (valid & allValid2) == allValid2)
         s << "VALID RINEX version 2";
      else s << "NOT VALID RINEX";
      s << ")." << endl;

      if(!(valid & validVersion)) s << " Version is NOT valid\n";
      if(!(valid & validRunBy  )) s << " Run by is NOT valid\n";
      if(!(valid & validEoH    )) s << " End of Header is NOT valid\n";

      s << "---------------------------------- OPTIONAL "
        << "----------------------------------\n";

      for(map<string,TimeSystemCorrection>::const_iterator tcit
             = mapTimeCorr.begin(); tcit != mapTimeCorr.end(); ++tcit)

      {
         tcit->second.dump(s);
         s << endl;
      }

      for (auto const & [type, value] : mapIonoCorr)
      {
         s << "Iono correction for " << value.asString() << ": "
           << scientific << setprecision(4);
         switch (type)
         {
         case IonoCorr::CorrType::GPSA:
         case IonoCorr::CorrType::QZSA:
         case IonoCorr::CorrType::BDSA:
         case IonoCorr::CorrType::IRNA:
            s << "aplha";
            break;
         case IonoCorr::CorrType::GPSB:
         case IonoCorr::CorrType::QZSB:
         case IonoCorr::CorrType::BDSB:
         case IonoCorr::CorrType::IRNB:
            s << "beta";
            break;
         default:
            s << "ai";
         }
         for (auto const& parameter : value.param)
            s << " " << parameter;
         s << std::endl;
      }

      if(valid & validLeapSeconds)
      {
         s << "Leap seconds: " << leapSeconds;
         if(leapDelta != 0)
            s << ", change " << leapDelta << " at week " << leapWeek
              << ", day " << leapDay;
         s << endl;
      }
      else s << " Leap seconds is NOT valid\n";

      if(commentList.size() > 0)
      {
         s << "Comments (" << commentList.size() << ") :\n";
         for(size_t i = 0; i < commentList.size(); i++)
            s << commentList[i] << endl;
      }

      s << "-------------------------------- END OF HEADER "
        << "-------------------------------\n";

   } // end of dump


   void Rinex3NavHeader::setFileSystem(const std::string& str)
   {
      try
      {
         if(str[0] == 'M' || str[0] == 'm')
         {
            if(version < 3)
            {
               Exception e("RINEX version 2 'Mixed' Nav files do not exist");
               GNSSTK_THROW(e);
            }
            fileType = "N: GNSS NAV DATA";
            fileSys = "MIXED";
            fileSysSat = SatID(-1, SatelliteSystem::Mixed);
         }
         else
         {
            RinexSatID sat(std::string(1,str[0]));
            fileSysSat = SatID(sat);
            fileSys = StringUtils::asString(sat.systemChar())
               + ": (" + sat.systemString3()+")";
            if(version >= 3)
            {
               fileType = "N: GNSS NAV DATA";
            }
            else
            {
                  // RINEX 2
               if(sat.system == SatelliteSystem::GPS)
                  fileType = "N: GPS NAV DATA";
               else if(sat.system == SatelliteSystem::Glonass)
                  fileType = "G: GLO NAV DATA)";
               else if(sat.system == SatelliteSystem::Geosync)
                  fileType = "H: GEO NAV DATA";
               else
               {
                  Exception e( std::string("RINEX version 2 ") +
                               sat.systemString3() +
                               std::string(" Nav files do not exist") );
                  GNSSTK_THROW(e);
               }
            }
         }
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }


   bool Rinex3NavHeader::compare(const Rinex3NavHeader& right,
                                 std::vector<std::string>& diffs,
                                 const std::vector<std::string>& inclExclList,
                                 bool incl)
   {
         // map header token to comparison result
      std::map<std::string,bool> lineMap;
      std::map<std::string,bool>::const_iterator lmi;
         // Put the comments in a sorted set, we don't really care
         // about the ordering.
      std::set<std::string>
         lcomments(commentList.begin(), commentList.end()),
         rcomments(right.commentList.begin(), right.commentList.end());
         // Compare everything first...
         // deliberately ignoring valid flags

         // only comparing first character of file type because that's
         // all that matters according to RINEX
      lineMap[stringVersion] =
         ((version == right.version) &&
          (fileType[0] == right.fileType[0]) &&
          (fileSysSat.system == right.fileSysSat.system));
      lineMap[stringRunBy] =
         ((fileProgram == right.fileProgram) &&
          (fileAgency == right.fileAgency) &&
          (date == right.date));
      lineMap[stringComment] = (lcomments == rcomments);
      lineMap[stringIonoCorr] = (mapIonoCorr == right.mapIonoCorr);
      lineMap[stringTimeSysCorr] = (mapTimeCorr == right.mapTimeCorr);
      lineMap[stringLeapSeconds] =
         ((leapSeconds == right.leapSeconds) &&
          (leapDelta == right.leapDelta) &&
          (leapWeek == right.leapWeek) &&
          (leapDay == right.leapDay));
      std::map<std::string,TimeSystemCorrection>::const_iterator ltci, rtci;
      std::map<std::string,IonoCorr>::const_iterator lici, rici;
      if (((ltci = mapTimeCorr.find("GPUT")) != mapTimeCorr.end()) &&
          ((rtci = right.mapTimeCorr.find("GPUT")) != right.mapTimeCorr.end()))

      {
         lineMap[stringDeltaUTC] = ltci->second == rtci->second;
      }
      else
      {
         lineMap[stringDeltaUTC] = true;
      }
/** @todo compare stringCorrSysTime... not clear how to do this since
 * the exact same data structure is used to store stuff from
 * TimeSysCorr and CorrSysTime.
 */
      if (((ltci = mapTimeCorr.find("SBUT")) != mapTimeCorr.end()) &&
          ((rtci = right.mapTimeCorr.find("SBUT")) != right.mapTimeCorr.end()))

      {
         lineMap[stringDUTC] = ltci->second == rtci->second;
      }
      else
      {
         lineMap[stringDUTC] = true;
      }
         // ...then filter by inclExclList later
      if (incl)
      {
         std::map<std::string,bool> oldLineMap(lineMap);
         std::map<std::string,bool>::const_iterator olmi;
         lineMap.clear();
         for (unsigned i = 0; i < inclExclList.size(); i++)

         {
            if ((olmi = oldLineMap.find(inclExclList[i])) != oldLineMap.end())

            {
               lineMap[olmi->first] = olmi->second;
            }
         }
      }
      else
      {
            // exclude, remove items in inclExclList
         for (unsigned i = 0; i < inclExclList.size(); i++)

         {
            lineMap.erase(inclExclList[i]);
         }
      }
         // check the equality of the final remaining set of header lines
      bool rv = true;
      for (lmi = lineMap.begin(); lmi != lineMap.end(); lmi++)
      {
         if (!lmi->second)
         {
            diffs.push_back(lmi->first);
            rv = false;
         }
      }
      return rv;
   } // bool Rinex3NavHeader::compare

} // namespace
