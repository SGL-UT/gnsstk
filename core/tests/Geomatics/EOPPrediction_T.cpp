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

#include <iostream>

#include "TestUtil.hpp"
#include "EOPPrediction.hpp"
#include "EarthOrientation.hpp"

class EOPPrediction_T
{
public:
    EOPPrediction_T();
    unsigned testEOPPredictionMethods();

};

EOPPrediction_T ::EOPPrediction_T(){
}

unsigned EOPPrediction_T :: testEOPPredictionMethods()
{
    TUDEF("EOPPrediction_T", "testEOPPredictionMethods");
    double epsilon=1e-7;
    int mjd;
    int mjd_exp =60573;
    int sno_exp = 437;
    double xp_exp = 0.222365;
    double yp_exp = 0.4214147593;
    double UT1mUTC_exp =0.0757214; 
    int exp_ssize=405;
    gnsstk::EOPPrediction eopp;
    std::ostringstream oss; 

    /* EOPP input file in 5 -line Format for 092024 from https://earth-info.nga.mil/;
     * copied to new test input file EOPP_5lineformat.txt 
    */
    std::string filepath = gnsstk::getPathData() + gnsstk::getFileSep() + "inputs" +  gnsstk::getFileSep() + "EOPP_5lineformat.txt";

    //loadFile() returns  0 if ok, -1 if error reading file 
    try {
        TUASSERT(!eopp.loadFile(filepath));

        //get Valid Time
        mjd = eopp.getValidTime();
        TUASSERTE(int, mjd_exp, mjd);

        //get serial number
        int sno = eopp.getSerialNumber(mjd);
        TUASSERTE(int, sno_exp, sno);

        //compute EOP for the given mjd
        gnsstk::EarthOrientation eo;
        eo = eopp.computeEOP(mjd);

        TUASSERTFEPS( xp_exp, eo.xp,epsilon);
        TUASSERTFEPS( yp_exp, eo.yp,epsilon);
        TUASSERTFEPS( UT1mUTC_exp, eo.UT1mUTC,epsilon);

        //Test overloaded operator '<<'
        oss<<eopp;
        TUASSERTE(int,exp_ssize, oss.str().size());

   }

    catch(gnsstk::Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
   }
   TURETURN();
}


int main()
{
   EOPPrediction_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testEOPPredictionMethods();
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
