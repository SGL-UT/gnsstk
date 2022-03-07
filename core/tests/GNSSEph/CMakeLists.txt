#Tests for GNSSEph Classes

add_executable(AlmOrbit_T AlmOrbit_T.cpp)
target_link_libraries(AlmOrbit_T gnsstk)
add_test(NAME GNSSEph_AlmOrbit COMMAND $<TARGET_FILE:AlmOrbit_T>)

add_executable(BrcKeplerOrbit_T BrcKeplerOrbit_T.cpp)
target_link_libraries(BrcKeplerOrbit_T gnsstk)
add_test(NAME GNSSEph_BrcKeplerOrbit COMMAND $<TARGET_FILE:BrcKeplerOrbit_T>)

add_executable(BrcClockCorrection_T BrcClockCorrection_T.cpp)
target_link_libraries(BrcClockCorrection_T gnsstk)
add_test(NAME GNSSEph_BrcClockCorrection COMMAND $<TARGET_FILE:BrcClockCorrection_T>)

add_executable(EngAlmanac_T EngAlmanac_T.cpp)
target_link_libraries(EngAlmanac_T gnsstk)
add_test(NAME GNSSEph_EngAlmanac COMMAND $<TARGET_FILE:EngAlmanac_T>)

add_executable(EngEphemeris_T EngEphemeris_T.cpp)
target_link_libraries(EngEphemeris_T gnsstk)
add_test(NAME GNSSEph_EngEphemeris COMMAND $<TARGET_FILE:EngEphemeris_T>)

add_executable(EngNav_T EngNav_T.cpp)
target_link_libraries(EngNav_T gnsstk)
add_test(NAME GNSSEph_EngNav COMMAND $<TARGET_FILE:EngNav_T>)

add_executable(EphemerisRange_T EphemerisRange_T.cpp)
target_link_libraries(EphemerisRange_T gnsstk)
add_test(NAME GNSSEph_EphemerisRange COMMAND $<TARGET_FILE:EphemerisRange_T>)

add_executable(NavID_T NavID_T.cpp)
target_link_libraries(NavID_T gnsstk)
add_test(NAME GNSSEph_NavID COMMAND $<TARGET_FILE:NavID_T>)

add_executable(PackedNavBits_T PackedNavBits_T.cpp)
target_link_libraries(PackedNavBits_T gnsstk)
add_test(NAME GNSSEph_PackedNavBits COMMAND $<TARGET_FILE:PackedNavBits_T>)
set_property(TEST GNSSEph_PackedNavBits PROPERTY LABELS GNSSEph PackedNavBits)

add_executable(SP3SatID_T SP3SatID_T.cpp)
target_link_libraries(SP3SatID_T gnsstk)
add_test(NAME GNSSEph_SP3SatID COMMAND $<TARGET_FILE:SP3SatID_T>)

add_executable(XvtStore_T XvtStore_T.cpp)
target_link_libraries(XvtStore_T gnsstk)
add_test(NAME GNSSEph_XvtStore COMMAND $<TARGET_FILE:XvtStore_T>)
