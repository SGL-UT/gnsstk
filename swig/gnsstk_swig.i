%import(module="gnsstk") "Exception.hpp"
%import(module="gnsstk") "TimeConstants.hpp"
%import(module="gnsstk") "EnumIterator.hpp"
%import(module="gnsstk") "TimeSystem.hpp"
%import(module="gnsstk") "TimeSystemConverter.hpp"
%import(module="gnsstk") "CommonTime.hpp"
%import(module="gnsstk") "HexDumpDataConfig.hpp"
 //%import(module="gnsstk") "StringUtils.hpp"
%import(module="gnsstk") "TimeTag.hpp"
%import(module="gnsstk") "ANSITime.hpp"
%import(module="gnsstk") "AllanDeviation.hpp"
%import(module="gnsstk") "gps_constants.hpp"
%import(module="gnsstk") "SatelliteSystem.hpp"
%template(std_vector_GNSS)       std::vector<gnsstk::SatelliteSystem>;
%import(module="gnsstk") "SatTimeSystem.hpp"
%import(module="gnsstk") "SatID.hpp"
%template(std_vector_SatID)      std::vector<gnsstk::SatID>;
%import(module="gnsstk") "MathBase.hpp"
%import(module="gnsstk") "VectorBase.hpp"
%import(module="gnsstk") "VectorBaseOperators.hpp"
%import(module="gnsstk") "Vector.hpp"
%import(module="gnsstk") "Triple.hpp"
%import(module="gnsstk") "EllipsoidModel.hpp"
%import(module="gnsstk") "RefFrameSys.hpp"
%import(module="gnsstk") "RefFrameRlz.hpp"
%import(module="gnsstk") "ReferenceFrame.hpp"
%import(module="gnsstk") "RefFrame.hpp"
%import(module="gnsstk") "DeprecatedConsts.hpp"
%import(module="gnsstk") "FreqConsts.hpp"
%import(module="gnsstk") "GNSSconstants.hpp"
%import(module="gnsstk") "Xvt.hpp"
%import(module="gnsstk") "AlmOrbit.hpp"
%import(module="gnsstk") "AngleType.hpp"
%import(module="gnsstk") "AngleReduced.hpp"
%import(module="gnsstk") "Angle.hpp"
%import(module="gnsstk") "FFStreamError.hpp"
%import(module="gnsstk") "FFData.hpp"
%import(module="gnsstk") "AntexBase.hpp"
%import(module="gnsstk") "FFStream.hpp"
%import(module="gnsstk") "AntexHeader.hpp"
%import(module="gnsstk") "CivilTime.hpp"
%feature("flatnested");
%import(module="gnsstk") "AntexData.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "FFTextStream.hpp"
%import(module="gnsstk") "AntexStream.hpp"
%import(module="gnsstk") "AntennaStore.hpp"
%import(module="gnsstk") "DataStatus.hpp"
%import(module="gnsstk") "AshtechData.hpp"
%import(module="gnsstk") "AshtechALB.hpp"
%import(module="gnsstk") "AshtechEPB.hpp"
%feature("flatnested");
%import(module="gnsstk") "AshtechMBEN.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "AshtechPBEN.hpp"
%import(module="gnsstk") "FFBinaryStream.hpp"
%import(module="gnsstk") "AshtechStream.hpp"
%import(module="gnsstk") "MJD.hpp"
%import(module="gnsstk") "TimeConverters.hpp"
%import(module="gnsstk") "EphTime.hpp"
%import(module="gnsstk") "AtmLoadTides.hpp"
/* %import(module="gnsstk") "BDSD1Bits.hpp" */
%import(module="gnsstk") "Rinex3ObsBase.hpp"
%import(module="gnsstk") "ObservationType.hpp"
%template(std_map_ObservationType_string) std::map<gnsstk::ObservationType, std::string>;
%template(std_map_char_ObservationType) std::map<char, gnsstk::ObservationType>;
%template(std_map_ObservationType_char) std::map<gnsstk::ObservationType,char>;
%import(module="gnsstk") "CarrierBand.hpp"
%template(std_map_CarrierBand_string) std::map<gnsstk::CarrierBand, std::string>;
%template(std_map_char_CarrierBand) std::map<char, gnsstk::CarrierBand>;
%template(std_map_CarrierBand_char) std::map<gnsstk::CarrierBand,char>;
%import(module="gnsstk") "TrackingCode.hpp"
%template(std_map_TrackingCode_string) std::map<gnsstk::TrackingCode, std::string>;
%template(std_map_char_TrackingCode) std::map<char, gnsstk::TrackingCode>;
%template(std_map_TrackingCode_char) std::map<gnsstk::TrackingCode,char>;
%import(module="gnsstk") "XmitAnt.hpp"
%import(module="gnsstk") "ObsID.hpp"
%template(std_vector_ObsID)      std::vector<gnsstk::ObsID>;
%template(std_map_SvObsEpoch) std::map< gnsstk::ObsID, double >;
%import(module="gnsstk") "NavType.hpp"
%import(module="gnsstk") "NavSignalID.hpp"
%import(module="gnsstk") "NavID.hpp"
%import(module="gnsstk") "NavSatelliteID.hpp"
%template(NavSatelliteIDSet) std::set<gnsstk::NavSatelliteID>;
%import(module="gnsstk") "NavMessageType.hpp"
%import(module="gnsstk") "NavMessageID.hpp"
%template(NavMessageIDSet) std::set<gnsstk::NavMessageID>;
%import(module="gnsstk") "DumpDetail.hpp"
%import(module="gnsstk") "SatMetaData.hpp"
%import(module="gnsstk") "SatMetaDataSort.hpp"
%feature("flatnested");
%import(module="gnsstk") "SatMetaDataStore.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "NavData.hpp"
%import(module="gnsstk") "SystemNavData.hpp"
%import(module="gnsstk") "OrbitData.hpp"
%import(module="gnsstk") "NavFit.hpp"
%import(module="gnsstk") "SVHealth.hpp"
%import(module="gnsstk") "OrbitDataKepler.hpp"
%import(module="gnsstk") "OrbitDataBDS.hpp"
%import(module="gnsstk") "BDSD1NavData.hpp"
%import(module="gnsstk") "BDSD1NavAlm.hpp"
%import(module="gnsstk") "BDSD1NavEph.hpp"
%import(module="gnsstk") "NavHealthData.hpp"
%import(module="gnsstk") "BDSD1NavHealth.hpp"
%import(module="gnsstk") "PackedNavBits.hpp"
%import(module="gnsstk") "InterSigCorr.hpp"
%import(module="gnsstk") "BDSD1NavISC.hpp"
%import(module="gnsstk") "Position.hpp"
%import(module="gnsstk") "IonoNavData.hpp"
%import(module="gnsstk") "KlobucharIonoNavData.hpp"
%import(module="gnsstk") "BDSD1NavIono.hpp"
%import(module="gnsstk") "TimeOffsetData.hpp"
%import(module="gnsstk") "StdNavTimeOffset.hpp"
%import(module="gnsstk") "BDSD1NavTimeOffset.hpp"
/* %import(module="gnsstk") "BDSD2Bits.hpp" */
%import(module="gnsstk") "BDSD2NavData.hpp"
%import(module="gnsstk") "BDSD2NavAlm.hpp"
%import(module="gnsstk") "BDSD2NavEph.hpp"
%import(module="gnsstk") "BDSD2NavHealth.hpp"
%import(module="gnsstk") "BDSD2NavISC.hpp"
%import(module="gnsstk") "BDSD2NavIono.hpp"
%import(module="gnsstk") "BDSD2NavTimeOffset.hpp"
%import(module="gnsstk") "Week.hpp"
%import(module="gnsstk") "WeekSecond.hpp"
%import(module="gnsstk") "BDSWeekSecond.hpp"
%import(module="gnsstk") "BDSconsts.hpp"
%import(module="gnsstk") "MatrixBase.hpp"
%import(module="gnsstk") "MatrixBaseOperators.hpp"
%import(module="gnsstk") "Matrix.hpp"
%import(module="gnsstk") "Matrix.i"
%import(module="gnsstk") "BLQDataReader.hpp"
/* %include "CommandOption.hpp" */
/* %include "CommandOptionParser.hpp" */
/* %include "BasicFramework.hpp" */
%import(module="gnsstk") "BasicTimeSystemConverter.hpp"
%feature("flatnested");
%import(module="gnsstk") "BinexData.hpp"
%feature("flatnested", "");
%template(std_binary_function_BinexData) std::binary_function<gnsstk::BinexData, gnsstk::BinexData, bool>;
%feature("flatnested");
%import(module="gnsstk") "FileSpec.hpp"
%feature("flatnested", "");
%feature("flatnested");
%import(module="gnsstk") "FileFilter.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "BinexFilterOperators.hpp"
%import(module="gnsstk") "BinexStream.hpp"
%import(module="gnsstk") "MiscMath.hpp"
%template(SimpleLagrangeInterpolation_double) gnsstk::SimpleLagrangeInterpolation<double>;
%template(LagrangeInterpolation_double) gnsstk::LagrangeInterpolation<double>;
%template(LagrangeInterpolating2ndDerivative_double) gnsstk::LagrangeInterpolating2ndDerivative<double>;
%template(RSS_double) gnsstk::RSS<double>;
%import(module="gnsstk") "Stats.hpp"
   // Template Instantiations for non-member Stats Functions
%template(median_double) gnsstk::median<double>;
%template(median_int) gnsstk::median<int>;
%template(mad_double) gnsstk::mad<double>;
   // Stats Class
%template(Stats_double) gnsstk::Stats<double>;
%template(SeqStats_double) gnsstk::SeqStats<double>;
%template(WtdStats_double) gnsstk::WtdStats<double>;
%template(TwoSampleStats_double) gnsstk::TwoSampleStats<double>;
%import(module="gnsstk") "BivarStats.hpp"
%template(BivarStats_double) gnsstk::BivarStats<double>;
%import(module="gnsstk") "GPSWeekSecond.hpp"
%import(module="gnsstk") "NMCTMeta.hpp"
%import(module="gnsstk") "EngNav.hpp"
%import(module="gnsstk") "YDSTime.hpp"
%import(module="gnsstk") "BrcClockCorrection.hpp"
%import(module="gnsstk") "WGS84Ellipsoid.hpp"
%import(module="gnsstk") "GPSEllipsoid.hpp"
%import(module="gnsstk") "RinexSatID.hpp"
%template(std_vector_RinexSatID) std::vector<gnsstk::RinexSatID>;
%import(module="gnsstk") "GPS_URA.hpp"
%import(module="gnsstk") "BrcKeplerOrbit.hpp"
%import(module="gnsstk") "CCIR.hpp"
%import(module="gnsstk") "CGCS2000Ellipsoid.hpp"
%import(module="gnsstk") "NavFilterKey.hpp"
%template(std_binary_function_NavFilterKey) std::binary_function<gnsstk::NavFilterKey *, gnsstk::NavFilterKey *, bool >;
%import(module="gnsstk") "NavFilter.hpp"
%import(module="gnsstk") "CNav2SanityFilter.hpp"
%template(std_binary_function_CNavFilterData) std::binary_function< gnsstk::CNavFilterData,gnsstk::CNavFilterData,bool >;
%template(std_binary_function_CNavFilterDataPtr) std::binary_function< gnsstk::CNavFilterData *, gnsstk::CNavFilterData *, bool >;
%import(module="gnsstk") "CNavFilterData.hpp"
%import(module="gnsstk") "CNavCookFilter.hpp"
%import(module="gnsstk") "NavFilterMgr.hpp"
%import(module="gnsstk") "CNavCrossSourceFilter.hpp"
/* %import(module="gnsstk") "CNavEmptyFilter.hpp" */
%import(module="gnsstk") "CNavParityFilter.hpp"
%import(module="gnsstk") "CNavTOWFilter.hpp"
%import(module="gnsstk") "ClockModel.hpp"
%import(module="gnsstk") "PCodeConst.hpp"
%import(module="gnsstk") "CodeBuffer.hpp"
%import(module="gnsstk") "Combinations.hpp"
/* %include "CommandLine.hpp" */
/* %import(module="gnsstk") "CommandOptionNavEnumHelp.hpp" */
/* %include "CommandOptionWithCommonTimeArg.hpp" */
/* %include "CommandOptionWithPositionArg.hpp" */
/* %include "CommandOptionWithTimeArg.hpp" */
%import(module="gnsstk") "ConfDataReader.hpp"
 /* %import(module="gnsstk") "ConfDataWriter.hpp" */ // error on typemaps for list of double
%import(module="gnsstk") "CubicSpline.hpp"
%import(module="gnsstk") "DCBDataReader.hpp"
/* %import(module="gnsstk") "DebugTrace.hpp" */
%import(module="gnsstk") "GPSZcount.hpp"
%import(module="gnsstk") "UnixTime.hpp"
%import(module="gnsstk") "SystemTime.hpp"
%import(module="gnsstk") "JulianDate.hpp"
%import(module="gnsstk") "GPSWeek.hpp"
%import(module="gnsstk") "GPSWeekZcount.hpp"
%import(module="gnsstk") "GALWeekSecond.hpp"
%import(module="gnsstk") "QZSWeekSecond.hpp"
/* %import(module="gnsstk") "Epoch.hpp" */
%import(module="gnsstk") "RinexObsBase.hpp"
%feature("flatnested");
%import(module="gnsstk") "RinexObsHeader.hpp"
%feature("flatnested", "");
%template(std_vector_RinexObsType) std::vector<gnsstk::RinexObsType>;
%template(std_unary_function_RinexObsHeader) std::unary_function<gnsstk::RinexObsHeader, bool>;
%import(module="gnsstk") "RinexDatum.hpp"
%template(std_vector_RinexDatum) std::vector<gnsstk::RinexDatum>;
%template(std_map_RinexSatID_std_vector_RinexDatum) std::map<gnsstk::RinexSatID, std::vector<gnsstk::RinexDatum> >;
%template(std_map_RinexObsType_RinexDatum) std::map<gnsstk::RinexObsType, gnsstk::RinexDatum>;
%template(std_map_RinexSatID_RinexObsTypeMap) std::map<gnsstk::SatID, std::map<gnsstk::RinexObsType, gnsstk::RinexDatum> >;
%import(module="gnsstk") "RinexObsData.hpp"
%template(std_binary_function_RinexObsData) std::binary_function<gnsstk::RinexObsData,gnsstk::RinexObsData,bool>;
%import(module="gnsstk") "RinexUtilities.hpp"
%import(module="gnsstk") "TimeString.hpp"
/* %import(module="gnsstk") "SatPass.hpp" */
/* %import(module="gnsstk") "DiscCorr.hpp" */
%import(module="gnsstk") "ENUUtil.hpp"
%import(module="gnsstk") "IERSConvention.hpp"
/* %import(module="gnsstk") "EarthOrientation.hpp" */
/* %import(module="gnsstk") "EOPPrediction.hpp" */
/* %import(module="gnsstk") "EOPStore.hpp" */
%import(module="gnsstk") "EngAlmanac.hpp"
%import(module="gnsstk") "EngEphemeris.hpp"
%import(module="gnsstk") "NavValidityType.hpp"
%import(module="gnsstk") "NavSearchOrder.hpp"
%import(module="gnsstk") "NavDataFactory.hpp"
%import(module="gnsstk") "NavLibrary.hpp"
%template(NavMessageTypeSet) std::set<gnsstk::NavMessageType>;
%import(module="gnsstk") "ValidType.hpp"
%import(module="gnsstk") "RawRange.hpp"
%import(module="gnsstk") "EphemerisRange.hpp"
%feature("flatnested");
%import(module="gnsstk") "IonoModel.hpp"
%feature("flatnested", "");
%feature("flatnested");
%import(module="gnsstk") "IonoModelStore.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "SvObsEpoch.hpp"
%template(std_map_ObsEpoch) std::map<gnsstk::SatID, gnsstk::SvObsEpoch>;
%import(module="gnsstk") "ObsEpochMap.hpp"
%template(ObsEpochMap) std::map<gnsstk::CommonTime, gnsstk::ObsEpoch>;
%import(module="gnsstk") "WxObsMap.hpp"
%import(module="gnsstk") "TropModel.hpp"
%import(module="gnsstk") "NBTropModel.hpp"
%import(module="gnsstk") "ObsRngDev.hpp"
%import(module="gnsstk") "ORDEpoch.hpp"
%import(module="gnsstk") "ObsClockModel.hpp"
%import(module="gnsstk") "EpochClockModel.hpp"
%import(module="gnsstk") "Expression.hpp"
%import(module="gnsstk") "StatsFilterHit.hpp"
%import(module="gnsstk") "RobustStats.hpp"
%import(module="gnsstk") "FDiffFilter.hpp"
%import(module="gnsstk") "FileSpecFind.hpp"
%import(module="gnsstk") "FileUtils.hpp"
%import(module="gnsstk") "FileFilterFrame.hpp"
%import(module="gnsstk") "RinexObsID.hpp"
%template(std_vector_RinexObsID) std::vector<gnsstk::RinexObsID>;
%template(std_map_string_vector_RinexObsID) std::map<std::string, std::vector<gnsstk::RinexObsID> >;
 // Use a special wrapper for the include file which resolves issues
 // with the nested definitions
%import(module="gnsstk") "Rinex3ObsHeader.i"
%template(std_unary_function_Rinex3ObsHeader) std::unary_function<gnsstk::Rinex3ObsHeader, bool>;
%import(module="gnsstk") "Rinex3ObsData.hpp"
%template(std_vector_Rinex3ObsData) std::vector<gnsstk::Rinex3ObsData>;
%template(std_binary_function_Rinex3ObsData) std::binary_function<gnsstk::Rinex3ObsData,gnsstk::Rinex3ObsData,bool>;
%import(module="gnsstk") "FileFilterFrameWithHeader.hpp"
%import(module="gnsstk") "FileStore.hpp"
%feature("flatnested");
%import(module="gnsstk") "FirstDiffFilter.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "FormattedDouble.hpp"
%import(module="gnsstk") "FreqConv.hpp"
%import(module="gnsstk") "GCATTropModel.hpp"
%import(module="gnsstk") "GGHeightTropModel.hpp"
%import(module="gnsstk") "GGTropModel.hpp"
/* %import(module="gnsstk") "GLOCBits.hpp" */
%import(module="gnsstk") "GLOCNavHeader.hpp"
%import(module="gnsstk") "GLOCNavData.hpp"
%import(module="gnsstk") "PZ90Ellipsoid.hpp"
%import(module="gnsstk") "GLOCSatType.hpp"
%import(module="gnsstk") "GLOCOrbitType.hpp"
%import(module="gnsstk") "GLOCNavAlm.hpp"
%import(module="gnsstk") "GLOCRegime.hpp"
%import(module="gnsstk") "GLOCNavLTDMP.hpp"
%import(module="gnsstk") "GLOCNavEph.hpp"
%import(module="gnsstk") "GLOCNavHealth.hpp"
%import(module="gnsstk") "GLOCNavIono.hpp"
%import(module="gnsstk") "GLOCNavUT1TimeOffset.hpp"
/* %import(module="gnsstk") "GLOFBits.hpp" */
%import(module="gnsstk") "GLOFNavPCode.hpp"
%import(module="gnsstk") "GLOFNavSatType.hpp"
%import(module="gnsstk") "GLOFNavData.hpp"
%import(module="gnsstk") "GLOFNavAlm.hpp"
%import(module="gnsstk") "GLOFNavEph.hpp"
%import(module="gnsstk") "GLOFNavHealth.hpp"
%import(module="gnsstk") "GLOFNavISC.hpp"
%import(module="gnsstk") "GLOFNavTimeOffset.hpp"
%import(module="gnsstk") "GLOFNavUT1TimeOffset.hpp"
%import(module="gnsstk") "GLONASSTime.hpp"
%import(module="gnsstk") "OrbitDataGPS.hpp"
%import(module="gnsstk") "GPSCNav2Alm.hpp"
%import(module="gnsstk") "GPSCNav2Eph.hpp"
%import(module="gnsstk") "GPSCNav2Health.hpp"
%import(module="gnsstk") "GPSCNav2ISC.hpp"
%import(module="gnsstk") "GPSCNav2Iono.hpp"
%import(module="gnsstk") "GPSCNav2TimeOffset.hpp"
%import(module="gnsstk") "GPSCNavData.hpp"
%import(module="gnsstk") "GPSCNavAlm.hpp"
%import(module="gnsstk") "GPSCNavEph.hpp"
%import(module="gnsstk") "GPSCNavHealth.hpp"
%import(module="gnsstk") "GPSCNavISC.hpp"
%import(module="gnsstk") "GPSCNavIono.hpp"
%import(module="gnsstk") "GPSCNavRedAlm.hpp"
%import(module="gnsstk") "GPSCNavTimeOffset.hpp"
%import(module="gnsstk") "GPSLNavL2Codes.hpp"
%import(module="gnsstk") "GPSLNavData.hpp"
%import(module="gnsstk") "GPSLNavAlm.hpp"
%import(module="gnsstk") "GPSLNavEph.hpp"
%import(module="gnsstk") "GPSLNavHealth.hpp"
%import(module="gnsstk") "GPSLNavISC.hpp"
%import(module="gnsstk") "GPSLNavIono.hpp"
%import(module="gnsstk") "GPSLNavTimeOffset.hpp"
%import(module="gnsstk") "GPSSVConfig.hpp"
%import(module="gnsstk") "GPSNavConfig.hpp"
%import(module="gnsstk") "GSatID.hpp"
%import(module="gnsstk") "GalDataValid.hpp"
%import(module="gnsstk") "OrbitDataGal.hpp"
%import(module="gnsstk") "GalHealthStatus.hpp"
%import(module="gnsstk") "GalFNavAlm.hpp"
%import(module="gnsstk") "GalFNavEph.hpp"
%import(module="gnsstk") "GalFNavHealth.hpp"
%import(module="gnsstk") "GalFNavISC.hpp"
%import(module="gnsstk") "MODIP.hpp"
%import(module="gnsstk") "GalileoEllipsoid.hpp"
%import(module="gnsstk") "GalileoIonoEllipsoid.hpp"
%feature("flatnested");
%import(module="gnsstk") "NeQuickIonoNavData.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "GalFNavIono.hpp"
%import(module="gnsstk") "GalFNavTimeOffset.hpp"
%import(module="gnsstk") "GalINavAlm.hpp"
%import(module="gnsstk") "GalINavEph.hpp"
%import(module="gnsstk") "GalINavHealth.hpp"
%import(module="gnsstk") "GalINavISC.hpp"
%import(module="gnsstk") "GalINavIono.hpp"
%import(module="gnsstk") "GalINavTimeOffset.hpp"
%import(module="gnsstk") "GenXSequence.hpp"
%import(module="gnsstk") "GenericNavFilterData.hpp"
%import(module="gnsstk") "GlobalTropModel.hpp"
%import(module="gnsstk") "HelmertTransform.hpp"
%import(module="gnsstk") "Transformer.hpp"
%import(module="gnsstk") "HelmertTransformer.hpp"
 //%import(module="gnsstk") "IERS1996NutationData.hpp"
 //%import(module="gnsstk") "IERS1996UT1mUTCData.hpp"
 //%import(module="gnsstk") "IERS2003NutationData.hpp"
/* %import(module="gnsstk") "IERS2010CIOSeriesData.hpp" */
%import(module="gnsstk") "IRNWeekSecond.hpp"
/* %include "LoopedFramework.hpp" */
/* %include "InOutFramework.hpp" */
%import(module="gnsstk") "IonexBase.hpp"
%feature("flatnested");
%import(module="gnsstk") "IonexHeader.hpp"
%feature("flatnested", "");
%template(FileStore_IonexHeader) gnsstk::FileStore<gnsstk::IonexHeader>;
%import(module="gnsstk") "IonexStream.hpp"
%feature("flatnested");
%import(module="gnsstk") "IonexData.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "IonexStoreStrategy.hpp"
%import(module="gnsstk") "IonexStore.hpp"
%import(module="gnsstk") "Namelist.hpp"
/* %import(module="gnsstk") "SRIMatrix.hpp" */
/* %import(module="gnsstk") "SparseVector.hpp" */
/* %import(module="gnsstk") "SparseMatrix.hpp" */
/* %import(module="gnsstk") "SRI.hpp" */
/* %import(module="gnsstk") "SRIFilter.hpp" */
/* %import(module="gnsstk") "logstream.hpp" */
%feature("flatnested");
%import(module="gnsstk") "KalmanFilter.hpp"
%feature("flatnested", "");
%template(std_binary_function_LNavFilterData) std::binary_function< gnsstk::LNavFilterData *, gnsstk::LNavFilterData *, bool >;
%import(module="gnsstk") "LNavFilterData.hpp"
%import(module="gnsstk") "LNavAlmValFilter.hpp"
%import(module="gnsstk") "LNavCookFilter.hpp"
%import(module="gnsstk") "LNavCrossSourceFilter.hpp"
%import(module="gnsstk") "LNavEmptyFilter.hpp"
%import(module="gnsstk") "LNavEphMaker.hpp"
%import(module="gnsstk") "LNavOrderFilter.hpp"
%import(module="gnsstk") "LNavParityFilter.hpp"
%import(module="gnsstk") "LNavTLMHOWFilter.hpp"
%import(module="gnsstk") "LinearClockModel.hpp"
%import(module="gnsstk") "MOPSTropModel.hpp"
%import(module="gnsstk") "MatrixImplementation.hpp"
%import(module="gnsstk") "MatrixFunctors.hpp"
%import(module="gnsstk") "MatrixOperators.hpp"
%import(module="gnsstk") "MetReader.hpp"
%import(module="gnsstk") "MostCommonValue.hpp"
%import(module="gnsstk") "NavDataFactoryWithStore.hpp"
%import(module="gnsstk") "NavDataFactoryCallback.hpp"
%import(module="gnsstk") "NavDataFactoryWithStoreFile.hpp"
%import(module="gnsstk") "NDFUniqIterator.hpp"
%template (NDFUniqIterator_NavDataFactoryMap) gnsstk::NDFUniqIterator<gnsstk::NavDataFactoryMap>;
%import(module="gnsstk") "MultiFormatNavDataFactory.hpp"
%import(module="gnsstk") "NDFUniqConstIterator.hpp"
%template (NDFUniqConstIterator_NavDataFactoryMap) gnsstk::NDFUniqConstIterator<gnsstk::NavDataFactoryMap>;
%import(module="gnsstk") "NEDUtil.hpp"
%import(module="gnsstk") "NavDataFactoryStoreCallback.hpp"
%import(module="gnsstk") "NavMsgData.hpp"
%import(module="gnsstk") "NavMsgDataBits.hpp"
%import(module="gnsstk") "NavMsgDataPNB.hpp"
%import(module="gnsstk") "NavMsgDataWords.hpp"
%template(NavMsgDataWords_30) gnsstk::NavMsgDataWords<30>;
%import(module="gnsstk") "NavOrderFilter.hpp"
%import(module="gnsstk") "NavTimeSystemConverter.hpp"
%import(module="gnsstk") "NeillTropModel.hpp"
%feature("flatnested");
%import(module="gnsstk") "OceanLoadTides.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "OceanLoading.hpp"
%import(module="gnsstk") "OrbitDataSP3.hpp"
%import(module="gnsstk") "PNBNavDataFactory.hpp"
%feature("flatnested");
%import(module="gnsstk") "PNBBDSD1NavDataFactory.hpp"
%feature("flatnested", "");
%feature("flatnested");
%import(module="gnsstk") "PNBBDSD2NavDataFactory.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "PNBGLOCNavDataFactory.hpp"
%import(module="gnsstk") "PNBGLOFNavDataFactory.hpp"
%import(module="gnsstk") "PNBGPSCNav2DataFactory.hpp"
%import(module="gnsstk") "PNBGPSCNavDataFactory.hpp"
%import(module="gnsstk") "PNBGPSLNavDataFactory.hpp"
%import(module="gnsstk") "PNBGalFNavDataFactory.hpp"
%import(module="gnsstk") "PNBGalINavDataFactory.hpp"
%import(module="gnsstk") "PNBMultiGNSSNavDataFactory.hpp"
%import(module="gnsstk") "CorrectorType.hpp"
%import(module="gnsstk") "CorrDupHandling.hpp"
%import(module="gnsstk") "GroupPathCorrector.hpp"
%import(module="gnsstk") "BCIonoCorrector.hpp"
%import(module="gnsstk") "BCISCorrector.hpp"
%import(module="gnsstk") "TropCorrector.hpp"
%import(module="gnsstk") "CorrectionResult.hpp"
%import(module="gnsstk") "CorrectionResults.hpp"
%import(module="gnsstk") "GroupPathCorr.hpp"
%import(module="gnsstk") "stl_helpers.hpp"
%import(module="gnsstk") "PRSolution.hpp"
%import(module="gnsstk") "PoleTides.hpp"
%import(module="gnsstk") "PolyFit.hpp"
%template(PolyFit_double) gnsstk::PolyFit<double>;
%import(module="gnsstk") "PosixTime.hpp"
 //%import(module="gnsstk") "PowerSum.hpp"
%import(module="gnsstk") "SolarSystemEphemeris.hpp"
%import(module="gnsstk") "SunEarthSatGeometry.i"
%import(module="gnsstk") "SolidEarthTides.hpp"
/* %import(module="gnsstk") "SolarSystem.hpp" */
/* %import(module="gnsstk") "PreciseRange.hpp" */
%import(module="gnsstk") "RACRotation.hpp"
%import(module="gnsstk") "RNDouble.hpp"
/* %import(module="gnsstk") "RTFileFrame.hpp" */
%feature("flatnested");
%import(module="gnsstk") "Rinex3ClockBase.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "Rinex3ClockData.hpp"
%import(module="gnsstk") "Rinex3ClockHeader.hpp"
%import(module="gnsstk") "Rinex3ClockStream.hpp"
%import(module="gnsstk") "Rinex3NavBase.hpp"
%import(module="gnsstk") "TimeSystemCorr.hpp"
%import(module="gnsstk") "Rinex3NavHeader.hpp"
%template(std_unary_function_Rinex3NavHeader) std::unary_function<gnsstk::Rinex3NavHeader, bool>;
%import(module="gnsstk") "Rinex3NavStream.hpp"
%import(module="gnsstk") "RinexNavBase.hpp"
%import(module="gnsstk") "RinexNavData.hpp"
%template(std_binary_function_RinexNavData) std::binary_function<gnsstk::RinexNavData,gnsstk::RinexNavData,bool>;
%template(std_unary_function_RinexNavData) std::unary_function<gnsstk::RinexNavData,bool>;
%import(module="gnsstk") "Rinex3NavData.hpp"
%template(std_binary_function_Rinex3NavData) std::binary_function<gnsstk::Rinex3NavData,gnsstk::Rinex3NavData,bool>;
%template(std_unary_function_Rinex3NavData) std::unary_function<gnsstk::Rinex3NavData,bool>;
%import(module="gnsstk") "Rinex3NavFilterOperators.hpp"
/* %import(module="gnsstk") "Rinex3ObsFileLoader.hpp" */
%import(module="gnsstk") "Rinex3ObsFilterOperators.hpp"
%import(module="gnsstk") "Rinex3ObsStream.hpp"
%feature("flatnested");
%import(module="gnsstk") "RinexClockBase.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "RinexClockData.hpp"
%feature("flatnested");
%import(module="gnsstk") "RinexClockHeader.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "RinexClockStream.hpp"
%import(module="gnsstk") "RinexMetBase.hpp"
%feature("flatnested");
%import(module="gnsstk") "RinexMetHeader.hpp"
%feature("flatnested", "");
%template(std_unary_function_RinexMetHeader) std::unary_function<gnsstk::RinexMetHeader,bool>;
%import(module="gnsstk") "RinexMetData.hpp"
%template(std_binary_function_RinexMetData) std::binary_function<gnsstk::RinexMetData,gnsstk::RinexMetData,bool>;
%import(module="gnsstk") "RinexMetFilterOperators.hpp"
%import(module="gnsstk") "RinexMetStream.hpp"
%import(module="gnsstk") "RinexNavDataFactory.hpp"
%import(module="gnsstk") "RinexNavHeader.hpp"
%template (FileStore_RinexNavHeader) gnsstk::FileStore<gnsstk::RinexNavHeader>;
%template(std_unary_function_RinexNavHeader) std::unary_function<gnsstk::RinexNavHeader,bool>;
%import(module="gnsstk") "RinexNavFilterOperators.hpp"
%import(module="gnsstk") "RinexNavStream.hpp"
%import(module="gnsstk") "RinexObsFilterOperators.hpp"
%import(module="gnsstk") "RinexObsStream.hpp"
%import(module="gnsstk") "RinexTimeOffset.hpp"
%import(module="gnsstk") "SEMBase.hpp"
%import(module="gnsstk") "SEMHeader.hpp"
%template(FileStore_SEMHeader) gnsstk::FileStore<gnsstk::SEMHeader>;
%import(module="gnsstk") "SEMData.hpp"
%import(module="gnsstk") "SEMNavDataFactory.hpp"
%import(module="gnsstk") "SEMStream.hpp"
%import(module="gnsstk") "SP3Base.hpp"
%import(module="gnsstk") "SP3SatID.hpp"
%import(module="gnsstk") "SP3Data.hpp"
%import(module="gnsstk") "SP3Header.hpp"
%import(module="gnsstk") "SP3NavDataFactory.hpp"
%import(module="gnsstk") "SP3Stream.hpp"
/* %import(module="gnsstk") "SRIleastSquares.hpp" */
%import(module="gnsstk") "X1Sequence.hpp"
%import(module="gnsstk") "X2Sequence.hpp"
%import(module="gnsstk") "SVPCodeGen.hpp"
%import(module="gnsstk") "SaasTropModel.hpp"
%import(module="gnsstk") "SatDataReader.hpp"
/* %import(module="gnsstk") "SatPassIterator.hpp" */
%import(module="gnsstk") "msecHandler.hpp"
/* %import(module="gnsstk") "SatPassUtilities.hpp" */
%import(module="gnsstk") "SimpleTropModel.hpp"
/* %import(module="gnsstk") "SinexBase.hpp" */
/* %import(module="gnsstk") "SinexHeader.hpp" */
/* %import(module="gnsstk") "SinexTypes.hpp" */
/* %import(module="gnsstk") "SinexBlock.hpp" */
/* %import(module="gnsstk") "SinexData.hpp" */
/* %import(module="gnsstk") "SinexStream.hpp" */
%import(module="gnsstk") "SolarPosition.i"
/* %import(module="gnsstk") "SpecialFuncs.hpp" */
%import(module="gnsstk") "StreamBuf.hpp"
%import(module="gnsstk") "TimeCorrection.hpp"
%import(module="gnsstk") "TimeNamedFileStream.hpp"
%feature("flatnested");
%import(module="gnsstk") "TimeRange.hpp"
%import(module="gnsstk") "TransformLibrary.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "VectorOperators.hpp"
%import(module="gnsstk") "WNJfilter.hpp"
%feature("flatnested");
%import(module="gnsstk") "WindowFilter.hpp"
%feature("flatnested", "");
%import(module="gnsstk") "YumaBase.hpp"
%import(module="gnsstk") "YumaHeader.hpp"
%template(FileStore_YumaHeader) gnsstk::FileStore<gnsstk::YumaHeader>;
%import(module="gnsstk") "YumaData.hpp"
%import(module="gnsstk") "YumaNavDataFactory.hpp"
%import(module="gnsstk") "YumaStream.hpp"
%import(module="gnsstk") "convhelp.hpp"
%import(module="gnsstk") "demangle.hpp"
%import(module="gnsstk") "expandtilde.hpp"
%import(module="gnsstk") "format.hpp"
/* %import(module="gnsstk") "gdc.hpp" */
%import(module="gnsstk") "ord.hpp"
%import(module="gnsstk") "random.hpp"
%import(module="gnsstk") "singleton.hpp"
