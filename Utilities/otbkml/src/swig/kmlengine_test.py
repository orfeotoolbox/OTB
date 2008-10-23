#!/usr/bin/env python

# Copyright 2008, Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of Google Inc. nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""
This file contains unit tests for the KML Engine Python SWIG bindings.
"""

import unittest
import kmldom
import kmlengine

# These VerySimple tests really only verify that the function/class exists.

class VerySimpleKmlDomTestCase(unittest.TestCase):
  """ Verify very simple KML DOM usage """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    assert name == placemark.get_name()

class VerySimpleBboxTestCase(unittest.TestCase):
  """ Verify very simple usage of class Bbox """
  def runTest(self):
    bbox = kmlengine.Bbox(38, 36, -120, -122)
    assert bbox.Contains(37, -121)

class VerySimpleCloneTestCase(unittest.TestCase):
  """ Verify very simple usage of Clone() """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    clone_element = kmlengine.Clone(placemark)
    assert clone_element
    clone_placemark = kmldom.AsPlacemark(clone_element)
    assert clone_placemark
    assert name == clone_placemark.get_name()
    # Change the name in the original
    new_name = 'new name'
    placemark.set_name(new_name)
    # Verify that the clone still has the old name
    assert name == clone_placemark.get_name()

class VerySimpleGetFeatureLatLonTestCase(unittest.TestCase):
  def runTest(self):
    kml = '<Placemark>' \
            '<Point><coordinates>1,2,3</coordinates></Point>' \
          '</Placemark>'
    element = kmldom.ParseKml(kml)
    assert element
    feature = kmldom.AsFeature(element)
    assert feature
    status,lat,lon = kmlengine.GetFeatureLatLon(feature)
    assert True == status
    assert 1 == lon
    assert 2 == lat

class VerySimpleGetRootFeatureTestCase(unittest.TestCase):
  """ Verify very simple usage of GetRootFeature() """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    feature = kmlengine.GetRootFeature(placemark)
    assert feature
    assert kmldom.AsPlacemark(feature)

class VerySimpleKmlFileTestCase(unittest.TestCase):
  """ Verify very simple usage of the KmlFile::CreateFromParse() method """
  def runTest(self):
    kml = '<Placemark><name>hello</name></Placemark>'
    kmlfile,errors = kmlengine.KmlFile.CreateFromParse(kml)
    assert kmlfile
    assert kmlfile.get_root()
    assert kmldom.AsPlacemark(kmlfile.get_root())

class VerySimpleKmzSplitTestCase(unittest.TestCase):
  def runTest(self):
    kml_url = 'http://foo.com/goo.kmz/bar.jpg'
    status,kmz_url,kmz_path = kmlengine.KmzSplit(kml_url)
    assert True == status
    assert 'http://foo.com/goo.kmz' == kmz_url
    assert 'bar.jpg' == kmz_path

class VerySimpleSplitUriTestCase(unittest.TestCase):
  def runTest(self):
    status,scheme,host,port,path,query,fragment = \
       kmlengine.SplitUri('http://host.com:82/a/b/c.kml?p=q#id')
    assert True == status
    assert 'http' == scheme
    assert 'host.com' == host
    assert 'a/b/c.kml' == path
    assert 'p=q' == query
    assert 'id' == fragment

class VerySimpleKmlFileTestCase(unittest.TestCase):
  """ Verify very simple usage of the KmlFile::CreateFromParse() method """
  def runTest(self):
    kml = '<Placemark><name>hello</name></Placemark>'
    kmlfile,errors = kmlengine.KmlFile.CreateFromParse(kml)
    assert kmlfile
    assert kmlfile.root()
    assert kmldom.AsPlacemark(kmlfile.root())

class BasicKmlFileTestCase(unittest.TestCase):
  def setUp(self):
    self.name = 'Hello, KmlFile!'
    self.id = 'placemark123'
    kml = '<Placemark id=\"%s\"><name>%s</name></Placemark>' % \
           (self.id, self.name)
    self.kmlfile,errors = kmlengine.KmlFile.CreateFromParse(kml)

  def testGetRoot(self):
    root = self.kmlfile.root()
    assert root
    placemark = kmldom.AsPlacemark(root)
    assert placemark
    assert self.name == placemark.get_name()
    assert self.id == placemark.get_id()
  
  def testGetObjectById(self):
    obj = self.kmlfile.GetObjectById(self.id)
    assert obj
    pm = kmldom.AsPlacemark(obj)
    assert pm
    assert self.id == pm.get_id()
    assert self.name == pm.get_name()

def suite():
  suite = unittest.TestSuite()
  suite.addTest(VerySimpleKmlDomTestCase())
  suite.addTest(VerySimpleBboxTestCase())
  suite.addTest(VerySimpleCloneTestCase())
  suite.addTest(VerySimpleGetRootFeatureTestCase())
  suite.addTest(VerySimpleGetFeatureLatLonTestCase())
  suite.addTest(VerySimpleKmzSplitTestCase())
  suite.addTest(VerySimpleSplitUriTestCase())
  suite.addTest(BasicKmlFileTestCase('testGetRoot'))
  suite.addTest(BasicKmlFileTestCase('testGetObjectById'))
  return suite


runner = unittest.TextTestRunner()
runner.run(suite())
