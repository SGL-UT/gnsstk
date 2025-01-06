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

#include "TestUtil.hpp"
#include "CommandLine.hpp"
#include "logstream.hpp"

using namespace std;
using namespace gnsstk;

class CommandLine_T
{
public:
    // EVERYTHING else is called by this so it's just going to be a lot of calling this and trying to hit all the private methods.
    int ProcessCommandLineInvalidOptsTest(void)
    {
        TUDEF("CommandLine", "ProcessCommandLine, ValidateCommandLine error cases");
        // want to get the coverage for debug logging
        ConfigureLOG::ReportingLevel() = ConfigureLOG::Level("DEBUG");
        int argc = 1;
        // argv is a double array, just a command first.
        char cmdToken[] = "test";
        char *argv[] = {cmdToken};
        string programDesc = "program description";
        string usage = "Usage";
        string errors = "";
        vector<string> unrecog;
        // hit error case with validate command line.
        CommandLine commandLine;
        char shortOpt = 'a';
        string longOpt("airplane");
        string arg("arg");
        string predesc("predesc for airplane");
        string desc("description for airplane");
        string syntax("syntax for airplane");
        string forPtr("787");
        bool repeat(false);
        bool required(false);
        commandLine.Add(shortOpt, longOpt, arg, repeat, required, &forPtr, predesc, desc);
        // add it a second time to get all the repetition errors.
        commandLine.Add(shortOpt, longOpt, arg, repeat, required, &forPtr, predesc, desc);
        bool hasArg = true;
        commandLine.Add_ignore("--" + longOpt, true);
        commandLine.Add_ignore("--" + longOpt, false);
        commandLine.Add_ignore_on("--" + longOpt);
        commandLine.Add_ignore_off("--" + longOpt);
        // add two deprecated ones, one for the one above, one where it doesn't exist so it hits the error condition
        commandLine.Add_deprecated("-z", "-a");
        commandLine.Add_deprecated("-y", "-b");
        string msg("");
        // should have all the invalid stuff from the first 4 conditions.
        TUASSERTE(int, -3, commandLine.ProcessCommandLine(argc, argv, programDesc, usage, errors, unrecog));
        TUASSERT(errors.find("with arg") != string::npos);
        TUASSERT(errors.find("w/o arg") != string::npos);
        TUASSERT(errors.find("ignore on") != string::npos);
        TUASSERT(errors.find("ignore off") != string::npos);
        // if any are repeated or shortform already used.
        TUASSERT(errors.find("is repeated") != string::npos);
        TUASSERT(errors.find("short form is") != string::npos);
        // deprecated arg error case
        TUASSERT(errors.find("replacement for deprecated") != string::npos);
        TURETURN();
    }

    int ProcessCommandLineBuildSyntaxPageTest(void)
    {
        TUDEF("CommandLine", "ProcessCommandLine, BuildSyntaxPage corner cases");
        int argc = 1;
        // argv is a double array, just a command first.
        char cmdToken[] = "test";
        char *argv[] = {cmdToken};
        string programDesc = "program description";
        string usage = "hahahaha";
        string errors = "";
        vector<string> unrecog;
        // hit error case with validate command line.
        CommandLine commandLine;
        char shortOpt = 'a';
        string longOpt("airplane");
        string arg("787");
        string predesc("predesc for airplane");
        string desc("description for airplane");
        string syntax("syntax for airplane");
        string forPtr("787");
        bool repeat(false);
        bool required(false);
        commandLine.Add(shortOpt, longOpt, arg, repeat, required, &forPtr, predesc, desc);
        // we need to add an arg for each one not hit by other tests, so
        // typeVectorInt,
        vector<int> vints;
        commandLine.Add('i', "ints", "ints", false, false, &vints, "predesc for ints", "desc for ints");
        // type double between >0.01, <0.1
        double dub(0);
        commandLine.Add('d', "dub", "dub", false, false, &dub, "predesc for dub", "desc for dub");
        // type sat
        RinexSatID satId;
        commandLine.Add('s', "sat", "sat", false, false, &satId, "predesc for sat", "desc for sat");
        // type vector sat
        vector<RinexSatID> vsats;
        commandLine.Add('S', "sats", "sats", false, false, &vsats, "predesc for more sats", "desc for more sats");
        // type typecount and type undefined i guess if we want a single line.
        // should get far enough to render usage but not necessarily finish successfully because argv is nearly empty.
        TUASSERTE(int, 1, commandLine.ProcessCommandLine(argc, argv, programDesc, usage, errors, unrecog));
        // original usage string should be completely replaced
        TUASSERT(usage.find("hahahaha") == string::npos);
        cout << usage << endl;
        // check an option that is always there in SyntaxPage
        TUASSERT(usage.find("--verbose") != string::npos);
        // check for our options
        TUASSERT(usage.find("--airplane <787>") != string::npos);
        TUASSERT(usage.find("--ints <ints>") != string::npos);
        TUASSERT(usage.find("--dub <dub>") != string::npos);
        TUASSERT(usage.find("--sat <sat>") != string::npos);
        TUASSERT(usage.find("--sats <sats>") != string::npos);
        TURETURN();
    }

    int ProcessCommandLinePreProcessArgsTest(void)
    {
        TUDEF("CommandLine", "ProcessCommandLine, PreProcessArgs corner cases");
        CommandLine commandLine;
        // hit all 4 of those ignore conditions with 4 options.
        // one with arg
        // one without arg
        // one to turn it on
        // one to turn it off
        // and then one more to if
        // test --aaa a --bbb --ccc meow --ddd
        vector<int> vints;
        commandLine.Add('i', "ints", "ints", false, false, &vints, "predesc for ints", "desc for ints");
        // type double between >0.01, <0.1
        double dub(0);
        commandLine.Add('d', "dub", "dub", false, false, &dub, "predesc for dub", "desc for dub");
        // type sat
        RinexSatID satId;
        commandLine.Add('s', "sat", "sat", false, false, &satId, "predesc for sat", "desc for sat");
        // type vector sat
        vector<RinexSatID> vsats;
        commandLine.Add('S', "sats", "sats", false, false, &vsats, "predesc for more sats", "desc for more sats");
        commandLine.Add_ignore("--ints", true);
        commandLine.Add_ignore("--dub", false);
        commandLine.Add_ignore_on("--ccc");
        commandLine.Add_ignore_off("--ddd");
        // should ignore ints with arg, dub without arg, --ccc will make --sat be ignored, and then --ddd ignore back off
        // but also, we should turn on everything that's preset like --file, --verbose, --debug, and --help
        string commandToTest("test --ints 1 --dub --ccc --sats --ddd --sats --verbose --debug --help --file ./tmpconfig");
        ofstream tmpConfig;
        tmpConfig.open("./tmpconfig");
        tmpConfig << "--ccc --ddd";
        tmpConfig.close();
        int argc = 13;
        char *argv[13];
        if (argc != getArgcArgv(commandToTest, argv))
        {
            TUFAIL("argcargv parsing is not working correctly, bailing out of test early");
            TURETURN();
        }
        // argv is a double array, just a command first.
        string programDesc = "program description";
        string usage = "hahahaha";
        string errors = "";
        vector<string> unrecog;
        TUCATCH(commandLine.ProcessCommandLine(argc, argv, programDesc, usage, errors, unrecog));
        try
        {
            remove("./tmpconfig");
        }
        catch (const std::exception &e)
        {
            cout << "failed to delete ./tmpconfig, might want to check why" << endl;
        }
        TURETURN();
    }

    int ProcessCommandLineParseTest(void)
    {
        TUDEF("CommandLine", "ProcessCommandLine, Parse corner cases");
        CommandLine commandLine;
        int num1 = 0;
        int num2 = 0;
        int num3 = 0;
        bool car = false;
        commandLine.Add('b', "boo", "boo", false, true, &num1, "predesc for boo", "desc for boo");
        commandLine.Add('n', "num", "num", false, true, &num2, "predesc for num", "desc for num");
        commandLine.Add('m', "meow", "meow", false, true, &num3, "predesc for meow", "desc for meow");
        commandLine.Add('c', "car", "car", false, true, &car, "predesc for car", "desc for car");
        // two cases of missing arg, one in the middle where next thing it finds is the next -- instead of an optarg.
        // the other case is is it the final arg so there's no optarg
        // need to his help, verbose, debug, but also need a repeated arg and a required arg.
        // finally, need an unrecognized arg.
        string commandToTest("test --help --verbose --debug --yessir --boo --meow 1 --meow 2 --num --num");
        int argc = 12;
        char *argv[12];
        if (argc != getArgcArgv(commandToTest, argv))
        {
            TUFAIL("argcargv parsing is not working correctly, bailing out of test early");
            TURETURN();
        }
        string programDesc = "program description";
        string usage = "hahahaha";
        string errors = "";
        vector<string> unrecog;
        TUCATCH(commandLine.ProcessCommandLine(argc, argv, programDesc, usage, errors, unrecog));
        TUASSERTE(int, 1, unrecog.size());
        TUASSERT(unrecog[0].find("yessir") != string::npos);
        cout << "errors : " << errors << endl;
        TUASSERT(errors.find("boo without value") != string::npos);
        TUASSERT(errors.find("num without value") != string::npos);
        TUASSERT(errors.find("meow was repeated") != string::npos);
        TUASSERT(errors.find("car is not found") != string::npos);
        TURETURN();
    }

    int ProcessCommandLinePostprocessTest(void)
    {
        TUDEF("CommandLine", "ProcessCommandLine, Postprocess corner cases");
        CommandLine commandLine;
        // bool arg
        bool boo = false;
        commandLine.Add('b', "boo", "boo", false, true, &boo, "predesc for boo", "desc for boo");
        // integer, invalid integer case
        int num1 = 0;
        commandLine.Add('n', "num", "num", false, true, &num1, "predesc for num", "desc for num");
        int num2 = 0;
        commandLine.Add('m', "meow", "meow", false, true, &num2, "predesc for num", "desc for num");
        // vector int, non digit string case, and valid case
        vector<int> numList;
        commandLine.Add('o', "ooo", "ooo", false, true, &numList, "predesc for numList", "desc for numList");
        vector<int> numList2;
        commandLine.Add('p', "ppp", "ppp", false, true, &numList2, "predesc for numList", "desc for numList");
        // type double invalid case
        double dub = 0.0;
        commandLine.Add('d', "dub", "dub", false, true, &dub, "predesc for dub", "desc for dub");
        // type sat  case
        RinexSatID sat;
        commandLine.Add('s', "sat", "sat", false, true, &sat, "predesc for sat", "predesc for sat");
        // type sat vector case, we're gonna load this from a file.
        vector<RinexSatID> sats;
        commandLine.Add('t', "ttt", "ttt", false, true, &sats, "predesc for sats", "desc for sats");
        string commandToTest("test --boo --num nope --meow 2 --ooo a,b,c --ppp 1,2,3 --dub nope --sat G20 --ttt @./tmpconfig.txt");
        int argc = 16;
        char *argv[16];
        if (argc != getArgcArgv(commandToTest, argv))
        {
            TUFAIL("argcargv parsing is not working correctly, bailing out of test early");
            TURETURN();
        }
        string programDesc = "program description";
        string usage = "hahahaha";
        string errors = "";
        vector<string> unrecog;
        ofstream tmpConfig;
        tmpConfig.open("./tmpconfig.txt");
        tmpConfig << "G21";
        tmpConfig.close();
        TUCATCH(commandLine.ProcessCommandLine(argc, argv, programDesc, usage, errors, unrecog));
        TUASSERT(errors.find("non-integer value for --num") != string::npos);
        TUASSERT(errors.find("non-integer value for --ooo: a") != string::npos);
        TUASSERT(errors.find("non-integer value for --ooo: b") != string::npos);
        TUASSERT(errors.find("non-integer value for --ooo: c") != string::npos);
        TUASSERT(errors.find("invalid value for --dub") != string::npos);
        TUASSERTE(bool, boo, true);
        TUASSERTE(int, num2, 2);
        TUASSERTE(int, numList2[0], 1);
        TUASSERTE(int, numList2[1], 2);
        TUASSERTE(int, numList2[2], 3);
        TUASSERTE(string, sat.toString(), "G20");
        TUASSERTE(string, sats[0].toString(), "G21");
        try
        {
            remove("./tmpconfig");
        }
        catch (const std::exception &e)
        {
            cout << "failed to delete ./tmpconfig, might want to check why" << endl;
        }
        TURETURN();
    }

    int DumpConfigurationTest(void)
    {

        TUDEF("CommandLine", "DumpConfiguration");
        CommandLine commandLine;
        ostringstream oss;
        string tag("TESTTAG");
        // bool arg
        bool boo = false;
        commandLine.Add('b', "boo", "boo", false, true, &boo, "predesc for boo", "desc for boo");
        // int
        int num1 = 0;
        commandLine.Add('n', "num", "num", false, true, &num1, "predesc for num", "desc for num");
        // int vector
        vector<int> numList;
        commandLine.Add('o', "ooo", "ooo", false, true, &numList, "predesc for numList", "desc for numList");
        // type double
        double dub = 0.0;
        commandLine.Add('d', "dub", "dub", false, true, &dub, "predesc for dub", "desc for dub");
        // string
        string line;
        commandLine.Add('e', "eee", "eee", false, true, &line, "predesc for line", "desc for line");
        // vec string
        vector<string> lines;
        commandLine.Add('f', "fff", "fff", false, true, &lines, "predesc for lines", "desc for lines");
        // type sat  case
        RinexSatID sat;
        commandLine.Add('s', "sat", "sat", false, true, &sat, "predesc for sat", "predesc for sat");
        // type sat vector case, we're gonna load this from a file.
        vector<RinexSatID> sats;
        commandLine.Add('t', "ttt", "ttt", false, true, &sats, "predesc for sats", "desc for sats");
        commandLine.DumpConfiguration(oss, tag);
        string checkThis = oss.str();
        cout << "Dump Config is this: " << checkThis << endl;
        TUASSERT(checkThis.find("--boo") != string::npos);
        TUASSERT(checkThis.find("--num") != string::npos);
        TUASSERT(checkThis.find("--ooo") != string::npos);
        TUASSERT(checkThis.find("--dub") != string::npos);
        TUASSERT(checkThis.find("--eee") != string::npos);
        TUASSERT(checkThis.find("--fff") != string::npos);
        TUASSERT(checkThis.find("--sat") != string::npos);
        TUASSERT(checkThis.find("--ttt") != string::npos);
        // check the predefined stuff too
        TUASSERT(checkThis.find("--verbose") != string::npos);
        TUASSERT(checkThis.find("--debug") != string::npos);
        TUASSERT(checkThis.find("--help") != string::npos);
        TURETURN();
    }

    int getArgcArgv(string theString, char **argv, char delimiter = ' ')
    {
        int i, j, startOfToken = 0, argc = 0;
        const char *charArr = theString.c_str();
        char *tokenArr;
        for (i = 0; i <= theString.length(); i++)
        {
            if (i == theString.length() || charArr[i] == delimiter)
            {
                // we know this is a delimiter, allocate a string, copy it in, set pointer in argv.
                int tokenLength = i - startOfToken;
                if (tokenLength < 1)
                {
                    // yeah there's nothing there
                    startOfToken = i + 1;
                    continue;
                }
                char *newToken = (char *)calloc(tokenLength + 1, sizeof(char));
                for (j = 0; j < tokenLength; j++)
                {
                    newToken[j] = theString[startOfToken + j];
                }
                argv[argc] = newToken;
                // start of the next token will be the NEXT char spot.
                startOfToken = i + 1;
                argc += 1;
            }
        }
        return argc;
    }
};

int main(void)
{
    int errorCount = 0;
    CommandLine_T tester;
    errorCount += tester.ProcessCommandLineInvalidOptsTest();
    errorCount += tester.ProcessCommandLineBuildSyntaxPageTest();
    errorCount += tester.ProcessCommandLinePreProcessArgsTest();
    errorCount += tester.ProcessCommandLineParseTest();
    errorCount += tester.ProcessCommandLinePostprocessTest();
    errorCount += tester.DumpConfigurationTest();
    cout << "final error count for CommandLine_T is " << errorCount << endl;
    return errorCount;
}