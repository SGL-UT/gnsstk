//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
//
//==============================================================================


//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin, under contract to an agency or agencies 
//  within the U.S. Department of Defense. The U.S. Government retains all 
//  rights to use, duplicate, distribute, disclose, or release this software. 
//
//  Pursuant to DoD Directive 523024 
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public 
//                            release, distribution is unlimited.
//
//==============================================================================
// This file is used in building the swig bindings of the GPSTk and is not really
// intended to be used by C++ code directly

#ifndef GPSTK_SWIG_HPP
#define GPSTK_SWIG_HPP

// time:
#include "TimeSystem.hpp"
#include "TimeTag.hpp"
#include "TimeConstants.hpp"
#include "TimeConverters.hpp"
#include "Week.hpp"
#include "WeekSecond.hpp"
#include "UnixTime.hpp"
#include "ANSITime.hpp"
#include "CivilTime.hpp"
#include "MathBase.hpp"
#include "CommonTime.hpp"
#include "Exception.hpp"
#include "GPSZcount.hpp"
#include "GPSWeek.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSWeekZcount.hpp"
#include "JulianDate.hpp"
#include "BDSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "QZSWeekSecond.hpp"
#include "MJD.hpp"
#include "SystemTime.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "TimeSystemCorr.hpp"

// general files:
#include "StringUtils.hpp"
#include "GNSSconstants.hpp"
#include "gps_constants.hpp"
#include "SatID.hpp"
#include "ObsID.hpp"
#include "Triple.hpp"
#include "ReferenceFrame.hpp"
#include "EllipsoidModel.hpp"
#include "Xvt.hpp"
#include "StringUtils.hpp"
#include "Position.hpp"
#include "convhelp.hpp"
#include "VectorBase.hpp"
#include "Vector.hpp"
#include "DataStatus.hpp"

// more specific almanac/ephemeris files:
#include "AstronomicalFunctions.hpp"
#include "XvtStore.hpp"
#include "OrbAlmStore.hpp"
#include "PZ90Ellipsoid.hpp"
#include "WGS84Ellipsoid.hpp"
#include "gpstkplatform.h"
#include "FFStreamError.hpp"
#include "FileStore.hpp"
#include "FFData.hpp"
#include "EngNav.hpp"
#include "YumaBase.hpp"
#include "FFStream.hpp"
#include "FFTextStream.hpp"
#include "AlmOrbit.hpp"
#include "YumaHeader.hpp"
#include "EngAlmanac.hpp"
#include "OrbElem.hpp"
#include "OrbElemStore.hpp"
#include "YumaStream.hpp"
#include "YumaData.hpp"
#include "GPSAlmanacStore.hpp"
#include "YumaAlmanacStore.hpp"
//#include "RinexSatID.hpp"
#include "GPS_URA.hpp"
#include "BrcClockCorrection.hpp"
#include "BrcKeplerOrbit.hpp"
#include "EphemerisRange.hpp"
// #include "EphReader.hpp"

// Ephemeris:
#include "OrbitEph.hpp"
#include "BDSEphemeris.hpp"
#include "EngEphemeris.hpp"
#include "GalEphemeris.hpp"
#include "GloEphemeris.hpp"
#include "GPSEphemeris.hpp"
#include "QZSEphemeris.hpp"

// RINEX format:
#include "RinexSatID.hpp"
#include "RinexDatum.hpp"
#include "RinexObsBase.hpp"
#include "RinexObsHeader.hpp"
#include "RinexObsData.hpp"
#include "RinexObsID.hpp"
#include "RinexObsStream.hpp"
#include "RinexNavBase.hpp"
#include "RinexNavHeader.hpp"
#include "RinexNavStream.hpp"
#include "RinexNavData.hpp"
#include "RinexMetBase.hpp"
#include "RinexMetHeader.hpp"
#include "RinexMetStream.hpp"
#include "RinexMetData.hpp"
#include "Rinex3NavBase.hpp"
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavStream.hpp"
#include "Rinex3NavData.hpp"
#include "OrbElemRinex.hpp"
#include "Rinex3ClockBase.hpp"
#include "Rinex3ObsBase.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"
#include "Rinex3ObsStream.hpp"
#include "Rinex3ClockHeader.hpp"
#include "Rinex3ClockData.hpp"
#include "Rinex3ClockStream.hpp"
#include "Rinex3EphemerisStore.hpp"

// Ephemeris stores:
#include "OrbitEphStore.hpp"
#include "BDSEphemerisStore.hpp"
#include "GalEphemerisStore.hpp"
#include "GloEphemerisStore.hpp"
#include "GPSEphemerisStore.hpp"
#include "QZSEphemerisStore.hpp"
#include "RinexEphemerisStore.hpp"

// SP3 format:
#include "TabularSatStore.hpp"
#include "ClockSatStore.hpp"
#include "SP3Base.hpp"
#include "SP3SatID.hpp"
#include "SP3Header.hpp"
#include "SP3Data.hpp"
#include "SP3Stream.hpp"
#include "PositionSatStore.hpp"
#include "SP3EphemerisStore.hpp"
#include "RinexUtilities.hpp"

// SEM format:
#include "SEMBase.hpp"
#include "SEMHeader.hpp"
#include "SEMStream.hpp"
#include "SEMData.hpp"
#include "SEMAlmanacStore.hpp"

#include "Matrix.hpp"
#include "ValidType.hpp"
#include "ObsEpochMap.hpp"
#include "WxObsMap.hpp"
#include "TropModel.hpp"
#include "GCATTropModel.hpp"
#include "GGHeightTropModel.hpp"
#include "GGTropModel.hpp"
#include "MOPSTropModel.hpp"
#include "NBTropModel.hpp"
#include "NeillTropModel.hpp"
#include "SaasTropModel.hpp"
#include "SimpleTropModel.hpp"
#include "PRSolutionLegacy.hpp"
#include "Expression.hpp"

// Ashtech serial data
#include "FFBinaryStream.hpp"
#include "AshtechStream.hpp"
#include "AshtechData.hpp"
#include "AshtechALB.hpp"
#include "AshtechEPB.hpp"
#include "AshtechMBEN.hpp"
#include "AshtechPBEN.hpp"

// Positioning/Tides
#include "MoonPosition.hpp"
#include "SunPosition.hpp"
#include "PoleTides.hpp"
#include "SolidTides.hpp"

// PosSol
#include "Combinations.hpp"
#include "PRSolution.hpp"

// Geomatics:
#include "SunEarthSatGeometry.hpp"

// ORD
#include "ord.hpp"

// So the python examples can find the test data
#include "build_config.h"

// OrbSysGPS
#include "OrbSysStore.hpp"
#include "OrbData.hpp"
#include "OrbDataSys.hpp"
#include "OrbSysGpsC.hpp"
#include "OrbSysGpsC_30.hpp"
#include "OrbSysGpsC_32.hpp"
#include "OrbSysGpsC_33.hpp"
#include "OrbSysGpsL.hpp"
#include "OrbSysGpsL_51.hpp"
#include "OrbSysGpsL_52.hpp"
#include "OrbSysGpsL_55.hpp"
#include "OrbSysGpsL_56.hpp"
#include "OrbSysGpsL_63.hpp"
#include "OrbSysGpsL_Reserved.hpp"
#include "PackedNavBits.hpp"
#include "OrbDataSysFactory.hpp"

// NewNav enums
#include "NavMessageType.hpp"
#include "SVHealth.hpp"
#include "NavSearchOrder.hpp"
#include "NavValidityType.hpp"
#include "DumpDetail.hpp"

// NewNav store
#include "NavSignalID.hpp"
#include "NavSatelliteID.hpp"
#include "NavMessageID.hpp"
#include "NavData.hpp"
#include "TimeOffsetData.hpp"
#include "NavHealthData.hpp"
#include "OrbitData.hpp"
#include "OrbitDataKepler.hpp"
#include "GPSLNavData.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavTimeOffset.hpp"

#include "NavDataFactory.hpp"
#include "NavLibrary.hpp"
#include "NavDataFactoryWithStore.hpp"
#include "NavDataFactoryWithStoreFile.hpp"
#include "RinexNavDataFactory.hpp"
#include "SP3NavDataFactory.hpp"
#include "SEMNavDataFactory.hpp"
#include "YumaNavDataFactory.hpp"
#include "MultiFormatNavDataFactory.hpp"

// #include "GPSCNav2Alm.hpp"
// #include "GPSCNav2Eph.hpp"
// #include "GPSCNav2Health.hpp"
// #include "GPSCNav2TimeOffset.hpp"
// #include "GPSCNavAlm.hpp"
// #include "GPSCNavData.hpp"
// #include "GPSCNavEph.hpp"
// #include "GPSCNavHealth.hpp"
// #include "GPSCNavRedAlm.hpp"
// #include "GPSCNavTimeOffset.hpp"
// #include "OrbitDataSP3.hpp"
// #include "PNBGPSCNav2DataFactory.hpp"
// #include "PNBGPSCNavDataFactory.hpp"
// #include "PNBGPSLNavDataFactory.hpp"
// #include "PNBMultiGNSSNavDataFactory.hpp"
// #include "PNBNavDataFactory.hpp"
// #include "RinexTimeOffset.hpp"

#endif
