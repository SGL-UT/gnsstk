//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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
#include <math.h>
#include "InterSigCorr.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

/// Expose protected data and implement abstract methods.
class TestClass : public gpstk::InterSigCorr
{
public:
   void addRefOID(const gpstk::ObsID& oid)
   { refOids.insert(oid); }
   void addValidOID(const gpstk::ObsID& oid)
   { validOids.insert(oid); }
   bool isRefOidsEmpty() const
   { return refOids.empty(); }
   bool isValidOidsEmpty() const
   { return validOids.empty(); }
   bool validate() const override
   { return true; }
};


class InterSigCorr_T
{
public:
   InterSigCorr_T()
         : oid1(gpstk::ObservationType::Unknown,
                gpstk::CarrierBand::L1,
                gpstk::TrackingCode::CA),
           oid2(gpstk::ObservationType::Unknown,
                gpstk::CarrierBand::L2,
                gpstk::TrackingCode::Y)
   {}
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
      /// Single frequency ISC test
   unsigned getISCSFTest();
      /// Dual frequency ISC test
   unsigned getISCDFTest();

   gpstk::ObsID oid1, oid2;
};


unsigned InterSigCorr_T ::
constructorTest()
{
   TUDEF("InterSigCorr", "InterSigCorr");
   TestClass uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(int, 1, isnan(uut.isc));
   TUASSERTE(bool, true, uut.isRefOidsEmpty());
   TUASSERTE(bool, true, uut.isValidOidsEmpty());
   TURETURN();
}


unsigned InterSigCorr_T ::
getISCSFTest()
{
   TUDEF("InterSigCorr", "getISC(single-frequency)");
   TestClass uut;
      // isc should be nan, and refOids should be empty
   double corr = 0.123456;
   double expCorr1 = 0.123456;
   double expCorr2 = -6.519258E-09;
   double expCorr3 = -1.0736855745e-08;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // set isc, refOids should still be empty causing false return
   uut.isc = -6.519258E-09;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // add something to refOids, now we should get something
   uut.addRefOID(oid1);
   TUASSERTE(bool, true, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr2, corr);
      // Attempt to get ISC for a different band/code, which should fail
   corr = expCorr1;
   TUASSERTE(bool, false, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr1, corr);
      // Try again after adding the secondary code to validOids
   uut.addValidOID(oid2);
   TUASSERTE(bool, true, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr3, corr);
   TURETURN();
}


unsigned InterSigCorr_T ::
getISCDFTest()
{
   TUDEF("InterSigCorr", "getISC(dual-frequency)");
   TestClass uut;
   double corr = 0.123456;
      // dual frequency getISC should always return 0 for this class
   TUASSERTE(bool, true, uut.getISC(oid1, oid2, corr));
   TUASSERTFE(0, corr);
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid2, oid1, corr));
   TUASSERTFE(0, corr);
   TURETURN();
}


int main()
{
   InterSigCorr_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getISCSFTest();
   errorTotal += testClass.getISCDFTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
