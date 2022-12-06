#!/usr/bin/env python

import unittest, sys, os
try:
    from collections.abc import Hashable
except ImportError:
    # Python2 has Hashable in a different location.
    from collections import Hashable

sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args, run_unit_tests
import gnsstk

class EnumConversion_test(unittest.TestCase):
    """
    Overkill?  Maybe... but it does catch swig errors related to Enumeration passing.
    """

    def test_SatelliteSystem(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.SatelliteSystem()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.SatelliteSystem(2), gnsstk.SatelliteSystem.Galileo, msg='int Galileo')
        self.assertEqual(gnsstk.SatelliteSystem(1), gnsstk.SatelliteSystem.GPS, msg='int GPS')
        self.assertEqual(gnsstk.SatelliteSystem(gnsstk.SatelliteSystem.GPS), gnsstk.SatelliteSystem.GPS, msg='gnsstk enum GPS')
        self.assertEqual(gnsstk.SatelliteSystem(gnsstk.SatelliteSystem.Galileo), gnsstk.SatelliteSystem.Galileo, msg='gnsstk enum Galileo')
        self.assertEqual(gnsstk.asSatelliteSystem('GPS'), gnsstk.SatelliteSystem.GPS, msg='str GPS')
        self.assertEqual(gnsstk.asSatelliteSystem('Galileo'), gnsstk.SatelliteSystem.Galileo, msg='str Galileo')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asSatelliteSystem(str(gnsstk.SatelliteSystem.Glonass)), gnsstk.SatelliteSystem.Glonass, msg='str enum Glonass')

    def test_CarrierBand(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.CarrierBand()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.CarrierBand(4), gnsstk.CarrierBand.L2, msg='int L2')
        self.assertEqual(gnsstk.CarrierBand(2), gnsstk.CarrierBand.L1L2, msg='int L1L2')
        self.assertEqual(gnsstk.CarrierBand(gnsstk.CarrierBand.L2), gnsstk.CarrierBand.L2, msg='gnsstk enum GPS')
        self.assertEqual(gnsstk.CarrierBand(gnsstk.CarrierBand.L1L2), gnsstk.CarrierBand.L1L2, msg='gnsstk enum L1L2')
        self.assertEqual(gnsstk.asCarrierBand('L2'), gnsstk.CarrierBand.L2, msg='str GPS')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asCarrierBand(str(gnsstk.CarrierBand.L1L2)), gnsstk.CarrierBand.L1L2, msg='str enum str L1L2')

    def test_TrackingCode(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.TrackingCode()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.TrackingCode(4), gnsstk.TrackingCode.Y, msg='int Y')
        self.assertEqual(gnsstk.TrackingCode(2), gnsstk.TrackingCode.CA, msg='int CA')
        self.assertEqual(gnsstk.TrackingCode(gnsstk.TrackingCode.Y), gnsstk.TrackingCode.Y, msg='gnsstk enum Y')
        self.assertEqual(gnsstk.TrackingCode(gnsstk.TrackingCode.CA), gnsstk.TrackingCode.CA, msg='gnsstk enum CA')
        self.assertEqual(gnsstk.asTrackingCode('Y'), gnsstk.TrackingCode.Y, msg='str Y')
        self.assertEqual(gnsstk.asTrackingCode('CA'), gnsstk.TrackingCode.CA, msg='str CA')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asTrackingCode(str(gnsstk.TrackingCode.CA)), gnsstk.TrackingCode.CA, msg='str enum str CA')

    def test_ObservationType(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.ObservationType()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.ObservationType(4), gnsstk.ObservationType.Doppler, msg='int Doppler')
        self.assertEqual(gnsstk.ObservationType(2), gnsstk.ObservationType.Range, msg='int Range')
        self.assertEqual(gnsstk.ObservationType(gnsstk.ObservationType.Range), gnsstk.ObservationType.Range, msg='gnsstk enum Range')
        self.assertEqual(gnsstk.ObservationType(gnsstk.ObservationType.Doppler), gnsstk.ObservationType.Doppler, msg='gnsstk enum Doppler')
        self.assertEqual(gnsstk.asObservationType('Range'), gnsstk.ObservationType.Range, msg='str Range')
        self.assertEqual(gnsstk.asObservationType('Doppler'), gnsstk.ObservationType.Doppler, msg='str Doppler')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asObservationType(str(gnsstk.ObservationType.Range)), gnsstk.ObservationType.Range, msg='str enum str Range')

    def test_NavType(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.NavType()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.NavType(6), gnsstk.NavType.GPSMNAV, msg='int GPSMNAV')
        self.assertEqual(gnsstk.NavType(4), gnsstk.NavType.GPSCNAVL5, msg='int GPS')
        self.assertEqual(gnsstk.NavType(gnsstk.NavType.GPSCNAVL5), gnsstk.NavType.GPSCNAVL5, msg='gnsstk enum GPSCNAVL5')
        self.assertEqual(gnsstk.NavType(gnsstk.NavType.GPSMNAV), gnsstk.NavType.GPSMNAV, msg='gnsstk enum GPSMNAV')

        self.assertEqual(gnsstk.asNavType('GPS_CNAV_L5'), gnsstk.NavType.GPSCNAVL5, msg='str GPSCNAVL5')
        self.assertEqual(gnsstk.asNavType('GPS_MNAV'), gnsstk.NavType.GPSMNAV, msg='str GPSMNAV')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asNavType(str(gnsstk.NavType.GPSCNAVL5)), gnsstk.NavType.GPSCNAVL5, msg='str enum str GPSCNAVL5')

    def test_TimeSystem(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.TimeSystem()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.TimeSystem(4), gnsstk.TimeSystem.GAL, msg='int GAL')
        self.assertEqual(gnsstk.TimeSystem(2), gnsstk.TimeSystem.GPS, msg='int GPS')
        self.assertEqual(gnsstk.TimeSystem(gnsstk.TimeSystem.GPS), gnsstk.TimeSystem.GPS, msg='gnsstk enum GPS')
        self.assertEqual(gnsstk.TimeSystem(gnsstk.TimeSystem.GAL), gnsstk.TimeSystem.GAL, msg='gnsstk enum GAL')
        self.assertEqual(gnsstk.asTimeSystem('GPS'), gnsstk.TimeSystem.GPS, msg='str GPS')
        self.assertEqual(gnsstk.asTimeSystem('GAL'), gnsstk.TimeSystem.GAL, msg='str GAL')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asTimeSystem(str(gnsstk.TimeSystem.GPS)), gnsstk.TimeSystem.GPS, msg='str GPS')

    def test_ReferenceFrame(self):
        with self.assertRaises(TypeError):
            _ = gnsstk.ReferenceFrame()
            self.fail("No default constructor")
        self.assertEqual(gnsstk.ReferenceFrame(4), gnsstk.ReferenceFrame.WGS84G1150, msg='int WGS84G1150')
        self.assertEqual(gnsstk.ReferenceFrame(2), gnsstk.ReferenceFrame.WGS84G730, msg='int WGS84G730')
        self.assertEqual(gnsstk.ReferenceFrame(gnsstk.ReferenceFrame.WGS84G1150), gnsstk.ReferenceFrame.WGS84G1150, msg='gnsstk enum WGS84G1150')
        self.assertEqual(gnsstk.ReferenceFrame(gnsstk.ReferenceFrame.WGS84G730), gnsstk.ReferenceFrame.WGS84G730, msg='gnsstk enum WGS84G730')
        self.assertEqual(gnsstk.asReferenceFrame('WGS84(G1150)'), gnsstk.ReferenceFrame.WGS84G1150, msg='str WGS84G1150')
        self.assertEqual(gnsstk.asReferenceFrame('WGS84(G730)'), gnsstk.ReferenceFrame.WGS84G730, msg='str WGS84G730')
        # TODO: Odd that enum -> str -> enum doesn't work.
        #self.assertEqual(gnsstk.asReferenceFrame(str(gnsstk.ReferenceFrame.WGS84G730)), gnsstk.ReferenceFrame.WGS84G730, msg='str WGS84G730')


class SatID_test(unittest.TestCase):
    def test_validity(self):
        s = gnsstk.SatID(1, gnsstk.SatelliteSystem.GPS)
        self.assertTrue(s.isValid())

    def test_invalid(self):
        s = gnsstk.SatID()  # creates an invalid SatID (id=-1)
        self.assertFalse(s.isValid())

    def test_str(self):
        a = gnsstk.SatID(3, gnsstk.SatelliteSystem.Glonass)
        self.assertEqual('GLONASS 3', str(a))

        b = gnsstk.SatID(1, gnsstk.SatelliteSystem.LEO)
        self.assertEqual('LEO 1', str(b))

        c = gnsstk.SatID(4)  # optional arg should result in a wildcard system
        self.assertEqual('* 4', str(c))

    def test_hashability(self):
        # Can we use SatID, etc as a dict-key or in a set.
        # Verifies that the SWIG python glue is correct.
        a = gnsstk.SatID(3, gnsstk.SatelliteSystem.Glonass)
        b = gnsstk.ObsID()
        c = gnsstk.NavID()
        d = gnsstk.RinexSatID()
        e = gnsstk.RinexObsID()
        self.assertTrue(isinstance(a, Hashable))
        self.assertTrue(isinstance(b, Hashable))
        self.assertTrue(isinstance(c, Hashable))
        self.assertTrue(isinstance(d, Hashable))
        self.assertTrue(isinstance(e, Hashable))


class Triple_test(unittest.TestCase):
    def test_copy_constructor(self):
        t = gnsstk.Triple(1.0, 2.0, 3.0)
        u = gnsstk.Triple(t)
        v = gnsstk.Triple(1.0, 2.0, 3.0)
        self.assertTrue(t == u)
        self.assertTrue(u == v)

    def test_access(self):
        t = gnsstk.Triple(1.5, 2.0, -3.0)
        self.assertEqual(1.5, t[0])
        self.assertEqual(2.0, t[1])
        self.assertEqual(-3.0, t[2])

    def test_operators(self):
        a = gnsstk.Triple(1.0, 2.0, 4.0)
        b = gnsstk.Triple(5.0, 6.0, 5.0)
        #      +   --------------
        c = gnsstk.Triple(6.0, 8.0, 9.0)
        self.assertEqual(c, a + b)

    def test_methods(self):
        t = gnsstk.Triple(1.5, 2.0, -3.0)
        u = gnsstk.Triple(10.0, 5.0, 2.0)
        self.assertEqual(15.25, t.dot(t))

        expected = gnsstk.Triple(4.0, 6.0, 8.0)
        self.assertEqual(expected, gnsstk.Triple(2.0, 3.0, 4.0).scale(2.0))

        self.assertAlmostEqual(3.905124837953327, t.mag())
        self.assertAlmostEqual(5.345455469884399, t.elvAngle(u))
        self.assertAlmostEqual(0.42837471052408865, t.cosVector(u))


class Position_test(unittest.TestCase):
    def test(self):
        p1 = gnsstk.Position(1.5, 6.2, 3.5)   # constructor with 3 floats
        p2 = gnsstk.Position([1.0, 1.8, 0.5]) # constructor with a list-like of 3 floats
        self.assertAlmostEqual(5.348831648126533, gnsstk.range(p1, p2))

    def test_spherical_cartesian(self):
        orig = gnsstk.Triple(45.0, 30.0, 12.0)
        p = gnsstk.Position.convertSphericalToCartesian(orig)
        q = gnsstk.Position.convertCartesianToSpherical(p)
        self.assertAlmostEqual(45.0, q[0], places=4)
        self.assertAlmostEqual(30.0, q[1], places=4)
        self.assertAlmostEqual(12.0, q[2], places=4)

    def test_cartesian_geodetic(self):
        a = gnsstk.PZ90Ellipsoid().a()
        eccSq = gnsstk.PZ90Ellipsoid().eccSquared()
        orig = gnsstk.Position(100000.0, 20000.0, 30000.0)
        p = gnsstk.Position.convertCartesianToGeodetic(orig, a, eccSq)
        q = gnsstk.Position.convertGeodeticToCartesian(p, a, eccSq)
        self.assertAlmostEqual(25.33498527029081, p[0], places=4)
        self.assertAlmostEqual(11.30993247402015, p[1], places=4)
        self.assertAlmostEqual(-6269217.08416736, p[2], places=4)
        self.assertAlmostEqual(99999.26269737557, q[0], places=4)
        self.assertAlmostEqual(19999.85253947465, q[1], places=4)
        self.assertAlmostEqual(29999.83821484564, q[2], places=4)

    def test_geocentric_geodetic(self):
        a = gnsstk.PZ90Ellipsoid().a()
        eccSq = gnsstk.PZ90Ellipsoid().eccSquared()
        orig = gnsstk.Position(40.0, 100.0, 2.5e5, gnsstk.Position.Geocentric)
        p = gnsstk.Position.convertGeocentricToGeodetic(orig, a, eccSq)
        q = gnsstk.Position.convertGeodeticToGeocentric(p, a, eccSq)
        self.assertAlmostEqual(44.90696703221949, p[0], places=4)
        self.assertAlmostEqual(100.0, p[1], places=4)
        self.assertAlmostEqual(-6118405.153409380, p[2], places=4)
        self.assertAlmostEqual(40.00000265961031, q[0], places=4)
        self.assertAlmostEqual(100.0, q[1], places=4)
        self.assertAlmostEqual(249998.49546297366, q[2], places=4)

    def test_cartesian_geocentric(self):
        orig = gnsstk.Triple(4000.0, 5000.0, 7000.0)
        p = gnsstk.Position.convertCartesianToGeocentric(orig)
        q = gnsstk.Position.convertGeocentricToCartesian(p)
        self.assertAlmostEqual(47.54984445710891, p[0], places=4)
        self.assertAlmostEqual(51.34019174590962, p[1], places=4)
        self.assertAlmostEqual(9486.832980505136, p[2], places=4)
        self.assertAlmostEqual(orig[0], q[0], places=4)
        self.assertAlmostEqual(orig[1], q[1], places=4)
        self.assertAlmostEqual(orig[2], q[2], places=4)

    def test_functions(self):
        system = gnsstk.Position.Cartesian
        ell = gnsstk.PZ90Ellipsoid()
        frame = gnsstk.RefFrame(gnsstk.RefFrameRlz.PZ90Y2007)
        p = gnsstk.Position(10000.0, 150000.0, 200000.0, system, ell, frame)
        q = gnsstk.Position(20000.0, 160000.0, 190000.0, system, ell, frame)
        self.assertAlmostEqual(1.32756277187, q.elevation(p))
        self.assertAlmostEqual(86.18592516570916, p.getPhi())
        self.assertAlmostEqual(57.5141089193572, p.geodeticLatitude())
        self.assertAlmostEqual(10000.0, p.X())

    def test_helpers(self):
        p = gnsstk.cartesian(100, 200, 300)
        self.assertEqual(gnsstk.Position.Cartesian, p.getCoordinateSystem())
        p = gnsstk.spherical(45, 60, 100000, model=gnsstk.PZ90Ellipsoid())
        self.assertEqual(gnsstk.Position.Spherical, p.getCoordinateSystem())
        p = gnsstk.geodetic(frame=gnsstk.RefFrame(gnsstk.RefFrameRlz.WGS84G0))
        self.assertEqual(gnsstk.Position.Geodetic, p.getCoordinateSystem())
        p = gnsstk.geocentric(latitude=60, radius=10000)
        self.assertEqual(gnsstk.Position.Geocentric, p.getCoordinateSystem())


class GPS_URA_test(unittest.TestCase):
    def test(self):
        self.assertEqual(15, gnsstk.SV_ACCURACY_GPS_MAX_INDEX_VALUE)
        self.assertEqual(9, gnsstk.accuracy2ura(100.1))
        self.assertEqual(0.0, gnsstk.sv_accuracy_gps_min_index(0))
        self.assertEqual(2.4, gnsstk.sv_accuracy_gps_min_index(1))
        self.assertRaises(gnsstk.IndexOutOfBoundsException, gnsstk.sv_accuracy_gps_min_index, 16)


class ObsID_test(unittest.TestCase):
    def test(self):
        o1 = gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.Any, gnsstk.TrackingCode.E1A)
        self.assertEqual('AnyBand GALA pseudorange', str(o1))


class std_template_test(unittest.TestCase):
    def test_vector(self):
        v = gnsstk.std_vector_int()
        v.push_back(5)
        v.push_back(3)
        v.push_back(10)
        self.assertEqual(5, v[0])
        self.assertEqual(3, v[1])
        self.assertEqual(10, v[2])
        v = gnsstk.std_vector_double()
        v.push_back(1.5)
        v.push_back(2.5)
        self.assertEqual(1.5, v[0])
        self.assertEqual(2.5, v[1])
        self.assertEqual(2, len(v))

    def test_vector_iter(self):
        v = gnsstk.std_vector_int()
        v.push_back(5)
        v.push_back(3)
        v.push_back(10)
        i = 0
        for x in v:
            self.assertEqual(v[i], x)
            i += 1

    def test_vector_conversions(self):
        def same_seq(seq1, seq2):
            self.assertEqual(len(seq1), len(seq2))
            for i in range(len(seq1)):
                self.assertEqual(seq1[i], seq2[i])

        list = [1.1, 2.2, 3.3]
        v = gnsstk.seqToVector(list)
        self.assertIsInstance(v, gnsstk.std_vector_double)
        same_seq(list, v)

        list = [1.1, 2.2, 3.3]
        v = gnsstk.seqToVector(list, outtype='std_vector_double')
        self.assertIsInstance(v, gnsstk.std_vector_double)
        same_seq(list, v)

        list = ['bar!', 'foo?']
        v = gnsstk.seqToVector(list)
        self.assertIsInstance(v, gnsstk.std_vector_string)
        same_seq(list, v)

        v = gnsstk.std_vector_int()
        v.push_back(3)
        v.push_back(5)
        list = gnsstk.vectorToSeq(v)
        same_seq(list, v)

        list = [1.1, 2.2, 3.3]
        self.assertRaises(TypeError, gnsstk.seqToVector, list, 'vector_doesnotexist')

        list = [1, 2.2, 'c']  # mismatching types not allowed
        self.assertRaises(TypeError, gnsstk.seqToVector, list)

    def test_map(self):
        map = gnsstk.std_map_int_char()
        map[1] = 'A'
        map[100] = 'z'
        self.assertEqual('A', map[1])
        self.assertEqual('z', map[100])
        map = gnsstk.std_map_string_int()
        map['key1'] = 123
        map['key2'] = 321
        self.assertEqual(123, map['key1'])
        self.assertEqual(321, map['key2'])

    def test_map_iter(self):
        map = gnsstk.std_map_int_string()
        map[5] = 'five'
        map[6] = 'six'
        list = []
        for key in map:
            list.append(key)
        self.assertEqual([5,6], sorted(list))

    def test_map_len(self):
        map = gnsstk.std_map_int_string()
        map[5] = 'five'
        map[6] = 'six'
        self.assertEqual(2, len(map))

    def test_map_conversions(self):
        def same(a, b):
            self.assertEqual(len(a), len(b))
            for x in a:
                self.assertEqual(a[x], b[x])

        d = {1: 'A', 2: 'B', 3: 'C'}
        m = gnsstk.dictToMap(d)
        same(d, m)
        self.assertIsInstance(m, gnsstk.std_map_int_string)
        same(d, gnsstk.mapToDict(m))

        d = {'A': 1.1, 'B': 2.2, 'C': 3.3}
        m = gnsstk.dictToMap(d)
        same(d, m)
        self.assertIsInstance(m, gnsstk.std_map_string_double)
        same(d, gnsstk.mapToDict(m))

        d = {'A': 1, 'B': 1.1}
        self.assertRaises(TypeError, gnsstk.dictToMap, d)


class convhelp_test(unittest.TestCase):
    def test(self):
        self.assertAlmostEqual(32.0, gnsstk.cel2far(0.0))
        self.assertAlmostEqual(0.0, gnsstk.far2cel(32.0))
        self.assertAlmostEqual(121.1, gnsstk.cel2far(49.5))


class Xvt_test(unittest.TestCase):
    def test(self):
        data = gnsstk.Xvt()
        data.x = gnsstk.Triple(1000.0, 2000.0, 1500.0)
        data.v = gnsstk.Triple(50.0, 25.0, -500.0)
        data.clkbias = 0.0001
        data.clkdrift = 0.05
        data.relcorr = 0.83
        data.frame = gnsstk.RefFrame(gnsstk.RefFrameRlz.WGS84G2139);
        self.assertAlmostEqual(0.0001, data.getClockBias())

        expected = 1.446445072869704e-11
        self.assertAlmostEqual(expected, data.computeRelativityCorrection())

        expected = ('x:(1000, 2000, 1500), v:(50, 25, -500), clk bias:0.0001, '
                    'clk drift:0.05, relcorr:1.44645e-11')
        self.assertEqual(expected, str(data))

class Tides_test(unittest.TestCase):
    def test_pole_tides(self):
        t = gnsstk.CivilTime(2000).toCommonTime()
        p = gnsstk.Position(1000.0, 2000.0, 3000.0)
        x = 5.0
        y = 10.0
        trip = gnsstk.poleTides(t, p, x, y)
        self.assertAlmostEqual(-0.03128457731297798, trip[0])

class Expression_test(unittest.TestCase):
    def test_class(self):
        e = gnsstk.Expression('1 + 1')
        self.assertAlmostEqual(2.0, e.evaluate())

        e = gnsstk.Expression('1.12*(4.0/2.0 - 0.1)')
        self.assertAlmostEqual(2.128, e.evaluate())

        e = gnsstk.Expression('1 + 2*x')
        e.set('x', 5.5)
        self.assertAlmostEqual(12.0, e.evaluate())

        e = gnsstk.Expression('L1 + L2')
        e.setGPSConstants()
        self.assertAlmostEqual(2803020000.0, e.evaluate())

    def test_function(self):
        self.assertAlmostEqual(2.0, gnsstk.eval('1 + 1'))
        self.assertAlmostEqual(2.128, gnsstk.eval('1.12*(4.0/2.0 - 0.1)'))
        self.assertAlmostEqual(12.0, gnsstk.eval('1 + 2*x', x=5.5))
        self.assertAlmostEqual(2803020000.0, gnsstk.eval('L1 + L2'))

        e = gnsstk.Expression('1 + 2*x')
        self.assertAlmostEqual(12.0, gnsstk.eval(e, x=5.5))



if __name__ == '__main__':
    run_unit_tests()
