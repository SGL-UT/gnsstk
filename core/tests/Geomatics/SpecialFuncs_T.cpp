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
//  Copyright 2004-2024, The Board of Regents of The University of Texas System
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

#include "SpecialFuncs.hpp"
#include "TestUtil.hpp"

using namespace std;
using namespace gnsstk;

class SpecialFuncs_T
{
public:
    int lnGammaTest(void)
    {
        TUDEF("SpecialFuncs", "lnGamma");
        // less than zero should throw
        TUTHROW(lnGamma(-1.0));
        // otherwise, should get us natural log of gamma function
        TUASSERTFEPS(lnGamma(1.0), 0, 1e-8);
        TUASSERTFEPS(lnGamma(2.0), 0, 1e-8);
        TURETURN();
    }
    int GammaTest(void)
    {
        TUDEF("SpecialFuncs", "Gamma");
        // less than zero should throw
        TUTHROW(Gamma(-1.0));
        // normal cases
        TUASSERTFEPS(Gamma(1.0), 1, 1e-8);
        TUASSERTFEPS(Gamma(2.0), 1, 1e-8);
        TURETURN();
    }
    int factorialTest(void)
    {
        TUDEF("SpecialFuncs", "factorial");
        // throw for negative arg
        TUTHROW(factorial(-3));
        // n>32 does gamma distro calculation
        TUASSERTFEPS(factorial(33), 8.6833176191009903954e+36, 1e20);
        // n>5 <=32 will actually do the loop to calculate instead of using hard
        // coded values.
        TUASSERTFEPS(factorial(6), 720, 1e-8);
        TURETURN();
    }
    int lnFactorialTest(void)
    {
        TUDEF("SpecialFuncs", "lnFactorial");
        // throw for negative arg
        TUTHROW(lnFactorial(-3));
        // n <= 1 will return 0 automatically, short circuit
        TUASSERTFEPS(lnFactorial(0), 0.0, 1e-8);
        TUASSERTFEPS(lnFactorial(1), 0.0, 1e-8);
        // otherwise, it'll do lnGamma with arg + 1, comparing against numpy result.
        TUASSERTFEPS(lnFactorial(2), 0.6931471805599453, 1e-8);
        TURETURN();
    }
    int binomialCoeffTest(void)
    {
        TUDEF("SpecialFuncs", "binomialCoeff");
        // die on n < 0
        TUTHROW(binomialCoeff(-1, 1));
        // die on k > n
        TUTHROW(binomialCoeff(3, 4));
        // n <= 32 case, 5 pick 5 should only result in 1.
        TUASSERTFEPS(binomialCoeff(5, 5), 1.0, 1e-8);
        // ok but with n > 32, it'll do the big lnFactorial calc. but we're gonna do
        // 33 pick 1 so it's just 33.
        TUASSERTFEPS(binomialCoeff(33, 1), 33.0, 1e-8);
        TURETURN();
    }
    int betaTest(void)
    {
        TUDEF("SpecialFuncs", "beta");
        // since it uses natural log gamma, <0 should die
        TUTHROW(beta(-1, 1));
        TUTHROW(beta(1, -1));
        // and then normal
        TUASSERTFEPS(beta(1, 1), 1.0, 1e-8);
        TURETURN();
    }
    int seriesIncompGammaTest(void)
    {
        TUDEF("SpecialFuncs", "seriesIncompGamma");
        // negative second argument
        TUTHROW(seriesIncompGamma(1, -1));
        // nonpositive first arg
        TUTHROW(seriesIncompGamma(0, 1));
        // first arg too big
        TUTHROW(seriesIncompGamma(1e100, 1e100));
        // get an actual value
        TUASSERTFEPS(seriesIncompGamma(3, 3), 0.5768099188731566, 1e-8);
        TURETURN();
    }
    int contfracIncompGammaTest(void)
    {
        TUDEF("SpecialFuncs", "contfracIncompGamma");
        TUTHROW(contfracIncompGamma(-1, 1));
        // nonpositive second arg
        TUTHROW(contfracIncompGamma(1, 0));
        // first arg too big
        TUTHROW(contfracIncompGamma(1e100, 1e100));
        // get an actual value
        TUASSERTFEPS(contfracIncompGamma(.5, .5), 1 - 0.6826894921370859, 1e-8);
        TURETURN();
    }
    int incompGammaTest(void)
    {
        TUDEF("SpecialFuncs", "incompGamma");
        // negative second arg
        TUTHROW(incompGamma(1, -1));
        // nonpositive first arg
        TUTHROW(incompGamma(0, 1));
        // first arg too big
        TUTHROW(incompGamma(1e100, 1e100));
        // get an actual value
        TUASSERTFEPS(incompGamma(3, 3), 0.5768099188731566, 1e-8);
        // series series and contfrac based on inputs.
        TUASSERTFEPS(incompGamma(.5, .5), 0.6826894921370859, 1e-8);
        TURETURN();
    }
    int compIncompGammaTest(void)
    {
        TUDEF("SpecialFuncs", "compIncompGamma");
        // negative second arg
        TUTHROW(compIncompGamma(1, -1));
        // nonpositive first arg
        TUTHROW(compIncompGamma(0, 1));
        // first arg too big
        TUTHROW(compIncompGamma(1e100, 1e100));
        // get an actual value
        TUASSERTFEPS(compIncompGamma(3, 3), 1 - 0.5768099188731566, 1e-8);
        // series series and contfrac based on inputs.
        TUASSERTFEPS(compIncompGamma(.5, .5), 1 - 0.6826894921370859, 1e-8);
        TURETURN();
    }
    int errorFuncTest(void)
    {
        TUDEF("SpecialFuncs", "errorFunc");
        TUTHROW(errorFunc(-1.0));
        TUASSERTFEPS(errorFunc(0), 0, 1e-8);
        TURETURN();
    }
    int cfIBetaTest(void)
    {
        TUDEF("SpecialFuncs", "cfIBeta");
        // a or b too big
        TUTHROW(cfIBeta(.78, 1e200, 1e200));
        TUCATCH(cfIBeta(.5, 2, 2));
        TURETURN();
    }
    int incompleteBetaTest(void)
    {
        TUDEF("SpecialFuncs", "incompleteBeta");
        // invalid x values, must be between 0 and 1
        TUTHROW(incompleteBeta(-1, 1, 1));
        TUTHROW(incompleteBeta(2, 1, 1));
        // a and be cannot be less than 0
        TUTHROW(incompleteBeta(.5, -1, 1));
        TUTHROW(incompleteBeta(.5, 1, -1));
        // shortcircuit special conditions
        TUASSERTFEPS(incompleteBeta(0, 1, 1), 0.0, 1e-8);
        TUASSERTFEPS(incompleteBeta(1, 1, 1), 1.0, 1e-8);
        TUASSERTFEPS(incompleteBeta(.5, 2, 2), .5, 1e-8);
        TURETURN();
    }
    int NormalPDFTest(void)
    {
        TUDEF("SpecialFuncs", "NormalPDF");
        TUASSERTFEPS(NormalPDF(0, 0, 1), 0.3989422804014327, 1e-8);
        TURETURN();
    }
    int NormalCDFTest(void)
    {
        TUDEF("SpecialFuncs", "NormalCDF");
        // negative variance
        TUTHROW(NormalCDF(0, 0, -1));
        // normal calculation
        TUASSERTFEPS(NormalCDF(0, 0, 1), .5, 1e-8);
        TURETURN();
    }
    int invNormalCDFTest(void)
    {
        TUDEF("SpecialFuncs", "invNormalCDF");
        // prob range
        TUTHROW(invNormalCDF(-.5, 0, 1));
        TUTHROW(invNormalCDF(.5, 0, -1));
        // corner case, close to 0
        TUASSERTFEPS(invNormalCDF(0, 0, 1), 0, 1e-16);
        // close to 1
        TUTHROW(invNormalCDF(.99999999999999999999, 0, 1));
        // normal calculation
        TUASSERTFEPS(invNormalCDF(.4999999999999, 0, 1), 0, 1e-8);
        TURETURN();
    }
    int ChisqPDFTest(void)
    {
        TUDEF("SpecialFuncs", "ChisqPDF");
        // negative statistic
        TUTHROW(ChisqPDF(-1, 3));
        // non-positive DOF
        TUTHROW(ChisqPDF(0, -1));
        TUASSERTFEPS(ChisqPDF(1, 1), 0.24197072451914337, 1e-8);
        TURETURN();
    }
    int ChisqCDFTest(void)
    {
        TUDEF("SpecialFuncs", "ChisqCDFTest");
        // negative statistic
        TUTHROW(ChisqCDF(-1, 3));
        // non-positive DOF
        TUTHROW(ChisqCDF(0, -1));
        TUASSERTFEPS(ChisqCDF(1, 1), 0.6826894921370859, 1e-8);
        TURETURN();
    }
    int invChisqCDFTest(void)
    {
        TUDEF("SpecialFuncs", "invChisqCDF");
        // negative prob
        TUTHROW(invChisqCDF(-1, 1));
        // non positive DOF
        TUTHROW(invChisqCDF(.5, 0));
        // close to 0
        TUASSERTFEPS(invChisqCDF(0, 1), 0, 1e-16);
        // close to 1
        TUTHROW(invChisqCDF(.99999999999999999999, 1));
        // calculation then
        TUASSERTFEPS(invChisqCDF(0.6826894921370859, 1), 1, 1e-8);
        // TODO: can we hit the failure to converge case???
        TURETURN();
    }
    int StudentsPDFTest(void)
    {
        TUDEF("SpecialFuncs", "StudentsPDF");
        // non positive DOF
        TUTHROW(StudentsPDF(0, -11));
        TUASSERTFEPS(StudentsPDF(0, 1), 0.31830988618379075, 1e-8);
        TURETURN();
    }
    int StudentsCDFTest(void)
    {
        TUDEF("SpecialFuncs", "StudentsCDFTest");
        TUTHROW(StudentsCDF(0, -11));
        TUASSERTFEPS(StudentsCDF(0, 1), .5, 1e-8);
        TURETURN();
    }
    int invStudentsCDFTest(void)
    {
        TUDEF("SpecialFuncs", "invStudentsCDFTest");
        // prob < 0
        TUTHROW(invStudentsCDF(-1, 1));
        // n < 0
        TUTHROW(invStudentsCDF(0, -1));
        // short circuit 0
        TUASSERTFEPS(invStudentsCDF(0, 1), 0, 1e-8);
        // short circuit near one
        TUTHROW(invStudentsCDF(.99999999999999999999, 1));
        // actual calculation
        TUASSERTFEPS(invStudentsCDF(.5, 1), 0, 1e-8);
        TURETURN();
    }
    int FDistPDFTest(void)
    {
        TUDEF("SpecialFuncs", "FDistPDFTest");
        TUASSERTFEPS(FDistPDF(1, 2, 3), 0.27885480092693415, 1e-8);
        TURETURN();
    }
    int FDistCDFTest(void)
    {
        TUDEF("SpecialFuncs", "FDistCDFTest");
        TUASSERTFEPS(FDistCDF(1, 2, 3), 0.53524199845511, 1e-8);
        TURETURN();
    }
    int invFDistCDFTest(void)
    {
        TUDEF("SpecialFuncs", "invFDistCDFTest");
        // prob range
        TUTHROW(invFDistCDF(-1, 1, 1));
        // n1 n2 <= 0
        TUTHROW(invFDistCDF(.5, -1, 1));
        // near 0
        TUASSERTFEPS(invFDistCDF(0, 1, 1), 0, 1e-8);
        // near 1
        TUTHROW(invFDistCDF(.99999999999999999999, 1, 1));
        // normal calculation
        TUASSERTFEPS(invFDistCDF(0.53524199845511, 2, 3), 1, 1e-8);
        TURETURN();
    }
};

int main()
{
    int errorCount = 0;
    SpecialFuncs_T tester;
    errorCount += tester.lnGammaTest();
    errorCount += tester.GammaTest();
    errorCount += tester.factorialTest();
    errorCount += tester.lnFactorialTest();
    errorCount += tester.binomialCoeffTest();
    errorCount += tester.betaTest();
    errorCount += tester.seriesIncompGammaTest();
    errorCount += tester.contfracIncompGammaTest();
    errorCount += tester.incompGammaTest();
    errorCount += tester.compIncompGammaTest();
    errorCount += tester.errorFuncTest();
    errorCount += tester.cfIBetaTest();
    errorCount += tester.incompleteBetaTest();
    errorCount += tester.NormalPDFTest();
    errorCount += tester.NormalCDFTest();
    errorCount += tester.invNormalCDFTest();
    errorCount += tester.ChisqPDFTest();
    errorCount += tester.ChisqCDFTest();
    errorCount += tester.invChisqCDFTest();
    errorCount += tester.StudentsPDFTest();
    errorCount += tester.StudentsCDFTest();
    errorCount += tester.invStudentsCDFTest();
    errorCount += tester.FDistPDFTest();
    errorCount += tester.FDistCDFTest();
    errorCount += tester.invFDistCDFTest();
    return errorCount;
}