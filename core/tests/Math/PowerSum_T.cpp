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

#include "Stats.hpp"
#include "PowerSum.hpp"
#include "TestUtil.hpp"
#include <map>

using namespace std;

class PowerSum_T
   {
   private:
      gnsstk::PowerSum ps;         // Main uut variable. Goes through heavy initialization
      gnsstk::Stats<double> s;

      // Utility functions
      double  initialize_data ();
      std::map<std::string, std::string>  parseAttributes(const std::string& input);

   public:
      unsigned dumpTest(); 
      unsigned AddSubtractITest();   // Calls add() & subtract() functions
      unsigned momentTest();
      unsigned averageTest();   
      unsigned varianceTest();
      unsigned skewTest();   
      unsigned kurtosisTest();   

      PowerSum_T ()
      {
         for (int i=0; i<100000; i++)
         {
            double rv = initialize_data();
            ps.add(rv);
            s.Add(rv);
         }
      };
   };


// Generate a norimally distributed deviate with zero mean and unit variance.
// Kudos to Press, Flannery, Teukolsky, and Veterling. And the man, Knuth.
// If you don't care about being thread safe, you can make v1 a static and use
// for every other call. Also this uses the libc standard rand() function
// which really blows by most accounts. If you want this to be a 'good'
// normal distribution, use a different rand()
double PowerSum_T ::
initialize_data()
{
   const double mr2 = 2.0 / RAND_MAX;
   double v1, v2, fac;

   double r;
   do
   {
      v1 = mr2*rand() - 1;
      v2 = mr2*rand() - 1;
      r = v1*v1 + v2*v2;
   } while (r >= 1);
   fac = sqrt(-2*log(r)/r);
   return v2*fac;
};


unsigned PowerSum_T ::
dumpTest()
{
   TUDEF("PowerSum", "dump");
   gnsstk::PowerSum psTmp;
   std::stringstream dumpOutputStream;
   psTmp.dump(dumpOutputStream);
   TUASSERTE(bool, dumpOutputStream.str().empty(), false);  
   TURETURN();
};


unsigned PowerSum_T ::
AddSubtractITest()
{
   TUDEF("PowerSum", "add_subtract_itertors");
   gnsstk::PowerSum psTmp;
   std::stringstream dumpOutputStream;
   std::list<double> values1 = {1, 1, 1, 1,};
   psTmp.add(values1.cbegin(), values1.cend());
   std::list<double> values2 = {1, 1, 1};
   psTmp.subtract(values2.cbegin(), values2.cend());
   bool result = psTmp.average() == 1.;
   TUASSERTE(bool, result, true);
   TURETURN();
};

unsigned PowerSum_T ::
momentTest()
{
   TUDEF("PowerSum", "moment");
   double expected = 0.66666666666666663;
   double epsilon =  1e-8;
   gnsstk::PowerSum psTmp;
   psTmp.clear();
   std::stringstream dumpOutputStream;
   std::list<double> values = {1, 2, 3};
   psTmp.add(values.cbegin(), values.cend());
   double result = psTmp.moment(2);
   TUASSERTFEPS (expected, result, epsilon);
   TURETURN();
};


unsigned PowerSum_T ::
averageTest()
{
   TUDEF("PowerSum", "average");
   double e1 = std::abs(s.Average() - ps.average());
   TUASSERT(e1 < 1e-3);
   TUASSERT(std::abs(ps.average()) < 1e-3);
   TURETURN();
};


unsigned PowerSum_T ::
varianceTest()
{
   TUDEF("PowerSum", "variance");
   double e2 = std::abs(s.StdDev() - sqrt(ps.variance()));
   TUASSERT(e2 < 1e-3);
   TUASSERT(std::abs(sqrt(ps.variance())-1) < 2e-3);
   TURETURN();
};


unsigned PowerSum_T ::
skewTest()
{
   TUDEF("PowerSum", "skew");
   TUASSERT(std::abs(ps.skew()) < 0.01);
   TURETURN();
};


unsigned PowerSum_T ::
kurtosisTest()
{
   TUDEF("PowerSum", "kurtosis");
   TUASSERT(std::abs(ps.kurtosis()-3) < 0.05);
   TURETURN();
};


int main()
{
   PowerSum_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.dumpTest(); 
   errorTotal += testClass.AddSubtractITest(); 
   errorTotal += testClass.momentTest();
   errorTotal += testClass.averageTest();   
   errorTotal += testClass.varianceTest();
   errorTotal += testClass.skewTest();   
   errorTotal += testClass.kurtosisTest();   

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
