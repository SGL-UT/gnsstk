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
//  Section 6: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "Exception.i"

// =============================================================
//  Section 8: Otherwise undefined std classes that SWIG complains about
// =============================================================
%import "STLTemplates.i"
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
%import(module="gnsstk.GNSSCore") "EllipsoidModel.hpp"
%import "Vector.hpp"
%import "Triple.hpp"
%import(module="gnsstk.FileHandling") "FFData.hpp"
%import "FFStream.hpp"
%import(module="gnsstk.FileHandling") "FFTextStream.hpp"
%import "Position.hpp"
%import "Matrix.hpp"
%import "NavLibrary.hpp"

%include "AntexBase.hpp"
%feature("flatnested");
%include "AntexData.hpp"
%feature("flatnested", "");
%include "AntexHeader.hpp"
%include "AntexStream.hpp"
%apply std::string& OUTPUT {std::string& name }; 
%include "AntennaStore.hpp"
%clear std::string& name; 
%include "EphTime.hpp"
%include "AtmLoadTides.hpp"
%include "CubicSpline.hpp"
/* %include "SatPass.hpp" */
/* %include "DiscCorr.hpp" */
%include "IERSConvention.hpp"
%include "EarthOrientation.hpp"
%include "EOPPrediction.hpp"
%include "EOPStore.hpp"
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
%typemap(in) double PV[6] {
}
%typemap(argout) double PV[6] {
    $result = PyList_New(6);
    for (int i = 0; i < 6; ++i) {
        PyList_SET_ITEM($result, i, PyFloat_FromDouble($1[i]));
    }
}
%apply double[6] { double PV[6] };
%ignore SolarSystemEphemeris::relativeInertialPositionVelocity(double, SolarSystemEphemeris::Planet, SolarSystemEphemeris::Planet, double[6], bool);
%rename(relativeInertialPositionVelocity) SolarSystemEphemeris::relativeInertialPositionVelocity(double, SolarSystemEphemeris::Planet, SolarSystemEphemeris::Planet, bool);
%extend SolarSystemEphemeris {
    void relativeInertialPositionVelocity(double MJD, Planet target, Planet center, bool kilometers=true) {
        double PV[6];
        $self->relativeInertialPositionVelocity(MJD, target, center, PV, kilometers);
        return PV;  // Will be handled by the argout typemap
    }
}
%include "SolidEarthTides.hpp"
%include "SunEarthSatGeometry.i"
%include "SolarSystem.hpp"
%include "PreciseRange.hpp"
/*%apply double& INOUT {double& shadow}; */
%include "PhaseWindup.hpp"
/* %clear double& shadow; */
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
