%module FileHandling

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

%include "IonexStoreStrategy.hpp"

%include "renameEnums.i"
%pythoncode %{
   renameEnums('IonexStoreStrategy')
%}
%include "cleanup.i"

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 8: C++ container template instances using only atomic types
// =============================================================

%import "STLTemplates.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%import(module="gnsstk.Utilities") "Utilities.i"
%import(module="gnsstk") "Exception.hpp"
%import "TimeSystem.hpp"
%import "CommonTime.hpp"
%import "TimeTag.hpp"
%import "SatelliteSystem.hpp"
%import "SatID.hpp"
%import "Vector.hpp"
%import "Triple.hpp"
%import "CivilTime.hpp"
%import "XmitAnt.hpp"
%import "ObsID.hpp"
%import "Position.hpp"
%import "Matrix.hpp"
%import "TimeTag.hpp"
%import "Week.hpp"
%import "WeekSecond.hpp"
%import "GPSWeekSecond.hpp"
%import "EngNav.hpp"
%import "RinexSatID.hpp"
%import "EngEphemeris.hpp"
%import "RinexObsID.hpp"
%import "FileStore.hpp"
%import(module="gnsstk") "FormattedDouble.hpp"
%import "RNDouble.hpp"
%import "TimeSystemCorr.hpp"
%import "SP3SatID.hpp"

%include "FFStreamError.hpp"
%include "FFData.hpp"
%include "FFStream.hpp"
%include "FFTextStream.hpp"
%include "BLQDataReader.hpp"
%feature("flatnested");
%include "BinexData.hpp"
%feature("flatnested", "");
%template(std_binary_function_BinexData) std::binary_function<gnsstk::BinexData, gnsstk::BinexData, bool>;
%include "BinexFilterOperators.i"
%include "FFBinaryStream.hpp"
%include "BinexStream.hpp"
%include "ConfDataReader.hpp"
 /* %include "ConfDataWriter.hpp" */ // error on typemaps for list of double
%include "DCBDataReader.hpp"
%include "IonexBase.hpp"
%feature("flatnested");
%include "IonexHeader.hpp"
%feature("flatnested", "");
%template(FileStore_IonexHeader) gnsstk::FileStore<gnsstk::IonexHeader>;
%include "IonexStream.hpp"
%feature("flatnested");
%include "IonexData.hpp"
%feature("flatnested", "");
%include "IonexStoreStrategy.hpp"
%include "IonexStore.hpp"
%feature("flatnested");
%include "Rinex3ClockBase.hpp"
%feature("flatnested", "");
%include "Rinex3ClockData.hpp"
%include "Rinex3ClockHeader.hpp"
%include "Rinex3ClockStream.hpp"
%include "Rinex3NavBase.hpp"
%include "Rinex3NavHeader.hpp"
%template(std_unary_function_Rinex3NavHeader) std::unary_function<gnsstk::Rinex3NavHeader, bool>;
%include "Rinex3NavStream.hpp"
%include "RinexNavBase.hpp"
%include "RinexNavData.i"
%template(std_binary_function_RinexNavData) std::binary_function<gnsstk::RinexNavData,gnsstk::RinexNavData,bool>;
%template(std_unary_function_RinexNavData) std::unary_function<gnsstk::RinexNavData,bool>;
%include "Rinex3NavData.i"
%template(std_binary_function_Rinex3NavData) std::binary_function<gnsstk::Rinex3NavData,gnsstk::Rinex3NavData,bool>;
%template(std_unary_function_Rinex3NavData) std::unary_function<gnsstk::Rinex3NavData,bool>;
%include "Rinex3NavFilterOperators.i"
%include "Rinex3ObsBase.hpp"
%include "Rinex3ObsHeader.i"
%include "RinexDatum.hpp"
%template(std_vector_RinexDatum) std::vector<gnsstk::RinexDatum>;
%template(std_map_RinexSatID_std_vector_RinexDatum) std::map<gnsstk::RinexSatID, std::vector<gnsstk::RinexDatum> >;
%template(std_map_RinexObsType_RinexDatum) std::map<gnsstk::RinexObsType, gnsstk::RinexDatum>;
%template(std_map_RinexSatID_RinexObsTypeMap) std::map<gnsstk::SatID, std::map<gnsstk::RinexObsType, gnsstk::RinexDatum> >;
%include "Rinex3ObsData.hpp"
%template(std_vector_Rinex3ObsData) std::vector<gnsstk::Rinex3ObsData>;
%template(std_binary_function_Rinex3ObsData) std::binary_function<gnsstk::Rinex3ObsData,gnsstk::Rinex3ObsData,bool>;
%include "Rinex3ObsFilterOperators.i"
%include "Rinex3ObsStream.hpp"
%feature("flatnested");
%include "RinexClockBase.hpp"
%feature("flatnested", "");
%include "RinexClockData.hpp"
%feature("flatnested");
%include "RinexClockHeader.hpp"
%feature("flatnested", "");
%include "RinexClockStream.hpp"
%include "RinexMetBase.hpp"
%feature("flatnested");
%include "RinexMetHeader.hpp"
%feature("flatnested", "");
%template(std_unary_function_RinexMetHeader) std::unary_function<gnsstk::RinexMetHeader,bool>;
%include "RinexMetData.i"
%template(std_binary_function_RinexMetData) std::binary_function<gnsstk::RinexMetData,gnsstk::RinexMetData,bool>;
%include "RinexMetFilterOperators.i"
%include "RinexMetStream.hpp"
%include "RinexNavHeader.hpp"
%template (FileStore_RinexNavHeader) gnsstk::FileStore<gnsstk::RinexNavHeader>;
%template(std_unary_function_RinexNavHeader) std::unary_function<gnsstk::RinexNavHeader,bool>;
%include "RinexNavFilterOperators.i"
%include "RinexNavStream.hpp"
%include "RinexObsBase.hpp"
%feature("flatnested");
%include "RinexObsHeader.hpp"
%feature("flatnested", "");
%template(std_vector_RinexObsType) std::vector<gnsstk::RinexObsType>;
%template(std_unary_function_RinexObsHeader) std::unary_function<gnsstk::RinexObsHeader, bool>;
%include "RinexObsData.hpp"
%template(std_binary_function_RinexObsData) std::binary_function<gnsstk::RinexObsData,gnsstk::RinexObsData,bool>;
%include "RinexObsFilterOperators.i"
%include "RinexObsStream.hpp"
%include "RinexUtilities.hpp"
%include "SEMBase.hpp"
%include "SEMHeader.hpp"
%template(FileStore_SEMHeader) gnsstk::FileStore<gnsstk::SEMHeader>;
%include "SEMData.hpp"
%include "SEMStream.hpp"
%include "SP3Base.hpp"
%include "SP3Data.hpp"
%include "SP3Header.hpp"
%include "SP3Stream.hpp"
%include "SatDataReader.hpp"
/* %include "SinexBase.hpp" */
/* %include "SinexHeader.hpp" */
/* %include "SinexTypes.hpp" */
/* %include "SinexBlock.hpp" */
/* %include "SinexData.hpp" */
/* %include "SinexStream.hpp" */
%include "StreamBuf.hpp"
%include "TimeNamedFileStream.hpp"
%include "YumaBase.hpp"
%include "YumaHeader.hpp"
%template(FileStore_YumaHeader) gnsstk::FileStore<gnsstk::YumaHeader>;
%include "YumaData.hpp"
%include "YumaStream.hpp"

// =============================================================
//  Section 11: Explicit Python wrappers
// =============================================================
%include "Stream.i"

// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================

STR_DUMP_HELPER(BinexData)
STR_DUMP_HELPER(Rinex3ClockData)
STR_DUMP_HELPER(Rinex3ClockHeader)
STR_DUMP_HELPER(Rinex3NavData)
STR_DUMP_HELPER(Rinex3NavHeader)
STR_DUMP_HELPER(Rinex3ObsData)
STR_DUMP_HELPER(Rinex3ObsHeader)
STR_DUMP_HELPER(RinexClockHeader)
STR_DUMP_HELPER(RinexClockData)
STR_DUMP_HELPER(RinexMetData)
STR_DUMP_HELPER(RinexMetHeader)
STR_DUMP_HELPER(RinexNavData)
STR_DUMP_HELPER(RinexNavHeader)
STR_DUMP_HELPER(RinexObsData)
STR_DUMP_HELPER(RinexObsHeader)
STR_DUMP_HELPER(SEMData)
STR_DUMP_HELPER(SEMHeader)
STR_DUMP_HELPER(SP3Data)
STR_DUMP_HELPER(SP3Header)
STR_DUMP_HELPER(SP3SatID)
STR_DUMP_HELPER(YumaData)
STR_DUMP_HELPER(YumaHeader)

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
