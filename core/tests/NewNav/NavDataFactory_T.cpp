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
#include "NavDataFactory.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s,
                            const gnsstk::NavMessageTypeSet& nmts)
   {
      s << "{";
      for (const auto& i : nmts)
      {
         s << " " << gnsstk::StringUtils::asString(i);
      }
      s << " }";
      return s;
   }
}

/// Make a non-abstract class derived from NavDataFactory so we can test it
class TestClass : public gnsstk::NavDataFactory
{
public:
   bool find(const gnsstk::NavMessageID& nmid, const gnsstk::CommonTime& when,
             gnsstk::NavDataPtr& navData, gnsstk::SVHealth xmitHealth,
             gnsstk::NavValidityType valid, gnsstk::NavSearchOrder order) override
   { return false; }
   bool getOffset(gnsstk::TimeSystem fromSys, gnsstk::TimeSystem toSys,
                  const gnsstk::CommonTime& when, gnsstk::NavDataPtr& offset,
                  gnsstk::SVHealth xmitHealth, gnsstk::NavValidityType valid)
      override
   { return false; }
   bool addDataSource(const std::string& source) override
   { return false; }
   gnsstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gnsstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
   std::string getFactoryFormats() const override
   { return "BUNK"; }
   gnsstk::NavSatelliteIDSet getAvailableSats(const gnsstk::CommonTime& fromTime,
                                             const gnsstk::CommonTime& toTime)
      const override
   {
      gnsstk::NavSatelliteIDSet rv;
      return rv;
   }
   gnsstk::NavSatelliteIDSet getAvailableSats(gnsstk::NavMessageType nmt,
                                             const gnsstk::CommonTime& fromTime,
                                             const gnsstk::CommonTime& toTime)
      const override
   {
      gnsstk::NavSatelliteIDSet rv;
      return rv;
   }
   gnsstk::NavMessageIDSet getAvailableMsgs(const gnsstk::CommonTime& fromTime,
                                           const gnsstk::CommonTime& toTime)
      const override
   {
      gnsstk::NavMessageIDSet rv;
      return rv;
   }
   bool isPresent(const gnsstk::NavMessageID& nmid,
                  const gnsstk::CommonTime& fromTime,
                  const gnsstk::CommonTime& toTime) override
   {
      return false;
   }
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
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::Any,
             obj.getValidityFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, gnsstk::allNavMessageTypes,
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
   obj.setValidityFilter(gnsstk::NavValidityType::ValidOnly);
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::ValidOnly,
             obj.getValidityFilter());
   obj.setValidityFilter(gnsstk::NavValidityType::InvalidOnly);
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::InvalidOnly,
             obj.getValidityFilter());
   TURETURN();
}


unsigned NavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("NavDataFactory", "setTypeFilter");
   TestClass obj;
      // some random examples
   gnsstk::NavMessageTypeSet s1 { gnsstk::NavMessageType::Almanac,
                                 gnsstk::NavMessageType::Health };
   gnsstk::NavMessageTypeSet s2 { gnsstk::NavMessageType::Ephemeris };
   gnsstk::NavMessageTypeSet s3 { gnsstk::NavMessageType::Ephemeris,
                                 gnsstk::NavMessageType::TimeOffset,
                                 gnsstk::NavMessageType::Health };
   obj.setTypeFilter(s1);
   TUASSERTE(gnsstk::NavMessageTypeSet, s1, obj.getTypeFilter());
   obj.setTypeFilter(s2);
   TUASSERTE(gnsstk::NavMessageTypeSet, s2, obj.getTypeFilter());
   obj.setTypeFilter(s3);
   TUASSERTE(gnsstk::NavMessageTypeSet, s3, obj.getTypeFilter());
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
