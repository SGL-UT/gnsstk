#Tests for GNSSEph Classes

add_executable(AlmOrbit_T AlmOrbit_T.cpp)
target_link_libraries(AlmOrbit_T gnsstk)
add_test(GNSSEph_AlmOrbit AlmOrbit_T)

add_executable(BrcKeplerOrbit_T BrcKeplerOrbit_T.cpp)
target_link_libraries(BrcKeplerOrbit_T gnsstk)
add_test(GNSSEph_BrcKeplerOrbit BrcKeplerOrbit_T)

add_executable(BrcClockCorrection_T BrcClockCorrection_T.cpp)
target_link_libraries(BrcClockCorrection_T gnsstk)
add_test(GNSSEph_BrcClockCorrection BrcClockCorrection_T)

add_executable(EngAlmanac_T EngAlmanac_T.cpp)
target_link_libraries(EngAlmanac_T gnsstk)
add_test(GNSSEph_EngAlmanac EngAlmanac_T)

add_executable(EngEphemeris_T EngEphemeris_T.cpp)
target_link_libraries(EngEphemeris_T gnsstk)
add_test(GNSSEph_EngEphemeris EngEphemeris_T)

add_executable(EngNav_T EngNav_T.cpp)
target_link_libraries(EngNav_T gnsstk)
add_test(GNSSEph_EngNav EngNav_T)

add_executable(EphemerisRange_T EphemerisRange_T.cpp)
target_link_libraries(EphemerisRange_T gnsstk)
add_test(GNSSEph_EphemerisRange EphemerisRange_T)

add_executable(GalEphemeris_T GalEphemeris_T.cpp)
target_link_libraries(GalEphemeris_T gnsstk)
add_test(GNSSEph_GalEphemeris GalEphemeris_T)

add_executable(NavID_T NavID_T.cpp)
target_link_libraries(NavID_T gnsstk)
add_test(GNSSEph_NavID NavID_T)

add_executable(PackedNavBits_T PackedNavBits_T.cpp)
target_link_libraries(PackedNavBits_T gnsstk)
add_test(GNSSEph_PackedNavBits PackedNavBits_T)
set_property(TEST GNSSEph_PackedNavBits PROPERTY LABELS GNSSEph PackedNavBits)

add_executable(SP3SatID_T SP3SatID_T.cpp)
target_link_libraries(SP3SatID_T gnsstk)
add_test(GNSSEph_SP3SatID SP3SatID_T)

add_executable(XvtStore_T XvtStore_T.cpp)
target_link_libraries(XvtStore_T gnsstk)
add_test(GNSSEph_XvtStore XvtStore_T)
