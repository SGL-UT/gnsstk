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

/** @file navfilterex-pseudo.cpp Pseudo-code for a NavFilterMgr
 * example.  Refer to navfilterex.cpp for a functional example. */

void filterEX()
{
      // Filter manager, where the work is done
   gnsstk::NavFilterMgr mgr;
      // Individual filters being applied
   gnsstk::LNavCookFilter filtCook;
   gnsstk::LNavParityFilter filtParity;
      // Data being passed to the filter
   gnsstk::LNavFilterData navFiltData;
      // Generic LNAV message storage
   std::vector<uint32_t> subframe(10,0);

      // Tell the manager what filters to use
   mgr.addFilter(&filtCook);
   mgr.addFilter(&filtParity);

      // Point the filter data to the LNAV message
   navFiltData.sf = &subframe[0];

      // validate the subframe
   gnsstk::NavFilter::NavMsgList l = mgr.validate(&navFiltData);

      // process the results
   gnsstk::NavFilter::NavMsgList::const_iterator nmli;
   for (nmli = l.begin(); nmli != l.end(); nmli++)
   {
      gnsstk::LNavFilterData *fd = dynamic_cast<gnsstk::LNavFilterData*>(*nmli);
         // do something with fd (nav message that passed the filters)
   }
}
