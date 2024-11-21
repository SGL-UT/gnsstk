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

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "IonexStore.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Ionex_T
{
public:
      /// Do a round-trip test on an Ionex 1.0 file
   unsigned roundTrip10Test();
   unsigned ionexHeaderDumpTest();
   unsigned ionexDataDumpTest();
   unsigned ionexStoreDumpTest();
   gnsstk::IonexData data;
   gnsstk::IonexHeader header;
};


unsigned Ionex_T ::
roundTrip10Test()
{
   TUDEF("IonexStream", "open");
   gnsstk::IonexStream is, os;
   string ps(gnsstk::getFileSep());
   string pd(gnsstk::getPathData());
   string tp(gnsstk::getPathTestTemp());
   string reffn(pd + ps + "igrg2680.20i");
   string outfn(tp + ps + "igrg2680.20i");
   TUCATCH(is.open(reffn.c_str(), ios::in));
   TUCATCH(os.open(outfn.c_str(), ios::out));
   TUCSM("reallyGetRecord");
   TUCATCH(is >> header);
   if (!is)
   {
      TUFAIL("Unable to read input stream");
   }
   /// Let's validate the dump the header of the ionex file
   ionexHeaderDumpTest();
   TUCSM("reallyPutRecord");
   TUCATCH(os << header);
   if (!os)
   {
      TUFAIL("Unable to write to output stream");
   }
   TUCSM("close");
   while (is)
   {
      TUCSM("reallyGetRecord");
      TUCATCH(is >> data);
      /// Let's validate the dump of the first TEC map data record of the ionex file
      if (data.mapID == 1 && data.type.type == "TEC")
      {
         ionexDataDumpTest();
      }
      TUCSM("reallyPutRecord");
      TUCATCH(os << data);
   }
   TUCATCH(is.close());
   TUCATCH(os.close());
   TUCMPFILE(reffn, outfn, 0);
   TURETURN();
}

unsigned Ionex_T ::
    ionexHeaderDumpTest()
{
   TUDEF("IonexHeader", "dump");
   stringstream ss;
   TUASSERT(header.isHeader());
   TUCATCH(header.dump(ss));
   TUASSERT(ss.str().find("First epoch            : 2459117 00000000 0.000000000000000 UNK") != std::string::npos);
   TUASSERT(ss.str().find("Last epoch             : 2459118 00000000 0.000000000000000 UNK") != std::string::npos);
   TUASSERT(ss.str().find("Interval               : 7200") != std::string::npos);
   TUASSERT(ss.str().find("Number of ionex maps   : 13") != std::string::npos);
   TUASSERT(ss.str().find("Mapping function       : COSZ") != std::string::npos);
   TUASSERT(ss.str().find("Elevation cut off      : 0") != std::string::npos);
   TUASSERT(ss.str().find("Number of stations     : 342") != std::string::npos);
   TUASSERT(ss.str().find("Number of satellites   : 30") != std::string::npos);
   TUASSERT(ss.str().find("Map dimensions         : 2") != std::string::npos);
   TUASSERT(ss.str().find("HGT1 / HGT2 / DHGT     : 450 / 450 / 0") != std::string::npos);
   TUASSERT(ss.str().find("LAT1 / LAT2 / DLAT     : 87.5 / -87.5 / -2.5") != std::string::npos);
   TUASSERT(ss.str().find("LON1 / LON2 / DLON     : -180 / 180 / 5") != std::string::npos);
   TUASSERT(ss.str().find("Valid object?          : 1") != std::string::npos);
   TURETURN();
}

unsigned Ionex_T ::
    ionexDataDumpTest()
{
   TUDEF("IonexData", "dump");
   stringstream ss;
   TUCSM("reallyGetRecord");
   TUCATCH(data.dump(ss));
   TUASSERT(ss.str().find("IonexData dump() function") != std::string::npos);
   TUASSERT(ss.str().find("Epoch                       : 2459117 00000000 0.000000000000000 UNK") != std::string::npos);
   TUASSERT(ss.str().find("Map index                   : 1") != std::string::npos);
   TUASSERT(ss.str().find("Data type                   : TEC (TECU)") != std::string::npos);
   TUASSERT(ss.str().find("Number of values            : 5183 values") != std::string::npos);
   TUASSERT(ss.str().find("Valid object?               : 1") != std::string::npos);
   TURETURN();
}

unsigned Ionex_T ::
    ionexStoreDumpTest()
{
   TUDEF("IonexStore", "dump");
   stringstream ss;

   /// Let's validate a dump (detailed and non-detailed) of the ionex store of files igrg2680.20i and ionex_vtec_plot.19i
   gnsstk::IonexStream is;
   gnsstk::IonexStore istr;
   string ps(gnsstk::getFileSep());
   string pd(gnsstk::getPathData());
   string tp(gnsstk::getPathTestTemp());
   string reffn1(pd + ps + "igrg2680.20i");
   string reffn2(pd + ps + "ionex_vtec_plot.19i");
   TUCATCH(istr.loadFile(reffn1));
   TUCATCH(istr.loadFile(reffn2));
   /// Passing flag 1 to get detail dump to include EPOCH TEC RMS data
   TUCATCH(istr.dump(ss, 1));
   TUASSERT(ss.str().find("IonexStore dump() function") != std::string::npos);
   TUASSERT(ss.str().find("igrg2680.20i") != std::string::npos);
   TUASSERT(ss.str().find("ionex_vtec_plot.19i") != std::string::npos);
   TUASSERT(ss.str().find("# 2 files") != std::string::npos);
   TUASSERT(ss.str().find("# 26 epochs") != std::string::npos);
   TUASSERT(ss.str().find("# over time span 2458485 00000000 0.000000000000000 UNK to 2459118 00000000 0.000000000000000 UNK") != std::string::npos);
   TUASSERT(ss.str().find("EPOCH                  TEC  RMS") != std::string::npos);
   TUASSERT(ss.str().find("2458485 00000000 0.000000000000000 UNK    YES  YES") != std::string::npos);
   TUASSERT(ss.str().find("2459118 00000000 0.000000000000000 UNK    YES  YES") != std::string::npos);
   TUASSERT(ss.str().find("Total epochs:           26   26") != std::string::npos);
   /// Passing flag 0 to exclude dump of EPOCH TEC RMS data
   ss.str("");
   TUCATCH(istr.dump(ss, 0));
   TUASSERT(ss.str().find("EPOCH                  TEC  RMS") == std::string::npos);
   TUASSERT(ss.str().find("2458485 00000000 0.000000000000000 UNK    YES  YES") == std::string::npos);
   TUASSERT(ss.str().find("2459118 00000000 0.000000000000000 UNK    YES  YES") == std::string::npos);
   TURETURN();
}

int main()
{
   unsigned errorTotal = 0;
   Ionex_T testClass;

   errorTotal += testClass.roundTrip10Test();
   errorTotal += testClass.ionexStoreDumpTest();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;
   return errorTotal;
}
