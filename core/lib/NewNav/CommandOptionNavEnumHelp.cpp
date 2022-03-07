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

#include "CommandOptionNavEnumHelp.hpp"
#include "StringUtils.hpp"
#include "SatelliteSystem.hpp"
#include "ObservationType.hpp"
#include "CarrierBand.hpp"
#include "TrackingCode.hpp"
#include "XmitAnt.hpp"
#include "NavType.hpp"
#include "NavMessageType.hpp"
#include "NavValidityType.hpp"
#include "NavSearchOrder.hpp"
#include "SVHealth.hpp"
#include "DumpDetail.hpp"

using namespace std;

namespace gnsstk
{
   CommandOptionNavEnumHelp ::
   CommandOptionNavEnumHelp(const char shOpt,
                            const std::string& loOpt,
                            const std::string& desc)
         : CommandOptionHelp(CommandOption::hasArgument, shOpt, loOpt, desc)
   {
   }


   void CommandOptionNavEnumHelp ::
   printHelp(std::ostream& out, bool pretty)
   {
      std::vector<std::string> enumStrs;
      for (unsigned i = 0; i < value.size(); i++)
      {
         std::string lowered(value[i]);
         gnsstk::StringUtils::lowerCase(lowered);
         if (lowered == "system")
         {
            out << "Satellite systems:" << endl;
            for (SatelliteSystem i : SatelliteSystemIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "obstype")
         {
            out << "Observation types:" << endl;
            for (ObservationType i : ObservationTypeIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "carrier")
         {
            out << "Carrier bands:" << endl;
            for (CarrierBand i : CarrierBandIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "range")
         {
            out << "Tracking/ranging codes:" << endl;
            for (TrackingCode i : TrackingCodeIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "antenna")
         {
            out << "Antenna types:" << endl;
            for (XmitAnt i : XmitAntIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "navtype")
         {
            out << "Navigation message formats:" << endl;
            for (NavType i : NavTypeIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "navmsgtype")
         {
            out << "Navigation message types:" << endl;
            for (NavMessageType i : NavMessageTypeIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "health")
         {
            out << "Satellite health:" << endl;
            for (SVHealth i : SVHealthIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "validity")
         {
            out << "Nav message validity types:" << endl;
            for (NavValidityType i : NavValidityTypeIterator())
            {
               enumStrs.push_back(StringUtils::asString(i));
            }
         }
         else if (lowered == "order")
         {
            out << "Nav message search orders:" << endl;
            for (NavSearchOrder i : NavSearchOrderIterator())
            {
                  // Don't even pretend to support "Unknown" as a search order.
               if (i != NavSearchOrder::Unknown)
               {
                  enumStrs.push_back(StringUtils::asString(i));
               }
            }
         }
         else if (lowered == "detail")
         {
            out << "Detail levels:" << endl;
            for (DumpDetail i : DumpDetailIterator())
            {
                  // Don't even pretend to support "Unknown" as a detail level.
               if (i != DumpDetail::Unknown)
               {
                  enumStrs.push_back(StringUtils::asString(i));
               }
            }
         }
         else
         {
            cerr << "Invalid enum type \"" << value[i] << "\"" << endl;
            break;
         }
         out << StringUtils::tabularize(enumStrs);
      } // for (unsigned i = 0; i < value.size(); i++)
   } // printHelp(std::ostream& out, bool pretty)
} // namespace gnsstk
