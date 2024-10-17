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

#include "DiscCorr.hpp"
#include "SatPass.hpp"
#include "SatPassUtilities.hpp"
#include "TestUtil.hpp"

using namespace gnsstk;

bool satPassEqual(SatPass& l, SatPass& r)
{
   if (l.size() != r.size()) return false;

   if (l.getFirstTime() != r.getFirstTime()) return false;
   if (l.getLastTime() != r.getLastTime()) return false;
   if (l.getSat() != r.getSat()) return false;
   if (l.getDT() != r.getDT()) return false;
   if (l.getObstypes() != r.getObstypes()) return false;

   for (const auto& obstype : l.getObstypes())
   {
      for (int i = 0; i < l.size(); ++i)
      {
         if (l.data(i, obstype) != r.data(i, obstype))
         {
            std::cerr << "Sat Passes Not Equal at index (" << i << ", " << obstype << "):"
               << " Expected " << l.data(i, obstype) << " but got " << r.data(i, obstype) << std::endl;
            return false;
         }
      }
   }

   return true;
}

void outputCmdComparison(const std::vector<std::string>& l, const std::vector<std::string>& r)
{
   std::cerr << "GDC cmds not equivalent." << std::endl;
   for (int i = 0; i < std::max(l.size(), r.size()); ++i)
   {
      if (i < l.size())
      {
         std::cerr << "< " << l.at(i);
      }

      if (i < r.size())
      {
         std::cerr << '\n' << "> " << r.at(i);
      }
      std::cerr << '\n';
   }
}


bool cmdsEqual(const std::vector<std::string>& l, const std::vector<std::string>& r)
{
   if (l.size() != r.size())
   {
      outputCmdComparison(l, r);
      return false;
   }

   for (int i = 0; i < l.size(); ++i)
   {
      if (l.at(i) != r.at(i))
      {
         outputCmdComparison(l, r);
         return false;
      }
   }
   return true;
}


struct DiscCorr_T
{
   unsigned testFixCmds();
};


unsigned DiscCorr_T :: testFixCmds()
{
   TUDEF("DiscCorr", "Fixing");

   std::vector<std::string> filePaths {
      {gnsstk::getPathData() + gnsstk::getFileSep() + "test_dfix_karr0880.ed.10o"}
   };

      // empty means load everything.
   std::vector<std::string> obstypes;
   std::vector<SatPass> satPasses;

   int rc = SatPassFromRinexFiles(
      filePaths, 
      obstypes,
      30.0,  // data interval in seconds
      satPasses
   );
      // The return code should be the number of files successfully read in. 
   TUASSERTE(int, 1, rc);
      // There is only a single satellite in the test file.
   TUASSERTE(unsigned, 1, satPasses.size());
   SatPass pass = satPasses.at(0);
   TUASSERTE(unsigned, 187, pass.size());
      // There are 7 obs types in the RINEX files: L1 L2 C1 P2 P1 D1 D2
   TUASSERTE(unsigned, 7, pass.getObsTypes().size());
   
   GDCconfiguration config;
   config.setParameter("DT=30");
   config.setParameter("Debug=7");
   std::string outputMsg;
   std::vector<std::string> outputCmds;
   SatPass originalPass(pass);
   int dcrc = DiscontinuityCorrector(pass, config, outputCmds, outputMsg);
   std::cerr << outputMsg << '\n';
   TUASSERTE(int, 0, dcrc);
   TUASSERT(!satPassEqual(originalPass, pass));

   std::vector<std::string> expectedCmds{
      "-DS+G28,2010,3,29,1,15,0.000000 # begin delete of 10 points",
      "-DS-G28,2010,3,29,1,30,0.000000 # end delete of 10 points",
      "-BD+G28,L1,2010,3,29,1,30,30.000000,-371175 # WL GF",
      "-BD+G28,L2,2010,3,29,1,30,30.000000,-311956 # WL GF",
      "-DSG28,2010,3,29,1,44,30.000000"
   };
   TUASSERT(cmdsEqual(expectedCmds, outputCmds));

   TURETURN();
}


int main(int argc, char *argv[])
{
   int  errorTotal = 0;

   DiscCorr_T testClass;  // test data is loaded here

   errorTotal += testClass.testFixCmds();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;

} // main()