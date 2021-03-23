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

/** \page apps
 * - \subpage timeconvert - Convert times between a variety of formats
 * \page timeconvert
 * \tableofcontents
 *
 * \section timeconvert_name NAME
 * timeconvert - Display time stamps in a variety of formats
 *
 * \section timeconvert_synopsis SYNOPSIS
 * \b timeconvert [\argarg{OPTION}] ...
 *
 * \section timeconvert_description DESCRIPTION
 * This application allows the user to convert between time formats
 * associated with GNSS. Time formats include, but are not limited to:
 * civilian time, Julian day of year and year, GPS week and seconds of
 * week, Z counts, and Modified Julian Date (MJD).
 *
 * \dictionary
 * \dicterm{-d, \--debug}
 * \dicdef{Increase debug level}
 * \dicterm{-v, \--verbose}
 * \dicdef{Increase verbosity}
 * \dicterm{-h, \--help}
 * \dicdef{Print help usage}
 * \dicterm{-A, \--ansi=\argarg{TIME}}
 * \dicdef{"ANSI-Second"}
 * \dicterm{-c, \--civil=\argarg{TIME}}
 * \dicdef{"Month(numeric) DayOfMonth Year Hour:Minute:Second"}
 * \dicterm{-R, \--rinex-file=\argarg{TIME}}
 * \dicdef{"Year(2-digit) Month(numeric) DayOfMonth Hour Minute Second"}
 * \dicterm{-o, \--ews=\argarg{TIME}}
 * \dicdef{"GPSEpoch 10bitGPSweek SecondOfWeek"}
 * \dicterm{-f, \--ws=\argarg{TIME}}
 * \dicdef{"FullGPSWeek SecondOfWeek"}
 * \dicterm{-w, \--wz=\argarg{TIME}}
 * \dicdef{"FullGPSWeek Zcount"}
 * \dicterm{\--z29=\argarg{TIME}}
 * \dicdef{"29bitZcount"}
 * \dicterm{-Z, \--z32=\argarg{TIME}}
 * \dicdef{"32bitZcount"}
 * \dicterm{-j, \--julian=\argarg{TIME}}
 * \dicdef{"JulianDate"}
 * \dicterm{-m, \--mjd=\argarg{TIME}}
 * \dicdef{"ModifiedJulianDate"}
 * \dicterm{-u, \--unixtime=\argarg{TIME}}
 * \dicdef{"UnixSeconds UnixMicroseconds"}
 * \dicterm{-y, \--doy=\argarg{TIME}}
 * \dicdef{"Year DayOfYear SecondsOfDay"}
 * \dicterm{\--input-format=\argarg{ARG}}
 * \dicdef{Time format to use on input}
 * \dicterm{\--input-time=\argarg{ARG}}
 * \dicdef{Time to be parsed by "input-format" option}
 * \dicterm{-F, \--format=\argarg{ARG}}
 * \dicdef{Time format to use on output}
 * \dicterm{-a, \--add-offset=\argarg{NUM}}
 * \dicdef{add \argarg{NUM} seconds to specified time}
 * \dicterm{-s, \--sub-offset=\argarg{NUM}}
 * \dicdef{subtract \argarg{NUM} seconds from specified time}
 * \enddictionary
 * \section timeconvert_examples EXAMPLES
 * \subsection timeconvert_example_default Get the current system time in UTC.
 * \code{.sh}
 * > timeconvert  
 * 
 *         Month/Day/Year H:M:S            08/10/2020 17:00:04
 *         Modified Julian Date            59071.708386331
 *         GPSweek DayOfWeek SecOfWeek     70 1  147604.579029
 *         FullGPSweek Zcount              2118  98403
 *         Year DayOfYear SecondOfDay      2020 223  61204.579029
 *         Unix: Second Microsecond        1597078804 579029
 *         Zcount: 29-bit (32-bit)         36798563 (1110540387)
 * \endcode
 * \subsection timeconvert_example_rinex Convert RINEX file time.
 * \code{.sh}
 * > timeconvert -R "85 05 06 13 50 02"
 *
 *         Month/Day/Year H:M:S            05/06/1985 13:50:02
 *         Modified Julian Date            46191.576412037
 *         GPSweek DayOfWeek SecOfWeek     278 1  136202.000000
 *         FullGPSweek Zcount              278  90801
 *         Year DayOfYear SecondOfDay      1985 126  49802.000000
 *         Unix: Second Microsecond        484235402      0
 *         Zcount: 29-bit (32-bit)         145842865 (145842865)
 * \endcode
 * \subsection timeconvert_example_civil Convert civil time.
 * \code{.sh}
 * > timeconvert -c "5 6 1985 13:50:02"
 *
 *         Month/Day/Year H:M:S            05/06/1985 13:50:02
 *         Modified Julian Date            46191.576412037
 *         GPSweek DayOfWeek SecOfWeek     278 1  136202.000000
 *         FullGPSweek Zcount              278  90801
 *         Year DayOfYear SecondOfDay      1985 126  49802.000000
 *         Unix: Second Microsecond        484235402      0
 *         Zcount: 29-bit (32-bit)         145842865 (145842865)
 * \endcode
 * \subsection timeconvert_example_format Output only year, day of year and seconds of day.
 * \code{.sh}
 * > timeconvert  -F "%04Y %03j %05.0s"
 * 2020 223 61347
 * \endcode
 * \subsection timeconvert_example_input_format Custom input format.
 * \code{.sh}
 * > timeconvert  --input-format="%Y %j %H:%M:%S" --input-time="1985 126 13:50:02"
 *
 *         Month/Day/Year H:M:S            05/06/1985 13:50:02
 *         Modified Julian Date            46191.576412037
 *         GPSweek DayOfWeek SecOfWeek     278 1  136202.000000
 *         FullGPSweek Zcount              278  90801
 *         Year DayOfYear SecondOfDay      1985 126  49802.000000
 *         Unix: Second Microsecond        484235402      0
 *         Zcount: 29-bit (32-bit)         145842865 (145842865)
 * \endcode
 * \subsection timeconvert_example_subtract Display beginning of yesterday.
 * \code{.sh}
 * > timeconvert -s 86400 -F "%4Y %03j 0"
 * 2020 222 0
 * \endcode
 * \section timeconvert_exit_status EXIT STATUS
 * The following exit values are returned:
 * \dictable
 * \dictentry{0,No errors occurred}
 * \dictentry{1,A C++ exception occurred (typically invalid time format or value)}
 * \enddictable
 * \section timeconvert_see_also SEE ALSO
 * printTime()
 */

#include "BasicFramework.hpp"

#include "TimeString.hpp"
#include "TimeConstants.hpp"

#include "ANSITime.hpp"
#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSWeekZcount.hpp"
#include "JulianDate.hpp"
#include "MJD.hpp"
#include "UnixTime.hpp"
#include "YDSTime.hpp"
#include "SystemTime.hpp"

#include "CommandOptionWithCommonTimeArg.hpp"

using namespace std;
using namespace gpstk;


class TimCvt : public BasicFramework
{
public:
   TimCvt(char* arg0);

protected:
   virtual void process();

private:
   CommandOptionWithCommonTimeArg ANSITimeOption;
   CommandOptionWithCommonTimeArg CivilTimeOption;
   CommandOptionWithCommonTimeArg RinexFileTimeOption;
   CommandOptionWithCommonTimeArg GPSEWSOption;
   CommandOptionWithCommonTimeArg GPSWSOption;
   CommandOptionWithCommonTimeArg GPSWZOption;
   CommandOptionWithCommonTimeArg GPSZ29Option;
   CommandOptionWithCommonTimeArg GPSZ32Option;
   CommandOptionWithCommonTimeArg JDOption;
   CommandOptionWithCommonTimeArg MJDOption;
   CommandOptionWithCommonTimeArg UnixTimeOption;
   CommandOptionWithCommonTimeArg YDSTimeOption;

   CommandOptionWithAnyArg inputFormatOption;
   CommandOptionWithAnyArg inputTimeOption;
   CommandOptionAllOf inputFormatAndTimeOption;

   CommandOptionWithAnyArg formatOption;
   CommandOptionWithNumberArg addOption;
   CommandOptionWithNumberArg subOption;
   CommandOptionMutex mutexOption;

   string stringToParse;
   string timeSpec;
};

TimCvt::TimCvt(char* arg0)
      : BasicFramework(arg0, "Converts from a given input time specification"
                       " to other time formats.  Include the quotation marks."
                       "  All year values are four digit years."),
        ANSITimeOption('A', "ansi", "%K", "\"ANSI-Second\""),
        CivilTimeOption('c', "civil", "%m %d %Y %H:%M:%f",
                        "\"Month(numeric) DayOfMonth Year Hour:Minute:Second\""),
        RinexFileTimeOption('R', "rinex-file", "%y %m %d %H %M %S",
                            "\"Year(2-digit) Month(numeric) DayOfMonth Hour Minute Second\""),
        GPSEWSOption('o', "ews", "%E %G %g", 
                     "\"GPSEpoch 10bitGPSweek SecondOfWeek\""),
        GPSWSOption('f', "ws", "%F %g", "\"FullGPSWeek SecondOfWeek\""),
        GPSWZOption('w', "wz", "%F %Z", "\"FullGPSWeek Zcount\""),
        GPSZ29Option(0, "z29", "%E %c", "\"29bitZcount\""),
        GPSZ32Option('Z', "z32", "%C", "\"32bitZcount\""),
        JDOption('j', "julian", "%J", "\"JulianDate\""),
        MJDOption('m', "mjd", "%Q", "\"ModifiedJulianDate\""),
        UnixTimeOption('u',"unixtime", "%U %u",
                       "\"UnixSeconds UnixMicroseconds\""),
        YDSTimeOption('y', "doy", "%Y %j %s",
                      "\"Year DayOfYear SecondsOfDay\""),
        inputFormatOption(0, "input-format", "Time format to use on input"),
        inputTimeOption(0, "input-time",
                        "Time to be parsed by \"input-format\" option"),
        formatOption('F', "format", "Time format to use on output"),
        addOption('a', "add-offset", "add NUM seconds to specified time"),
        subOption('s', "sub-offset",
                  "subtract NUM seconds from specified time")
{
   ANSITimeOption.setMaxCount(1);
   CivilTimeOption.setMaxCount(1);
   RinexFileTimeOption.setMaxCount(1);
   GPSEWSOption.setMaxCount(1);
   GPSWSOption.setMaxCount(1);
   GPSWZOption.setMaxCount(1);
   GPSZ29Option.setMaxCount(1);
   GPSZ32Option.setMaxCount(1);
   JDOption.setMaxCount(1);
   MJDOption.setMaxCount(1);
   UnixTimeOption.setMaxCount(1);
   YDSTimeOption.setMaxCount(1);
   formatOption.setMaxCount(1);

   inputFormatOption.setMaxCount(1);
   inputTimeOption.setMaxCount(1);
   inputFormatAndTimeOption.addOption(&inputFormatOption);
   inputFormatAndTimeOption.addOption(&inputTimeOption);

   mutexOption.addOption(&ANSITimeOption);
   mutexOption.addOption(&CivilTimeOption);
   mutexOption.addOption(&RinexFileTimeOption);
   mutexOption.addOption(&GPSEWSOption);
   mutexOption.addOption(&GPSWSOption);
   mutexOption.addOption(&GPSWZOption);
   mutexOption.addOption(&GPSZ29Option);
   mutexOption.addOption(&GPSZ32Option);
   mutexOption.addOption(&JDOption);
   mutexOption.addOption(&MJDOption);
   mutexOption.addOption(&UnixTimeOption);
   mutexOption.addOption(&YDSTimeOption);
   mutexOption.addOption(&inputFormatAndTimeOption);
}

void TimCvt::process()
{
   CommonTime ct;
   ct.setTimeSystem(TimeSystem::GPS);
   CommandOption *whichOpt = mutexOption.whichOne();

   if (whichOpt)
   {
      CommandOptionWithCommonTimeArg *cta = 
         dynamic_cast<CommandOptionWithCommonTimeArg *>(whichOpt);
      if (cta)
      {
         ct = cta->getTime().front();
         ct.setTimeSystem(TimeSystem::GPS);
      }
      else // whichOpt == &inputFormatAndTimeOption
      {
         mixedScanTime( ct, 
                        inputTimeOption.getValue().front(),
                        inputFormatOption.getValue().front() );
         ct.setTimeSystem(TimeSystem::GPS);
      }
   }
   else
   {
      ct = SystemTime();
      ct.setTimeSystem(TimeSystem::GPS); 
   }

   int i;
   int addOptions = addOption.getCount();
   int subOptions = subOption.getCount();
   for (i = 0; i < addOptions; i++)
      ct += StringUtils::asDouble(addOption.getValue()[i]);
   for (i = 0; i < subOptions; i++)
      ct -= StringUtils::asDouble(subOption.getValue()[i]);

   if (formatOption.getCount())
   {
      cout << printTime(ct, formatOption.getValue()[0]) << endl;
   }
   else
   {
      using StringUtils::leftJustify;
      string eight(8, ' '); // eight spaces
      
      GPSWeekZcount wz(ct);
      CivilTime civ(ct);

      cout << endl
           << eight << leftJustify("Month/Day/Year H:M:S", 32) 
           << CivilTime(ct).printf("%02m/%02d/%04Y %02H:%02M:%02S") << endl

           << eight << leftJustify("Modified Julian Date", 32)
           << setprecision(15) << MJD(ct).printf("%15.9Q") << endl

           << eight << leftJustify("GPSweek DayOfWeek SecOfWeek", 32)
           << GPSWeekSecond(ct).printf("%G %w % 13.6g") << endl

           << eight << leftJustify("FullGPSweek Zcount", 32)
           << wz.printf("%F % 6z") << endl

           << eight << leftJustify("Year DayOfYear SecondOfDay", 32)
           << YDSTime(ct).printf("%Y %03j % 12.6s") << endl

           << eight << leftJustify("Unix: Second Microsecond", 32)
           << UnixTime(ct).printf("%U % 6u") << endl

           << eight << leftJustify("Zcount: 29-bit (32-bit)", 32)
           << wz.printf("%c (%C)") << endl

           << endl << endl;
   }

   return;
}

int main(int argc, char* argv[])
{
   try
   {
      TimCvt m(argv[0]);
      if (!m.initialize(argc, argv))
         return m.exitCode;
      if (!m.run())
         return m.exitCode;
      
      return m.exitCode;
   }
   catch(Exception& e)
   {
      cout << e << endl;
   }
   catch(std::exception& e)
   {
      cout << e.what() << endl;
   }
   catch(...)
   {
      cout << "unknown error" << endl;
   }
      // only reach this point if an exception was caught
   return BasicFramework::EXCEPTION_ERROR;
}
