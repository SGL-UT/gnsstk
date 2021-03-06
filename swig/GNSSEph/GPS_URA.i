%exceptionclass Exception;

%include "Exception.i"

%include "GPS_URA.hpp"

// Wrappers on the GPS_URA arrays that SWIG can't use.
// Also wraps a similar array from GNSSconstants.hpp
// (it puts them in PyObject double* wrappers, which are useless in Python)
// For each array:
//  - add an ignore
//  - add the function
//  - add the function to the override dict key for constants in gnsstk_builder.py

%inline {
   double sv_accuracy_gps_min_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_ACCURACY_GPS_MAX_INDEX_VALUE)
         return gnsstk::SV_ACCURACY_GPS_MIN_INDEX[index];
   }

   double sv_accuracy_gps_nominal_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_ACCURACY_GPS_MAX_INDEX_VALUE)
         return gnsstk::SV_ACCURACY_GPS_NOMINAL_INDEX[index];
   }

   double sv_accuracy_gps_max_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_ACCURACY_GPS_MAX_INDEX_VALUE)
         return gnsstk::SV_ACCURACY_GPS_MAX_INDEX[index];
   }

   double sv_cnav_accuracy_gps_min_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_CNAV_NOMINAL_MAX_INDEX)
         return gnsstk::SV_CNAV_ACCURACY_GPS_MIN_INDEX[index];
   }

   double sv_cnav_accuracy_gps_nom_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_CNAV_NOMINAL_MAX_INDEX)
         return gnsstk::SV_CNAV_ACCURACY_GPS_NOM_INDEX[index];
   }

   double sv_cnav_accuracy_gps_max_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_CNAV_NOMINAL_MAX_INDEX)
         return gnsstk::SV_CNAV_ACCURACY_GPS_MAX_INDEX[index];
   }

   double sv_accuracy_glo_index(int index)
   {
      CHECK_BOUNDS(gnsstk::SV_ACCURACY_GLO_INDEX_MAX)
         return gnsstk::SV_ACCURACY_GLO_INDEX[index];
   }
 }

