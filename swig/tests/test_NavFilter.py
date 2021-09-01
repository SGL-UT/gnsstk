import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests
import gpstk


class TestNavFilter(unittest.TestCase):
    """Tests for the NewFilter classes"""

    def test_CNav2SanityFilter(self):
        a = gpstk.CNav2SanityFilter()
        self.assertTrue(isinstance(a, gpstk.CNav2SanityFilter))
        self.assertTrue(a.filterName() == "CNav2Sanity")

    def test_CNavCookFilter(self):
        a = gpstk.CNavCookFilter()
        self.assertTrue(isinstance(a, gpstk.CNavCookFilter))
        self.assertTrue(a.filterName() == "Cook")

    def test_CNavFilterData(self):
        a = gpstk.CNavFilterData()
        b = gpstk.CNavMsgSort()
        self.assertTrue(isinstance(a, gpstk.CNavFilterData))
        self.assertTrue(isinstance(b, gpstk.CNavMsgSort))

    def test_CNavCrossSourceFilter(self):
        a = gpstk.CNavCrossSourceFilter()
        self.assertTrue(isinstance(a, gpstk.CNavCrossSourceFilter))
        self.assertTrue(2 == a.getMinIdentical())

    # This is not wrapped right now
    # def test_CNavEmptyFilter(self):
    #     a = gpstk.CNavEmptyFilter()
    #     self.assertTrue(isinstance(a, gpstk.CNavEmptyFilter))
    #     self.assertTrue(a.filterName(), "Empty")

    def test_CNavParityFilter(self):
        a = gpstk.CNavParityFilter()
        self.assertTrue(isinstance(a, gpstk.CNavParityFilter))
        self.assertTrue(a.filterName() == "CRC")

    def test_CNavTOWFilter(self):
        a = gpstk.CNavTOWFilter()
        self.assertTrue(isinstance(a, gpstk.CNavTOWFilter))
        self.assertTrue(a.filterName() == "TOW")

    def test_LNavAlmValFilter(self):
        a = gpstk.LNavAlmValFilter()
        self.assertTrue(isinstance(a, gpstk.LNavAlmValFilter))
        self.assertTrue(a.filterName() == "AlmVal")

    def test_LNavCookFilter(self):
        a = gpstk.LNavCookFilter()
        self.assertTrue(isinstance(a, gpstk.LNavCookFilter))
        self.assertTrue(a.processingDepth() == 0)

    def test_LNavCrossSourceFilter(self):
        a = gpstk.LNavCrossSourceFilter()
        self.assertTrue(isinstance(a, gpstk.LNavCrossSourceFilter))
        self.assertTrue(a.processingDepth() == 1)

    def test_LNavEmptyFilter(self):
        a = gpstk.LNavEmptyFilter()
        self.assertTrue(isinstance(a, gpstk.LNavEmptyFilter))
        self.assertTrue(a.filterName() == "Empty")

    def test_LNavEphMaker(self):
        a = gpstk.LNavEphMaker()
        b = gpstk.LNavEphMakerSort()
        self.assertTrue(isinstance(a, gpstk.LNavEphMaker))
        self.assertTrue(isinstance(b, gpstk.LNavEphMakerSort))
        self.assertTrue(a.processingDepth() == 3)

    def test_LNavFilterData(self):
        a = gpstk.LNavFilterData()
        b = gpstk.LNavMsgSort()
        self.assertTrue(isinstance(a, gpstk.LNavFilterData))
        self.assertTrue(isinstance(b, gpstk.LNavMsgSort))

    def test_LNavOrderFilter(self):
        a = gpstk.LNavOrderFilter()
        b = gpstk.LNavTimeSort()
        self.assertTrue(isinstance(a, gpstk.LNavOrderFilter))
        self.assertTrue(isinstance(b, gpstk.LNavTimeSort))
        self.assertTrue(a.filterName() == "Order")

    def test_LNavParityFilter(self):
        a = gpstk.LNavParityFilter()
        self.assertTrue(isinstance(a, gpstk.LNavParityFilter))
        self.assertTrue(a.filterName() == "Parity")

    def test_LNavTLMHOWFilter(self):
        a = gpstk.LNavTLMHOWFilter()
        self.assertTrue(isinstance(a, gpstk.LNavTLMHOWFilter))
        self.assertTrue(a.filterName() == "TLMHOW")

    def test_NavFilterKey(self):
        a = gpstk.NavFilterKey()
        self.assertTrue(isinstance(a, gpstk.NavFilterKey))
        self.assertTrue(isinstance(str(a), str))

    def test_NavFilterMgr(self):
        a = gpstk.NavFilterMgr()
        self.assertTrue(isinstance(a, gpstk.NavFilterMgr))
        self.assertTrue(isinstance(a.processingDepth(), int))

    def test_NavMsgDataBits(self):
        a = gpstk.NavMsgDataBits()
        self.assertTrue(isinstance(a, gpstk.NavMsgDataBits))
        self.assertTrue(isinstance(a.getBits(1, 0), int))

    # Woud test getBits here but the object constructed with default constructor throws seg fault on it
    def test_NavMsgDataPNB(self):
        a = gpstk.NavMsgDataPNB()
        self.assertTrue(isinstance(a, gpstk.NavMsgDataPNB))

    def test_NavMsgDataWords(self):
        a = gpstk.NavMsgDataWords_30()
        self.assertTrue(isinstance(a, gpstk.NavMsgDataWords_30))
        self.assertTrue(isinstance(a.getBits(1, 0), int))

    def test_NavOrderFilter(self):
        a = gpstk.NavOrderFilter()
        b = gpstk.NavTimeSort()
        self.assertTrue(isinstance(a, gpstk.NavOrderFilter))
        self.assertTrue(isinstance(b, gpstk.NavTimeSort))
        self.assertTrue(isinstance(a.epochInSeconds, int))
        self.assertTrue(a.filterName() == "Order")



if __name__ == '__main__':
    run_unit_tests()
