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
 * @file IonexHeader.cpp
 * This class encapsulates the header of Ionex file, including I/O
 */

#include <cctype>

#include "StringUtils.hpp"
#include "MathBase.hpp"
#include "IonexHeader.hpp"
#include "IonexStream.hpp"
#include "CivilTime.hpp"

using namespace std;
using namespace gnsstk::StringUtils;

namespace gnsstk
{
   const string IonexHeader::versionString         =  "IONEX VERSION / TYPE";
   const string IonexHeader::runByString           =  "PGM / RUN BY / DATE";
   const string IonexHeader::descriptionString     =  "DESCRIPTION";
   const string IonexHeader::commentString         =  "COMMENT";
   const string IonexHeader::firstTimeString       =  "EPOCH OF FIRST MAP";
   const string IonexHeader::lastTimeString        =  "EPOCH OF LAST MAP";
   const string IonexHeader::intervalString        =  "INTERVAL";
   const string IonexHeader::numMapsString         =  "# OF MAPS IN FILE";
   const string IonexHeader::mappingFunctionString =  "MAPPING FUNCTION";
   const string IonexHeader::elevationString       =  "ELEVATION CUTOFF";
   const string IonexHeader::observablesUsedString =  "OBSERVABLES USED";
   const string IonexHeader::numStationsString     =  "# OF STATIONS";
   const string IonexHeader::numSatsString         =  "# OF SATELLITES";
   const string IonexHeader::baseRadiusString      =  "BASE RADIUS";
   const string IonexHeader::mapDimensionString    =  "MAP DIMENSION";
   const string IonexHeader::hgtGridString         =  "HGT1 / HGT2 / DHGT";
   const string IonexHeader::latGridString         =  "LAT1 / LAT2 / DLAT";
   const string IonexHeader::lonGridString         =  "LON1 / LON2 / DLON";
   const string IonexHeader::exponentString        =  "EXPONENT";
   const string IonexHeader::startAuxDataString    =  "START OF AUX DATA";
   const string IonexHeader::endAuxDataString      =  "END OF AUX DATA";
   const string IonexHeader::endOfHeader           =  "END OF HEADER";

   const string IonexHeader::DCB::svsAuxDataString       =  "PRN / BIAS / RMS";
   const string IonexHeader::DCB::stationsAuxDataString  =
      "STATION / BIAS / RMS";


   IonexHeader ::
   IonexHeader()
         : version(1.0), exponent(-1), valid(false)
   {
   }


   IonexHeader ::
   ~IonexHeader()
   {
   }


      // Clear (empty out) header
   void IonexHeader ::
   clear()
   {
      version = 1.0;
      descriptionList.clear();
      commentList.clear();
      interval = 0;
      numMaps = numStations = numSVs = mapDims = 0;
      elevation = baseRadius = 0;

      hgt[0] = hgt[1] = hgt[2] = 0.0;
      lat[0] = lat[1] = lat[2] = 0.0;
      lon[0] = lon[1] = lon[2] = 0.0;

      exponent = -1;    // that's the default value
      svsmap.clear();
      stamap.clear();
      valid = auxDataFlag = false;
   }  // End of method 'IonexHeader::clear()'



      /* Simple debug output function.
       *
       * It simply outputs the version, name and number of maps contained
       * in this Ionex header.
       */
   void IonexHeader ::
   dump(std::ostream& os) const
   {
      os << "-------------------------------- IONEX HEADER"
         << "--------------------------------" << endl;

      os << "First epoch            : " << firstEpoch << endl;
      os << "Last epoch             : " << lastEpoch << endl;
      os << "Interval               : " << interval << endl;
      os << "Number of ionex maps   : " << numMaps << endl;
      os << "Mapping function       : " << mappingFunction << endl;
      os << "Elevation cut off      : " << elevation << endl;
      os << "Number of stations     : " << numStations << endl;
      os << "Number of satellites   : " << numSVs << endl;
      os << "Map dimensions         : " << mapDims << endl;

      os << "HGT1 / HGT2 / DHGT     : " << hgt[0] << " / "
         << hgt[1] << " / "
         << hgt[2] << endl;
      os << "LAT1 / LAT2 / DLAT     : " << lat[0] << " / "
         << lat[1] << " / "
         << lat[2] << endl;
      os << "LON1 / LON2 / DLON     : " << lon[0] << " / "
         << lon[1] << " / "
         << lon[2] << endl;
      os << "Valid object?          : " << valid  << endl;

      os << "-------------------------------- END OF HEADER"
         << "-------------------------------" << endl;

      os << endl;
   }  //End of method 'IonexHeader::dump()'



      /*
       * Parse a single auxiliary header record that contains "Differential
       * code biases".
       */
   void IonexHeader ::
   parseDcbRecord(const std::string &line)
   {
      string label(line, 60, 20);

      if (label == DCB::svsAuxDataString)
      {
            // prepare the DCB structure
            // This makes round-trip conversion impossible.
            //char c = isspace(line[3]) ? 'G' : line[3];
         char c = line[3];
         int prn     = asInt(line.substr(4,2));
         double bias = asDouble(line.substr(6,16));
         double rms  = asDouble(line.substr(16,26));

            // prepare SatID object that is the key of the map
         SatelliteSystem satsys;
         switch(line[3])
         {
            case ' ':
            case 'G':
            case 'g':
               satsys = SatelliteSystem::GPS;
               break;

            case 'R':
            case 'r':
               satsys = SatelliteSystem::Glonass;
               break;

            default:                   // non-IONEX system character
               FFStreamError e(std::string("Invalid system character \"")
                               + c + std::string("\""));
               GNSSTK_THROW(e);
         }  // End of 'switch(line[3])'

         SatID svid = SatID(prn,satsys);

            // add to map
         svsmap[svid] = DCB(c,prn,bias,rms);
      }  // End of 'if (label == DCB::svsAuxDataString)'...
      else if (label == DCB::stationsAuxDataString)
      {
         string name = line.substr(6,4);
            /** @warning The length of the number and the start
             * position of the bias is just a guess since I can't find
             * an Ionex 1.0 document and the 1.1 threw out all the DCB
             * information */
         string num = line.substr(11,15);
         double bias = asDouble(line.substr(26,10));
         double rms = asDouble(line.substr(36,10));
         stamap[name] = DCB(name, num, bias, rms);
      }
      else if (label == commentString)
      {
            // CODE's product has a comment line before aux data end
         string s = strip(line.substr(0,60));
         commentList.push_back(s);
      }
      else if (label == endAuxDataString)
      {
         auxDataFlag = false;          // End of aux data
      }
      else
      {
         FFStreamError e(std::string( "Unidentified IONEX::DCB label: "
                                      + label) );
         GNSSTK_THROW(e);
      }  // End of 'if (label == endAuxDataString)'...
   }  // End of method 'IonexHeader::parseDcbRecord()'



      /* Parse a single header record, and modify 'valid' accordingly.
       *
       * Used by reallyGetRecord for both IonexHeader and IonexData.
       */
   void IonexHeader ::
   parseHeaderRecord(const std::string& line)
   {
      string label(line, 60, 20);

      if (label == versionString)
      {
         version  = asDouble(line.substr(0,20));
         fileType = strip(line.substr(20,20));
         system   = strip(line.substr(40,20));
      }
      else if (label == runByString)
      {
         fileProgram = strip(line.substr( 0,20));
         fileAgency  = strip(line.substr(20,20));
         date        = strip(line.substr(40,20));
      }
      else if (label == descriptionString)
      {
         string s = line.substr(0,60);
         descriptionList.push_back(s);
      }
      else if (label == commentString)
      {
         string s = line.substr(0,60);
         commentList.push_back(s);
      }
      else if (label == firstTimeString)
      {
         firstEpoch = parseTime(line);
      }
      else if (label == lastTimeString)
      {
         lastEpoch = parseTime(line);
      }
      else if (label == intervalString)
      {
         interval = asInt(line.substr(0,6));
      }
      else if (label == numMapsString)
      {
         numMaps = asInt(line.substr(0,6));
      }
      else if (label == mappingFunctionString)
      {
         mappingFunction = strip(line.substr(0, 6));
      }
      else if (label == elevationString)
      {
         elevation = asDouble(line.substr(0, 8));
      }
      else if (label == observablesUsedString)
      {
         observablesUsed = strip(line.substr(0,60));
      }
      else if (label == numStationsString)
      {
         numStations = asInt(line.substr(0,6));
      }
      else if (label == numSatsString)
      {
         numSVs = asInt(line.substr(0,6));
      }
      else if (label == baseRadiusString)
      {
         baseRadius = asDouble(line.substr(0, 8));
      }
      else if (label == mapDimensionString)
      {
         mapDims = asInt(line.substr(0,6));
      }
      else if (label == hgtGridString)
      {
         hgt[0] = asDouble(line.substr( 2, 6));
         hgt[1] = asDouble(line.substr( 8, 6));
         hgt[2] = asDouble(line.substr(14, 6));
      }
      else if (label == latGridString)
      {
         lat[0] = asDouble(line.substr( 2, 6));
         lat[1] = asDouble(line.substr( 8, 6));
         lat[2] = asDouble(line.substr(14, 6));
      }
      else if (label == lonGridString)
      {
         lon[0] = asDouble(line.substr( 2, 6));
         lon[1] = asDouble(line.substr( 8, 6));
         lon[2] = asDouble(line.substr(14, 6));
      }
      else if (label == exponentString)
      {
         exponent = asInt(line.substr(0,6));
      }
      else if (label == startAuxDataString)
      {
         auxData = strip(line.substr(0,60));
         auxDataFlag = true;
      }
      else if (label == endOfHeader)
      {
         auxDataFlag = true;
         valid = true;
      }
      else
      {
         FFStreamError e("Unidentified IONEX header record: " + label);
         GNSSTK_THROW(e);
      }
   }  // End of method 'IonexHeader::parseHeaderRecord()'



      // This function parses the entire header from the given stream
   void IonexHeader ::
   reallyGetRecord(FFStream& ffs)
   {
      IonexStream& strm = dynamic_cast<IonexStream&> (ffs);

         // if already read, just return
      if (strm.headerRead == true)
      {
         return;
      }

         // since we read a new header, we need to reinitialize
         // all our list structures. All the other objects should be ok.
         // This also applies if we threw an exception the first time we read
         // the header and are now re-reading it. Some of these data
         // structures could be full and we need to empty them.
      clear();

      string line;

      while (!valid)
      {
         strm.formattedGetLine(line);
         StringUtils::stripTrailing(line);

            // skip empty lines
         if (line.length() == 0)
         {
            continue;
         }
         else
         {
            if (line.length() < 60 || line.length() > 80)
            {
               FFStreamError e("Invalid line length");
               GNSSTK_THROW(e);
            }
         }  // End of 'if (line.length() == 0)...'

         if (auxDataFlag)     // when it is set true, then parse auxiliar data
         {
            try
            {
               parseDcbRecord(line);
            }
            catch (FFStreamError& e)
            {
               GNSSTK_RETHROW(e);
            }
         }
         else
         {
            try
            {
               parseHeaderRecord(line);
            }
            catch (FFStreamError& e)
            {
               GNSSTK_RETHROW(e);
            }
         }  // End of 'if (auxDataFlag)...'
      }  // End of 'while (!valid)...' (not for the header)

         // Here come some validity checkings
         // Checking ionex version
      if (version != 1.0)
      {
         FFStreamError e( "Invalid IONEX version number " +
                          asString(version));
         GNSSTK_THROW(e);
      }

         // time arguments consistency
      double interval0( (lastEpoch - firstEpoch) / (numMaps -1.0) );
      if (interval != static_cast<int>(interval0))
      {
         FFStreamError e("Inconsistent time arguments.");
         GNSSTK_THROW(e);
      }

         // map dimension consistency
      if (mapDims == 2)
      {
         if ( (hgt[0] != hgt[1]) || (hgt[2] != 0.0) )
         {
            FFStreamError e("Error concerning map dimension.");
            GNSSTK_THROW(e);
         }
      }
      else
      {
         if ( (hgt[0] == hgt[1]) || (hgt[2] == 0.0) )
         {
            FFStreamError e("Error concerning map dimension.");
            GNSSTK_THROW(e);
         }
      }  // End of 'if (mapDims == 2)...'

         // grid checkings
      double grdfac[4];
      try
      {
         grdfac[0] = lat[0]/lat[2];
         grdfac[1] = lat[1]/lat[2];
         grdfac[2] = lon[0]/lon[2];
         grdfac[3] = lon[1]/lon[2];
      }
      catch(std::exception& e)
      {
         throw;
      }

      for (int i = 0; i < 4; i++)
      {
            //const double xdif1( grdfac[i] - static_cast<int>(grdfac[i]) );
         const double xdif( ABS(grdfac[i] - static_cast<int>(grdfac[i])) );

         if (xdif > 1e-4)
         {
            FFStreamError e("Irregular Ionex data grid.");
            GNSSTK_THROW(e);
         }
      }  // End of 'for (int i = 0; i < 4; i++)...'

         // reach end of header line
      strm.header = *this;
      strm.headerRead = true;
   }  // End of method 'IonexHeader::reallyGetRecord()'



   void IonexHeader ::
   reallyPutRecord(FFStream& ffs) const
   {
      IonexStream& strm = dynamic_cast<IonexStream&>(ffs);

      if (version != 1.0)
      {
         FFStreamError err( "Unknown IONEX version: " + asString(version,2) );
         err.addText("Make sure to set the version correctly.");
         GNSSTK_THROW(err);
      }

      try
      {
         writeHeaderRecords(strm);
      }
      catch(FFStreamError& e)
      {
         GNSSTK_RETHROW(e);
      }
      catch(StringException& e)
      {
         GNSSTK_RETHROW(e);
      }
   }  // End of method 'IonexHeader::reallyPutRecord()'


      // this function writes all valid header records
   void IonexHeader ::
   writeHeaderRecords(FFStream& ffs) const
   {
      IonexStream& strm = dynamic_cast<IonexStream&>(ffs);
      string line;

      if (valid)
      {
            // write first IONEX record
         line.clear();
         line  = rightJustify(asString(version,1), 8);
         line += string(12, ' ');
         if ((fileType[0] != 'I') && (fileType[0] != 'i'))
         {
            FFStreamError err("This isn't a Ionex file: " +
                              fileType.substr(0,1));
            GNSSTK_THROW(err);
         }

         line += leftJustify(fileType, 20);
         line += leftJustify(system, 20);
         line += versionString;
         strm << line << endl;
         strm.lineNumber++;

            // write second IONEX record
         line.clear();
         line += leftJustify(fileProgram,20);
         line += leftJustify(fileAgency,20);
         line += leftJustify(date,20);
         line += runByString;
         strm << line << endl;
         strm.lineNumber++;

            // write multi-line description (optional)
         if (descriptionList.size() > 0)
         {
            vector<std::string>::size_type i = 0;

            for( ; i < descriptionList.size(); i++)
            {
               line.clear();
               line += leftJustify(descriptionList[i],60);
               line += descriptionString;
               strm << line << endl;
               strm.lineNumber++;
            }
         }  // End of 'if (descriptionList.size() > 0) ...'

            // write epoch of first epoch
         line.clear();
         line += writeTime(firstEpoch);
         line += string(24, ' ');
         line += firstTimeString;
         strm << line << endl;
         strm.lineNumber++;

            // write epoch of last epoch
         line.clear();
         line += writeTime(lastEpoch);
         line += string(24, ' ');
         line += lastTimeString;
         strm << line << endl;
         strm.lineNumber++;

            // write interval
         line.clear();
         line += rightJustify( asString(interval), 6 );
         line += string(54, ' ');
         line += intervalString;
         strm << line << endl;
         strm.lineNumber++;

            // write # of maps
         line.clear();
         line += rightJustify( asString<short>(numMaps), 6 );
         line += string(54, ' ');
         line += numMapsString;
         strm << line << endl;
         strm.lineNumber++;

            // write mapping function
         line.clear();
         line += string(2, ' ');
         line += rightJustify(mappingFunction, 4);
         line += string(54, ' ');
         line += mappingFunctionString;
         strm << line << endl;
         strm.lineNumber++;

            // write elevation cutoff
         line.clear();
         line += rightJustify( asString(elevation,1), 8 );
         line += string(52, ' ');
         line += elevationString;
         strm << line << endl;
         strm.lineNumber++;

            // write observables used
         line.clear();
         line += leftJustify(observablesUsed,60);
         line += observablesUsedString;
         strm << line << endl;
         strm.lineNumber++;

            // write # of stations (optional)
         if (numStations > 0)
         {
            line.clear();
            line += rightJustify( asString<short>(numStations), 6 );
            line += string(54, ' ');
            line += numStationsString;
            strm << line << endl;
            strm.lineNumber++;
         }

            // write # of satellites (optional)
         if (numSVs > 0)
         {
            line.clear();
            line += rightJustify( asString<short>(numSVs), 6 );
            line += string(54, ' ');
            line += numSatsString;
            strm << line << endl;
            strm.lineNumber++;
         }

            // write base radius
         line.clear();
         line += rightJustify( asString(baseRadius,1), 8 );
         line += string(52, ' ');
         line += baseRadiusString;
         strm << line << endl;
         strm.lineNumber++;

            // write map dimension
         line.clear();
         line += rightJustify( asString(mapDims), 6 );
         line += string(54, ' ');
         line += mapDimensionString;
         strm << line << endl;
         strm.lineNumber++;

            // write grid specifications
         line.clear();
         line += string(2, ' ');
         line += rightJustify( asString(hgt[0],1), 6 );
         line += rightJustify( asString(hgt[1],1), 6 );
         line += rightJustify( asString(hgt[2],1), 6 );
         line += string(40, ' ');
         line += hgtGridString;
         strm << line << endl;
         strm.lineNumber++;

         line.clear();
         line += string(2, ' ');
         line += rightJustify( asString(lat[0],1), 6 );
         line += rightJustify( asString(lat[1],1), 6 );
         line += rightJustify( asString(lat[2],1), 6 );
         line += string(40, ' ');
         line += latGridString;
         strm << line << endl;
         strm.lineNumber++;

         line.clear();
         line += string(2, ' ');
         line += rightJustify( asString(lon[0],1), 6 );
         line += rightJustify( asString(lon[1],1), 6 );
         line += rightJustify( asString(lon[2],1), 6 );
         line += string(40, ' ');
         line += lonGridString;
         strm << line << endl;
         strm.lineNumber++;

            // write default exponent (optional)
         line.clear();
         line += rightJustify( asString(exponent), 6 );
         line += string(54, ' ');
         line += exponentString;
         strm << line << endl;
         strm.lineNumber++;

            // write multi-line comment
         for( vector<std::string>::size_type i = 0;
              i < commentList.size(); i++)
         {
            line.clear();
            line += leftJustify(commentList[i],60);
            line += commentString;
            strm << line << endl;
            strm.lineNumber++;
         }

            // write auxiliary data (optional)
         if (auxDataFlag)
         {
               // start of aux data
            line.clear();
            line += leftJustify(auxData,60);
            line += startAuxDataString;
            strm << line << endl;
            strm.lineNumber++;

            for(const auto& isv : svsmap)
            {
               line.clear();
               line += isv.second.toString();
               line += string(34, ' ');
               line += DCB::svsAuxDataString;
               strm << line << endl;
               strm.lineNumber++;
            }

            for (const auto& ista : stamap)
            {
               line.clear();
               line += leftJustify(ista.second.toString(), 60);
               line += DCB::stationsAuxDataString;
               strm << line << endl;
               strm.lineNumber++;
            }

               // end of aux data
            line.clear();
            line += leftJustify(auxData,60);
            line += endAuxDataString;
            strm << line << endl;
            strm.lineNumber++;

         }  // End of 'if (auxDataFlag)...'

            // write record closing Ionex header
         line.clear();
         line += string(60, ' ');
         line += endOfHeader;
         strm << line << endl;
         strm.lineNumber++;
      }  // End of 'if (valid)...'
   }


      /* This function sets the time for this header.
       *
       * It looks at \a line to obtain the needed information.
       */
   CommonTime IonexHeader ::
   parseTime(const std::string& line) const
   {
      int year, month, day, hour, min, sec;

      year  = asInt(line.substr( 0,6));
      month = asInt(line.substr( 6,6));
      day   = asInt(line.substr(12,6));
      hour  = asInt(line.substr(18,6));
      min   = asInt(line.substr(24,6));
      sec   = asInt(line.substr(30,6));

      return CivilTime(year, month, day, hour, min, (double)sec);
   }  // End of method 'IonexHeader::parseTime()'


      /** Converts the CommonTime \a dt into a Ionex Obs time
       * string for the header
       */
   string IonexHeader ::
   writeTime(const CommonTime& dt) const
   {
      string line;

      line  = rightJustify(asString<short>(static_cast<CivilTime>(dt).year), 6);
      line += rightJustify(asString<short>(static_cast<CivilTime>(dt).month),
                           6);
      line += rightJustify(asString<short>(static_cast<CivilTime>(dt).day), 6);
      line += rightJustify(asString<short>(static_cast<CivilTime>(dt).hour), 6);
      line += rightJustify(asString<short>(static_cast<CivilTime>(dt).minute),
                           6);
      line += rightJustify(
         asString (static_cast<int>(static_cast<CivilTime>(dt).second)), 6);

      return line;
   }  // End of method 'IonexHeader::writeTime()'


   IonexHeader::DCB ::
   DCB()
         : system('U'), prn(-1), bias(0), rms(0.0), isPRN(true)
   {
   }


   IonexHeader::DCB ::
   DCB(char s, int p, double b, double r)
         : system(s), prn(p), bias(b), rms(r), isPRN(true)
   {
   }


   IonexHeader::DCB ::
   DCB(const std::string& s, const std::string& n, double b, double r)
         : system('U'), prn(-1), bias(b), rms(r), isPRN(false), name(s),
           number(n)
   {
   }


   std::string IonexHeader::DCB ::
   toString() const
   {
      std::string line;
      if (isPRN)
      {
         line = string(3, ' ');
         line += std::string(3, '0');

            // update with the system char
         line[3] = system;

            // convert the prn into 2-digit string
         std::string s = StringUtils::asString(prn);
         if (prn < 10)
         {
            line[5] = s[0];
         }
         else
         {
            line[4] = s[0];
            line[5] = s[1];
         }
      }
      else
      {
         line = string(6, ' ');
         line += StringUtils::leftJustify(name, 4);
         line += ' ';
         line += StringUtils::leftJustify(number, 15);
      }
         // append bias and rms
      line += StringUtils::rightJustify( StringUtils::asString(bias,3), 10 );
      line += StringUtils::rightJustify( StringUtils::asString(rms, 3), 10 );

      return line;
   }  // End of method 'DCB::toString()'
}  // End of namespace gnsstk
