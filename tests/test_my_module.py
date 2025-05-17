import sys
import os
import unittest

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../build")))

import my_module

class MyModuleTests(unittest.TestCase):
    def test_hello(self):
        obj = my_module.MyClass("S")
        self.assertEqual(obj.hello(), "Hello, S")

if __name__ == "__main__":
    unittest.main()

