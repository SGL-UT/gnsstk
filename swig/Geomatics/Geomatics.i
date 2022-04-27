%module Geomatics

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
//  Section 8: Otherwise undefined std classes that SWIG complains about
// =============================================================

namespace std
{
   class fstream {};
}

// =============================================================
//  Section 10: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================
%import "TimeSystem.hpp"
%import "CommonTime.hpp"
%import "SatelliteSystem.hpp"
%import(module="gnsstk.GNSSCore") "SatID.hpp"
%import "Vector.hpp"
%import "Triple.hpp"
%import(module="gnsstk.FileHandling") "FFData.hpp"
%import "FFStream.hpp"
%import(module="gnsstk.FileHandling") "FFTextStream.hpp"
%import "Position.hpp"
%import "Matrix.hpp"

%include "AntexBase.hpp"
%feature("flatnested");
%include "AntexData.hpp"
%feature("flatnested", "");
%include "AntexHeader.hpp"
%include "AntexStream.hpp"
%include "AntennaStore.hpp"
%include "EphTime.hpp"
%include "AtmLoadTides.hpp"
%include "CubicSpline.hpp"
/* %include "SatPass.hpp" */
/* %include "DiscCorr.hpp" */
%include "IERSConvention.hpp"
/* %include "EarthOrientation.hpp" */
/* %include "EOPPrediction.hpp" */
/* %include "EOPStore.hpp" */
%include "RobustStats.hpp"
%include "StatsFilterHit.hpp"
%include "FDiffFilter.hpp"
%feature("flatnested");
%include "FirstDiffFilter.hpp"
%feature("flatnested", "");
%include "GSatID.hpp"
 //%include "IERS1996NutationData.hpp"
 //%include "IERS1996UT1mUTCData.hpp"
 //%include "IERS2003NutationData.hpp"
/* %include "IERS2010CIOSeriesData.hpp" */
%include "Namelist.i"
/* %include "SRIMatrix.hpp" */
/* %include "SparseVector.hpp" */
/* %include "SparseMatrix.hpp" */
/* %include "SRI.hpp" */
/* %include "SRIFilter.hpp" */
%feature("flatnested");
%include "KalmanFilter.hpp"
%feature("flatnested", "");
%include "MostCommonValue.hpp"
%feature("flatnested");
%include "OceanLoadTides.hpp"
%feature("flatnested", "");
%include "SolarSystemEphemeris.hpp"
%include "SolidEarthTides.hpp"
%include "SunEarthSatGeometry.i"
/* %include "SolarSystem.hpp" */
/* %include "PreciseRange.hpp" */
/* %include "Rinex3ObsFileLoader.hpp" */
/* %include "SRIleastSquares.hpp" */
/* %include "SatPassIterator.hpp" */
%include "msecHandler.hpp"
/* %include "SatPassUtilities.hpp" */
%include "SolarPosition.i"
/* %include "SpecialFuncs.hpp" */
%include "WNJfilter.hpp"
%feature("flatnested");
%include "WindowFilter.hpp"
%feature("flatnested", "");
%include "format.hpp"
/* %include "gdc.hpp" */
%include "random.hpp"

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
