mixed.06n:
        This is a straight copy of the contents of RINEX 3.04 Appendix 11.

NewNavToRinex_bds2_a.exp:
        Expected results of NewNavToRinex_T round-trip test with
        test_input_rinex3_nav_BeiDou.14n.  Note that it differs slightly from
        the source because of differences in how the numbers are printed.
        This has been confirmed to be within tolerance via rnwdiff.

NewNavToRinex_bds1_b.exp:
        Expected results of NewNavToRinex_T round-trip test with
        rinnav_BDS_D1_test.14n.  Note that it differs slightly from
        the source because of differences in how the numbers are printed.
        This has been confirmed to be within tolerance via rnwdiff.

NewNavToRinex_bds2_b.exp:
        Expected results of NewNavToRinex_T round-trip test with
        rinnav_BDS_D2_test.14n.  Note that it differs slightly from
        the source because of differences in how the numbers are printed.
        This has been confirmed to be within tolerance via rnwdiff.

NewNavToRinex_mixed_gal.exp:
        Expected results of NewNavToRinex_T round-trip test with
        test_input_rinex3_nav_gal.20n.  Note that it differs slightly from
        the source because of differences in how the numbers are printed.
        This has been confirmed to be within tolerance via rnwdiff.

NewNavToRinex_mixed_glo.exp:
        Expected results of NewNavToRinex_T round-trip test with
        mixed.06n.  Note that it differs slightly from mixed.06n which
        uses C/C++ style numbers, while gnsstk defaults to the legacy
        Fortran style numbers.

rinnav_BDS_D1_test.14n:
        Test input file with bogus BDS D1 data whose primary purpose is to
        confirm the immutability of the data over the course of
        processing (e.g. in round-trip tests).  By making each
        (non-spare) datum non-zero and unique.

rinnav_BDS_D2_test.14n:
        Test input file with bogus BDS D2 data whose primary purpose is to
        confirm the immutability of the data over the course of
        processing (e.g. in round-trip tests).  By making each
        (non-spare) datum non-zero and unique.

test_input_rinex3_nav_gal.20n:
        Edited navigation data containing one GPS and several Galileo I/Nav
        and F/Nav ephemerides.
