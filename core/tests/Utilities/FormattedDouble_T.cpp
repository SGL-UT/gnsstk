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
#include "FormattedDouble.hpp"

namespace std
{
   std::ostream& operator<<(std::ostream& s, gnsstk::StringUtils::FFLead v)
   {
      s << static_cast<int>(v);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::StringUtils::FFSign v)
   {
      s << static_cast<int>(v);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::StringUtils::FFAlign v)
   {
      s << static_cast<int>(v);
      return s;
   }
}

// EXP = expected value
// FD = FormattedDouble object
// VAL = double precision value expected after reading (precision is
//   being thrown away deliberately in the formatting tests, so we
//   need this).
#define STRMASSERT(EXP, FD, VAL)                                        \
   {                                                                    \
      std::ostringstream oss;                                           \
      oss << FD;                                                        \
      std::string formatted = oss.str();                                \
      TUCSM("operator<<");                                              \
      TUASSERTE(std::string, EXP, formatted);                           \
      std::istringstream iss(formatted);                                \
      gnsstk::FormattedDouble fdin(FD.totalLen, FD.exponentChar);        \
      iss >> fdin;                                                      \
      TUCSM("operator>>");                                              \
      TUASSERTFE(VAL, fdin.val);                                        \
      gnsstk::FormattedDouble fdin2(FD);                                 \
      fdin2 = formatted;                                                \
      TUCSM("operator=(string)");                                       \
      TUASSERTFE(VAL, fdin2.val);                                       \
   }

class FormattedDouble_T
{
public:
   unsigned constructorTest();
   unsigned streamTest();
   unsigned castTest();
   unsigned scaleTest();
};


unsigned FormattedDouble_T ::
constructorTest()
{
   TUDEF("FormattedDouble", "");
      // please don't mess with defaults...
   double d(1.234);
   gnsstk::FormattedDouble t1(d,gnsstk::StringUtils::FFLead::Zero);
   gnsstk::FormattedDouble t2(d,gnsstk::StringUtils::FFLead::Decimal, 99, 12, 128,
                             'F', gnsstk::StringUtils::FFSign::NegPos,
                             gnsstk::StringUtils::FFAlign::Right);
   gnsstk::FormattedDouble t3;

   TUCSM("FormattedDouble(double,...)");
   TUASSERTFE(d, t1.val);
   TUASSERTE(gnsstk::StringUtils::FFLead,
             gnsstk::StringUtils::FFLead::Zero, t1.leadChar);
   TUASSERTE(unsigned, 0, t1.mantissaLen);
   TUASSERTE(unsigned, 2, t1.exponentLen);
   TUASSERTE(unsigned, 0, t1.totalLen);
   TUASSERTE(char, 'e', t1.exponentChar);
   TUASSERTE(gnsstk::StringUtils::FFSign,
             gnsstk::StringUtils::FFSign::NegOnly, t1.leadSign);
   TUASSERTE(gnsstk::StringUtils::FFAlign,
             gnsstk::StringUtils::FFAlign::Left, t1.alignment);

   TUASSERTFE(d, t2.val);
   TUASSERTE(gnsstk::StringUtils::FFLead,
             gnsstk::StringUtils::FFLead::Decimal, t2.leadChar);
   TUASSERTE(unsigned, 99, t2.mantissaLen);
   TUASSERTE(unsigned, 12, t2.exponentLen);
   TUASSERTE(unsigned, 128, t2.totalLen);
   TUASSERTE(char, 'F', t2.exponentChar);
   TUASSERTE(gnsstk::StringUtils::FFSign,
             gnsstk::StringUtils::FFSign::NegPos, t2.leadSign);
   TUASSERTE(gnsstk::StringUtils::FFAlign,
             gnsstk::StringUtils::FFAlign::Right, t2.alignment);

   TUCSM("FormattedDouble(unsigned,char)");
   TUASSERTFE(0., t3.val);
   TUASSERTE(gnsstk::StringUtils::FFLead,
             gnsstk::StringUtils::FFLead::NonZero, t3.leadChar);
   TUASSERTE(unsigned, 0, t3.mantissaLen);
   TUASSERTE(unsigned, 2, t3.exponentLen);
   TUASSERTE(unsigned, 0, t3.totalLen);
   TUASSERTE(char, 'e', t3.exponentChar);
   TUASSERTE(gnsstk::StringUtils::FFSign,
             gnsstk::StringUtils::FFSign::NegOnly, t3.leadSign);
   TUASSERTE(gnsstk::StringUtils::FFAlign,
             gnsstk::StringUtils::FFAlign::Left, t3.alignment);
   TURETURN();
}


unsigned FormattedDouble_T ::
streamTest()
{
   TUDEF("FormattedDouble", "operator<<");

   double p1 = 1.2345;  // positive test value
   double n1 = -1.2345; // negative test value
   gnsstk::FormattedDouble t1(p1,gnsstk::StringUtils::FFLead::Zero,5,2);
   gnsstk::FormattedDouble t2(p1,gnsstk::StringUtils::FFLead::Decimal,5,2);
   gnsstk::FormattedDouble t3(p1,gnsstk::StringUtils::FFLead::NonZero,5,2);
   gnsstk::FormattedDouble t4(p1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'d');
   gnsstk::FormattedDouble t5(p1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'D');
   gnsstk::FormattedDouble t6(p1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'x');
   gnsstk::FormattedDouble t7(p1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'E',
                             gnsstk::StringUtils::FFSign::NegOnly);
   gnsstk::FormattedDouble t8(p1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'E',
                             gnsstk::StringUtils::FFSign::NegSpace);
   gnsstk::FormattedDouble t9(p1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'E',
                             gnsstk::StringUtils::FFSign::NegPos);
   gnsstk::FormattedDouble t10(p1,gnsstk::StringUtils::FFLead::Zero,5,4,0,'E',
                              gnsstk::StringUtils::FFSign::NegPos);
   gnsstk::FormattedDouble t11(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0);
   gnsstk::FormattedDouble t12(n1,gnsstk::StringUtils::FFLead::Decimal,5,2,0);
   gnsstk::FormattedDouble t13(n1,gnsstk::StringUtils::FFLead::NonZero,5,2,0);
   gnsstk::FormattedDouble t14(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'d');
   gnsstk::FormattedDouble t15(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'D');
   gnsstk::FormattedDouble t16(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'x');
   gnsstk::FormattedDouble t17(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'E',
                              gnsstk::StringUtils::FFSign::NegOnly);
   gnsstk::FormattedDouble t18(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'E',
                              gnsstk::StringUtils::FFSign::NegSpace);
   gnsstk::FormattedDouble t19(n1,gnsstk::StringUtils::FFLead::Zero,5,2,0,'E',
                              gnsstk::StringUtils::FFSign::NegPos);
   gnsstk::FormattedDouble t20(n1,gnsstk::StringUtils::FFLead::Zero,5,4,0,'E',
                              gnsstk::StringUtils::FFSign::NegPos);

   STRMASSERT("0.1234e+01", t1, 0.1234e+01);
   STRMASSERT(".12345e+01", t2, .12345e+01);
   STRMASSERT("1.2345e+00", t3, 1.2345e+00);
   STRMASSERT("0.1234d+01", t4, 0.1234e+01);
   STRMASSERT("0.1234D+01", t5, 0.1234e+01);
   STRMASSERT("0.1234x+01", t6, 0.1234e+01);
   STRMASSERT("0.1234E+01", t7, 0.1234E+01);
   STRMASSERT(" 0.1234E+01", t8,  0.1234E+01);
   STRMASSERT("+0.1234E+01", t9, +0.1234E+01);
   STRMASSERT("+0.1234E+0001", t10, +0.1234E+0001);

   STRMASSERT("-0.1234e+01", t11, -0.1234e+01);
   STRMASSERT("-.12345e+01", t12, -.12345e+01);
   STRMASSERT("-1.2345e+00", t13, -1.2345e+00);
   STRMASSERT("-0.1234d+01", t14, -0.1234e+01);
   STRMASSERT("-0.1234D+01", t15, -0.1234e+01);
   STRMASSERT("-0.1234x+01", t16, -0.1234e+01);
   STRMASSERT("-0.1234E+01", t17, -0.1234E+01);
   STRMASSERT("-0.1234E+01", t18, -0.1234E+01);
   STRMASSERT("-0.1234E+01", t19, -0.1234E+01);
   STRMASSERT("-0.1234E+0001", t20, -0.1234E+0001);

   TURETURN();
}


unsigned FormattedDouble_T ::
castTest()
{
   TUDEF("FormattedDouble", "operator=(double)");

   gnsstk::FormattedDouble t1;
   gnsstk::FormattedDouble &t1ref(t1 = 9.0);
   TUASSERTFE(9.0, t1.val);
   TUASSERTE(gnsstk::FormattedDouble*, &t1, &t1ref);
   double foo = t1 * 123.0;
   TUCSM("operator double()");
   TUASSERTFE(1107.0, foo);
   double bar = t1;
   TUASSERTFE(9.0, bar);

   TUCSM("operator=(double)");
   gnsstk::FormattedDouble t2(123.0, gnsstk::StringUtils::FFLead::Zero, 10, 3, 0,
                             'x', gnsstk::StringUtils::FFSign::NegPos);
   t2 = 9.0;
   TUASSERTFE(9.0, t2.val);
      // make sure the assignment didn't reset any existing data fields
   TUASSERTE(gnsstk::StringUtils::FFLead,
             gnsstk::StringUtils::FFLead::Zero, t2.leadChar);
   TUASSERTE(unsigned, 10, t2.mantissaLen);
   TUASSERTE(unsigned, 3, t2.exponentLen);
   TUASSERTE(char, 'x', t2.exponentChar);
   TUASSERTE(gnsstk::StringUtils::FFSign,
             gnsstk::StringUtils::FFSign::NegPos, t2.leadSign);

      // implicit conversions
   unsigned u = 9;
   gnsstk::FormattedDouble t3;
   t3 = u;
   TUASSERTFE(9.0, t3.val);
   int i = 9;
   gnsstk::FormattedDouble t4;
   t4 = i;
   TUASSERTFE(9.0, t4.val);

   TURETURN();
}


unsigned FormattedDouble_T ::
scaleTest()
{
   TUDEF("FormattedDouble", "");
   double d(100.0);
   gnsstk::FormattedDouble t1(d,gnsstk::StringUtils::FFLead::Decimal, 99, 12, 128,
                             'F', gnsstk::StringUtils::FFSign::NegPos,
                             gnsstk::StringUtils::FFAlign::Right);
   gnsstk::FormattedDouble t2 = t1 / 2.0;
   gnsstk::FormattedDouble t3 = t1 * 0.5;

   TUCSM("operator/(double)");
   TUASSERTFE(50.0, t2.val);
   TUASSERTE(gnsstk::StringUtils::FFLead,
             gnsstk::StringUtils::FFLead::Decimal, t2.leadChar);
   TUASSERTE(unsigned, 99, t2.mantissaLen);
   TUASSERTE(unsigned, 12, t2.exponentLen);
   TUASSERTE(unsigned, 128, t2.totalLen);
   TUASSERTE(char, 'F', t2.exponentChar);
   TUASSERTE(gnsstk::StringUtils::FFSign,
             gnsstk::StringUtils::FFSign::NegPos, t2.leadSign);
   TUASSERTE(gnsstk::StringUtils::FFAlign,
             gnsstk::StringUtils::FFAlign::Right, t2.alignment);

   TUCSM("operator*(double)");
   TUASSERTFE(50.0, t3.val);
   TUASSERTE(gnsstk::StringUtils::FFLead,
             gnsstk::StringUtils::FFLead::Decimal, t3.leadChar);
   TUASSERTE(unsigned, 99, t3.mantissaLen);
   TUASSERTE(unsigned, 12, t3.exponentLen);
   TUASSERTE(unsigned, 128, t3.totalLen);
   TUASSERTE(char, 'F', t3.exponentChar);
   TUASSERTE(gnsstk::StringUtils::FFSign,
             gnsstk::StringUtils::FFSign::NegPos, t3.leadSign);
   TUASSERTE(gnsstk::StringUtils::FFAlign,
             gnsstk::StringUtils::FFAlign::Right, t3.alignment);

   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   FormattedDouble_T testClass;
   errorTotal += testClass.constructorTest();
   errorTotal += testClass.streamTest();
   errorTotal += testClass.castTest();
   errorTotal += testClass.scaleTest();
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;
   return errorTotal;
}
