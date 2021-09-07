//FILE TO HOLD VARIOUS (PREVIOUSLY RANDOMLY PLACED) MACRO INSTANTIATIONS

STR_DUMP_HELPER(AlmOrbit)
STR_DUMP_HELPER(BDSEphemeris)
STR_DUMP_HELPER(BinexData)
STR_DUMP_HELPER(BrcClockCorrection)
STR_DUMP_HELPER(BrcKeplerOrbit)
STR_DUMP_HELPER(EngAlmanac)
STR_DUMP_HELPER(EngEphemeris)
STR_DUMP_HELPER(EngNav)
STR_DUMP_HELPER(GPSEphemeris)
STR_DUMP_HELPER(GalEphemeris)
STR_DUMP_HELPER(GloEphemeris)
STR_DUMP_HELPER(OrbElem)
STR_DUMP_HELPER(OrbElemStore)
STR_DUMP_HELPER(QZSEphemeris)
STR_DUMP_HELPER(Rinex3ClockData)
STR_DUMP_HELPER(Rinex3ClockHeader)
STR_DUMP_HELPER(Rinex3NavData)
STR_DUMP_HELPER(Rinex3NavHeader)
STR_DUMP_HELPER(Rinex3ObsData)
STR_DUMP_HELPER(Rinex3ObsHeader)
STR_DUMP_HELPER(RinexClockHeader)
STR_DUMP_HELPER(RinexClockData)
STR_DUMP_HELPER(RinexMetData)
STR_DUMP_HELPER(RinexMetHeader)
STR_DUMP_HELPER(RinexNavData)
STR_DUMP_HELPER(RinexNavHeader)
STR_DUMP_HELPER(RinexObsData)
STR_DUMP_HELPER(RinexObsHeader)
STR_DUMP_HELPER(SEMData)
STR_DUMP_HELPER(SEMHeader)
STR_DUMP_HELPER(SP3Data)
STR_DUMP_HELPER(SP3Header)
STR_DUMP_HELPER(SP3SatID)
STR_DUMP_HELPER(YumaData)
STR_DUMP_HELPER(YumaHeader)
STR_DUMP_HELPER(CNavCrossSourceFilter)
/** @todo Check why this seg faults at least on objects created with
 * default constructor */
STR_DUMP_HELPER(CNavFilterData)
STR_DUMP_HELPER(GenericNavFilterData)

STR_PRINT_HELPER(Expression)

STR_DUMP_DETAIL_HELPER(ClockSatStore)
STR_DUMP_DETAIL_HELPER(GalEphemerisStore)
STR_DUMP_DETAIL_HELPER(GloEphemerisStore)
STR_DUMP_DETAIL_HELPER(GPSEphemerisStore)
STR_DUMP_DETAIL_HELPER(Rinex3EphemerisStore)
STR_DUMP_DETAIL_HELPER(SP3EphemerisStore)
STR_DUMP_DETAIL_HELPER(NavMsgDataWords)
STR_DUMP_DETAIL_HELPER(NavMsgDataBits)
STR_DUMP_DETAIL_HELPER(NavMsgDataPNB)

STR_NNDUMP_DETAIL_HELPER(NavLibrary)
STR_NNDUMP_DETAIL_HELPER(NavDataFactory)
STR_NNDUMP_DETAIL_HELPER(OrbitDataKepler)
STR_NNDUMP_DETAIL_HELPER(NavData)

STR_STREAM_HELPER(GPSZcount)
STR_STREAM_HELPER(Position)
STR_STREAM_HELPER(SvObsEpoch)
STR_STREAM_HELPER(ObsEpoch)
STR_STREAM_HELPER(NavSignalID)
STR_STREAM_HELPER(NavSatelliteID)
STR_STREAM_HELPER(NavMessageID)
STR_STREAM_HELPER(Stats)
STR_STREAM_HELPER(SeqStats)
STR_STREAM_HELPER(WtdStats)
STR_STREAM_HELPER(TwoSampleStats)
STR_STREAM_HELPER(BivarStats)
STR_STREAM_HELPER(Vector)
STR_STREAM_HELPER(SatMetaData)
STR_STREAM_HELPER(NavFilterKey)
/** @todo Check why this seg faults at least on objects created with
 * default constructor */
STR_STREAM_HELPER(LNavFilterData)
STR_STREAM_HELPER(Matrix)

STR_STREAM_VEC_HELPER(std::vector<int>)
STR_STREAM_VEC_HELPER(std::vector<double>)
STR_STREAM_VEC_HELPER(std::vector<std::string>)
STR_STREAM_VEC_HELPER(gnsstk::Triple)

STR_FILTER_HELPER(CNavParityFilter)
STR_FILTER_HELPER(CNavCookFilter)
STR_FILTER_HELPER(CNavTOWFilter)
STR_FILTER_HELPER(CNav2SanityFilter)
STR_FILTER_HELPER(LNavAlmValFilter)
STR_FILTER_HELPER(LNavCookFilter)
STR_FILTER_HELPER(LNavCrossSourceFilter)
STR_FILTER_HELPER(LNavEmptyFilter)
STR_FILTER_HELPER(LNavEphMaker)
STR_FILTER_HELPER(LNavOrderFilter)
STR_FILTER_HELPER(LNavParityFilter)
STR_FILTER_HELPER(LNavTLMHOWFilter)

AS_STRING_HELPER(ObsID)
AS_STRING_HELPER(SatID)
AS_STRING_HELPER(NavID)
AS_STRING_HELPER(RinexSatID)

AS_STRING_HELPER2(RinexObsID)



// STR_STREAM_HELPER(Xvt)
// Q: Why is this (below) here instead of the macro for Xvt?
// A: There is an ambiguity issue for the operator<< for Xvt,
// see the end of TabularSatStore.hpp for a conflicting defintion
// of the Xvt out stream operator and Xvt.hpp+Xvt.cpp.
%extend gnsstk::Xvt {
   std::string __str__() {
      std::ostringstream os;
         os << "x:" << $self->x
            << ", v:" << $self->v
            << ", clk bias:" << $self->clkbias
            << ", clk drift:" << $self->clkdrift
            << ", relcorr:" << $self->relcorr;
      return os.str();
   }
}
