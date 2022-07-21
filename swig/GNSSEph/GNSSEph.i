%module GNSSEph

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
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%import(module="gnsstk") "Exception.hpp"
%import "CommonTime.hpp"
%import "SatelliteSystem.hpp"
%import(module="gnsstk.GNSSCore") "SatID.hpp"
%import "Triple.hpp"
%import(module="gnsstk.GNSSCore") "Xvt.hpp"
%import "ObsID.hpp"
%import "SVHealth.hpp"
%import "Position.hpp"
%import "NavValidityType.hpp"
%import "NavSearchOrder.hpp"
%import "NavLibrary.hpp"
%import "ValidType.hpp"

%include "AlmOrbit.hpp"
%include "NMCTMeta.hpp"
%include "EngNav.hpp"
%include "BrcClockCorrection.hpp"
%include "GPS_URA.i"
%include "BrcKeplerOrbit.hpp"
%include "NavType.hpp"
%include "NavID.i"
%include "PackedNavBits.hpp"
%include "EngAlmanac.hpp"
%include "EngEphemeris.hpp"
%include "EphemerisRange.hpp"
%include "GloEphemeris.hpp"
%include "SP3SatID.hpp"


// =============================================================
//  Section 13: Aggregated features (e.g. string translation)
// =============================================================
STR_DUMP_HELPER(AlmOrbit)
STR_DUMP_HELPER(BrcClockCorrection)
STR_DUMP_HELPER(BrcKeplerOrbit)
STR_DUMP_HELPER(EngAlmanac)
STR_DUMP_HELPER(EngEphemeris)
STR_DUMP_HELPER(EngNav)
STR_DUMP_HELPER(GloEphemeris)
AS_STRING_HELPER(NavID)

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
