// Include all the appropriate swig files for this subdirectory
%include "BinexFilterOperators.i"
%include "Rinex3NavData.i"
%include "Rinex3NavFilterOperators.i"
%include "Rinex3ObsFilterOperators.i"
 // Explicitly included by gpstk_swig.i to achieve proper wrapping
 //%include "Rinex3ObsHeader.i"
%include "RinexMetData.i"
%include "RinexNavData.i"
%include "RinexMetFilterOperators.i"
%include "RinexNavFilterOperators.i"
%include "RinexObsFilterOperators.i"
