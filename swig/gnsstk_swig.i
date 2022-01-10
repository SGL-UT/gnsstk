%include "Exception.hpp"
%include "TimeConstants.hpp"
%include "EnumIterator.hpp"
%include "TimeSystem.hpp"
%include "TimeSystemConverter.hpp"
%include "CommonTime.hpp"
%include "HexDumpDataConfig.hpp"
 //%include "StringUtils.hpp"
%include "TimeTag.hpp"
%include "ANSITime.hpp"
%include "AllanDeviation.hpp"
%include "gps_constants.hpp"
%include "SatelliteSystem.hpp"
%template(std_vector_GNSS)       std::vector<gnsstk::SatelliteSystem>;
%include "SatID.hpp"
%template(std_vector_SatID)      std::vector<gnsstk::SatID>;
%include "MathBase.hpp"
%include "VectorBase.hpp"
%include "VectorBaseOperators.hpp"
%include "Vector.hpp"
%include "Triple.hpp"
%include "EllipsoidModel.hpp"
%include "ReferenceFrame.hpp"
%include "DeprecatedConsts.hpp"
%include "FreqConsts.hpp"
%include "GNSSconstants.hpp"
%include "Xvt.hpp"
%include "AlmOrbit.hpp"
%include "AngleReduced.hpp"
%include "Angle.hpp"
%include "AngleType.hpp"
%include "FFStreamError.hpp"
%include "FFData.hpp"
%include "AntexBase.hpp"
%include "FFStream.hpp"
%include "AntexHeader.hpp"
%include "CivilTime.hpp"
%feature("flatnested");
%include "AntexData.hpp"
%feature("flatnested", "");
%include "FFTextStream.hpp"
%include "AntexStream.hpp"
%include "AntennaStore.hpp"
%include "DataStatus.hpp"
%include "AshtechData.hpp"
%include "AshtechALB.hpp"
%include "AshtechEPB.hpp"
%feature("flatnested");
%include "AshtechMBEN.hpp"
%feature("flatnested", "");
%include "AshtechPBEN.hpp"
%include "FFBinaryStream.hpp"
%include "AshtechStream.hpp"
%include "MJD.hpp"
%include "TimeConverters.hpp"
%include "EphTime.hpp"
%include "AtmLoadTides.hpp"
/* %include "BDSD1Bits.hpp" */
%include "Rinex3ObsBase.hpp"
%include "ObservationType.hpp"
%template(std_map_ObservationType_string) std::map<gnsstk::ObservationType, std::string>;
%template(std_map_char_ObservationType) std::map<char, gnsstk::ObservationType>;
%template(std_map_ObservationType_char) std::map<gnsstk::ObservationType,char>;
%include "CarrierBand.hpp"
%template(std_map_CarrierBand_string) std::map<gnsstk::CarrierBand, std::string>;
%template(std_map_char_CarrierBand) std::map<char, gnsstk::CarrierBand>;
%template(std_map_CarrierBand_char) std::map<gnsstk::CarrierBand,char>;
%include "TrackingCode.hpp"
%template(std_map_TrackingCode_string) std::map<gnsstk::TrackingCode, std::string>;
%template(std_map_char_TrackingCode) std::map<char, gnsstk::TrackingCode>;
%template(std_map_TrackingCode_char) std::map<gnsstk::TrackingCode,char>;
%include "XmitAnt.hpp"
%include "ObsID.hpp"
%template(std_vector_ObsID)      std::vector<gnsstk::ObsID>;
%template(std_map_SvObsEpoch) std::map< gnsstk::ObsID, double >;
%include "NavType.hpp"
%include "NavSignalID.hpp"
%include "NavID.hpp"
%include "NavSatelliteID.hpp"
%template(NavSatelliteIDSet) std::set<gnsstk::NavSatelliteID>;
%include "NavMessageType.hpp"
%template(NavMessageTypeSet) std::set<gnsstk::NavMessageType>;
%include "NavMessageID.hpp"
%template(NavMessageIDSet) std::set<gnsstk::NavMessageID>;
%include "DumpDetail.hpp"
%include "SatMetaData.hpp"
%include "SatMetaDataSort.hpp"
%feature("flatnested");
%include "SatMetaDataStore.hpp"
%feature("flatnested", "");
%include "NavData.hpp"
%include "OrbitData.hpp"
%include "SVHealth.hpp"
%include "OrbitDataKepler.hpp"
%include "BDSD1NavData.hpp"
%include "BDSD1NavAlm.hpp"
%include "BDSD1NavEph.hpp"
%include "NavHealthData.hpp"
%include "BDSD1NavHealth.hpp"
%include "PackedNavBits.hpp"
%include "InterSigCorr.hpp"
%include "BDSD1NavISC.hpp"
%include "Position.hpp"
%include "IonoData.hpp"
%include "KlobucharIonoData.hpp"
%include "BDSD1NavIono.hpp"
%include "TimeOffsetData.hpp"
%include "StdNavTimeOffset.hpp"
%include "BDSD1NavTimeOffset.hpp"
/* %include "BDSD2Bits.hpp" */
%include "BDSD2NavData.hpp"
%include "BDSD2NavAlm.hpp"
%include "BDSD2NavEph.hpp"
%include "BDSD2NavHealth.hpp"
%include "BDSD2NavISC.hpp"
%include "BDSD2NavIono.hpp"
%include "BDSD2NavTimeOffset.hpp"
%include "Week.hpp"
%include "WeekSecond.hpp"
%include "BDSWeekSecond.hpp"
%include "BDSconsts.hpp"
%include "MatrixBase.hpp"
%include "MatrixBaseOperators.hpp"
%include "Matrix.hpp"
%include "Matrix.i"
%include "BLQDataReader.hpp"
/* %include "CommandOption.hpp" */
/* %include "CommandOptionParser.hpp" */
/* %include "BasicFramework.hpp" */
%include "BasicTimeSystemConverter.hpp"
%feature("flatnested");
%include "BinexData.hpp"
%feature("flatnested", "");
%template(std_binary_function_BinexData) std::binary_function<gnsstk::BinexData, gnsstk::BinexData, bool>;
%feature("flatnested");
%include "FileSpec.hpp"
%feature("flatnested", "");
%feature("flatnested");
%include "FileFilter.hpp"
%feature("flatnested", "");
%include "BinexFilterOperators.hpp"
%include "BinexStream.hpp"
%include "MiscMath.hpp"
%template(SimpleLagrangeInterpolation_double) gnsstk::SimpleLagrangeInterpolation<double>;
%template(LagrangeInterpolation_double) gnsstk::LagrangeInterpolation<double>;
%template(LagrangeInterpolating2ndDerivative_double) gnsstk::LagrangeInterpolating2ndDerivative<double>;
%template(RSS_double) gnsstk::RSS<double>;
%include "Stats.hpp"
   // Template Instantiations for non-member Stats Functions
%template(median_double) gnsstk::median<double>;
%template(median_int) gnsstk::median<int>;
%template(mad_double) gnsstk::mad<double>;
   // Stats Class
%template(Stats_double) gnsstk::Stats<double>;
%template(SeqStats_double) gnsstk::SeqStats<double>;
%template(WtdStats_double) gnsstk::WtdStats<double>;
%template(TwoSampleStats_double) gnsstk::TwoSampleStats<double>;
%include "BivarStats.hpp"
%template(BivarStats_double) gnsstk::BivarStats<double>;
%include "GPSWeekSecond.hpp"
%include "NMCTMeta.hpp"
%include "EngNav.hpp"
%include "YDSTime.hpp"
%include "BrcClockCorrection.hpp"
%include "WGS84Ellipsoid.hpp"
%include "GPSEllipsoid.hpp"
%include "RinexSatID.hpp"
%template(std_vector_RinexSatID) std::vector<gnsstk::RinexSatID>;
%include "GPS_URA.hpp"
%include "BrcKeplerOrbit.hpp"
%include "CCIR.hpp"
%include "CGCS2000Ellipsoid.hpp"
%include "NavFilterKey.hpp"
%template(std_binary_function_NavFilterKey) std::binary_function<gnsstk::NavFilterKey *, gnsstk::NavFilterKey *, bool >;
%include "NavFilter.hpp"
%include "CNav2SanityFilter.hpp"
%template(std_binary_function_CNavFilterData) std::binary_function< gnsstk::CNavFilterData,gnsstk::CNavFilterData,bool >;
%template(std_binary_function_CNavFilterDataPtr) std::binary_function< gnsstk::CNavFilterData *, gnsstk::CNavFilterData *, bool >;
%include "CNavFilterData.hpp"
%include "CNavCookFilter.hpp"
%include "NavFilterMgr.hpp"
%include "CNavCrossSourceFilter.hpp"
/* %include "CNavDataElement.hpp" */
/* %include "CNavDataElementStore.hpp" */
/* %include "CNavEOP.hpp" */
/* %include "CNavEmptyFilter.hpp" */
/* %include "CNavGGTO.hpp" */
/* %include "CNavISC.hpp" */
%include "CNavParityFilter.hpp"
%include "CNavReducedAlm.hpp"
%include "CNavTOWFilter.hpp"
/* %include "CNavText.hpp" */
/* %include "CNavUTC.hpp" */
%include "ClockModel.hpp"
%include "PCodeConst.hpp"
%include "CodeBuffer.hpp"
%include "Combinations.hpp"
/* %include "CommandLine.hpp" */
/* %include "CommandOptionWithCommonTimeArg.hpp" */
/* %include "CommandOptionWithPositionArg.hpp" */
/* %include "CommandOptionWithTimeArg.hpp" */
%include "ConfDataReader.hpp"
 /* %include "ConfDataWriter.hpp" */ // error on typemaps for list of double
%include "CubicSpline.hpp"
%include "DCBDataReader.hpp"
/* %include "DebugTrace.hpp" */
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
%feature("flatnested");
%include "RinexObsHeader.hpp"
%feature("flatnested", "");
%template(std_vector_RinexObsType) std::vector<gnsstk::RinexObsType>;
%template(std_unary_function_RinexObsHeader) std::unary_function<gnsstk::RinexObsHeader, bool>;
%include "RinexDatum.hpp"
%template(std_vector_RinexDatum) std::vector<gnsstk::RinexDatum>;
%template(std_map_RinexSatID_std_vector_RinexDatum) std::map<gnsstk::RinexSatID, std::vector<gnsstk::RinexDatum> >;
%template(std_map_RinexObsType_RinexDatum) std::map<gnsstk::RinexObsType, gnsstk::RinexDatum>;
%template(std_map_RinexSatID_RinexObsTypeMap) std::map<gnsstk::SatID, std::map<gnsstk::RinexObsType, gnsstk::RinexDatum> >;
%include "RinexObsData.hpp"
%template(std_binary_function_RinexObsData) std::binary_function<gnsstk::RinexObsData,gnsstk::RinexObsData,bool>;
%include "RinexUtilities.hpp"
%include "TimeString.hpp"
/* %include "SatPass.hpp" */
/* %include "DiscCorr.hpp" */
%include "ENUUtil.hpp"
%include "IERSConvention.hpp"
/* %include "EarthOrientation.hpp" */
/* %include "EOPPrediction.hpp" */
/* %include "EOPStore.hpp" */
%include "EngAlmanac.hpp"
%include "EngEphemeris.hpp"
%include "NavValidityType.hpp"
%include "NavSearchOrder.hpp"
%include "NavDataFactory.hpp"
%include "NavLibrary.hpp"
%include "ValidType.hpp"
%include "EphemerisRange.hpp"
%feature("flatnested");
%include "IonoModel.hpp"
%feature("flatnested", "");
%feature("flatnested");
%include "IonoModelStore.hpp"
%feature("flatnested", "");
%include "SvObsEpoch.hpp"
%template(std_map_ObsEpoch) std::map<gnsstk::SatID, gnsstk::SvObsEpoch>;
%include "ObsEpochMap.hpp"
%template(ObsEpochMap) std::map<gnsstk::CommonTime, gnsstk::ObsEpoch>;
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
%include "RinexObsID.hpp"
%template(std_vector_RinexObsID) std::vector<gnsstk::RinexObsID>;
%template(std_map_string_vector_RinexObsID) std::map<std::string, std::vector<gnsstk::RinexObsID> >;
 // Use a special wrapper for the include file which resolves issues
 // with the nested definitions
%include "Rinex3ObsHeader.i"
%template(std_unary_function_Rinex3ObsHeader) std::unary_function<gnsstk::Rinex3ObsHeader, bool>;
%include "Rinex3ObsData.hpp"
%template(std_vector_Rinex3ObsData) std::vector<gnsstk::Rinex3ObsData>;
%template(std_binary_function_Rinex3ObsData) std::binary_function<gnsstk::Rinex3ObsData,gnsstk::Rinex3ObsData,bool>;
%include "FileFilterFrameWithHeader.hpp"
%include "FileStore.hpp"
%feature("flatnested");
%include "FirstDiffFilter.hpp"
%feature("flatnested", "");
%include "FormattedDouble.hpp"
%include "FreqConv.hpp"
%include "GCATTropModel.hpp"
%include "GGHeightTropModel.hpp"
%include "GGTropModel.hpp"
%include "GLOFBits.hpp"
%include "GLOFNavData.hpp"
%include "PZ90Ellipsoid.hpp"
%include "GLOFNavAlm.hpp"
%include "GLOFNavEph.hpp"
%include "GLOFNavHealth.hpp"
%include "GLOFNavISC.hpp"
%include "GLOFNavTimeOffset.hpp"
%include "GLOFNavUT1TimeOffset.hpp"
%include "GPSCNav2Alm.hpp"
%include "GPSCNav2Eph.hpp"
%include "GPSCNav2Health.hpp"
%include "GPSCNav2ISC.hpp"
%include "GPSCNav2Iono.hpp"
%include "GPSCNav2TimeOffset.hpp"
%include "GPSCNavData.hpp"
%include "GPSCNavAlm.hpp"
%include "GPSCNavEph.hpp"
%include "GPSCNavHealth.hpp"
%include "GPSCNavISC.hpp"
%include "GPSCNavIono.hpp"
%include "GPSCNavRedAlm.hpp"
%include "GPSCNavTimeOffset.hpp"
%include "GPSLNavData.hpp"
%include "GPSLNavAlm.hpp"
%include "GPSLNavEph.hpp"
%include "GPSLNavHealth.hpp"
%include "GPSLNavISC.hpp"
%include "GPSLNavIono.hpp"
%include "GPSLNavTimeOffset.hpp"
%include "GSatID.hpp"
%include "GalDataValid.hpp"
%include "GalHealthStatus.hpp"
%include "GalFNavAlm.hpp"
%include "GalFNavEph.hpp"
%include "GalFNavHealth.hpp"
%include "GalFNavISC.hpp"
%include "MODIP.hpp"
%include "GalileoEllipsoid.hpp"
%include "GalileoIonoEllipsoid.hpp"
%feature("flatnested");
%include "NeQuickIonoData.hpp"
%feature("flatnested", "");
%include "GalFNavIono.hpp"
%include "GalFNavTimeOffset.hpp"
%include "GalINavAlm.hpp"
%include "GalINavEph.hpp"
%include "GalINavHealth.hpp"
%include "GalINavISC.hpp"
%include "GalINavIono.hpp"
%include "GalINavTimeOffset.hpp"
%include "GenXSequence.hpp"
%include "GenericNavFilterData.hpp"
%include "GloEphemeris.hpp"
%include "GlobalTropModel.hpp"
%include "HelmertTransform.hpp"
 //%include "IERS1996NutationData.hpp"
 //%include "IERS1996UT1mUTCData.hpp"
 //%include "IERS2003NutationData.hpp"
/* %include "IERS2010CIOSeriesData.hpp" */
%include "IRNWeekSecond.hpp"
/* %include "LoopedFramework.hpp" */
/* %include "InOutFramework.hpp" */
%include "IonexBase.hpp"
%feature("flatnested");
%include "IonexHeader.hpp"
%feature("flatnested", "");
%template(FileStore_IonexHeader) gnsstk::FileStore<gnsstk::IonexHeader>;
%include "IonexStream.hpp"
%feature("flatnested");
%include "IonexData.hpp"
%feature("flatnested", "");
%include "IonexStore.hpp"
%include "Namelist.hpp"
/* %include "SRIMatrix.hpp" */
/* %include "SparseVector.hpp" */
/* %include "SparseMatrix.hpp" */
/* %include "SRI.hpp" */
/* %include "SRIFilter.hpp" */
/* %include "logstream.hpp" */
%feature("flatnested");
%include "KalmanFilter.hpp"
%feature("flatnested", "");
%template(std_binary_function_LNavFilterData) std::binary_function< gnsstk::LNavFilterData *, gnsstk::LNavFilterData *, bool >;
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
%include "MatrixImplementation.hpp"
%include "MatrixFunctors.hpp"
%include "MatrixOperators.hpp"
%include "MetReader.hpp"
%include "MostCommonValue.hpp"
%include "NavDataFactoryWithStore.hpp"
%include "NavDataFactoryWithStoreFile.hpp"
%include "NDFUniqIterator.hpp"
%template (NDFUniqIterator_NavDataFactoryMap) gnsstk::NDFUniqIterator<gnsstk::NavDataFactoryMap>;
%include "MultiFormatNavDataFactory.hpp"
%include "NDFUniqConstIterator.hpp"
%template (NDFUniqConstIterator_NavDataFactoryMap) gnsstk::NDFUniqConstIterator<gnsstk::NavDataFactoryMap>;
%include "NEDUtil.hpp"
%include "NavMsgData.hpp"
%include "NavMsgDataBits.hpp"
%include "NavMsgDataPNB.hpp"
%include "NavMsgDataWords.hpp"
%template(NavMsgDataWords_30) gnsstk::NavMsgDataWords<30>;
%include "NavOrderFilter.hpp"
%include "NavTimeSystemConverter.hpp"
%include "NeillTropModel.hpp"
%feature("flatnested");
%include "OceanLoadTides.hpp"
%feature("flatnested", "");
%include "OceanLoading.hpp"
%include "OrbitDataSP3.hpp"
%include "PNBNavDataFactory.hpp"
%feature("flatnested");
%include "PNBBDSD1NavDataFactory.hpp"
%feature("flatnested", "");
%feature("flatnested");
%include "PNBBDSD2NavDataFactory.hpp"
%feature("flatnested", "");
%include "PNBGLOFNavDataFactory.hpp"
%include "PNBGPSCNav2DataFactory.hpp"
%include "PNBGPSCNavDataFactory.hpp"
%include "PNBGPSLNavDataFactory.hpp"
%include "PNBGalFNavDataFactory.hpp"
%include "PNBGalINavDataFactory.hpp"
%include "PNBMultiGNSSNavDataFactory.hpp"
%include "stl_helpers.hpp"
%include "PRSolution.hpp"
%include "PoleTides.hpp"
%include "PolyFit.hpp"
%template(PolyFit_double) gnsstk::PolyFit<double>;
%include "PosixTime.hpp"
 //%include "PowerSum.hpp"
%include "SolarSystemEphemeris.hpp"
%include "SunEarthSatGeometry.i"
%include "SolidEarthTides.hpp"
/* %include "SolarSystem.hpp" */
/* %include "PreciseRange.hpp" */
%include "RACRotation.hpp"
%include "RNDouble.hpp"
/* %include "RTFileFrame.hpp" */
%feature("flatnested");
%include "Rinex3ClockBase.hpp"
%feature("flatnested", "");
%include "Rinex3ClockData.hpp"
%include "Rinex3ClockHeader.hpp"
%include "Rinex3ClockStream.hpp"
%include "Rinex3NavBase.hpp"
%include "TimeSystemCorr.hpp"
%include "Rinex3NavHeader.hpp"
%template(std_unary_function_Rinex3NavHeader) std::unary_function<gnsstk::Rinex3NavHeader, bool>;
%include "Rinex3NavStream.hpp"
%include "RinexNavBase.hpp"
%include "RinexNavData.hpp"
%template(std_binary_function_RinexNavData) std::binary_function<gnsstk::RinexNavData,gnsstk::RinexNavData,bool>;
%template(std_unary_function_RinexNavData) std::unary_function<gnsstk::RinexNavData,bool>;
%include "Rinex3NavData.hpp"
%template(std_binary_function_Rinex3NavData) std::binary_function<gnsstk::Rinex3NavData,gnsstk::Rinex3NavData,bool>;
%template(std_unary_function_Rinex3NavData) std::unary_function<gnsstk::Rinex3NavData,bool>;
%include "Rinex3NavFilterOperators.hpp"
/* %include "Rinex3ObsFileLoader.hpp" */
%include "Rinex3ObsFilterOperators.hpp"
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
%include "RinexMetData.hpp"
%template(std_binary_function_RinexMetData) std::binary_function<gnsstk::RinexMetData,gnsstk::RinexMetData,bool>;
%include "RinexMetFilterOperators.hpp"
%include "RinexMetStream.hpp"
%include "RinexNavDataFactory.hpp"
%include "RinexNavHeader.hpp"
%template (FileStore_RinexNavHeader) gnsstk::FileStore<gnsstk::RinexNavHeader>;
%template(std_unary_function_RinexNavHeader) std::unary_function<gnsstk::RinexNavHeader,bool>;
%include "RinexNavFilterOperators.hpp"
%include "RinexNavStream.hpp"
%include "RinexObsFilterOperators.hpp"
%include "RinexObsStream.hpp"
%include "RinexTimeOffset.hpp"
%include "SEMBase.hpp"
%include "SEMHeader.hpp"
%template(FileStore_SEMHeader) gnsstk::FileStore<gnsstk::SEMHeader>;
%include "SEMData.hpp"
%include "SEMNavDataFactory.hpp"
%include "SEMStream.hpp"
%include "SP3Base.hpp"
%include "SP3SatID.hpp"
%include "SP3Data.hpp"
%include "SP3Header.hpp"
%include "SP3NavDataFactory.hpp"
%include "SP3Stream.hpp"
/* %include "SRIleastSquares.hpp" */
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
%include "SolarPosition.i"
/* %include "SpecialFuncs.hpp" */
%include "StreamBuf.hpp"
%include "TimeCorrection.hpp"
%include "TimeNamedFileStream.hpp"
%feature("flatnested");
%include "TimeRange.hpp"
%feature("flatnested", "");
%include "VectorOperators.hpp"
%include "WNJfilter.hpp"
%feature("flatnested");
%include "WindowFilter.hpp"
%feature("flatnested", "");
%include "YumaBase.hpp"
%include "YumaHeader.hpp"
%template(FileStore_YumaHeader) gnsstk::FileStore<gnsstk::YumaHeader>;
%include "YumaData.hpp"
%include "YumaNavDataFactory.hpp"
%include "YumaStream.hpp"
%include "convhelp.hpp"
%include "demangle.hpp"
%include "expandtilde.hpp"
%include "format.hpp"
/* %include "gdc.hpp" */
%include "ord.hpp"
%include "random.hpp"
%include "singleton.hpp"
