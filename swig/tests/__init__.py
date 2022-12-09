import unittest

class AbstractBaseTesters:
    class EnumTester(unittest.TestCase):
        """
        Generic Enum testing framework

        Tests expected values of enum and string conversions
        Inherit this class to test typical gnsstk strongly typed enums.

        For each subclass, create the `ENUM`, `EXP_ENUM_STRS`, and `STR_CONVERTER`
        class variables.

        ENUM: The enum to test (e.g. gnsstk.CorrDupHandling)
        EXP_ENUM_STRS: A dictionary of enum values and their expected string equivalent
        STR_CONVERTER: the function the converts strings into enum values

        """

        def test_enum(self):
            """
            Test enum
            """
            actual_items = set(self.ENUM) - {self.ENUM.Last}
            self.assertEqual(set(self.EXP_ENUM_STRS.keys()), actual_items)

            for exp_enum, exp_enum_str in self.EXP_ENUM_STRS.items():
                with self.subTest(enum=exp_enum):
                    self.assertEqual(str(exp_enum), exp_enum_str)
                    # A weird hack since adding a function as an attribute
                    # causes automatic binding.
                    str_converter = self.STR_CONVERTER.__func__
                    self.assertEqual(str_converter(exp_enum_str), exp_enum)