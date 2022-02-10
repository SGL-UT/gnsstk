#!/usr/bin/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk

class TestTropModelsExist(unittest.TestCase):
    def test_TropModelsExist(self):
        tropMissing = 'Trop Model is missing'
        
        self.assertTrue(hasattr(gnsstk, 'GCATTropModel'),     tropMissing)
        self.assertTrue(hasattr(gnsstk, 'GGHeightTropModel'), tropMissing)
        self.assertTrue(hasattr(gnsstk, 'GGTropModel'),       tropMissing)
        self.assertTrue(hasattr(gnsstk, 'MOPSTropModel'),     tropMissing)
        self.assertTrue(hasattr(gnsstk, 'NBTropModel'),       tropMissing)
        self.assertTrue(hasattr(gnsstk, 'NeillTropModel'),    tropMissing)
        self.assertTrue(hasattr(gnsstk, 'SaasTropModel'),     tropMissing)
        self.assertTrue(hasattr(gnsstk, 'SimpleTropModel'),   tropMissing)
        self.assertTrue(hasattr(gnsstk, 'ZeroTropModel'),     tropMissing)


class TestTropModelsValid(unittest.TestCase):
    def test_TropModelsValid(self):
        trop_list = ['GCAT', 'GG', 'GGHeight', 'MOPS', 'NB', 'Saas', 'Simple', 'Zero']
        for tropName in trop_list:

            # check if the reference file exists in gnsstk/data/TropModel_*.exp
            self.fname = os.path.join(os.path.dirname(__file__), '../../data/TropModel_'+tropName+'.exp')
            self.assertTrue(os.path.exists(self.fname), 'data path not found')

            # check that the data matches between the current run and the test file
            trop_data, pt_dryz, pt_wetz = self.run_test(tropName)
            data_list, dry_zenith, wet_zenith = self.read_data(tropName)
            self.assertEqual(trop_data, data_list)
            self.assertEqual(pt_dryz, dry_zenith)
            self.assertEqual(pt_wetz, wet_zenith)


    def setup_trop(self, tropName):
        # from Tropdump.cpp

        # set up the trop model from the input string
        if   tropName == "Simple"  : pTrop = gnsstk.SimpleTropModel()
        elif tropName == "Zero"    : pTrop = gnsstk.ZeroTropModel()
        elif tropName == "Saas"    : pTrop = gnsstk.SaasTropModel()
        elif tropName == "NB"      : pTrop = gnsstk.NBTropModel()
        elif tropName == "GG"      : pTrop = gnsstk.GGTropModel()
        elif tropName == "GGHeight": pTrop = gnsstk.GGHeightTropModel()
        #elif tropName == "Neill"   : pTrop = gnsstk.NeillTropModel() # doesn't have a matching output file
        #elif tropName == "Global"  : pTrop = gnsstk.GlobalTropModel() # doesn't have a swig wrapper
        elif tropName == "GCAT"    : pTrop = gnsstk.GCATTropModel()
        elif tropName == "MOPS"    : pTrop = gnsstk.MOPSTropModel()
        else:
            print(" Warning - unknown trop model: " + tropName+ "; use Simple")
            tropName = "Simple"
            pTrop = gnsstk.SimpleTropModel()

        return pTrop


    def set_defaults(self, pTrop):
        # from Tropdump.cpp
        # trop model and weather
        # tropstr = "Global,20,1013,50" # use tropName instead
        Temp  = 20.0
        Press = 1013.
        Humid = 50.
        elevmin = 3.0
        elevmax = 90.0
        delev   = 1.5
        # position
        #refPosstr = "-740376.5046,-5457019.3545,3207315.7299" # ARL:UT, in ECEF
        refPosstr_x =  -740376.5046
        refPosstr_y = -5457019.3545
        refPosstr_z =  3207315.7299
        #timestr = "2017,103,0.0"
        doy = 103

        # set up weather
        pTrop.setWeather(Temp,Press,Humid)

        # set up position
        refPos = gnsstk.Position()
        refPos.setECEF(refPosstr_x, refPosstr_y, refPosstr_z)
        pTrop.setReceiverHeight(refPos.getHeight())
        pTrop.setReceiverLatitude(refPos.getGeodeticLatitude())
        pTrop.setReceiverLongitude(refPos.getLongitude())

        # set up doy
        pTrop.setDayOfYear(doy)

        return pTrop, elevmin, elevmax, delev


    def run_test(self, tropName):

        pTrop = self.setup_trop(tropName)
        pTrop, elevmin, elevmax, delev = self.set_defaults(pTrop)

        header = ["Name", "elev", "drymap", "drytotal", "wetmap", "wettotal", "total(m)"]

        trop_data = []

        elev = elevmin
        while elev <= elevmax:
            pt_name = pTrop.name()
            pt_elev = '{:2.2f}'.format(elev)
            pt_drym = '{:2.4f}'.format(pTrop.dry_mapping_function(elev))
            pt_dryt = '{:2.4f}'.format(pTrop.dry_zenith_delay()*pTrop.dry_mapping_function(elev))
            pt_wetm = '{:2.4f}'.format(pTrop.wet_mapping_function(elev))
            pt_wett = '{:2.4f}'.format(pTrop.wet_zenith_delay()*pTrop.wet_mapping_function(elev))
            pt_totm = '{:2.4f}'.format(pTrop.correction(elev))

            pt_data = [pt_name, pt_elev, pt_drym, pt_dryt, pt_wetm, pt_wett, pt_totm]
            data_dict = dict(zip(header, pt_data))
            trop_data.append(data_dict)

            elev += delev

        pt_dryz = '{:2.4f}'.format(pTrop.dry_zenith_delay())
        pt_wetz = '{:2.4f}'.format(pTrop.wet_zenith_delay())

        return trop_data, pt_dryz, pt_wetz


    def read_data(self, tropName):

        data_list = []
        header = ["Name", "elev", "drymap", "drytotal", "wetmap", "wettotal", "total(m)"]
        dry_zenith = float('nan')
        wet_zenith = float('nan')

        with open(self.fname, 'r') as datafile:
            data = datafile.readlines()
            for dline in data:
                line = dline.split()
                if line[0] != tropName: continue
                if 'zenith' in dline:
                    dry_zenith = line[8]
                    wet_zenith = line[11]
                    continue
                data_dict = dict(zip(header, line))
                data_list.append(data_dict)
        return data_list, dry_zenith, wet_zenith


if __name__ == '__main__':
    run_unit_tests()
