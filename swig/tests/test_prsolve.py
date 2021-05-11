import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests
import gpstk

class TestPRSolve(unittest.TestCase):

     def test_gnss_vector(self):
        # Allowed GNSS for passing data to PRSolution::RAIMCompute().
        allowed_GNSS = [
            gpstk.SatelliteSystem.GPS,
            gpstk.SatelliteSystem.Galileo,
            gpstk.SatelliteSystem.Glonass,
            gpstk.SatelliteSystem.Geosync,
            gpstk.SatelliteSystem.BeiDou,
            gpstk.SatelliteSystem.QZSS,
            gpstk.SatelliteSystem.IRNSS,
        ]

        prs = gpstk.PRSolution()
        prs.allowedGNSS = gpstk.vector_GNSS()
        for g in allowed_GNSS:
            prs.allowedGNSS.append(g)

        self.assertEqual(len(prs.allowedGNSS), len(allowed_GNSS))

     def test_gnss_vector_2(self):
         # Allowed GNSS for passing data to PRSolution::RAIMCompute().
         allowed_GNSS = [
             gpstk.SatelliteSystem.GPS,
             gpstk.SatelliteSystem.Galileo,
             gpstk.SatelliteSystem.Glonass,
             gpstk.SatelliteSystem.Geosync,
             gpstk.SatelliteSystem.BeiDou,
             gpstk.SatelliteSystem.QZSS,
             gpstk.SatelliteSystem.IRNSS,
         ]

         vector = gpstk.vector_GNSS(allowed_GNSS)

         self.assertEqual(len(vector), len(allowed_GNSS))

if __name__ == '__main__':
    unittest.main()
