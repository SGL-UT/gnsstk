%module "ClockModel"

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

%import "CommonTime.hpp"
%import "SatID.hpp"
%import "ValidType.hpp"
%import "Triple.hpp"
%import "Position.hpp"
%import "NavLibrary.hpp"
%import "EllipsoidModel.hpp"
%import "NavSearchOrder.hpp"
%import "SVHealth.hpp"
%import "NavValidityType.hpp"
%import "IonoModelStore.hpp"
%import "CarrierBand.hpp"
%import "TropModel.hpp"
%import "Stats.hpp"
%import(module="gnsstk.GNSSCore") "ObsID.i"

%include "ClockModel.hpp"
%include "SvObsEpoch.hpp"
%template(std_map_ObsEpoch) std::map<gnsstk::SatID, gnsstk::SvObsEpoch>;
%include "ObsEpochMap.hpp"
%template(ObsEpochMap) std::map<gnsstk::CommonTime, gnsstk::ObsEpoch>;
%include "ObsRngDev.hpp"
%include "ORDEpoch.hpp"
%include "ObsClockModel.hpp"
%include "EpochClockModel.hpp"
%include "LinearClockModel.hpp"

// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================

STR_STREAM_HELPER(SvObsEpoch)
STR_STREAM_HELPER(ObsEpoch)

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
