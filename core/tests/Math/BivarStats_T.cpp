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

#include "BivarStats.hpp"
#include "Vector.hpp"
#include "TestUtil.hpp"
#include <iostream>

class BivarStats_T
{
        public:
		BivarStats_T(){eps = 1e-12;}// Default Constructor, set the precision value
		~BivarStats_T() {} // Default Desructor
		double eps;
		int addTest()
		// Test to add data. Want to add single values to empty Stats class.
		// Then add another stat on top with weight. I will use the average to check
		// that data was added and that the data added was correct.
		{
                   gnsstk::TestUtil testFramework("BivarStats","Add()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The Add method was unsuccessful";
			//std::cout << "The Average is: " << test.average() << std::endl;
			testFramework.assert(test.averageX() == 3, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int AverageXTest()
		// Verify the X average calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","AverageX()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The AverageX method was unsuccessful";
			//std::cout << "The Average of X is: " << test.averageX() << std::endl;
			testFramework.assert(test.averageX() == 3, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int AverageYTest()
		// Verify the Y average calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","AverageY()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The AverageY method was unsuccessful";
			//std::cout << "The Average of Y is: " << test.averageY() << std::endl;
			testFramework.assert(test.averageY() == 3, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int MaxXTest()
		// Verify the X maximum calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","MaximumX()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The MaximumX method was unsuccessful";
			//std::cout << "The Maximum of X is: " << test.maximumX() << std::endl;
			testFramework.assert(test.maximumX() == 5, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int MaxYTest()
		// Verify the Y maximum calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","MaximumY()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The MaximumY method was unsuccessful";
			//std::cout << "The Maximum of Y is: " << test.maximumY() << std::endl;
			testFramework.assert(test.maximumY() == 5, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int MinXTest()
		// Verify the X minimum calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","MinimumX()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The MinimumX method was unsuccessful";
			//std::cout << "The Minimum of X is: " << test.minimumX() << std::endl;
			testFramework.assert(test.minimumX() == 1, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int MinYTest()
		// Verify the Y minimum calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","MinimumY()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The MinimumY method was unsuccessful";
			//std::cout << "The Minimum of Y is: " << test.minimumY() << std::endl;
			testFramework.assert(test.minimumY() == 1, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int VarianceXTest()
		// Verify the X variance calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","VarianceX()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The VarianceX method was unsuccessful";
			//std::cout << "The Variance of X is: " << test.varianceX() << std::endl;
			testFramework.assert(fabs(test.varianceX() - 2.5) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int VarianceYTest()
		// Verify the Y variance calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","VarianceY()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The VarianceY method was unsuccessful";
			//std::cout << "The Variance of Y is: " << test.varianceY() << std::endl;
			testFramework.assert(fabs(test.varianceY() - 2.5) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int StdDevXTest()
		// Verify the X Standard Deviation calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","StdDevX()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The stdDevX method was unsuccessful";
			//std::cout << "The Standard Deviation of X is: " << test.stdDevX() << std::endl;
			testFramework.assert(fabs(test.stdDevX() - sqrt(2.5)) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}
		int StdDevYTest()
		// Verify the Y Standard Deviation calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","StdDevY()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The stdDevY method was unsuccessful";
			//std::cout << "The Standard Deviation of Y is: " << test.stdDevY() << std::endl;
			testFramework.assert(fabs(test.stdDevY() - sqrt(2.5)) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int SlopeTest()
		// Verify the Slope calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","Slope()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The Slope method was unsuccessful";
			//std::cout << "The Slope is: " << test.slope() << std::endl;
			testFramework.assert(fabs(test.slope() - 0.1) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int InterceptTest()
		// Verify the Intercept calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","Intercept()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The Intercept method was unsuccessful";
			//std::cout << "The Intercept is: " << test.intercept() << std::endl;
			testFramework.assert(fabs(test.intercept() - 2.7) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int SlopeUncertaintyTest()
		// Verify the Slope Uncertainty calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","SlopeUncertainty()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The sigmaSlope method was unsuccessful";
			//std::cout << "The Slope Uncertainty is: " << test.sigmaSlope() << std::endl;
			testFramework.assert(fabs(test.sigmaSlope() - 0.574456264653803) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int ConditionalUncertaintyTest()
		// Verify the Conditional Uncertainty calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","ConditionalUncertaintyTest()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The sigmaYX method was unsuccessful";
			//std::cout << "The Conditional Uncertainty is: " << test.sigmaYX() << std::endl;
			testFramework.assert(fabs(test.sigmaYX() - 1.81659021245849) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int CorrelationTest()
		// Verify the Correlation calculation.
		{
			gnsstk::TestUtil testFramework("BivarStats","Correlation()",__FILE__,__LINE__);
			std::string failMesg;

			gnsstk::BivarStats<double> test;
			test.add(1, 2);
			test.add(2, 5);
			test.add(3, 1);
			test.add(4, 4);
			test.add(5, 3);

			failMesg  = "The correlation method was unsuccessful";
			//std::cout << "The Correlation is: " << test.correlation() << std::endl;
			testFramework.assert(fabs(test.correlation() - 0.1) < eps, failMesg, __LINE__);
			return testFramework.countFails();
		}

		int ConstructorTest()
		{
			// by nature of the constructors being written using Add, these are also Add Tests. 
			TUDEF("BiVarStats", "constructors");
			TUCATCH(gnsstk::BivarStats<double>());
			TUCATCH(gnsstk::BivarStats<double>(1, 2, true));
			std::vector<double> x = {1.0, 2.0, 3.0};
			std::vector<double> y = {-1.0, -2.0, -3.0};
			TUCATCH(gnsstk::BivarStats<double>(x, y, false));
			std::vector<std::pair<double, double>> xy = {std::pair<double, double>(1.0, -1.0), std::pair<double, double>(2.0, -2.0), std::pair<double, double>(3.0, -3.0)};
			TUCATCH(gnsstk::BivarStats<double>(xy, true));
			double xxData[3] = {1.0, 2.0, 3.0};
			double yyData[3] = {-1.0, -2.0, -3.0};
			gnsstk::Vector<double> xx(3);
			xx = xxData;
			gnsstk::Vector<double> yy(3);
			yy = yyData;
			TUCATCH(gnsstk::BivarStats<double>(xx, yy, true));
			TURETURN();
		}

		int SubtractTest() {
			TUDEF("BiVarStats", "subtract");
			// let's do one scaled
			gnsstk::BivarStats<double> a(1, 2, true);
			a.add(3, 4);
			TUASSERTE(double, a.averageX(), 3.0);
			TUASSERTE(double, a.averageY(), 3.0);
			// subtract const
			a.subtract(3, 4);
			TUASSERTE(double, a.averageX(), 3.0);
			TUASSERTE(double, a.averageY(), 2.0);
			// ok the rest, we'll all do unscaled. 
			gnsstk::BivarStats<double> b(0.0, 0.0, false);
			// subtract 2 vectors
            std::vector<double> x = {1.0, 2.0, 3.0};
			std::vector<double> y = {-1.0, -2.0, -3.0};
			b.add(x,y);
			TUASSERTE(double, b.averageX(), 1.5);
			TUASSERTE(double, b.averageY(), -1.5);
			b.subtract(x, y);
			TUASSERTE(double, b.averageX(), 0);
			TUASSERTE(double, b.averageY(), 0);
			// subtract vector pairs
            std::vector<std::pair<double, double>> xy = {std::pair<double, double>(1.0, -1.0), std::pair<double, double>(2.0, -2.0), std::pair<double, double>(3.0, -3.0)};
			b.add(xy);
			TUASSERTE(double, b.averageX(), 1.5);
			TUASSERTE(double, b.averageY(), -1.5);
			b.subtract(xy);
			TUASSERTE(double, b.averageX(), 0);
			TUASSERTE(double, b.averageY(), 0)
			// subtract 2 gnsstk vectors
            double xxData[3] = {1.0, 2.0, 3.0};
			double yyData[3] = {-1.0, -2.0, -3.0};
			gnsstk::Vector<double> xx(3);
			xx = xxData;
			gnsstk::Vector<double> yy(3);
			yy = yyData;
			b.add(xx, yy);
			TUASSERTE(double, b.averageX(), 1.5);
			TUASSERTE(double, b.averageY(), -1.5);
			b.subtract(xx, yy);
			TUASSERTE(double, b.averageX(), 0);
			TUASSERTE(double, b.averageY(), 0)
			TURETURN();
		} 

		int EmptyStatsMethodsTest()
		{
			TUDEF("BiVarStats", "methods with empty BivarStats");
			gnsstk::BivarStats<double> a(1, 2);
			a.clear();
			TUASSERTE(double, a.minimumX(), 0);
			TUASSERTE(double, a.minimumY(), 0);
			TUASSERTE(double, a.maximumX(), 0);
			TUASSERTE(double, a.maximumY(), 0);
			TUASSERTE(double, a.averageX(), 0);
			TUASSERTE(double, a.averageY(), 0);
			TUASSERTE(double, a.varianceX(), 0);
			TUASSERTE(double, a.varianceY(), 0);
			TUASSERTE(double, a.stdDevX(), 0);
			TUASSERTE(double, a.stdDevY(), 0);
			TUASSERTE(double, a.slope(), 0);
			TUASSERTE(double, a.intercept(), 0);
			TUASSERTE(double, a.sigmaSlope(), 0);
			TUASSERTE(double, a.correlation(), 0);
			TUASSERTE(double, a.sigmaYX(), 0);
			TURETURN();
		}

		int CombineTest() {
			TUDEF("BiVarStats", "operator+=");
			gnsstk::BivarStats<double> a(1, 2, false);
			gnsstk::BivarStats<double> b(3, 4, false);
			a += b;
			TUASSERTE(double, a.averageX(), 2);
			TUASSERTE(double, a.averageY(), 3);
			TURETURN();
		}

		int EstimateDeviationTest() {
			TUDEF("BiVarStats", "EstimateDeviation");
			gnsstk::BivarStats<double> a(1, 2, false);
            std::vector<std::pair<double, double>> xy = {std::pair<double, double>(1.0, -1.0), std::pair<double, double>(2.0, -2.0), std::pair<double, double>(3.0, -3.0)};
            TUCATCH(a.estimateDeviation(xy));
			TURETURN();
		}

 };

int main()
{
	int check, errorCounter = 0;
	BivarStats_T testClass;

	check = testClass.addTest();
	errorCounter += check;

	check = testClass.AverageXTest();
	errorCounter += check;

	check = testClass.AverageYTest();
	errorCounter += check;

	check = testClass.MaxXTest();
	errorCounter += check;

	check = testClass.MaxYTest();
	errorCounter += check;

	check = testClass.MinXTest();
	errorCounter += check;

	check = testClass.MinYTest();
	errorCounter += check;

	check = testClass.VarianceXTest();
	errorCounter += check;

	check = testClass.VarianceYTest();
	errorCounter += check;

	check = testClass.StdDevXTest();
	errorCounter += check;

	check = testClass.StdDevYTest();
	errorCounter += check;

	check = testClass.SlopeTest();
	errorCounter += check;

	check = testClass.InterceptTest();
	errorCounter += check;

	check = testClass.ConditionalUncertaintyTest();
	errorCounter += check;

	check = testClass.SlopeUncertaintyTest();
	errorCounter += check;

	check = testClass.CorrelationTest();
	errorCounter += check;

	check = testClass.ConstructorTest();
	errorCounter += check;

	check = testClass.SubtractTest();
	errorCounter += check;

	check = testClass.EmptyStatsMethodsTest();
	errorCounter += check;
	
	check = testClass.EstimateDeviationTest();
	errorCounter += check;

	std::cout << "Total Errors: " << errorCounter << std::endl;

	return errorCounter;
}
