import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk


class TestNavFilter(unittest.TestCase):
    """Tests for the NewFilter classes"""

    def test_CNav2SanityFilter(self):
        a = gnsstk.CNav2SanityFilter()
        self.assertTrue(isinstance(a, gnsstk.CNav2SanityFilter))
        self.assertTrue(a.filterName() == "CNav2Sanity")

    def test_CNavCookFilter(self):
        a = gnsstk.CNavCookFilter()
        self.assertTrue(isinstance(a, gnsstk.CNavCookFilter))
        self.assertTrue(a.filterName() == "Cook")

    def test_CNavFilterData(self):
        a = gnsstk.CNavFilterData()
        b = gnsstk.CNavMsgSort()
        self.assertTrue(isinstance(a, gnsstk.CNavFilterData))
        self.assertTrue(isinstance(b, gnsstk.CNavMsgSort))

    def test_CNavCrossSourceFilter(self):
        a = gnsstk.CNavCrossSourceFilter()
        self.assertTrue(isinstance(a, gnsstk.CNavCrossSourceFilter))
        self.assertTrue(2 == a.getMinIdentical())

    # This is not wrapped right now
    # def test_CNavEmptyFilter(self):
    #     a = gnsstk.CNavEmptyFilter()
    #     self.assertTrue(isinstance(a, gnsstk.CNavEmptyFilter))
    #     self.assertTrue(a.filterName(), "Empty")

    def test_CNavParityFilter(self):
        a = gnsstk.CNavParityFilter()
        self.assertTrue(isinstance(a, gnsstk.CNavParityFilter))
        self.assertTrue(a.filterName() == "CRC")

    def test_CNavTOWFilter(self):
        a = gnsstk.CNavTOWFilter()
        self.assertTrue(isinstance(a, gnsstk.CNavTOWFilter))
        self.assertTrue(a.filterName() == "TOW")

    def test_LNavAlmValFilter(self):
        a = gnsstk.LNavAlmValFilter()
        self.assertTrue(isinstance(a, gnsstk.LNavAlmValFilter))
        self.assertTrue(a.filterName() == "AlmVal")

    def test_LNavCookFilter(self):
        a = gnsstk.LNavCookFilter()
        self.assertTrue(isinstance(a, gnsstk.LNavCookFilter))
        self.assertTrue(a.processingDepth() == 0)

    def test_LNavCrossSourceFilter(self):
        a = gnsstk.LNavCrossSourceFilter()
        self.assertTrue(isinstance(a, gnsstk.LNavCrossSourceFilter))
        self.assertTrue(a.processingDepth() == 1)

    def test_LNavEmptyFilter(self):
        a = gnsstk.LNavEmptyFilter()
        self.assertTrue(isinstance(a, gnsstk.LNavEmptyFilter))
        self.assertTrue(a.filterName() == "Empty")

    def test_LNavEphMaker(self):
        a = gnsstk.LNavEphMaker()
        b = gnsstk.LNavEphMakerSort()
        self.assertTrue(isinstance(a, gnsstk.LNavEphMaker))
        self.assertTrue(isinstance(b, gnsstk.LNavEphMakerSort))
        self.assertTrue(a.processingDepth() == 3)

    def test_LNavFilterData(self):
        a = gnsstk.LNavFilterData()
        b = gnsstk.LNavMsgSort()
        self.assertTrue(isinstance(a, gnsstk.LNavFilterData))
        self.assertTrue(isinstance(b, gnsstk.LNavMsgSort))

    def test_LNavOrderFilter(self):
        a = gnsstk.LNavOrderFilter()
        b = gnsstk.LNavTimeSort()
        self.assertTrue(isinstance(a, gnsstk.LNavOrderFilter))
        self.assertTrue(isinstance(b, gnsstk.LNavTimeSort))
        self.assertTrue(a.filterName() == "Order")

    def test_LNavParityFilter(self):
        a = gnsstk.LNavParityFilter()
        self.assertTrue(isinstance(a, gnsstk.LNavParityFilter))
        self.assertTrue(a.filterName() == "Parity")

    def test_LNavTLMHOWFilter(self):
        a = gnsstk.LNavTLMHOWFilter()
        self.assertTrue(isinstance(a, gnsstk.LNavTLMHOWFilter))
        self.assertTrue(a.filterName() == "TLMHOW")

    def test_NavFilterKey(self):
        a = gnsstk.NavFilterKey()
        self.assertTrue(isinstance(a, gnsstk.NavFilterKey))
        self.assertTrue(isinstance(str(a), str))

    def test_NavFilterMgr(self):
        a = gnsstk.NavFilterMgr()
        self.assertTrue(isinstance(a, gnsstk.NavFilterMgr))
        self.assertTrue(isinstance(a.processingDepth(), int))

    def test_NavMsgDataBits(self):
        a = gnsstk.NavMsgDataBits()
        self.assertTrue(isinstance(a, gnsstk.NavMsgDataBits))
        self.assertTrue(isinstance(a.getBits(1, 0), int))

    # Woud test getBits here but the object constructed with default constructor throws seg fault on it
    def test_NavMsgDataPNB(self):
        a = gnsstk.NavMsgDataPNB()
        self.assertTrue(isinstance(a, gnsstk.NavMsgDataPNB))

    def test_NavMsgDataWords(self):
        a = gnsstk.NavMsgDataWords_30()
        self.assertTrue(isinstance(a, gnsstk.NavMsgDataWords_30))
        self.assertTrue(isinstance(a.getBits(1, 0), int))

    def test_NavOrderFilter(self):
        a = gnsstk.NavOrderFilter()
        b = gnsstk.NavTimeSort()
        self.assertTrue(isinstance(a, gnsstk.NavOrderFilter))
        self.assertTrue(isinstance(b, gnsstk.NavTimeSort))
        self.assertTrue(isinstance(a.epochInSeconds, int))
        self.assertTrue(a.filterName() == "Order")



if __name__ == '__main__':
    run_unit_tests()
