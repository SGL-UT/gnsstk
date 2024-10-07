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
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsStream.hpp"

using namespace std;
using namespace gnsstk;

class Rinex3ObsHeaderFacade : public Rinex3ObsHeader
{
public:
    void reallyPutRecordWrapper(FFStream &stream)
    {
        return reallyPutRecord(stream);
    }
};

class Rinex3ObsHeader_T
{
public:
    // default stuff.
    Rinex3ObsHeader_T(){};
    ~Rinex3ObsHeader_T(){};

    // plain constructor runs clear so it should get us coverage for clear()
    Rinex3ObsHeaderFacade createHeader(void)
    {
        Rinex3ObsHeaderFacade header;
        return header;
    }

    void dropFile(char *filename)
    {
        try
        {
            std::remove(filename);
        }
        catch (...)
        {
            std::cout << "failed to drop temp file " << std::string(filename) << ", may have to delete it manually later.";
        }
    }

    int reallyPutRecordTest(void)
    {
        TUDEF("Rinex3ObsHeader", "reallyPutRecord");
        Rinex3ObsHeaderFacade header = createHeader();
        char filename[] = "tempoutput";
        FFStream *stream = new FFStream(filename, std::ios::out | std::ios::trunc);
        // empty should throw exception
        TUTHROW(header.reallyPutRecordWrapper(*stream));
        // set the version, should throw again on being invalid since no required fields are present.
        header.version = 3.0;
        TUTHROW(header.reallyPutRecordWrapper(*stream));
        // set some header fields to make it at least get the writing part.
        header.valid = Rinex3ObsHeader::Fields::getRequired(3.0);
        TUTHROW(header.reallyPutRecordWrapper(*stream));
        // drop the file
        dropFile(filename);
        TURETURN();
    }

    int writeHeaderRecordsTest(void)
    {
        TUDEF("Rinex3ObsHeader", "writeHeaderRecords");
        Rinex3ObsHeaderFacade header = createHeader();
        char filename[] = "tempoutput";
        Rinex3ObsStream *stream = new Rinex3ObsStream(filename, std::ios::out | std::ios::trunc);
        // valid and validVersion needs to be set, and then fileType should be non 'o' 'O'
        header.valid = Rinex3ObsHeader::Fields::getRequired(3.01);
        header.fileType = 'f';
        TUTHROW(header.writeHeaderRecords(*stream));
        // obs file, but not preserveVerType, and fileSysSat.system == SatelliteSystem::Unknown
        header.fileType = 'o';
        header.preserveVerType = false;
        header.fileSysSat.system = SatelliteSystem::Unknown;
        TUTHROW(header.writeHeaderRecords(*stream));
        // ok, let's try to hit those 3.0 blocks.
        header.preserveVerType = true;
        header.valid.set(Rinex3ObsHeader::Field::validAntennaDeltaXYZ);   //triple
        header.valid.set(Rinex3ObsHeader::Field::validAntennaPhaseCtr);   //triple
        header.valid.set(Rinex3ObsHeader::Field::validAntennaBsightXYZ);  //triple
        header.valid.set(Rinex3ObsHeader::Field::validAntennaZeroDirAzi); //float
        header.valid.set(Rinex3ObsHeader::Field::validAntennaZeroDirXYZ); //triple
        header.valid.set(Rinex3ObsHeader::Field::validCenterOfMass);      //triple
        header.valid.set(Rinex3ObsHeader::Field::validSigStrengthUnit);   // string
        header.sigStrengthUnit = std::string("Unitless");
        header.valid.set(Rinex3ObsHeader::Field::validSystemDCBSapplied);
        header.valid.set(Rinex3ObsHeader::Field::validSystemPCVSapplied);
        header.valid.set(Rinex3ObsHeader::Field::validGlonassSlotFreqNo);
        TUCATCH(header.writeHeaderRecords(*stream));
        // TODO: verify file outputs.
        dropFile(filename);
        TURETURN();
    }

    int parseHeaderRecordTest(void)
    {
        TUDEF("Rinex3ObsHeader", "parseHeaderRecord");
        Rinex3ObsHeaderFacade header = createHeader();
        std::string record = "            10            11            12                  " + Rinex3ObsHeader::hsAntennaDeltaXYZ;
        header.parseHeaderRecord(record);
        TUASSERTFEPS(header.antennaDeltaXYZ[0], 10.0, 1e-8);
        TUASSERTFEPS(header.antennaDeltaXYZ[1], 11.0, 1e-8);
        TUASSERTFEPS(header.antennaDeltaXYZ[2], 12.0, 1e-8);
        record = "AA1         10            11            12                  " + Rinex3ObsHeader::hsAntennaPhaseCtr;
        header.parseHeaderRecord(record);
        TUASSERTE(string, header.antennaSatSys, std::string("AA"));
        TUASSERTE(string, header.antennaObsCode, std::string("1"));
        TUASSERTFEPS(header.antennaPhaseCtr[0], 10.0, 1e-8);
        TUASSERTFEPS(header.antennaPhaseCtr[1], 11.0, 1e-8);
        TUASSERTFEPS(header.antennaPhaseCtr[2], 12.0, 1e-8);
        record = "            10            11            12                  " + Rinex3ObsHeader::hsAntennaBsightXYZ;
        header.parseHeaderRecord(record);
        TUASSERTFEPS(header.antennaBsightXYZ[0], 10.0, 1e-8);
        TUASSERTFEPS(header.antennaBsightXYZ[1], 11.0, 1e-8);
        TUASSERTFEPS(header.antennaBsightXYZ[2], 12.0, 1e-8);
        record = "            10                                              " + Rinex3ObsHeader::hsAntennaZeroDirAzi;
        header.parseHeaderRecord(record);
        TUASSERTFEPS(header.antennaZeroDirAzi, 10.0, 1e-8);
        record = "            10            11            12                  " + Rinex3ObsHeader::hsAntennaZeroDirXYZ;
        header.parseHeaderRecord(record);
        TUASSERTFEPS(header.antennaZeroDirXYZ[0], 10.0, 1e-8);
        TUASSERTFEPS(header.antennaZeroDirXYZ[1], 11.0, 1e-8);
        TUASSERTFEPS(header.antennaZeroDirXYZ[2], 12.0, 1e-8);
        record = "            10            11            12                  " + Rinex3ObsHeader::hsCenterOfMass;
        header.parseHeaderRecord(record);
        TUASSERTFEPS(header.centerOfMass[0], 10.0, 1e-8);
        TUASSERTFEPS(header.centerOfMass[1], 11.0, 1e-8);
        TUASSERTFEPS(header.centerOfMass[2], 12.0, 1e-8);
        record = "G           NAME    SOURCE                                  " + Rinex3ObsHeader::hsSystemDCBSapplied;
        header.parseHeaderRecord(record);
        TUASSERTE(string, header.infoDCBS[0].satSys, string("G"));
        TUASSERTE(string, header.infoDCBS[0].name, string("NAME"));
        TUASSERTE(string, header.infoDCBS[0].source, string("SOURCE"));
        record = "P           NAME    SOURCE                                  " + Rinex3ObsHeader::hsSystemPCVSapplied;
        header.parseHeaderRecord(record);
        TUASSERTE(string, header.infoPCVS[0].satSys, string("P"));
        TUASSERTE(string, header.infoPCVS[0].name, string("NAME"));
        TUASSERTE(string, header.infoPCVS[0].source, string("SOURCE"));
        record = "Unitless                                                    " + Rinex3ObsHeader::hsSigStrengthUnit;
        header.parseHeaderRecord(record);
        TUASSERTE(string, header.sigStrengthUnit, string("Unitless"));
        TURETURN();
    }

    int parseHeaderRecordSystemScaleFacTest(void)
    {
        TUDEF("Rinex3ObsHeader", "parseHeaderRecord, systemScaleFac");
        Rinex3ObsHeaderFacade header = createHeader();
        // need 2 to hit the condition for continuation line.
        std::string record = "A    2    7   A   B   C                                     " + Rinex3ObsHeader::hsSystemScaleFac;
        header.parseHeaderRecord(record);
        record = "     2    7   D   E   F   G                                 " + Rinex3ObsHeader::hsSystemScaleFac;
        header.parseHeaderRecord(record);
        // TODO: verify the content of the sys fac map
        TURETURN();
    }

    int parseHeaderRecordSystemPhaseShift(void)
    {
        TUDEF("Rinex3ObsHeader", "parseHeaderRecord, systemPhaseShift");
        Rinex3ObsHeaderFacade header = createHeader();
        // need 2 to hit the condition for continuation line.
        // satSystemp, obsid, cor, nsat, satID
        std::string record = "A ID        10 01  SID                                     " + Rinex3ObsHeader::hsSystemPhaseShift;
        header.parseHeaderRecord(record);
        // have another sat ID. weirdly, if there's a continuation, we ignore everything before the sat IDs.
        record = "  ID        10 01  TID                                     " + Rinex3ObsHeader::hsSystemPhaseShift;
        header.parseHeaderRecord(record);
        // TODO: verify
        TURETURN();
    }

    int parseHeaderRecordExceptionsTest(void)
    {
        TUDEF("RinexObsHeader", "parseHeaderRecord, error cases");
        Rinex3ObsHeaderFacade header = createHeader();
        header.version = 2.0;
        std::string record = "A ID        10 01  SID                                     " + Rinex3ObsHeader::hsSystemNumObs;
        TUTHROW(header.parseHeaderRecord(record));
        // too many sats, 7 is max.
        record = "     5     5     9                                         " + Rinex3ObsHeader::hsWaveFact;
        TUTHROW(header.parseHeaderRecord(record));
        // hsPrnObs pushes to comment list.
        header.version = 3.1;
        record = "     5     5 00009                                          " + Rinex3ObsHeader::hsPrnObs;
        TUCATCH(header.parseHeaderRecord(record));
        TUASSERTE(string, header.commentList[0].substr(0, 7), string("Warning"));
        // unidentified label
        record = "     5     5 00009                                         GARBANZOHEADERTWENTY";
        TUTHROW(header.parseHeaderRecord(record));
        TURETURN();
    }

    int getObsIndexTest(void)
    {

        TUDEF("rinexObsHeader", "getObsIndex");
        Rinex3ObsHeaderFacade header = createHeader();
        header.version = 3.1;
        // first, we'll do the new style, 2 args
        //RinexObsID(ObservationType ot, CarrierBand cb, TrackingCode tc, double version = Rinex3ObsBase::currentVersion
        RinexObsID obsID(ObservationType::Range, CarrierBand::L1, TrackingCode::CA, 3.1);
        TUTHROW(header.getObsIndex("A", obsID));
        // add gnss to the stuff to be remapped so it'll make it past the first thing, but it won't have the obsid we're looking for. 
        std::string record = "G   2 C1C C1C                                               " + Rinex3ObsHeader::hsSystemNumObs;
        TUCATCH(header.parseHeaderRecord(record));
        TUTHROW(header.getObsIndex("A", obsID));
        // now we'll do the old style with just type string input
        string type("AA");
        // should die from invalid type, size 2. 
        TUTHROW(header.getObsIndex(type));
        // should die from invalid obs id, G1Z, observation isn't in the G system. 
        type = string("GC1Z");
        TUTHROW(header.getObsIndex(type));
        TURETURN();
    }

    int compareTest(void)
    {

        TUDEF("rinexObsHeader", "compare");
        Rinex3ObsHeaderFacade header = createHeader();
        std::vector<string> diffs;
        std::vector<string> inclExclList;
        bool incl = true;
        // comparison to itself should be true
        TUASSERT(header.compare(header, diffs, inclExclList, true));
        // inclExclList is empty, so true and false should more or less get the same result.
        TUASSERT(header.compare(header, diffs, inclExclList, false));
        // TODO: more detailed testing using inclExclList and the incl boolean. 
        TURETURN();
    }

    int asStringTest(void)
    {
        TUDEF("rinexObsHeader", "asString");
        Rinex3ObsHeaderFacade header = createHeader();
        TUASSERTE(string, header.asString(header.validVersion), Rinex3ObsHeader::hsVersion);
        TUASSERTE(string, header.asString(header.validRunBy), Rinex3ObsHeader::hsRunBy);
        TUASSERTE(string, header.asString(header.validVersion), Rinex3ObsHeader::hsVersion);
        TUASSERTE(string, header.asString(header.validRunBy), Rinex3ObsHeader::hsRunBy);
        TUASSERTE(string, header.asString(header.validComment), Rinex3ObsHeader::hsComment);
        TUASSERTE(string, header.asString(header.validMarkerName), Rinex3ObsHeader::hsMarkerName);
        TUASSERTE(string, header.asString(header.validMarkerNumber), Rinex3ObsHeader::hsMarkerNumber);
        TUASSERTE(string, header.asString(header.validMarkerType), Rinex3ObsHeader::hsMarkerType);
        TUASSERTE(string, header.asString(header.validObserver), Rinex3ObsHeader::hsObserver);
        TUASSERTE(string, header.asString(header.validReceiver), Rinex3ObsHeader::hsReceiver);
        TUASSERTE(string, header.asString(header.validAntennaType), Rinex3ObsHeader::hsAntennaType);
        TUASSERTE(string, header.asString(header.validAntennaPosition), Rinex3ObsHeader::hsAntennaPosition);
        TUASSERTE(string, header.asString(header.validAntennaDeltaHEN), Rinex3ObsHeader::hsAntennaDeltaHEN);
        TUASSERTE(string, header.asString(header.validAntennaDeltaXYZ), Rinex3ObsHeader::hsAntennaDeltaXYZ);
        TUASSERTE(string, header.asString(header.validAntennaPhaseCtr), Rinex3ObsHeader::hsAntennaPhaseCtr);
        TUASSERTE(string, header.asString(header.validAntennaBsightXYZ), Rinex3ObsHeader::hsAntennaBsightXYZ);
        TUASSERTE(string, header.asString(header.validAntennaZeroDirAzi), Rinex3ObsHeader::hsAntennaZeroDirAzi);
        TUASSERTE(string, header.asString(header.validAntennaZeroDirXYZ), Rinex3ObsHeader::hsAntennaZeroDirXYZ);
        TUASSERTE(string, header.asString(header.validCenterOfMass), Rinex3ObsHeader::hsCenterOfMass);
        TUASSERTE(string, header.asString(header.validNumObs), Rinex3ObsHeader::hsNumObs);
        TUASSERTE(string, header.asString(header.validSystemNumObs), Rinex3ObsHeader::hsSystemNumObs);
        TUASSERTE(string, header.asString(header.validWaveFact), Rinex3ObsHeader::hsWaveFact);
        TUASSERTE(string, header.asString(header.validSigStrengthUnit), Rinex3ObsHeader::hsSigStrengthUnit);
        TUASSERTE(string, header.asString(header.validInterval), Rinex3ObsHeader::hsInterval);
        TUASSERTE(string, header.asString(header.validFirstTime), Rinex3ObsHeader::hsFirstTime);
        TUASSERTE(string, header.asString(header.validLastTime), Rinex3ObsHeader::hsLastTime);
        TUASSERTE(string, header.asString(header.validReceiverOffset), Rinex3ObsHeader::hsReceiverOffset);
        TUASSERTE(string, header.asString(header.validSystemDCBSapplied), Rinex3ObsHeader::hsSystemDCBSapplied);
        TUASSERTE(string, header.asString(header.validSystemPCVSapplied), Rinex3ObsHeader::hsSystemPCVSapplied);
        TUASSERTE(string, header.asString(header.validSystemScaleFac), Rinex3ObsHeader::hsSystemScaleFac);
        TUASSERTE(string, header.asString(header.validSystemPhaseShift), Rinex3ObsHeader::hsSystemPhaseShift);
        TUASSERTE(string, header.asString(header.validGlonassSlotFreqNo), Rinex3ObsHeader::hsGlonassSlotFreqNo);
        TUASSERTE(string, header.asString(header.validGlonassCodPhsBias), Rinex3ObsHeader::hsGlonassCodPhsBias);
        TUASSERTE(string, header.asString(header.validLeapSeconds), Rinex3ObsHeader::hsLeapSeconds);
        TUASSERTE(string, header.asString(header.validNumSats), Rinex3ObsHeader::hsNumSats);
        TUASSERTE(string, header.asString(header.validPrnObs), Rinex3ObsHeader::hsPrnObs);
        TUASSERTE(string, header.asString(header.validInvalid), "???");
        TURETURN();
    }

    int asFieldTest(void)
    {
        TUDEF("rinexObsHeader", "asField");
        Rinex3ObsHeaderFacade header = createHeader();
        TUASSERT(header.validVersion == header.asField(Rinex3ObsHeader::hsVersion));
        TUASSERT(header.validRunBy == header.asField(Rinex3ObsHeader::hsRunBy));
        TUASSERT(header.validComment == header.asField(Rinex3ObsHeader::hsComment));
        TUASSERT(header.validMarkerName == header.asField(Rinex3ObsHeader::hsMarkerName));
        TUASSERT(header.validMarkerNumber == header.asField(Rinex3ObsHeader::hsMarkerNumber));
        TUASSERT(header.validMarkerType == header.asField(Rinex3ObsHeader::hsMarkerType));
        TUASSERT(header.validObserver == header.asField(Rinex3ObsHeader::hsObserver));
        TUASSERT(header.validReceiver == header.asField(Rinex3ObsHeader::hsReceiver));
        TUASSERT(header.validAntennaType == header.asField(Rinex3ObsHeader::hsAntennaType));
        TUASSERT(header.validAntennaPosition == header.asField(Rinex3ObsHeader::hsAntennaPosition));
        TUASSERT(header.validAntennaDeltaHEN == header.asField(Rinex3ObsHeader::hsAntennaDeltaHEN));
        TUASSERT(header.validAntennaDeltaXYZ == header.asField(Rinex3ObsHeader::hsAntennaDeltaXYZ));
        TUASSERT(header.validAntennaPhaseCtr == header.asField(Rinex3ObsHeader::hsAntennaPhaseCtr));
        TUASSERT(header.validAntennaBsightXYZ == header.asField(Rinex3ObsHeader::hsAntennaBsightXYZ));
        TUASSERT(header.validAntennaZeroDirAzi == header.asField(Rinex3ObsHeader::hsAntennaZeroDirAzi));
        TUASSERT(header.validAntennaZeroDirXYZ == header.asField(Rinex3ObsHeader::hsAntennaZeroDirXYZ));
        TUASSERT(header.validCenterOfMass == header.asField(Rinex3ObsHeader::hsCenterOfMass));
        TUASSERT(header.validNumObs == header.asField(Rinex3ObsHeader::hsNumObs));
        TUASSERT(header.validSystemNumObs == header.asField(Rinex3ObsHeader::hsSystemNumObs));
        TUASSERT(header.validWaveFact == header.asField(Rinex3ObsHeader::hsWaveFact));
        TUASSERT(header.validSigStrengthUnit == header.asField(Rinex3ObsHeader::hsSigStrengthUnit));
        TUASSERT(header.validInterval == header.asField(Rinex3ObsHeader::hsInterval));
        TUASSERT(header.validFirstTime == header.asField(Rinex3ObsHeader::hsFirstTime));
        TUASSERT(header.validLastTime == header.asField(Rinex3ObsHeader::hsLastTime));
        TUASSERT(header.validReceiverOffset == header.asField(Rinex3ObsHeader::hsReceiverOffset));
        TUASSERT(header.validSystemDCBSapplied == header.asField(Rinex3ObsHeader::hsSystemDCBSapplied));
        TUASSERT(header.validSystemPCVSapplied == header.asField(Rinex3ObsHeader::hsSystemPCVSapplied));
        TUASSERT(header.validSystemScaleFac == header.asField(Rinex3ObsHeader::hsSystemScaleFac));
        TUASSERT(header.validSystemPhaseShift == header.asField(Rinex3ObsHeader::hsSystemPhaseShift));
        TUASSERT(header.validGlonassSlotFreqNo == header.asField(Rinex3ObsHeader::hsGlonassSlotFreqNo));
        TUASSERT(header.validGlonassCodPhsBias == header.asField(Rinex3ObsHeader::hsGlonassCodPhsBias));
        TUASSERT(header.validLeapSeconds == header.asField(Rinex3ObsHeader::hsLeapSeconds));
        TUASSERT(header.validNumSats == header.asField(Rinex3ObsHeader::hsNumSats));
        TUASSERT(header.validPrnObs == header.asField(Rinex3ObsHeader::hsPrnObs));
        TUASSERT(header.validInvalid == header.asField("Whatever"));
        TURETURN();
    }
};

int main()
{
    int errorCount = 0;
    Rinex3ObsHeader_T tester;
    errorCount += tester.reallyPutRecordTest();
    errorCount += tester.writeHeaderRecordsTest();
    errorCount += tester.parseHeaderRecordTest();
    errorCount += tester.parseHeaderRecordSystemScaleFacTest();
    errorCount += tester.parseHeaderRecordExceptionsTest();
    errorCount += tester.getObsIndexTest();
    errorCount += tester.compareTest();
    errorCount += tester.asStringTest();
    errorCount += tester.asFieldTest();
    cout << "error count is " << errorCount << endl;
    return errorCount;
}