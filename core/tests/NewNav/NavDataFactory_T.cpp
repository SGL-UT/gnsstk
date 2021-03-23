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
#include "NavDataFactory.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s,
                            const gpstk::NavMessageTypeSet& nmts)
   {
      s << "{";
      for (const auto& i : nmts)
      {
         s << " " << gpstk::StringUtils::asString(i);
      }
      s << " }";
      return s;
   }
}

/// Make a non-abstract class derived from NavDataFactory so we can test it
class TestClass : public gpstk::NavDataFactory
{
public:
   bool find(const gpstk::NavMessageID& nmid, const gpstk::CommonTime& when,
             gpstk::NavDataPtr& navData, gpstk::SVHealth xmitHealth,
             gpstk::NavValidityType valid, gpstk::NavSearchOrder order) override
   { return false; }
   bool getOffset(gpstk::TimeSystem fromSys, gpstk::TimeSystem toSys,
                  const gpstk::CommonTime& when, gpstk::NavDataPtr& offset,
                  gpstk::SVHealth xmitHealth, gpstk::NavValidityType valid)
      override
   { return false; }
   bool addDataSource(const std::string& source) override
   { return false; }
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
   std::string getFactoryFormats() const override
   { return "BUNK"; }
};


class NavDataFactory_T
{
public:
   unsigned constructorTest();
   unsigned setValidityFilterTest();
   unsigned setTypeFilterTest();
};


unsigned NavDataFactory_T ::
constructorTest()
{
   TUDEF("NavDataFactory", "NavDataFactory");
   TestClass obj;
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
             obj.getValidityFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
             obj.getTypeFilter());
      // nothing should be in supportedSignals at this level.
   TUASSERT(obj.supportedSignals.empty());
   TURETURN();
}


unsigned NavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("NavDataFactory", "setValidityFilter");
   TestClass obj;
   obj.setValidityFilter(gpstk::NavValidityType::ValidOnly);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
             obj.getValidityFilter());
   obj.setValidityFilter(gpstk::NavValidityType::InvalidOnly);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::InvalidOnly,
             obj.getValidityFilter());
   TURETURN();
}


unsigned NavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("NavDataFactory", "setTypeFilter");
   TestClass obj;
      // some random examples
   gpstk::NavMessageTypeSet s1 { gpstk::NavMessageType::Almanac,
                                 gpstk::NavMessageType::Health };
   gpstk::NavMessageTypeSet s2 { gpstk::NavMessageType::Ephemeris };
   gpstk::NavMessageTypeSet s3 { gpstk::NavMessageType::Ephemeris,
                                 gpstk::NavMessageType::TimeOffset,
                                 gpstk::NavMessageType::Health };
   obj.setTypeFilter(s1);
   TUASSERTE(gpstk::NavMessageTypeSet, s1, obj.getTypeFilter());
   obj.setTypeFilter(s2);
   TUASSERTE(gpstk::NavMessageTypeSet, s2, obj.getTypeFilter());
   obj.setTypeFilter(s3);
   TUASSERTE(gpstk::NavMessageTypeSet, s3, obj.getTypeFilter());
   TURETURN();
}


int main()
{
   NavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
