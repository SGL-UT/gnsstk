add_executable(NavDataFactoryWithStore_T NavDataFactoryWithStore_T.cpp)
target_link_libraries(NavDataFactoryWithStore_T gnsstk)
add_test(NavDataFactoryWithStore_T NavDataFactoryWithStore_T)
set_property(TEST NavDataFactoryWithStore_T PROPERTY LABELS NewNav)

add_executable(RinexNavDataFactory_T RinexNavDataFactory_T.cpp)
target_link_libraries(RinexNavDataFactory_T gnsstk)
add_test(RinexNavDataFactory_T RinexNavDataFactory_T)
set_property(TEST RinexNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(SP3NavDataFactory_T SP3NavDataFactory_T.cpp)
target_link_libraries(SP3NavDataFactory_T gnsstk)
add_test(SP3NavDataFactory_T SP3NavDataFactory_T)
set_property(TEST SP3NavDataFactory_T PROPERTY LABELS NewNav)

add_executable(MultiFormatNavDataFactory_T MultiFormatNavDataFactory_T.cpp)
target_link_libraries(MultiFormatNavDataFactory_T gnsstk)
add_test(MultiFormatNavDataFactory_T MultiFormatNavDataFactory_T)
set_property(TEST MultiFormatNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(KlobucharIonoNavData_T KlobucharIonoNavData_T.cpp)
target_link_libraries(KlobucharIonoNavData_T gnsstk)
add_test(KlobucharIonoNavData_T KlobucharIonoNavData_T)
set_property(TEST KlobucharIonoNavData_T PROPERTY LABELS NewNav)

add_executable(NeQuickIonoNavData_T NeQuickIonoNavData_T.cpp)
target_link_libraries(NeQuickIonoNavData_T gnsstk)
add_test(NeQuickIonoNavData_T NeQuickIonoNavData_T)
set_property(TEST NeQuickIonoNavData_T PROPERTY LABELS NewNav)

add_executable(MODIP_T MODIP_T.cpp)
target_link_libraries(MODIP_T gnsstk)
add_test(MODIP_T MODIP_T)
set_property(TEST MODIP_T PROPERTY LABELS NewNav)

add_executable(CCIR_T CCIR_T.cpp)
target_link_libraries(CCIR_T gnsstk)
add_test(CCIR_T CCIR_T)
set_property(TEST CCIR_T PROPERTY LABELS NewNav)

add_executable(GPSLNavData_T GPSLNavData_T.cpp)
target_link_libraries(GPSLNavData_T gnsstk)
add_test(GPSLNavData_T GPSLNavData_T)
set_property(TEST GPSLNavData_T PROPERTY LABELS NewNav)

add_executable(GPSLNavEph_T GPSLNavEph_T.cpp)
target_link_libraries(GPSLNavEph_T gnsstk)
add_test(GPSLNavEph_T GPSLNavEph_T)
set_property(TEST GPSLNavEph_T PROPERTY LABELS NewNav)

add_executable(GPSLNavAlm_T GPSLNavAlm_T.cpp)
target_link_libraries(GPSLNavAlm_T gnsstk)
add_test(GPSLNavAlm_T GPSLNavAlm_T)
set_property(TEST GPSLNavAlm_T PROPERTY LABELS NewNav)

add_executable(GPSLNavHealth_T GPSLNavHealth_T.cpp)
target_link_libraries(GPSLNavHealth_T gnsstk)
add_test(GPSLNavHealth_T GPSLNavHealth_T)
set_property(TEST GPSLNavHealth_T PROPERTY LABELS NewNav)

add_executable(GPSLNavTimeOffset_T GPSLNavTimeOffset_T.cpp)
target_link_libraries(GPSLNavTimeOffset_T gnsstk)
add_test(GPSLNavTimeOffset_T GPSLNavTimeOffset_T)
set_property(TEST GPSLNavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(GPSLNavIono_T GPSLNavIono_T.cpp)
target_link_libraries(GPSLNavIono_T gnsstk)
add_test(GPSLNavIono_T GPSLNavIono_T)
set_property(TEST GPSLNavIono_T PROPERTY LABELS NewNav)

add_executable(GPSLNavISC_T GPSLNavISC_T.cpp)
target_link_libraries(GPSLNavISC_T gnsstk)
add_test(GPSLNavISC_T GPSLNavISC_T)
set_property(TEST GPSLNavISC_T PROPERTY LABELS NewNav)

add_executable(GPSCNavData_T GPSCNavData_T.cpp)
target_link_libraries(GPSCNavData_T gnsstk)
add_test(GPSCNavData_T GPSCNavData_T)
set_property(TEST GPSCNavData_T PROPERTY LABELS NewNav)

add_executable(GPSCNavEph_T GPSCNavEph_T.cpp)
target_link_libraries(GPSCNavEph_T gnsstk)
add_test(GPSCNavEph_T GPSCNavEph_T)
set_property(TEST GPSCNavEph_T PROPERTY LABELS NewNav)

add_executable(GPSCNavAlm_T GPSCNavAlm_T.cpp)
target_link_libraries(GPSCNavAlm_T gnsstk)
add_test(GPSCNavAlm_T GPSCNavAlm_T)
set_property(TEST GPSCNavAlm_T PROPERTY LABELS NewNav)

add_executable(GPSCNavHealth_T GPSCNavHealth_T.cpp)
target_link_libraries(GPSCNavHealth_T gnsstk)
add_test(GPSCNavHealth_T GPSCNavHealth_T)
set_property(TEST GPSCNavHealth_T PROPERTY LABELS NewNav)

add_executable(GPSCNavTimeOffset_T GPSCNavTimeOffset_T.cpp)
target_link_libraries(GPSCNavTimeOffset_T gnsstk)
add_test(GPSCNavTimeOffset_T GPSCNavTimeOffset_T)
set_property(TEST GPSCNavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(GPSCNavIono_T GPSCNavIono_T.cpp)
target_link_libraries(GPSCNavIono_T gnsstk)
add_test(GPSCNavIono_T GPSCNavIono_T)
set_property(TEST GPSCNavIono_T PROPERTY LABELS NewNav)

add_executable(GPSCNavISC_T GPSCNavISC_T.cpp)
target_link_libraries(GPSCNavISC_T gnsstk)
add_test(GPSCNavISC_T GPSCNavISC_T)
set_property(TEST GPSCNavISC_T PROPERTY LABELS NewNav)

add_executable(GPSCNav2Eph_T GPSCNav2Eph_T.cpp)
target_link_libraries(GPSCNav2Eph_T gnsstk)
add_test(GPSCNav2Eph_T GPSCNav2Eph_T)
set_property(TEST GPSCNav2Eph_T PROPERTY LABELS NewNav)

add_executable(GPSCNav2Alm_T GPSCNav2Alm_T.cpp)
target_link_libraries(GPSCNav2Alm_T gnsstk)
add_test(GPSCNav2Alm_T GPSCNav2Alm_T)
set_property(TEST GPSCNav2Alm_T PROPERTY LABELS NewNav)

add_executable(GPSCNav2Health_T GPSCNav2Health_T.cpp)
target_link_libraries(GPSCNav2Health_T gnsstk)
add_test(GPSCNav2Health_T GPSCNav2Health_T)
set_property(TEST GPSCNav2Health_T PROPERTY LABELS NewNav)

add_executable(GPSCNav2TimeOffset_T GPSCNav2TimeOffset_T.cpp)
target_link_libraries(GPSCNav2TimeOffset_T gnsstk)
add_test(GPSCNav2TimeOffset_T GPSCNav2TimeOffset_T)
set_property(TEST GPSCNav2TimeOffset_T PROPERTY LABELS NewNav)

add_executable(GPSCNav2Iono_T GPSCNav2Iono_T.cpp)
target_link_libraries(GPSCNav2Iono_T gnsstk)
add_test(GPSCNav2Iono_T GPSCNav2Iono_T)
set_property(TEST GPSCNav2Iono_T PROPERTY LABELS NewNav)

add_executable(NavDataFactory_T NavDataFactory_T.cpp)
target_link_libraries(NavDataFactory_T gnsstk)
add_test(NavDataFactory_T NavDataFactory_T)
set_property(TEST NavDataFactory_T PROPERTY LABELS NewNav)

add_executable(NavHealthData_T NavHealthData_T.cpp)
target_link_libraries(NavHealthData_T gnsstk)
add_test(NavHealthData_T NavHealthData_T)
set_property(TEST NavHealthData_T PROPERTY LABELS NewNav)

add_executable(NavMessageID_T NavMessageID_T.cpp)
target_link_libraries(NavMessageID_T gnsstk)
add_test(NavMessageID_T NavMessageID_T)
set_property(TEST NavMessageID_T PROPERTY LABELS NewNav)

add_executable(NavMessageType_T NavMessageType_T.cpp)
target_link_libraries(NavMessageType_T gnsstk)
add_test(NavMessageType_T NavMessageType_T)
set_property(TEST NavMessageType_T PROPERTY LABELS NewNav)

add_executable(NavValidityType_T NavValidityType_T.cpp)
target_link_libraries(NavValidityType_T gnsstk)
add_test(NavValidityType_T NavValidityType_T)
set_property(TEST NavValidityType_T PROPERTY LABELS NewNav)

add_executable(SVHealth_T SVHealth_T.cpp)
target_link_libraries(SVHealth_T gnsstk)
add_test(SVHealth_T SVHealth_T)
set_property(TEST SVHealth_T PROPERTY LABELS NewNav)

add_executable(NavLibrary_T NavLibrary_T.cpp)
target_link_libraries(NavLibrary_T gnsstk)
add_test(NavLibrary_T NavLibrary_T)
set_property(TEST NavLibrary_T PROPERTY LABELS NewNav)

add_executable(NavSignalID_T NavSignalID_T.cpp)
target_link_libraries(NavSignalID_T gnsstk)
add_test(NavSignalID_T NavSignalID_T)
set_property(TEST NavSignalID_T PROPERTY LABELS NewNav)

add_executable(OrbitDataKepler_T OrbitDataKepler_T.cpp)
target_link_libraries(OrbitDataKepler_T gnsstk)
add_test(OrbitDataKepler_T OrbitDataKepler_T)
set_property(TEST OrbitDataKepler_T PROPERTY LABELS NewNav)

add_executable(GNSSTKFormatInitializer_T GNSSTKFormatInitializer_T.cpp)
target_link_libraries(GNSSTKFormatInitializer_T gnsstk)
add_test(GNSSTKFormatInitializer_T GNSSTKFormatInitializer_T)
set_property(TEST GNSSTKFormatInitializer_T PROPERTY LABELS NewNav)

add_executable(NavSatelliteID_T NavSatelliteID_T.cpp)
target_link_libraries(NavSatelliteID_T gnsstk)
add_test(NavSatelliteID_T NavSatelliteID_T)
set_property(TEST NavSatelliteID_T PROPERTY LABELS NewNav)

add_executable(OrbitDataSP3_T OrbitDataSP3_T.cpp)
target_link_libraries(OrbitDataSP3_T gnsstk)
add_test(OrbitDataSP3_T OrbitDataSP3_T)
set_property(TEST OrbitDataSP3_T PROPERTY LABELS NewNav)

add_executable(PNBNavDataFactory_T PNBNavDataFactory_T.cpp)
target_link_libraries(PNBNavDataFactory_T gnsstk)
add_test(PNBNavDataFactory_T PNBNavDataFactory_T)
set_property(TEST PNBNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(PNBMultiGNSSNavDataFactory_T PNBMultiGNSSNavDataFactory_T.cpp)
target_link_libraries(PNBMultiGNSSNavDataFactory_T gnsstk)
add_test(PNBMultiGNSSNavDataFactory_T PNBMultiGNSSNavDataFactory_T)
set_property(TEST PNBMultiGNSSNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(PNBGPSLNavDataFactory_T PNBGPSLNavDataFactory_T.cpp)
target_link_libraries(PNBGPSLNavDataFactory_T gnsstk)
add_test(PNBGPSLNavDataFactory_T PNBGPSLNavDataFactory_T)
set_property(TEST PNBGPSLNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(PNBGPSCNavDataFactory_T PNBGPSCNavDataFactory_T.cpp)
target_link_libraries(PNBGPSCNavDataFactory_T gnsstk)
add_test(PNBGPSCNavDataFactory_T PNBGPSCNavDataFactory_T)
set_property(TEST PNBGPSCNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(PNBGPSCNav2DataFactory_T PNBGPSCNav2DataFactory_T.cpp)
target_link_libraries(PNBGPSCNav2DataFactory_T gnsstk)
add_test(PNBGPSCNav2DataFactory_T PNBGPSCNav2DataFactory_T)
set_property(TEST PNBGPSCNav2DataFactory_T PROPERTY LABELS NewNav)

add_executable(PNBBDSD1NavDataFactory_T PNBBDSD1NavDataFactory_T.cpp)
target_link_libraries(PNBBDSD1NavDataFactory_T gnsstk)
add_test(PNBBDSD1NavDataFactory_T PNBBDSD1NavDataFactory_T)
set_property(TEST PNBBDSD1NavDataFactory_T PROPERTY LABELS NewNav)

add_executable(PNBBDSD2NavDataFactory_T PNBBDSD2NavDataFactory_T.cpp)
target_link_libraries(PNBBDSD2NavDataFactory_T gnsstk)
add_test(PNBBDSD2NavDataFactory_T PNBBDSD2NavDataFactory_T)
set_property(TEST PNBBDSD2NavDataFactory_T PROPERTY LABELS NewNav)

add_executable(RinexTimeOffset_T RinexTimeOffset_T.cpp)
target_link_libraries(RinexTimeOffset_T gnsstk)
add_test(RinexTimeOffset_T RinexTimeOffset_T)
set_property(TEST RinexTimeOffset_T PROPERTY LABELS NewNav)

add_executable(StdNavTimeOffset_T StdNavTimeOffset_T.cpp)
target_link_libraries(StdNavTimeOffset_T gnsstk)
add_test(StdNavTimeOffset_T StdNavTimeOffset_T)
set_property(TEST StdNavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(GalDataValid_T GalDataValid_T.cpp)
target_link_libraries(GalDataValid_T gnsstk)
add_test(GalDataValid_T GalDataValid_T)
set_property(TEST GalDataValid_T PROPERTY LABELS NewNav)

add_executable(GalHealthStatus_T GalHealthStatus_T.cpp)
target_link_libraries(GalHealthStatus_T gnsstk)
add_test(GalHealthStatus_T GalHealthStatus_T)
set_property(TEST GalHealthStatus_T PROPERTY LABELS NewNav)

add_executable(GalINavTimeOffset_T GalINavTimeOffset_T.cpp)
target_link_libraries(GalINavTimeOffset_T gnsstk)
add_test(GalINavTimeOffset_T GalINavTimeOffset_T)
set_property(TEST GalINavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(GalINavAlm_T GalINavAlm_T.cpp)
target_link_libraries(GalINavAlm_T gnsstk)
add_test(GalINavAlm_T GalINavAlm_T)
set_property(TEST GalINavAlm_T PROPERTY LABELS NewNav)

add_executable(GalINavHealth_T GalINavHealth_T.cpp)
target_link_libraries(GalINavHealth_T gnsstk)
add_test(GalINavHealth_T GalINavHealth_T)
set_property(TEST GalINavHealth_T PROPERTY LABELS NewNav)

add_executable(GalINavEph_T GalINavEph_T.cpp)
target_link_libraries(GalINavEph_T gnsstk)
add_test(GalINavEph_T GalINavEph_T)
set_property(TEST GalINavEph_T PROPERTY LABELS NewNav)

add_executable(GalINavISC_T GalINavISC_T.cpp)
target_link_libraries(GalINavISC_T gnsstk)
add_test(GalINavISC_T GalINavISC_T)

add_executable(PNBGalINavDataFactory_T PNBGalINavDataFactory_T.cpp)
target_link_libraries(PNBGalINavDataFactory_T gnsstk)
add_test(PNBGalINavDataFactory_T PNBGalINavDataFactory_T)
set_property(TEST PNBGalINavDataFactory_T PROPERTY LABELS NewNav)

add_executable(GalFNavTimeOffset_T GalFNavTimeOffset_T.cpp)
target_link_libraries(GalFNavTimeOffset_T gnsstk)
add_test(GalFNavTimeOffset_T GalFNavTimeOffset_T)
set_property(TEST GalFNavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(GalFNavAlm_T GalFNavAlm_T.cpp)
target_link_libraries(GalFNavAlm_T gnsstk)
add_test(GalFNavAlm_T GalFNavAlm_T)
set_property(TEST GalFNavAlm_T PROPERTY LABELS NewNav)

add_executable(GalFNavHealth_T GalFNavHealth_T.cpp)
target_link_libraries(GalFNavHealth_T gnsstk)
add_test(GalFNavHealth_T GalFNavHealth_T)
set_property(TEST GalFNavHealth_T PROPERTY LABELS NewNav)

add_executable(GalFNavEph_T GalFNavEph_T.cpp)
target_link_libraries(GalFNavEph_T gnsstk)
add_test(GalFNavEph_T GalFNavEph_T)
set_property(TEST GalFNavEph_T PROPERTY LABELS NewNav)

add_executable(GalFNavISC_T GalFNavISC_T.cpp)
target_link_libraries(GalFNavISC_T gnsstk)
add_test(GalFNavISC_T GalFNavISC_T)

add_executable(PNBGalFNavDataFactory_T PNBGalFNavDataFactory_T.cpp)
target_link_libraries(PNBGalFNavDataFactory_T gnsstk)
add_test(PNBGalFNavDataFactory_T PNBGalFNavDataFactory_T)
set_property(TEST PNBGalFNavDataFactory_T PROPERTY LABELS NewNav)

add_executable(InterSigCorr_T InterSigCorr_T.cpp)
target_link_libraries(InterSigCorr_T gnsstk)
add_test(InterSigCorr_T InterSigCorr_T)
set_property(TEST InterSigCorr_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavData_T BDSD1NavData_T.cpp)
target_link_libraries(BDSD1NavData_T gnsstk)
add_test(BDSD1NavData_T BDSD1NavData_T)
set_property(TEST BDSD1NavData_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavAlm_T BDSD1NavAlm_T.cpp)
target_link_libraries(BDSD1NavAlm_T gnsstk)
add_test(BDSD1NavAlm_T BDSD1NavAlm_T)
set_property(TEST BDSD1NavAlm_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavEph_T BDSD1NavEph_T.cpp)
target_link_libraries(BDSD1NavEph_T gnsstk)
add_test(BDSD1NavEph_T BDSD1NavEph_T)
set_property(TEST BDSD1NavEph_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavHealth_T BDSD1NavHealth_T.cpp)
target_link_libraries(BDSD1NavHealth_T gnsstk)
add_test(BDSD1NavHealth_T BDSD1NavHealth_T)
set_property(TEST BDSD1NavHealth_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavISC_T BDSD1NavISC_T.cpp)
target_link_libraries(BDSD1NavISC_T gnsstk)
add_test(BDSD1NavISC_T BDSD1NavISC_T)
set_property(TEST BDSD1NavISC_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavIono_T BDSD1NavIono_T.cpp)
target_link_libraries(BDSD1NavIono_T gnsstk)
add_test(BDSD1NavIono_T BDSD1NavIono_T)
set_property(TEST BDSD1NavIono_T PROPERTY LABELS NewNav)

add_executable(BDSD1NavTimeOffset_T BDSD1NavTimeOffset_T.cpp)
target_link_libraries(BDSD1NavTimeOffset_T gnsstk)
add_test(BDSD1NavTimeOffset_T BDSD1NavTimeOffset_T)
set_property(TEST BDSD1NavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(BDSD2NavAlm_T BDSD2NavAlm_T.cpp)
target_link_libraries(BDSD2NavAlm_T gnsstk)
add_test(BDSD2NavAlm_T BDSD2NavAlm_T)
set_property(TEST BDSD2NavAlm_T PROPERTY LABELS NewNav)

add_executable(BDSD2NavEph_T BDSD2NavEph_T.cpp)
target_link_libraries(BDSD2NavEph_T gnsstk)
add_test(BDSD2NavEph_T BDSD2NavEph_T)
set_property(TEST BDSD2NavEph_T PROPERTY LABELS NewNav)

add_executable(GLOFNavAlm_T GLOFNavAlm_T.cpp)
target_link_libraries(GLOFNavAlm_T gnsstk)
add_test(GLOFNavAlm_T GLOFNavAlm_T)
set_property(TEST GLOFNavAlm_T PROPERTY LABELS NewNav)

add_executable(GLOFNavData_T GLOFNavData_T.cpp)
target_link_libraries(GLOFNavData_T gnsstk)
add_test(GLOFNavData_T GLOFNavData_T)
set_property(TEST GLOFNavData_T PROPERTY LABELS NewNav)

add_executable(GLOFNavEph_T GLOFNavEph_T.cpp)
target_link_libraries(GLOFNavEph_T gnsstk)
add_test(GLOFNavEph_T GLOFNavEph_T)
set_property(TEST GLOFNavEph_T PROPERTY LABELS NewNav)

add_executable(GLOFNavHealth_T GLOFNavHealth_T.cpp)
target_link_libraries(GLOFNavHealth_T gnsstk)
add_test(GLOFNavHealth_T GLOFNavHealth_T)
set_property(TEST GLOFNavHealth_T PROPERTY LABELS NewNav)

add_executable(GLOFNavTimeOffset_T GLOFNavTimeOffset_T.cpp)
target_link_libraries(GLOFNavTimeOffset_T gnsstk)
add_test(GLOFNavTimeOffset_T GLOFNavTimeOffset_T)
set_property(TEST GLOFNavTimeOffset_T PROPERTY LABELS NewNav)

add_executable(GLOFNavUT1TimeOffset_T GLOFNavUT1TimeOffset_T.cpp)
target_link_libraries(GLOFNavUT1TimeOffset_T gnsstk)
add_test(GLOFNavUT1TimeOffset_T GLOFNavUT1TimeOffset_T)
set_property(TEST GLOFNavUT1TimeOffset_T PROPERTY LABELS NewNav)

add_executable(GLOFNavISC_T GLOFNavISC_T.cpp)
target_link_libraries(GLOFNavISC_T gnsstk)
add_test(GLOFNavISC_T GLOFNavISC_T)
set_property(TEST GLOFNavISC_T PROPERTY LABELS NewNav)
