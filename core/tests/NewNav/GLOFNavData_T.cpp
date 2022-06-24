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
#include "TestUtil.hpp"
#include "GLOFNavData.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, GLOFNavData::SatType e)
   {
      s << StringUtils::asString(e);
      return s;
   }


   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


/// Make a testable non-abstract class
class TestClass : public gnsstk::GLOFNavData
{
public:
   bool getXvt(const gnsstk::CommonTime& t, gnsstk::Xvt& xvt,
               const gnsstk::ObsID& oid = gnsstk::ObsID()) override
   { return false; }
};


class GLOFNavData_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
};


unsigned GLOFNavData_T ::
constructorTest()
{
   TUDEF("GLOFNavData", "GLOFNavData()");
   TestClass uut;
   gnsstk::CommonTime exp;
   TUASSERTE(gnsstk::CommonTime, exp, uut.xmit2);
   TUASSERTE(gnsstk::GLOFNavData::SatType,gnsstk::GLOFNavData::SatType::Unknown,
             uut.satType);
   TUASSERTE(unsigned, -1, uut.slot);
   TUASSERTE(bool, false, uut.lhealth);
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown, uut.health);
   TUASSERTE(gnsstk::CommonTime, exp, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, exp, uut.endFit);
   TURETURN();
}


unsigned GLOFNavData_T ::
validateTest()
{
   TUDEF("GLOFNavData", "validate()");
   TestClass uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}



int main()
{
   GLOFNavData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
