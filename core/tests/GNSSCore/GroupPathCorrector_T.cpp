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
#include "GroupPathCorrector.hpp"
#include "NavLibrary.hpp"
#include "MultiFormatNavDataFactory.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, CorrectorType t)
   {
      s << StringUtils::asString(t);
      return s;
   }
}


/// Make GroupPathCorrector instantiatable for testing.
class TestClass : public gnsstk::GroupPathCorrector
{
public:
   TestClass()
   {}
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Position& svPos,
                 const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                 const gnsstk::CommonTime& when, gnsstk::NavType nav,
                 double& corrOut) override
   { return false; }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Xvt& svPos,
                 const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                 const gnsstk::CommonTime& when, gnsstk::NavType nav,
                 double& corrOut) override
   { return false; }
};


class GroupPathCorrector_T
{
public:
   unsigned constructorTest();
};


unsigned GroupPathCorrector_T ::
constructorTest()
{
   TUDEF("GroupPathCorrector", "GroupPathCorrector");
   TestClass uut;
   TUASSERTE(gnsstk::CorrectorType, gnsstk::CorrectorType::Unknown,
             uut.corrType);
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   GroupPathCorrector_T testClass;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
