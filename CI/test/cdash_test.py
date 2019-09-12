#!/usr/bin/env python3
#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

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

  def test_GetBuildStatus (self):
    handler = cdash_handler.Handler()
    # We need to test error in configure, build, test, and no error
    # test buildid 720
    (status ,  error) = handler.GetBuildStatus("720")
    self.assertTrue( status == "failed")
    self.assertTrue( error == "Errors occur during tests")
    # configure buildid 352
    (status ,  error) = handler.GetBuildStatus("352")
    self.assertTrue( status == "failed")
    self.assertTrue( error == "Errors occur during configure")
    # build buildid 358
    (status ,  error) = handler.GetBuildStatus("358")
    self.assertTrue( status == "failed")
    self.assertTrue( error == "Errors occur during build")
    # passing buildid 373
    (status ,  error) = handler.GetBuildStatus("373")
    self.assertTrue( status == "success")
    self.assertTrue( error == "")


if __name__ == '__main__':
  unittest.main()
