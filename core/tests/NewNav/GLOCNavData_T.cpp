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
#include "GLOCNavData.hpp"
#include "GLOCBits.hpp"


namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


/// Make a testable non-abstract class
class TestClass : public gnsstk::GLOCNavData
{
public:
    bool getXvt(const gnsstk::CommonTime& t, gnsstk::Xvt& xvt,
               const gnsstk::ObsID& oid = gnsstk::ObsID()) override
    { return false; }
    gnsstk::NavDataPtr clone() const override
    { return std::make_shared<TestClass>(*this); }
};


class GLOCNavData_T
{
public:
    unsigned constructorTest();
    unsigned validateTest();
    unsigned isSameDataTest();
};


unsigned GLOCNavData_T ::
constructorTest()
{
    TUDEF("GLOCNavData", "GLOCNavData()");
    TestClass uut;
    TUASSERTE(bool, uut.timeStamp.getTimeSystem() == gnsstk::TimeSystem::Any, true)
    TURETURN();
}


unsigned GLOCNavData_T ::
validateTest()
{
    TUDEF("GLOCNavData", "validate()");
    TestClass uut;

    uut.header.dataInvalid = false;
    uut.header.preamble = gnsstk::gloc::valPreamble; 
    TUASSERTE(bool, true, uut.validate());
    TURETURN();
}


unsigned GLOCNavData_T ::isSameDataTest() {
    TUDEF("GLOCNavData", "isSameData()");

    // set up GLOCNavData objects
    TestClass uut;
    auto uut2 = std::make_shared<TestClass>(uut);

    // Test that it compares
    TUASSERTE(bool, true, uut.isSameData(uut2, true));

    // Test that it fails
    uut.signal.messageType = gnsstk::NavMessageType::Last;
    TUASSERTE(bool, false, uut.isSameData(uut2, true));

    TURETURN();
}


int main()
{
    GLOCNavData_T testClass;
    unsigned errorTotal = 0;

    errorTotal += testClass.constructorTest();
    errorTotal += testClass.validateTest();
    errorTotal += testClass.isSameDataTest();

    std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

    return errorTotal;
}
