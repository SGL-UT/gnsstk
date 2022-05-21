%module RefTime

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
#include "TimeSystem.hpp"
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

ENUM_MAPPER(gnsstk::TimeSystem, TimeSystem, "gnsstk")
ENUM_MAPPER(gnsstk::ReferenceFrame, ReferenceFrame, "gnsstk")
%include "TimeSystem.hpp"
%include "ReferenceFrame.hpp"

%include "renameEnums.i"
%pythoncode %{
   renameEnums('TimeSystem')
   renameEnums('ReferenceFrame')
%}
%include "cleanup.i"

// =============================================================
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================
%import "Vector.hpp"
%import "Triple.hpp"
%import "Xvt.hpp"
%import "Position.hpp"
%import "EngNav.hpp"
%import "FFData.hpp"
%import "RinexObsBase.hpp"
%import "NavLibrary.hpp"

%include "TimeSystemConverter.hpp"
%include "BasicTimeSystemConverter.hpp"
%include "ReferenceFrame.hpp"
%include "HelmertTransform.hpp"
%include "NavTimeSystemConverter.hpp"
%include "TimeSystem.hpp"
%include "TimeSystemCorr.hpp"

// =============================================================
//  Section 11: Explicit Python wrappers
// =============================================================

%pythoncode %{

from gnsstk.TimeHandling import CommonTime

def now(timeSystem=TimeSystem.UTC):
    """
    Returns the current time (defined by what SystemTime() returns)
    in a CommonTime format, in the given TimeSystem.

    Parameters:
            -----------

        timeSystem:  the TimeSystem to assign to the output
    """
    t = SystemTime().toCommonTime()
    t.setTimeSystem(timeSystem)
    return t


def times(starttime, endtime, seconds=0.0, days=0):
    """
    Returns a generator expression of CommonTime objects between (or equal to)
    starttime and endtime.

    You may specify a timestep in seconds (floating/integral type)
    and/or days (integral type). Not specifying a timestep will
    return a generator that yields the starttime and endtime parameters.
    The timestep must be positive, or a gnsstk.exceptions.InvalidRequest
    will be raised.
    """
    if (seconds < 0.0) or (days < 0):
        raise InvalidRequest('Negative time steps may not be used.')
        return

    if (seconds == 0.0) and (days == 0):
        # empty generator:
        yield starttime
        yield endtime
        return

    t = CommonTime(starttime)
    while t <= endtime:
        yield CommonTime(t)
        t.addSeconds(seconds)
        t.addDays(days)

%}

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
