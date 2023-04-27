%module(directors="1") NewNav

%pythonbegin %{
from __future__ import absolute_import
%}

// =============================================================
//  Section 0: C++ template containers & typedefs
// =============================================================

%{
// The header includes are kept in a separate file so they can be used
// to build other swig modules
#include "gnsstk_swig.hpp"
%}


// =============================================================
//  Section 1: C++ template containers & typedefs
// =============================================================
%feature("autodoc", "1");
%include "doc/doc.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"
%include "std_list.i"
%include "std_set.i"
%include "std_multimap.i"
%include "stdint.i"
%include "typemaps.i"
%include "exception.i"
%include "carrays.i"

// =============================================================
//  Section 2: Macros
// =============================================================
// Must be included here so that everything else has access to them.
%include "Macros.i"

// =============================================================
//  Section 3: Renaming C++ methods and types to Python
// =============================================================

%include "Renames.i"

// =============================================================
//  Section 4: C++ things to ignore when generating code
// =============================================================

%include "Ignore.i"

// =============================================================
//  Section 5: Enumerations
// =============================================================

ENUM_MAPPER(gnsstk::NavType, NavType, "gnsstk")
ENUM_MAPPER(gnsstk::NavMessageType, NavMessageType, "gnsstk")

%include "TimeOffsetFilter.hpp"
%include "NavType.hpp"
%include "NavMessageType.hpp"
%include "SVHealth.hpp"
%include "NavSearchOrder.hpp"
%include "NavValidityType.hpp"
%include "DumpDetail.hpp"
%include "GPSLNavL2Codes.hpp"
%include "GPSSVConfig.hpp"
%include "GalHealthStatus.hpp"
%include "GalDataValid.hpp"
%include "GLOFNavPCode.hpp"
%include "GLOFNavSatType.hpp"

%include "renameEnums.i"
%pythoncode %{
   renameEnums('TimeOffsetFilter')
   renameEnums('NavType')
   renameEnums('NavMessageType')
   renameEnums('SVHealth')
   renameEnums('NavSearchOrder')
   renameEnums('NavValidityType')
   renameEnums('DumpDetail')
   renameEnums('GPSLNavL2Codes')
   renameEnums('GPSSVConfig')
   renameEnums('GalHealthStatus')
   renameEnums('GalDataValid')
   renameEnums('GLOFNavPCode')
   renameEnums('GLOFNavSatType')
%}
%include "cleanup.i"

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 7: C++11 shared_ptr handling
// =============================================================

%include "NewNavSharedPtr.i"

// =============================================================
//  Section 9: typemaps that must be declared before the %includes
// =============================================================

%include "NewNavTypemaps.i"
%include "gnsstk_typemaps.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%import "TimeSystem.hpp"
%import "CommonTime.hpp"
%import "SatelliteSystem.hpp"
%import "SatID.hpp"
%import "Triple.i"
%import "EllipsoidModel.hpp"
%import "RefFrameSys.hpp"
%import "RefFrameRlz.hpp"
%import "ReferenceFrame.hpp"
%import "RefFrame.hpp"
%import "Xvt.hpp"
%import "FFData.hpp"
%import "CarrierBand.hpp"
%import "TrackingCode.hpp"
%import "XmitAnt.hpp"
%import "ObsID.hpp"
%import "NavType.hpp"
%import "NavID.hpp"
%import "PackedNavBits.hpp"
%import "Position.hpp"
%import "ValidType.hpp"
%import "Rinex3NavBase.hpp"
%import(module="gnsstk.RefTime") "TimeSystemCorr.hpp"
%import "Rinex3NavHeader.hpp"
%import "Rinex3NavData.hpp"
%import "SEMBase.hpp"
%import "SEMData.hpp"
%import "SP3Base.hpp"
%import "SP3Data.hpp"
%import "SP3Header.hpp"
%import "YumaBase.hpp"
%import "YumaData.hpp"
%import "Vector.hpp"

/* %include "BDSD1Bits.hpp" */
%include "NavSignalID.hpp"
%template() std::set<gnsstk::NavSignalID>;
%include "NavSatelliteID.hpp"
%template(NavSatelliteIDSet) std::set<gnsstk::NavSatelliteID>;
%include "NavMessageType.hpp"
%include "NavMessageID.hpp"
%template(NavMessageIDSet) std::set<gnsstk::NavMessageID>;
%include "DumpDetail.hpp"
%include "NavData.hpp"
%include "SystemNavData.hpp"
%template() std::map<gnsstk::CommonTime, gnsstk::NavDataPtr>;
%template() std::map<gnsstk::NavSatelliteID, gnsstk::NavMap>;
%template() std::list<gnsstk::NavDataPtr>;
%template() std::map<gnsstk::CommonTime, gnsstk::NavDataPtrList>;
%template() std::map<gnsstk::NavSatelliteID, gnsstk::NavNearMap>;
%include "OrbitData.hpp"
%include "NavFit.hpp"
%include "SVHealth.hpp"
%include "OrbitDataKepler.hpp"
%include "OrbitDataBDS.hpp"
%include "BDSD1NavData.hpp"
%include "BDSD1NavAlm.hpp"
%include "BDSD1NavEph.hpp"
%include "NavHealthData.hpp"
%include "BDSD1NavHealth.hpp"
%include "InterSigCorr.hpp"
%include "BDSD1NavISC.hpp"
%include "IonoNavData.hpp"
%include "KlobucharIonoNavData.hpp"
%include "BDSD1NavIono.hpp"
%include "TimeOffsetData.hpp"
%template() std::pair<gnsstk::TimeSystem, gnsstk::TimeSystem>;
%template() std::set<gnsstk::TimeCvtKey>;
%include "StdNavTimeOffset.hpp"
%include "BDSD1NavTimeOffset.hpp"
/* %include "BDSD2Bits.hpp" */
%include "BDSD2NavData.hpp"
%include "BDSD2NavAlm.hpp"
%include "BDSD2NavEph.hpp"
%include "BDSD2NavHealth.hpp"
%include "BDSD2NavISC.hpp"
%include "BDSD2NavIono.hpp"
%include "BDSD2NavTimeOffset.hpp"
%include "BDSconsts.hpp"
%include "CCIR.hpp"
/* %include "CommandOptionNavEnumHelp.hpp" */
%include "NavValidityType.hpp"
%include "NavSearchOrder.hpp"
%include "TimeOffsetFilter.hpp"
%include "FactoryControl.hpp"
%include "NavDataFactory.hpp"
%include "NavLibrary.hpp"
%template(NavMessageTypeSet) std::set<gnsstk::NavMessageType>;
/* %include "GLOCBits.hpp" */
%include "GLOCNavHeader.hpp"
%include "GLOCNavData.hpp"
%include "GLOCSatType.hpp"
%include "GLOCOrbitType.hpp"
%include "GLOCNavAlm.hpp"
%include "GLOCRegime.hpp"
%include "GLOCNavLTDMP.hpp"
%include "GLOCNavEph.hpp"
%include "GLOCNavHealth.hpp"
%include "GLOCNavIono.hpp"
%include "GLOCNavUT1TimeOffset.hpp"
/* %include "GLOFBits.hpp" */
%include "GLOFNavData.hpp"
%include "GLOFNavAlm.hpp"
%include "GLOFNavEph.hpp"
%include "GLOFNavHealth.hpp"
%include "GLOFNavISC.hpp"
%include "GLOFNavTimeOffset.hpp"
%include "GLOFNavUT1TimeOffset.hpp"
%include "OrbitDataGPS.hpp"
%include "GPSCNav2Alm.hpp"
%include "GPSCNav2Eph.hpp"
%include "GPSCNav2Health.hpp"
%include "GPSCNav2ISC.hpp"
%include "GPSCNav2Iono.hpp"
%include "GPSCNav2TimeOffset.hpp"
%include "GPSCNavData.hpp"
%include "GPSCNavAlm.hpp"
%include "GPSCNavEph.hpp"
%include "GPSCNavHealth.hpp"
%include "GPSCNavISC.hpp"
%include "GPSCNavIono.hpp"
%include "GPSCNavRedAlm.hpp"
%include "GPSCNavTimeOffset.hpp"
%include "GPSLNavData.hpp"
%include "GPSLNavAlm.hpp"
%include "GPSLNavEph.hpp"
%include "GPSLNavHealth.hpp"
%include "GPSLNavISC.hpp"
%include "GPSLNavIono.hpp"
%include "GPSLNavTimeOffset.hpp"
%include "GPSNavConfig.hpp"
%include "GalDataValid.hpp"
%include "OrbitDataGal.hpp"
%include "GalHealthStatus.hpp"
%include "GalFNavAlm.hpp"
%include "GalFNavEph.hpp"
%include "GalFNavHealth.hpp"
%include "GalFNavISC.hpp"
%include "MODIP.hpp"
%feature("flatnested");
%include "NeQuickIonoNavData.hpp"
%feature("flatnested", "");
%include "GalFNavIono.hpp"
%include "GalFNavTimeOffset.hpp"
%include "GalINavAlm.hpp"
%include "GalINavEph.hpp"
%include "GalINavHealth.hpp"
%include "GalINavISC.hpp"
%include "GalINavIono.hpp"
%include "GalINavTimeOffset.hpp"
%include "NavDataFactoryWithStore.hpp"
%template() std::map<gnsstk::NavSatelliteID, gnsstk::NavDataPtr>;
%template() std::map<gnsstk::CommonTime, gnsstk::NavDataFactoryWithStore::OffsetMap>;
%template() std::map<gnsstk::TimeCvtKey, gnsstk::NavDataFactoryWithStore::OffsetEpochMap>;
%include "NavDataFactoryCallback.hpp"
%include "NavDataFactoryWithStoreFile.hpp"
%include "NDFUniqIterator.hpp"
%template(NDFUniqIterator_NavDataFactoryMap) gnsstk::NDFUniqIterator<gnsstk::NavDataFactoryMap>;
%include "MultiFormatNavDataFactory.hpp"
%include "NDFUniqConstIterator.hpp"
%template(NDFUniqConstIterator_NavDataFactoryMap) gnsstk::NDFUniqConstIterator<gnsstk::NavDataFactoryMap>;
%include "NavDataFactoryStoreCallback.hpp"
%include "NewNavToRinex.hpp"
%include "OrbitDataSP3.hpp"
%include "PNBNavDataFactory.hpp"
%feature("flatnested");
%include "PNBBDSD1NavDataFactory.hpp"
%feature("flatnested", "");
%feature("flatnested");
%include "PNBBDSD2NavDataFactory.hpp"
%template() std::list<gnsstk::AlmPtr>;
%feature("flatnested", "");
%include "PNBGLOCNavDataFactory.hpp"
%include "PNBGLOFNavDataFactory.hpp"
%include "PNBGPSCNav2DataFactory.hpp"
%include "PNBGPSCNavDataFactory.hpp"
%include "PNBGPSLNavDataFactory.hpp"
%include "PNBGalFNavDataFactory.hpp"
%include "PNBGalINavDataFactory.hpp"
%include "PNBMultiGNSSNavDataFactory.hpp"
%include "RinexNavDataFactory.hpp"
%include "RinexTimeOffset.hpp"
%include "SEMNavDataFactory.hpp"
%include "SP3NavDataFactory.hpp"
%include "YumaNavDataFactory.hpp"

%feature("director") gnsstk::NavDataFactoryCallback;


// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================
STR_NNDUMP_DETAIL_HELPER(NavLibrary)
STR_NNDUMP_DETAIL_HELPER(NavDataFactory)
STR_NNDUMP_DETAIL_HELPER(OrbitDataKepler)
STR_NNDUMP_DETAIL_HELPER(NavData)
STR_STREAM_HELPER(NavSignalID)
STR_STREAM_HELPER(NavSatelliteID)
STR_STREAM_HELPER(NavMessageID)

// =============================================================
//  Section 14: Final clean-up
// =============================================================
%pythoncode %{
        # clean-up the dir listing by removing *_swigregister.
        import gnsstk
        stuff=None
        to_remove=[]
        for stuff in locals():
            if stuff.endswith('_swigregister'):
                to_remove.append(stuff)
        for stuff in to_remove:
            del locals()[stuff]
        del locals()['stuff']
        del locals()['to_remove']
    %}
