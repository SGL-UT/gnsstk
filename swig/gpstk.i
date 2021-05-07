%module gpstk

%{
// The header includes are kept in a separate file so they can be used
// to build other swig modules
#include "gpstk_swig.hpp"
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
%include "std_shared_ptr.i"
%include "typemaps.i"
%include "exception.i"

// =============================================================
//  Section 2: C++ exception class handling
// =============================================================

%exceptionclass Exception;

%include "GPSTkException.i"

// =============================================================
//  Section 3: C++11 shared_ptr handling
// =============================================================

 /* note: If you get this compiler error
  *
  * In function ‘PyObject* _wrap_delete_OrbitDataKepler(PyObject*, PyObject*)’:
  * error: ‘smartarg1’ was not declared in this scope
  *
  * That means that you have a derived class from a base smart pointer
  * that needs to be declared in the list below.  In this case,
  * OrbitDataKepler needed to be added to the %shared_ptr declarations
  * below.
  */
%shared_ptr(gpstk::NavData)
%shared_ptr(gpstk::TimeOffsetData)
%shared_ptr(gpstk::NavHealthData)
%shared_ptr(gpstk::OrbitData)
%shared_ptr(gpstk::OrbitDataKepler)
%shared_ptr(gpstk::GPSLNavData)
%shared_ptr(gpstk::GPSLNavEph)
%shared_ptr(gpstk::GPSLNavAlm)
%shared_ptr(gpstk::GPSLNavHealth)
%shared_ptr(gpstk::GPSLNavTimeOffset)
%shared_ptr(gpstk::GPSCNavData)
%shared_ptr(gpstk::GPSCNavEph)
%shared_ptr(gpstk::GPSCNavAlm)
%shared_ptr(gpstk::GPSCNavHealth)
%shared_ptr(gpstk::GPSCNavTimeOffset)
%shared_ptr(gpstk::GPSCNavRedAlm)
%shared_ptr(gpstk::GPSCNav2Eph)
%shared_ptr(gpstk::GPSCNav2Alm)
%shared_ptr(gpstk::GPSCNav2Health)
%shared_ptr(gpstk::GPSCNav2TimeOffset)
%shared_ptr(gpstk::OrbitDataSP3)
%shared_ptr(gpstk::RinexTimeOffset)

%shared_ptr(gpstk::NavDataFactory)
%shared_ptr(gpstk::NavDataFactoryWithStore)
%shared_ptr(gpstk::NavDataFactoryWithStoreFile)
%shared_ptr(gpstk::RinexNavDataFactory)
%shared_ptr(gpstk::SP3NavDataFactory)
%shared_ptr(gpstk::SEMNavDataFactory)
%shared_ptr(gpstk::YumaNavDataFactory)
%shared_ptr(gpstk::MultiFormatNavDataFactory);
%shared_ptr(gpstk::PNBNavDataFactory)
%shared_ptr(gpstk::PNBGPSLNavDataFactory)
%shared_ptr(gpstk::PNBGPSCNavDataFactory)
%shared_ptr(gpstk::PNBGPSCNav2DataFactory)
%shared_ptr(gpstk::PNBMultiGNSSNavDataFactory)
 // silence warnings from swig
%shared_ptr(gpstk::TimeSystemCorrection)

// =============================================================
//  Section 4: Renaming C++ methods and types to Python
// =============================================================

%rename(__str__) *::asString() const;
%rename(toCommonTime) *::convertToCommonTime() const;
 // Long doubles are used in several interfaces but
 // swig really doesn't handle them
%apply double { long double };

// =============================================================
//  Section 5: C++ things to ignore when generating code
// =============================================================

%ignore gpstk::Triple::operator[](const size_t index);
%ignore gpstk::Triple::operator()(const size_t index);
%ignore gpstk::Triple::operator*(double right, const Triple& rhs);
%ignore gpstk::Triple::theArray;
// Ignores on things we can't wrap
%ignore *::dump;  // takes a stream as a parameter
%ignore operator<<;
%ignore operator>>;
%ignore *::operator<<;
%ignore *::operator>>;
%ignore *::operator=;
%ignore *::operator++;
%ignore *::operator--;
%ignore *::operator||;
%ignore *::operator&&;
// The gpstk::StringUtils generate a bunch of shadowing warnings,
// since SWIG can't tell them apart.
// This may be fixed in SWIG 4.0.
%ignore gpstk::StringUtils::asString;
%ignore gpstk::SatMetaData::asString;
%ignore gpstk::scanTime(TimeTag& btime, const std::string& str, const std::string& fmt);
%ignore gpstk::scanTime(CommonTime& btime, const std::string& str, const std::string& fmt);
%ignore gpstk::mixedScanTime(CommonTime& btime, const std::string& str, const std::string& fmt);
%ignore gpstk::Position::convertSphericalToCartesian(const Triple& tpr, Triple& xyz) throw();
%ignore gpstk::Position::convertCartesianToSpherical(const Triple& xyz, Triple& tpr) throw();
%ignore gpstk::Position::convertCartesianToGeodetic(const Triple& xyz, Triple& llh, const double A, const double eccSq) throw();
%ignore gpstk::Position::convertGeodeticToCartesian(const Triple&, llh, Triple& xyz, const double A, const double eccSq) throw();
%ignore gpstk::Position::convertCartesianToGeocentric(const Triple& xyz, Triple& llr) throw();
%ignore gpstk::Position::convertGeocentricToCartesian(const Triple& llr, Triple& xyz) throw();
%ignore gpstk::Position::convertGeocentricToGeodetic(const Triple& llr, Triple& geodeticllr, const double A, const double eccSq) throw();
%ignore gpstk::Position::convertGeodeticToGeocentric(const Triple& geodeticllh, Triple& llr, const double A, const double eccSq) throw();
%ignore gpstk::CivilTime::MonthNames;
%ignore gpstk::CivilTime::MonthAbbrevNames;

// This is to silence warning about not knowing about the fstream base class
namespace std
{
   class fstream {};
   template <typename ArgumentType, typename ResultType> struct unary_function {};
   template <class Arg1, class Arg2, class Result> struct binary_function {};
}

// =============================================================
//  Section 6: C++ container template instances using only atomic types
// =============================================================

// see pythonfunctions.i for stream output

%template(vector_int)        std::vector<int>;
%template(vector_double)     std::vector<double>;
%template(vector_string)     std::vector<std::string>;
%template(map_int_char)      std::map<int, char>;
%template(map_int_string)    std::map<int, std::string>;
%template(map_string_int)    std::map<std::string, int>;
%template(map_string_double) std::map<std::string, double>;

// =============================================================
//  Section 7: typemaps that must be declared before the %includes
// =============================================================

// convert output gpstk::NavDataPtr references

%typemap(in, numinputs=0) std::shared_ptr<gpstk::NavData> &navOut ()
{
   std::shared_ptr<gpstk::NavData> *smartresult = 
      new std::shared_ptr<gpstk::NavData>();
   $1 = smartresult;
}

%typemap(argout) std::shared_ptr<gpstk::NavData> &
{
      // What this does is change the python interface to the C++
      // method so that you get a list containing the original C++
      // return value (typically a bool) and the resulting NavData
      // object.
      // The shared_ptr<NavData> object needs to be converted to the
      // leaf class in order for python to be able to use it properly.
   if (!PyList_Check(resultobj))
   {
         // Turn the return value into a list and add the original
         // return value as the first member of the list.
      PyObject *temp = resultobj;
      resultobj = PyList_New(1);
      PyList_SetItem(resultobj, 0, temp);
      temp = nullptr;
         // First check to see if the NavData object has been set.
         // Usually it will be unset if, for example, find() fails,
         // and the bool return value will also be false in this case.
      if ($1->get() != nullptr)
      {
            // The NavData tree has a getClassName() method that
            // returns the qualified class name,
            // e.g. gpstk::GPSLNavEph.  We add the shared_ptr
            // qualifiers in order for SWIG_TypeQuery to get the
            // correct python data type that we need to return.
         std::string cn = "std::shared_ptr< " + (*$1)->getClassName() + " > *";
         swig_type_info *desc = SWIG_TypeQuery(cn.c_str());
         if (desc != nullptr)
         {
               // We have a valid python type, so now we can create a
               // SWIG pointer that properly identifies the derived
               // shared_ptr<NavData> object.
            temp = SWIG_NewPointerObj(SWIG_as_voidptr($1), desc,
                                      SWIG_POINTER_OWN);
         }
      }
      if (temp == nullptr)
      {
            // We were unable to get a valid python type, probably
            // because the output shared_ptr<NavData> object was null,
            // so create a null shared_ptr<NavData> python object to
            // use as the return value.
            // This gets turned into "None" in python.
         temp = SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor,
                                   SWIG_POINTER_OWN);
      }
         // Add the derived shared_ptr<NavData> object to the return list
      PyList_Append(resultobj, temp);
      Py_DECREF(temp);
   }
}

// convert output gpstk::SVHealth references

%typemap(in, numinputs=0) gpstk::SVHealth &healthOut (gpstk::SVHealth temp)
{
      // healthOut typemap(in)
   temp = gpstk::SVHealth::Unknown;
   $1 = &temp;
}

%typemap(argout) gpstk::SVHealth&
{
      // healthOut typemap(argout)
      // What this does is change the python interface to the C++
      // method so that you get a list containing the original C++
      // return value (typically a bool) and the resulting NavData
      // object.
      // The shared_ptr<NavData> object needs to be converted to the
      // leaf class in order for python to be able to use it properly.
   if (!PyList_Check(resultobj))
   {
         // Turn the return value into a list and add the original
         // return value as the first member of the list.
      PyObject *temp = resultobj;
      resultobj = PyList_New(1);
      PyList_SetItem(resultobj, 0, temp);
      PyObject* sys_mod_dict = PyImport_GetModuleDict();
      PyObject* gpstk_mod = PyMapping_GetItemString(sys_mod_dict, "gpstk");
      PyObject* enum_instance = PyObject_CallMethod(gpstk_mod, "SVHealth", "i", (int)*$1);
      Py_DECREF(gpstk_mod);
      PyList_Append(resultobj, enum_instance);
   }
}

// =============================================================
// These typemaps must occur before the headers are included.
// =============================================================

%include "gpstk_typemaps.i"
%include "gpstk_enum_typemaps.i"

// =============================================================
//  Section 8: C++ include files
//  Include classes IN DEPENDENCY ORDER otherwise swig will fail.
// =============================================================

%include "Exception.hpp"
%include "TimeConstants.hpp"
%include "TimeSystem.hpp"
%include "TimeSystemConverter.hpp"
%include "CommonTime.hpp"
%include "HexDumpDataConfig.hpp"
// %include "StringUtils.hpp"
%include "TimeTag.hpp"
%include "ANSITime.hpp"
%include "AllanDeviation.hpp"
%include "gps_constants.hpp"
%include "EnumIterator.hpp"
%include "SatelliteSystem.hpp"
%template(vector_GNSS)       std::vector<gpstk::SatelliteSystem>;
%include "SatID.hpp"
%template(vector_SatID)      std::vector<gpstk::SatID>;
%include "MathBase.hpp"
%include "VectorBase.hpp"
%include "VectorBaseOperators.hpp"
%include "Vector.hpp"
%include "VectorOperators.hpp"
%include "Triple.hpp"
%include "EllipsoidModel.hpp"
%include "ReferenceFrame.hpp"
%include "GNSSconstants.hpp"
%include "DeprecatedConsts.hpp"
%include "FreqConsts.hpp"
%include "Xvt.hpp"
%include "AlmOrbit.hpp"
%include "FFStreamError.hpp"
%include "FFData.hpp"
%include "AntexBase.hpp"
%include "FFStream.hpp"
%include "AntexHeader.hpp"
%include "CivilTime.hpp"
%include "AntexData.hpp"
%include "FFTextStream.hpp"
%include "AntexStream.hpp"
%include "AntennaStore.hpp"
%include "DataStatus.hpp"
%include "AshtechData.hpp"
%include "AshtechALB.hpp"
%include "AshtechEPB.hpp"
%include "AshtechMBEN.hpp"
%include "AshtechPBEN.hpp"
%include "FFBinaryStream.hpp"
%include "AshtechStream.hpp"
%include "MJD.hpp"
%include "TimeConverters.hpp"
%include "EphTime.hpp"
%include "AtmLoadTides.hpp"
%include "Rinex3ObsBase.hpp"
%include "ObservationType.hpp"
%include "CarrierBand.hpp"
%include "TrackingCode.hpp"
%include "ObsID.hpp"
%template(vector_ObsID)      std::vector<gpstk::ObsID>;
%template(map_SvObsEpoch) std::map< gpstk::ObsID, double >;
%include "OrbitEph.hpp"
%include "BDSEphemeris.hpp"
%include "XvtStore.hpp"
%template(XvtStore_SatID)  gpstk::XvtStore<gpstk::SatID>;
%template(XvtStore_string) gpstk::XvtStore<std::string>;
%include "OrbitEphStore.hpp"
%include "BDSEphemerisStore.hpp"
%include "Week.hpp"
%include "WeekSecond.hpp"
%include "BDSWeekSecond.hpp"
%include "MatrixBase.hpp"
%include "MatrixBaseOperators.hpp"
%include "Matrix.hpp"
%template(matrix_double) gpstk::Matrix<double>;
%include "MatrixImplementation.hpp"
%include "MiscMath.hpp"
%include "MatrixFunctors.hpp"
%include "MatrixOperators.hpp"
%include "BLQDataReader.hpp"
/* %include "CommandOption.hpp" */
/* %include "CommandOptionParser.hpp" */
/* %include "MainAdapter.hpp" */
/* %include "BasicFramework.hpp" */
%include "BasicTimeSystemConverter.hpp"
%include "BinexData.hpp"
%template(binary_function_BinexData) std::binary_function<gpstk::BinexData, gpstk::BinexData, bool>;
%include "FileSpec.hpp"
%include "FileFilter.hpp"
%include "BinexFilterOperators.hpp"
%include "BinexStream.hpp"
%include "Stats.hpp"
%include "BivarStats.hpp"
%include "GPSWeekSecond.hpp"
%include "NMCTMeta.hpp"
%include "EngNav.hpp"
%include "YDSTime.hpp"
%include "BrcClockCorrection.hpp"
%include "WGS84Ellipsoid.hpp"
%include "GPSEllipsoid.hpp"
%include "RinexSatID.hpp"
%template(vector_RinexSatID) std::vector<gpstk::RinexSatID>;
%include "GPS_URA.hpp"
%include "BrcKeplerOrbit.hpp"
%include "CGCS2000Ellipsoid.hpp"
%include "NavFilterKey.hpp"
%include "NavFilter.hpp"
%include "CNav2SanityFilter.hpp"
%include "NavType.hpp"
%include "NavID.hpp"
%include "PackedNavBits.hpp"
%include "CNavFilterData.hpp"
%include "CNavCookFilter.hpp"
%include "NavFilterMgr.hpp"
%include "CNavCrossSourceFilter.hpp"
%include "SatMetaData.hpp"
%include "SatMetaDataSort.hpp"
%include "SatMetaDataStore.hpp"
/* %include "CNavDataElement.hpp" */
/* %include "CNavDataElementStore.hpp" */
/* %include "CNavEOP.hpp" */
/* %include "CNavEmptyFilter.hpp" */
/* %include "CNavGGTO.hpp" */
/* %include "CNavISC.hpp" */
%include "OrbData.hpp"
%include "OrbDataSys.hpp"
%include "CNavMidiAlm.hpp"
%include "CNavParityFilter.hpp"
%include "CNavReducedAlm.hpp"
%include "CNavTOWFilter.hpp"
/* %include "CNavText.hpp" */
/* %include "CNavUTC.hpp" */
%include "ClockModel.hpp"
%include "TimeString.hpp"
/* %include "TabularSatStore.hpp" */
%include "FileStore.hpp"
/* %include "ClockSatStore.hpp" */
%include "PCodeConst.hpp"
%include "CodeBuffer.hpp"
%include "Combinations.hpp"
/* %include "CommandLine.hpp" */
/* %include "CommandOptionWithCommonTimeArg.hpp" */
%include "Position.hpp"
/* %include "CommandOptionWithPositionArg.hpp" */
/* %include "CommandOptionWithTimeArg.hpp" */
%include "ConfDataReader.hpp"
 /* %include "ConfDataWriter.hpp" */ // error on typemaps for list of double
%include "CubicSpline.hpp"
%include "DCBDataReader.hpp"
%include "DiffCorrBase.hpp"
%include "DiffCorrClk.hpp"
%include "DiffCorrEph.hpp"
%include "GPSZcount.hpp"
%include "UnixTime.hpp"
%include "SystemTime.hpp"
%include "JulianDate.hpp"
%include "GPSWeek.hpp"
%include "GPSWeekZcount.hpp"
%include "GALWeekSecond.hpp"
%include "QZSWeekSecond.hpp"
/* %include "Epoch.hpp" */
%include "RinexObsBase.hpp"
%include "RinexObsHeader.hpp"
%template(unary_function_RinexObsHeader) std::unary_function<gpstk::RinexObsHeader, bool>;
%include "RinexDatum.hpp"
%template(vector_RinexDatum) std::vector<gpstk::RinexDatum>;
%include "RinexObsData.hpp"
%template(binary_function_RinexObsData) std::binary_function<gpstk::RinexObsData,gpstk::RinexObsData,bool>;
%include "GPSEphemeris.hpp"
%include "RinexNavBase.hpp"
%include "EngEphemeris.hpp"
%include "FormattedDouble.hpp"
%include "RNDouble.hpp"
%include "RinexNavData.hpp"
%include "GPSEphemerisStore.hpp"
%include "SP3SatID.hpp"
%include "SP3Base.hpp"
%include "SP3Data.hpp"
/* %include "PositionSatStore.hpp" */
%include "SP3Header.hpp"
%include "Rinex3ClockBase.hpp"
%include "RinexObsID.hpp"
%template(vector_RinexObsID) std::vector<gpstk::RinexObsID>;
%include "Rinex3ClockHeader.hpp"
/* %include "SP3EphemerisStore.hpp" */
%include "RinexUtilities.hpp"
/* %include "SatPass.hpp" */
/* %include "DiscCorr.hpp" */
%include "DumpDetail.hpp"
%include "ENUUtil.hpp"
%include "IERSConvention.hpp"
/* %include "EarthOrientation.hpp" */
/* %include "EOPPrediction.hpp" */
/* %include "EOPStore.hpp" */
%include "EngAlmanac.hpp"
%include "EphemerisRange.hpp"
%include "IonoModel.hpp"
%include "IonoModelStore.hpp"
%include "ValidType.hpp"
%include "SvObsEpoch.hpp"
%template(map_ObsEpoch) std::map<gpstk::SatID, gpstk::SvObsEpoch>;
%include "ObsEpochMap.hpp"
%template(ObsEpochMap) std::map<gpstk::CommonTime, gpstk::ObsEpoch>;
%include "WxObsMap.hpp"
%include "TropModel.hpp"
%include "NBTropModel.hpp"
%include "ObsRngDev.hpp"
%include "ORDEpoch.hpp"
%include "ObsClockModel.hpp"
%include "EpochClockModel.hpp"
%include "Expression.hpp"
%include "StatsFilterHit.hpp"
%include "RobustStats.hpp"
%include "FDiffFilter.hpp"
%include "FileSpecFind.hpp"
%include "FileUtils.hpp"
%include "FileFilterFrame.hpp"
 // Use a special wrapper for the include file which resolves issues
 // with the nested definitions
%include "Rinex3ObsHeader.i"
%template(unary_function_Rinex3ObsHeader) std::unary_function<gpstk::Rinex3ObsHeader, bool>;
%include "Rinex3ObsData.hpp"
%template(vector_Rinex3ObsData) std::vector<gpstk::Rinex3ObsData>;
%template(binary_function_Rinex3ObsData) std::binary_function<gpstk::Rinex3ObsData,gpstk::Rinex3ObsData,bool>;
%include "FileFilterFrameWithHeader.hpp"
%include "FirstDiffFilter.hpp"
%include "GCATTropModel.hpp"
%include "GGHeightTropModel.hpp"
%include "GGTropModel.hpp"
%include "Rinex3NavBase.hpp"
%include "IRNWeekSecond.hpp"
%include "TimeSystemCorr.hpp"
%include "Rinex3NavHeader.hpp"
%template(unary_function_Rinex3NavHeader) std::unary_function<gpstk::Rinex3NavHeader, bool>;
%include "Rinex3NavStream.hpp"
%include "PZ90Ellipsoid.hpp"
%include "GloEphemeris.hpp"
%include "GalEphemeris.hpp"
%include "QZSEphemeris.hpp"
%include "Rinex3NavData.hpp"
%template(binary_function_Rinex3NavData) std::binary_function<gpstk::Rinex3NavData,gpstk::Rinex3NavData,bool>;
%template(unary_function_Rinex3NavData) std::unary_function<gpstk::Rinex3NavData,bool>;
%include "OrbElemBase.hpp"
%include "OrbElemStore.hpp"
%include "GPSAlmanacStore.hpp"
%include "NavSignalID.hpp"
%include "NavSatelliteID.hpp"
%include "NavMessageType.hpp"
%include "NavMessageID.hpp"
%include "NavData.hpp"
%include "OrbitData.hpp"
%include "SVHealth.hpp"
%include "OrbitDataKepler.hpp"
%include "GPSCNav2Alm.hpp"
%include "GPSCNav2Eph.hpp"
%include "NavHealthData.hpp"
%include "GPSCNav2Health.hpp"
%include "TimeOffsetData.hpp"
%include "GPSCNav2TimeOffset.hpp"
%include "GPSCNavData.hpp"
%include "GPSCNavAlm.hpp"
%include "GPSCNavEph.hpp"
%include "GPSCNavHealth.hpp"
%include "GPSCNavRedAlm.hpp"
%include "GPSCNavTimeOffset.hpp"
%include "GPSLNavData.hpp"
%include "GPSLNavAlm.hpp"
%include "GPSLNavEph.hpp"
%include "GPSLNavHealth.hpp"
%include "GPSLNavTimeOffset.hpp"
%include "GPSOrbElemStore.hpp"
%include "GSatID.hpp"
%include "GalEphemerisStore.hpp"
%include "GalileoEllipsoid.hpp"
%include "GenXSequence.hpp"
%include "GenericNavFilterData.hpp"
%include "GloEphemerisStore.hpp"
%include "GlobalTropModel.hpp"
%include "HelmertTransform.hpp"
 //%include "IERS1996NutationData.hpp"
 //%include "IERS1996UT1mUTCData.hpp"
 //%include "IERS2003NutationData.hpp"
/* %include "IERS2010CIOSeriesData.hpp" */
/* %include "LoopedFramework.hpp" */
/* %include "InOutFramework.hpp" */
%include "IonexBase.hpp"
%include "IonexHeader.hpp"
%template(FileStore_IonexHeader) gpstk::FileStore<gpstk::IonexHeader>;
%include "IonexStream.hpp"
%include "IonexData.hpp"
%include "IonexStore.hpp"
%include "Namelist.hpp"
%include "SRIMatrix.hpp"
%include "SparseVector.hpp" // too many warnings
%include "SparseMatrix.hpp" // too many warnings
%include "SRI.hpp"
/* %include "SRIFilter.hpp" */
%include "logstream.hpp"
%include "KalmanFilter.hpp"
%include "LNavFilterData.hpp"
%include "LNavAlmValFilter.hpp"
%include "LNavCookFilter.hpp"
%include "LNavCrossSourceFilter.hpp"
%include "LNavEmptyFilter.hpp"
%include "LNavEphMaker.hpp"
%include "LNavOrderFilter.hpp"
%include "LNavParityFilter.hpp"
%include "LNavTLMHOWFilter.hpp"
%include "LinearClockModel.hpp"
%include "MOPSTropModel.hpp"
%include "MetReader.hpp"
%include "MostCommonValue.hpp"
%include "NavValidityType.hpp"
%include "NavSearchOrder.hpp"
%include "NavDataFactory.hpp"
%include "NavDataFactoryWithStore.hpp"
%include "NavDataFactoryWithStoreFile.hpp"
%include "MultiFormatNavDataFactory.hpp"
%include "NEDUtil.hpp"
%include "NavLibrary.hpp"
%include "NavMsgData.hpp"
%include "NavMsgDataBits.hpp"
%include "NavMsgDataPNB.hpp"
%include "NavMsgDataWords.hpp"
%include "NavOrderFilter.hpp"
%include "NavTimeSystemConverter.hpp"
%include "NeillTropModel.hpp"
%include "OceanLoadTides.hpp"
%include "OceanLoading.hpp"
%include "OrbElem.hpp"
/* %include "OrbAlm.hpp" */
/* %include "OrbAlmFactory.hpp" */
/* %include "OrbAlmGen.hpp" */
/* %include "OrbAlmStore.hpp" */
%include "OrbDataSysFactory.hpp"
%include "OrbDataUTC.hpp"
%include "OrbElemRinex.hpp"
%include "OrbSysGpsC.hpp"
%include "OrbSysGpsC_30.hpp"
%include "OrbSysGpsC_33.hpp"
/* %include "OrbSysGpsC_32.hpp" */
%include "OrbSysGpsL.hpp"
/* %include "OrbSysGpsL_51.hpp" */
%include "OrbSysGpsL_52.hpp"
%include "OrbSysGpsL_55.hpp"
%include "OrbSysGpsL_56.hpp"
/* %include "OrbSysGpsL_63.hpp" */
%include "OrbSysGpsL_Reserved.hpp"
%include "OrbSysStore.hpp"
%include "OrbitDataSP3.hpp"
%include "PNBNavDataFactory.hpp"
%include "PNBGPSCNav2DataFactory.hpp"
%include "PNBGPSCNavDataFactory.hpp"
%include "PNBGPSLNavDataFactory.hpp"
%include "PNBMultiGNSSNavDataFactory.hpp"
%include "stl_helpers.hpp"
%include "PRSolution.hpp"
%include "PRSolutionLegacy.hpp"
%include "PoleTides.hpp"
%include "PolyFit.hpp"
%include "PosixTime.hpp"
 //%include "PowerSum.hpp"
%include "SolarSystemEphemeris.hpp"
%include "SunEarthSatGeometry.hpp"
%include "SolidEarthTides.hpp"
/* %include "SolarSystem.hpp" */
/* %include "PreciseRange.hpp" */
%include "QZSEphemerisStore.hpp"
%include "RACRotation.hpp"
/* %include "RTFileFrame.hpp" */
%include "RationalizeRinexNav.hpp"
%include "Rinex3ClockData.hpp"
%include "Rinex3ClockStream.hpp"
%include "Rinex3EphemerisStore.hpp"
%include "Rinex3NavFilterOperators.hpp"
/* %include "Rinex3ObsFileLoader.hpp" */
%include "Rinex3ObsFilterOperators.hpp"
%include "Rinex3ObsStream.hpp"
%include "RinexClockBase.hpp"
%include "RinexClockData.hpp"
%include "RinexClockHeader.hpp"
%include "RinexClockStream.hpp"
 //%include "RinexClockStore.hpp"
%include "RinexNavHeader.hpp"
%template (FileStore_RinexNavHeader) gpstk::FileStore<gpstk::RinexNavHeader>;
%include "RinexNavStream.hpp"
%include "RinexEphemerisStore.hpp"
%include "RinexMetBase.hpp"
%include "RinexMetHeader.hpp"
%template(unary_function_RinexMetHeader) std::unary_function<gpstk::RinexMetHeader,bool>;
%include "RinexMetData.hpp"
%template(binary_function_RinexMetData) std::binary_function<gpstk::RinexMetData,gpstk::RinexMetData,bool>;
%include "RinexMetFilterOperators.hpp"
%include "RinexMetStream.hpp"
%include "RinexNavDataFactory.hpp"
%include "RinexNavFilterOperators.hpp"
%include "RinexObsFilterOperators.hpp"
%include "RinexObsStream.hpp"
%include "RinexTimeOffset.hpp"
%include "SEMBase.hpp"
%include "SEMHeader.hpp"
%template(FileStore_SEMHeader) gpstk::FileStore<gpstk::SEMHeader>;
%include "SEMData.hpp"
%include "SEMStream.hpp"
%include "SEMAlmanacStore.hpp"
%include "SEMNavDataFactory.hpp"
%include "SP3NavDataFactory.hpp"
%include "SP3Stream.hpp"
%include "SRIleastSquares.hpp"
%include "X1Sequence.hpp"
%include "X2Sequence.hpp"
%include "SVPCodeGen.hpp"
%include "SaasTropModel.hpp"
%include "SatDataReader.hpp"
/* %include "SatPassIterator.hpp" */
%include "msecHandler.hpp"
/* %include "SatPassUtilities.hpp" */
%include "SimpleTropModel.hpp"
/* %include "SinexBase.hpp" */
/* %include "SinexHeader.hpp" */
/* %include "SinexTypes.hpp" */
/* %include "SinexBlock.hpp" */
/* %include "SinexData.hpp" */
/* %include "SinexStream.hpp" */
%include "SolarPosition.hpp"
/* %include "SpecialFuncs.hpp" */
%include "StreamBuf.hpp"
%include "TimeCorrection.hpp"
%include "TimeNamedFileStream.hpp"
%include "TimeRange.hpp"
%include "WNJfilter.hpp"
%include "WindowFilter.hpp"
%include "YumaBase.hpp"
%include "YumaHeader.hpp"
%template(FileStore_YumaHeader) gpstk::FileStore<gpstk::YumaHeader>;
%include "YumaData.hpp"
%include "YumaStream.hpp"
%include "YumaAlmanacStore.hpp"
%include "YumaNavDataFactory.hpp"
%include "convhelp.hpp"
%include "expandtilde.hpp"
%include "format.hpp"
/* %include "gdc.hpp" */
%include "ord.hpp"
%include "random.hpp"
%include "singleton.hpp"

// =============================================================
//  Section 9: Enumerations
// =============================================================

// This code allows us to turn C++ enums into Python enums while
// maintaining compatibility in SWIG bindings.  We specifically use an
// IntEnum class in Python to allow the implicit conversion to int
// when calling C++ code from Python, as SWIG treats the enums as ints
// when generating code.
/** @note During the implementation of this code, Python would
 * commonly issue an exception with the text "Wrong number or type of
 * arguments for overloaded function".  This turned out to be caused
 * by SWIG not having knowledge of the C++ enumeration's definition
 * before generating code to use it, thus resulting in the enumeration
 * being handled like an object.  To resolve this issue, we have
 * inline forward declarations for the enums at the top of this
 * file.
 * However, this must occur _after_ the std*.i files are included, or it won't
 * be able to handle things like python string conversions. */
%pythoncode %{
from enum import IntEnum
def renameEnums(prefix):
    tmpD = {k:v for k,v in globals().items() if k.startswith(prefix+'_')}
    for k,v in tmpD.items():
        del globals()[k]
    tmpD = {k[len(prefix)+1:]:v for k,v in tmpD.items()}
    globals()[prefix] = IntEnum(prefix,tmpD)
    globals()[prefix].__str__ = lambda x: str(x.name)
# Turn the gpstk.SatelliteSystem_* constants into a Python enum
renameEnums('SatelliteSystem')
renameEnums('CarrierBand')
renameEnums('TrackingCode')
renameEnums('ObservationType')
renameEnums('NavType')
renameEnums('TimeSystem')
renameEnums('ReferenceFrame')
renameEnums('NavMessageType')
renameEnums('SVHealth')
renameEnums('NavSearchOrder')
renameEnums('NavValidityType')
renameEnums('DumpDetail')
del renameEnums
del IntEnum
%}

// =============================================================
//  Section 10: Explicit Python wrappers
// =============================================================

%include "Triple.i"
// Encapsulation of many the __str__, __getitem__, etc. functions to
// avoid clutter.  When the only change to a class is adding a simple
// wrapper, add to pythonfunctions instead of creating another small
// file.
%include "pythonfunctions.i"
%include "FileIO.i"
%include "GPS_URA.i"
%include "Position.i"
%include "TimeString.i"
%include "Vector.i"

// =============================================================
//  Section 11: Template declarations
// =============================================================

%include "STLHelpers.i"
/* %include "gpstk_templates.i" */
%template(NavSatelliteIDSet) std::set<gpstk::NavSatelliteID>;
%template(NavMessageIDSet) std::set<gpstk::NavMessageID>;
%template(NavMessageTypeSet) std::set<gpstk::NavMessageType>;

/* %rename(__str__) gpstk::Exception::what() const; */


/* // ============================================================= */
/* //  Section 2: Time classes */
/* // ============================================================= */
/* %rename (toString) *::operator std::string() const; */
/* %ignore *::operator CommonTime() const; */



/* %ignore gpstk::CommonTime::get;  // takes non-const values as parameters for output */
/* %feature("notabstract") UnixTime; */
/* %feature("notabstract") SystemTime; */
/* %feature("notabstract") ANSITime; */
/* %feature("notabstract") CivilTime; */
/* %include "CivilTime.hpp" */
/* %include "GPSZcount.hpp" */
/* %include "GPSWeek.hpp" */
/* %feature("notabstract") GPSWeekSecond; */
/* %include "GPSWeekSecond.hpp" */
/* %feature("notabstract") GPSWeekZcount; */
/* %include "GPSWeekZcount.hpp" */
/* %feature("notabstract") JulianDate; */
/* %include "JulianDate.hpp" */
/* %feature("notabstract") BDSWeekSecond; */
/* %include "BDSWeekSecond.hpp" */
/* %feature("notabstract") GALWeekSecond; */
/* %include "GALWeekSecond.hpp" */
/* %feature("notabstract") QZSWeekSecond; */
/* %include "QZSWeekSecond.hpp" */
/* %feature("notabstract") MJD; */
/* %include "MJD.hpp" */
/* %feature("notabstract") YDSTime; */
/* %include "YDSTime.hpp" */

/* %include "TimeString.hpp" */
/* %include "TimeSystemCorr.hpp" */


/* // ============================================================= */
/* //  Section 3: General/Utils classes */
/* // ============================================================= */
/* // Utils stuff */
%include "SatID.i"
%include "ObsID.i"
%include "NavID.i"
/* %ignore gpstk::SV_ACCURACY_GLO_INDEX;  // wrapper added in GPS_URA.i */

// These are needed to properly interpret an ObsID instance
%template(map_ObservationType_string) std::map<enum gpstk::ObservationType, std::string>;
%template(map_CarrierBand_string) std::map<enum gpstk::CarrierBand, std::string>;
%template(map_TrackingCode_string) std::map<enum gpstk::TrackingCode, std::string>;
%template(map_char_ObservationType) std::map<char, enum gpstk::ObservationType>;
%template(map_char_CarrierBand) std::map<char, enum gpstk::CarrierBand>;
%template(map_char_TrackingCode) std::map<char, enum gpstk::TrackingCode>;
%template(map_ObservationType_char) std::map<enum gpstk::ObservationType,char>;
%template(map_CarrierBand_char) std::map<enum gpstk::CarrierBand,char>;
%template(map_TrackingCode_char) std::map<enum gpstk::TrackingCode,char>;


/* %ignore gpstk::VectorBase::operator[] (size_t i) const; */
/* %ignore gpstk::VectorBase::operator() (size_t i) const; */
/* %ignore gpstk::RefVectorBaseHelper::zeroTolerance; */
/* %ignore gpstk::RefVectorBaseHelper::perator[] (size_t i); */
/* %ignore gpstk::RefVectorBaseHelper::operator() (size_t i); */
/* %ignore gpstk::RefVectorBaseHelper::zeroize(); */
/* %include "Vector.i" */

%pythoncode %{
    import sys
    if sys.version_info[0] < 3:
        from collections import Iterable
    else:
        from collections.abc import Iterable
    enum_vec_classes = [vector_GNSS]
    for cls in enum_vec_classes:
        orig_constructor = cls.__init__
        def new_constructor(self, *args):
            # We assume that the argument is not exhaustible
            if len(args) == 1 and isinstance(args[0], Iterable) and all(isinstance(x, int) for x in args[0]):
                orig_constructor(self)
                for x in args[0]:
                    self.append(x)
            else:
                orig_constructor(self, *args)
        cls.__init__ = new_constructor
%}


/* // ============================================================= */
/* //  Section 4: "XvtStore and friends" */
/* // ============================================================= */
/* // Renames on a few commonly used operators */
/* %rename (toEngEphemeris) *::operator EngEphemeris() const; */
/* %rename (toGalEphemeris) *::operator GalEphemeris() const; */
/* %rename (toGloEphemeris) *::operator GloEphemeris() const; */
/* %rename (toAlmOrbit) *::operator AlmOrbit() const; */

/* %include "AstronomicalFunctions.hpp" */
/* %include "PZ90Ellipsoid.hpp" */
/* %include "WGS84Ellipsoid.hpp" */


/* %include "gpstkplatform.h" */
/* %include "FFStreamError.hpp" */
/* %include "FileStore.hpp" */
/* %include "FFData.hpp" */
/* %include "EngNav.hpp" */
/* %include "YumaBase.hpp" */
/* %include "FFStream.hpp" */
/* %include "FFTextStream.hpp" */
/* %include "AlmOrbit.hpp" */
/* %include "YumaHeader.hpp" */
/* %ignore gpstk::EngAlmanac::getUTC; */
/* %include "EngAlmanac.hpp" */

/* %include "OrbAlmStore.hpp" */
/* %include "OrbElemBase.hpp" */
/* %include "OrbElem.hpp" */
/* %include "OrbElemStore.hpp" */
/* %include "AlmOrbit.hpp" */
/* %include "YumaStream.hpp" */
/* %include "YumaData.hpp" */
/* %include "GPSAlmanacStore.hpp" */
/* %include "YumaAlmanacStore.hpp" */

/* //%include "RinexSatID.hpp" */

/* %ignore gpstk::SV_ACCURACY_GPS_MIN_INDEX; */
/* %ignore gpstk::SV_ACCURACY_GPS_NOMINAL_INDEX; */
/* %ignore gpstk::SV_ACCURACY_GPS_MAX_INDEX; */
/* %ignore gpstk::SV_CNAV_ACCURACY_GPS_MIN_INDEX; */
/* %ignore gpstk::SV_CNAV_ACCURACY_GPS_NOM_INDEX; */
/* %ignore gpstk::SV_CNAV_ACCURACY_GPS_MAX_INDEX; */
/* %include "GPS_URA.hpp" */

/* %include "BrcClockCorrection.hpp" */
/* %include "BrcKeplerOrbit.hpp" */
/* %include "EphemerisRange.hpp" */
/* // %include "EphReader.hpp" */

/* // Ephemeris: */
/* %include "OrbitEph.hpp" */
/* %include "BDSEphemeris.hpp" */
/* %include "EngEphemeris.hpp" */
/* %include "GalEphemeris.hpp" */
/* %include "GloEphemeris.hpp" */
/* %include "GPSEphemeris.hpp" */
/* %include "QZSEphemeris.hpp" */

/* // RINEX format: */
/* %include "RinexSatID.hpp" */
/* %include "RinexObsID.hpp" */
%include "RinexSatID.i"
%include "RinexObsID.i"
/* // RINEX obs: */
/* %include "RinexObsBase.hpp" */
/* %include "RinexObsHeader.hpp" */
/* %include "RinexObsData.hpp" */
/* %include "RinexObsStream.hpp" */
/* // RINEX nav: */
/* %include "RinexNavBase.hpp" */
/* %include "RinexNavHeader.hpp" */
/* %include "RinexNavStream.hpp" */
/* %include "RinexNavData.hpp" */
/* // RINEX meteorological: */
/* %include "RinexMetBase.hpp" */
/* %include "RinexMetHeader.hpp" */
/* %include "RinexMetStream.hpp" */

/* %ignore gpstk::RinexMetData::data; */
/* %include "RinexMetData.hpp" */
/* %include "RinexMetData.i" */

/*  // RINEX 3 nav: */
/* %include "Rinex3NavBase.hpp" */
/* %include "Rinex3NavHeader.hpp" */
/* %include "Rinex3NavStream.hpp" */
/* %include "Rinex3NavData.hpp" */
/* %include "OrbElemRinex.hpp" */

/* // RINEX 3 clock/obs: */
/* %include "RinexDatum.hpp" */
/* %include "Rinex3ClockBase.hpp" */
/* %include "Rinex3ObsBase.hpp" */


/* %include "Rinex3ObsData.hpp" */
/* %include "Rinex3ObsStream.hpp" */
/* %include "Rinex3ClockHeader.hpp" */
/* %include "Rinex3ClockData.hpp" */
/* %include "Rinex3ClockStream.hpp" */
/* %include "Rinex3EphemerisStore.hpp" */

/* // Ephemeris stores: */
/* %include "OrbitEphStore.hpp" */
/* %include "BDSEphemerisStore.hpp" */
/* %include "GalEphemerisStore.hpp" */
/* %include "GloEphemerisStore.hpp" */
/* %include "GPSEphemerisStore.hpp" */
/* %include "QZSEphemerisStore.hpp" */
/* %include "RinexEphemerisStore.hpp" */

/* // SP3 format: */
/* %include "TabularSatStore.hpp" */
/* %include "ClockSatStore.hpp" */
/* %include "SP3Base.hpp" */
/* %include "SP3SatID.hpp" */
/* %include "SP3Header.hpp" */
/* %include "SP3Data.hpp" */
/* %include "SP3Stream.hpp" */
/* %include "PositionSatStore.hpp" */
/* %include "SP3EphemerisStore.hpp" */
/* %include "RinexUtilities.hpp" */

/* // SEM format: */
/* %include "SEMBase.hpp" */
/* %include "SEMHeader.hpp" */
/* %include "SEMStream.hpp" */
/* %include "SEMData.hpp" */
/* %include "SEMAlmanacStore.hpp" */

/* %include "ObsEpochMap.hpp" */
/* %include "WxObsMap.hpp" */
/* %include "TropModel.hpp" */
/* %include "GCATTropModel.hpp" */
/* %include "GGHeightTropModel.hpp" */
/* %include "GGTropModel.hpp" */
/* %include "MOPSTropModel.hpp" */
/* %include "NBTropModel.hpp" */
/* %include "NeillTropModel.hpp" */
/* %include "SaasTropModel.hpp" */
/* %include "SimpleTropModel.hpp" */
/* %include "PRSolutionLegacy.hpp" */
/* %ignore gpstk::Expression::print(std::ostream& ostr) const; */
/* %include "Expression.hpp" */

/* %include "FFBinaryStream.hpp" */
/* %include "AshtechStream.hpp" */
/* %include "AshtechData.hpp" */
/* %include "AshtechALB.hpp" */
/* %include "AshtechEPB.hpp" */
/* %include "AshtechMBEN.hpp" */
/* %include "AshtechPBEN.hpp" */

/* %include "MoonPosition.hpp" */
/* %include "SunPosition.hpp" */
/* %include "PoleTides.hpp" */
/* %include "SolidTides.hpp" */


/* // Ionosphere Model */
/* %include "IonoModel.hpp" */

/* // Geomatics: */
/* %include "Geomatics.i" */
/* %include "SunEarthSatGeometry.hpp" */

/* // OrbSysGPS */
/* %include "OrbSysStore.hpp" */
/* %include "OrbData.hpp" */
/* %include "OrbDataSys.hpp" */
/* %include "OrbSysGpsC.hpp" */
/* %include "OrbSysGpsC_30.hpp" */
/* // %include "OrbSysGpsC_32.hpp" */
/* // %include "OrbSysGpsC_33.hpp" */
/* %include "OrbSysGpsL.hpp" */
/* // %include "OrbSysGpsL_51.hpp" */
/* // %include "OrbSysGpsL_52.hpp" */
/* // %include "OrbSysGpsL_55.hpp" */
/* %include "OrbDataUTC.hpp" */
/* %include "OrbSysGpsL_56.hpp" */
/* // %include "OrbSysGpsL_63.hpp" */
/* %include "OrbSysGpsL_Reserved.hpp" */
/* %include "PackedNavBits.hpp" */
/* %include "OrbDataSysFactory.hpp" */

/*  /\* note: If you get this compiler error */
/*   * */
/*   * In function ‘PyObject* _wrap_delete_OrbitDataKepler(PyObject*, PyObject*)’: */
/*   * error: ‘smartarg1’ was not declared in this scope */
/*   * */
/*   * That means that you have a derived class from a base smart pointer */
/*   * that needs to be declared in the list below.  In this case, */
/*   * OrbitDataKepler needed to be added to the %shared_ptr declarations */
/*   * below. */
/*   *\/ */
/* %shared_ptr(gpstk::NavData) */
/* %shared_ptr(gpstk::TimeOffsetData) */
/* %shared_ptr(gpstk::NavHealthData) */
/* %shared_ptr(gpstk::OrbitData) */
/* %shared_ptr(gpstk::OrbitDataKepler) */
/* %shared_ptr(gpstk::GPSLNavData) */
/* %shared_ptr(gpstk::GPSLNavEph) */
/* %shared_ptr(gpstk::GPSLNavAlm) */
/* %shared_ptr(gpstk::GPSLNavHealth) */
/* %shared_ptr(gpstk::GPSLNavTimeOffset) */
/* %shared_ptr(gpstk::GPSCNavData) */
/* %shared_ptr(gpstk::GPSCNavEph) */
/* %shared_ptr(gpstk::GPSCNavAlm) */
/* %shared_ptr(gpstk::GPSCNavHealth) */
/* %shared_ptr(gpstk::GPSCNavTimeOffset) */
/* %shared_ptr(gpstk::GPSCNavRedAlm) */
/* %shared_ptr(gpstk::GPSCNav2Eph) */
/* %shared_ptr(gpstk::GPSCNav2Alm) */
/* %shared_ptr(gpstk::GPSCNav2Health) */
/* %shared_ptr(gpstk::GPSCNav2TimeOffset) */
/* %shared_ptr(gpstk::OrbitDataSP3) */
/* %shared_ptr(gpstk::RinexTimeOffset) */

/* %shared_ptr(gpstk::NavDataFactory) */
/* %shared_ptr(gpstk::NavDataFactoryWithStore) */
/* %shared_ptr(gpstk::NavDataFactoryWithStoreFile) */
/* %shared_ptr(gpstk::RinexNavDataFactory) */
/* %shared_ptr(gpstk::SP3NavDataFactory) */
/* %shared_ptr(gpstk::SEMNavDataFactory) */
/* %shared_ptr(gpstk::YumaNavDataFactory) */
/* %shared_ptr(gpstk::MultiFormatNavDataFactory); */
/* %shared_ptr(gpstk::PNBNavDataFactory) */
/* %shared_ptr(gpstk::PNBGPSLNavDataFactory) */
/* %shared_ptr(gpstk::PNBGPSCNavDataFactory) */
/* %shared_ptr(gpstk::PNBGPSCNav2DataFactory) */
/* %shared_ptr(gpstk::PNBMultiGNSSNavDataFactory) */
/*  // silence warnings from swig */
/* %shared_ptr(gpstk::TimeSystemCorrection) */

/* // NewNav store */
/* %include "NavSignalID.hpp" */
/* %include "NavSatelliteID.hpp" */
/* %include "NavMessageID.hpp" */
/* %include "NavData.hpp" */
/* %include "TimeOffsetData.hpp" */
/* %include "NavHealthData.hpp" */
/* %include "OrbitData.hpp" */
/* %include "OrbitDataKepler.hpp" */
/* %include "GPSLNavData.hpp" */
/* %include "GPSLNavEph.hpp" */
/* %include "GPSLNavAlm.hpp" */
/* %include "GPSLNavHealth.hpp" */
/* %include "GPSLNavTimeOffset.hpp" */
/* %include "GPSCNavData.hpp" */
/* %include "GPSCNavEph.hpp" */
/* %include "GPSCNavAlm.hpp" */
/* %include "GPSCNavHealth.hpp" */
/* %include "GPSCNavTimeOffset.hpp" */
/* %include "GPSCNavRedAlm.hpp" */
/* %include "GPSCNav2Eph.hpp" */
/* %include "GPSCNav2Alm.hpp" */
/* %include "GPSCNav2Health.hpp" */
/* %include "GPSCNav2TimeOffset.hpp" */
/* %include "OrbitDataSP3.hpp" */
/* %include "RinexTimeOffset.hpp" */

/* %include "NavDataFactory.hpp" */
/* %include "NavLibrary.hpp" */
/* %include "NavDataFactoryWithStore.hpp" */
/* %include "NavDataFactoryWithStoreFile.hpp" */
/* %include "RinexNavDataFactory.hpp" */
/* %include "SP3NavDataFactory.hpp" */
/* %include "SEMNavDataFactory.hpp" */
/* %include "YumaNavDataFactory.hpp" */
/* %include "MultiFormatNavDataFactory.hpp" */

/* %include "PNBNavDataFactory.hpp" */
/* %include "PNBGPSLNavDataFactory.hpp" */
/* %include "PNBGPSCNavDataFactory.hpp" */
/* %include "PNBGPSCNav2DataFactory.hpp" */
/* %include "PNBMultiGNSSNavDataFactory.hpp" */

// v 2.0.12 required for KLOBUCHAR IONO MODEL
// We need to provide methods that allow casting objects to their instance types.
#define KLOBUCHAR_SUPPORT (SWIG_VERSION >= 0x020012 ? True : False)
#if SWIG_VERSION >= 0x020012
%inline %{
  gpstk::OrbSysGpsC_30* cast_to_OrbSysGpsC_30(gpstk::OrbDataSys* ods) {
    return static_cast<gpstk::OrbSysGpsC_30*>(ods);
  }
  gpstk::OrbSysGpsL_56* cast_to_OrbSysGpsL_56(gpstk::OrbDataSys* ods) {
    return static_cast<gpstk::OrbSysGpsL_56*>(ods);
  }
%}
// Include to generate swig::trait::typename()
%{
  namespace swig {
    template <> struct traits<gpstk::OrbDataSys> {
        typedef pointer_category category;
        static const char* type_name() {return "gpstk::OrbDataSys";}
    };
  }
%}

// SWIG out the msgMap in gpstk::OrbSysStore
%template (TimeMsgMap) std::map<gpstk::CommonTime, gpstk::OrbDataSys* >;
%template (UIDMsgMap) std::map<uint16_t, std::map<gpstk::CommonTime, gpstk::OrbDataSys* > >;
%template (NavIDMsgMap) std::map<gpstk::NavID, std::map<uint16_t, std::map<gpstk::CommonTime, gpstk::OrbDataSys* > > >;
%template (SatIDMsgMap) std::map<gpstk::SatID, std::map<gpstk::NavID, std::map<uint16_t, std::map<gpstk::CommonTime, gpstk::OrbDataSys* > > > >;
#endif


/* // Note that the path functions really don't make sense outside of the build */
/* // environment */
/* %include "build_config.h" */

/* %include "ord.i" */


%pythoncode %{
        # clean-up the dir listing by removing *_swigregister.
        import gpstk
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
