import unittest
import os.path
import sys

# This is done so we can access the module handler
synthetic_build_dir = os.path.dirname(os.path.realpath(__file__))
(module_path , _)  = os.path.split(synthetic_build_dir)
sys.path.append(module_path)

import cdash_handler


class TestHandler(unittest.TestCase):

  def test_GetConfigureFile (self):
    handler = cdash_handler.Handler()
    handler.build_dir = synthetic_build_dir
    config_path = handler.GetConfigureFile()
    self.assertTrue(os.path.exists(config_path))
  
  def test_GetName (self):
    handler = cdash_handler.Handler()
    handler.build_dir = synthetic_build_dir
    name = handler.GetName()
    self.assertTrue("185_f-54sf_ghs-dgh (out_space)" == name)

  def test_GetSite (self):
    handler = cdash_handler.Handler()
    handler.build_dir = synthetic_build_dir
    site = handler.GetSite()
    self.assertTrue("debian-unstable-gcc.42" == site)

  def test_GetStamp (self):
    handler = cdash_handler.Handler()
    handler.build_dir = synthetic_build_dir
    stamp = handler.GetStamp()
    self.assertTrue("20190320-1706-Experimental_test" == stamp)

  def test_GetBuildId (self):
    handler = cdash_handler.Handler()
    buildid= handler.GetBuildId( site="debian-unstable-gcc" ,\
      stamp="20190319-1243-Experimental" , name="0388a356 (debian_CI)")
    self.assertTrue( "1" == buildid )

  def test_GetBuildUrl (self):
    handler = cdash_handler.Handler()
    url = handler.GetBuildUrl("1")
    self.assertTrue( \
      "https://cdash.orfeo-toolbox.org/buildSummary.php?buildid=1" == url)

if __name__ == '__main__':
  unittest.main()