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
This file contains unit tests for the KML DOM Python SWIG bindings.
"""

import unittest
import kmldom


class VerySimpleParseTestCase(unittest.TestCase):
  """ This simply verifies the existence of the binding to ParseKml() """

  def runTest(self):
    assert kmldom.ParseKml('<kml/>')


class VerySimpleSerializeTestCase(unittest.TestCase):
  """ This verifies the existence of the binding to SerializeRaw() """

  def runTest(self):
    kml = '<kml/>'
    assert kml == kmldom.SerializeRaw(kmldom.ParseKml(kml))


class SimpleFactoryTestCase(unittest.TestCase):
  """
  This verifies the existence of the binding to KmlFactory::GetFactory
  and the exitence of bindings for all factory methods.
  """

  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    assert factory.CreateAlias()
    assert factory.CreateBalloonStyle()
    assert factory.CreateCamera()
    assert factory.CreateChange()
    assert factory.CreateCreate()
    assert factory.CreateData()
    assert factory.CreateDelete()
    assert factory.CreateDocument()
    assert factory.CreateExtendedData()
    assert factory.CreateFolder()
    assert factory.CreateGroundOverlay()
    assert factory.CreateIcon()
    assert factory.CreateIconStyle()
    assert factory.CreateImagePyramid()
    assert factory.CreateItemIcon()
    assert factory.CreateLabelStyle()
    assert factory.CreateLatLonAltBox()
    assert factory.CreateLatLonBox()
    assert factory.CreateLineString()
    assert factory.CreateLineStyle()
    assert factory.CreateLinearRing()
    assert factory.CreateLink()
    assert factory.CreateListStyle()
    assert factory.CreateLocation()
    assert factory.CreateLod()
    assert factory.CreateLookAt()
    assert factory.CreateModel()
    assert factory.CreateMultiGeometry()
    assert factory.CreateNetworkLink()
    assert factory.CreateNetworkLinkControl()
    assert factory.CreateOrientation()
    assert factory.CreatePair()
    assert factory.CreatePhotoOverlay()
    assert factory.CreatePlacemark()
    assert factory.CreatePoint()
    assert factory.CreatePolyStyle()
    assert factory.CreatePolygon()
    assert factory.CreateRegion()
    assert factory.CreateResourceMap()
    assert factory.CreateScale()
    assert factory.CreateSchema()
    assert factory.CreateSchemaData()
    assert factory.CreateScreenOverlay()
    assert factory.CreateSimpleData()
    assert factory.CreateSimpleField()
    assert factory.CreateSnippet()
    assert factory.CreateStyle()
    assert factory.CreateStyleMap()
    assert factory.CreateTimeSpan()
    assert factory.CreateTimeStamp()
    assert factory.CreateUpdate()
    assert factory.CreateViewVolume()
    assert factory.CreateHotSpot()
    assert factory.CreateInnerBoundaryIs()
    assert factory.CreateKml()
    assert factory.CreateLinkSnippet()
    assert factory.CreateOuterBoundaryIs()
    assert factory.CreateOverlayXY()
    assert factory.CreateRotationXY()
    assert factory.CreateScreenXY()
    assert factory.CreateSize()


class VerySimpleCastTestCase(unittest.TestCase):
  """ This verifies the existence of the bindings to a few of the casts """

  def runTest(self):
    """ kmldom.ParseKml() returns an ElementPtr """
    assert kmldom.AsCoordinates(kmldom.ParseKml('<coordinates/>'))
    assert not kmldom.AsCoordinates(kmldom.ParseKml('<Document/>'))
    assert kmldom.AsFolder(kmldom.ParseKml('<Folder/>'))
    assert not kmldom.AsFolder(kmldom.ParseKml('<Document/>'))
    assert kmldom.AsKml(kmldom.ParseKml('<kml/>'))
    assert not kmldom.AsKml(kmldom.ParseKml('<Placemark/>'))
    assert kmldom.AsPlacemark(kmldom.ParseKml('<Placemark/>'))
    assert not kmldom.AsPlacemark(kmldom.ParseKml('<NetworkLink/>'))
    assert kmldom.AsPoint(kmldom.ParseKml('<Point/>'))
    assert not kmldom.AsPoint(kmldom.ParseKml('<LineString/>'))


class VerySimpleSimpleChildTestCase(unittest.TestCase):
  """
  This tests that an element derived from Feature can properly set/get/clear
  the simple value <name> element.
  """

  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    folder = factory.CreateFolder()
    assert not folder.has_name()
    name = 'my name'
    folder.set_name(name)
    assert folder.has_name()
    assert name == folder.get_name()
    folder.clear_name()
    assert not folder.has_name()


class VerySimpleComplexChildTestCase(unittest.TestCase):
  """
  This tests that a complex child of a specific type can be set and
  cleared as a child of another element.
  """

  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    region = factory.CreateRegion()
    assert not region.has_lod()
    assert not region.has_latlonaltbox()
    region.set_lod(factory.CreateLod())
    region.set_latlonaltbox(factory.CreateLatLonAltBox())
    assert region.has_lod()
    assert region.has_latlonaltbox()
    region.clear_lod()
    region.clear_latlonaltbox()
    assert not region.has_lod()
    assert not region.has_latlonaltbox()


class SimpleCoordinatesTestCase(unittest.TestCase):
  """ This tests the methods on Coordinates and Vec3 """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    coordinates = factory.CreateCoordinates()
    # Test the Element methods
    assert kmldom.Type_coordinates == coordinates.Type()
    assert coordinates.IsA(kmldom.Type_coordinates)
    # Test the Coordinates methods
    lon = -123.456
    lat = 38.765
    alt = 1001.909
    assert 0 == coordinates.get_coordinates_array_size()
    coordinates.add_latlng(lat, lon)
    assert 1 == coordinates.get_coordinates_array_size()
    coordinates.add_latlngalt(lat, lon, alt)
    assert 2 == coordinates.get_coordinates_array_size()
    vec0 = coordinates.get_coordinates_array_at(0)
    vec1 = coordinates.get_coordinates_array_at(1)
    # Test the Vec3 methods
    assert lon == vec0.get_longitude()
    assert lat == vec0.get_latitude()
    assert 0 == vec0.get_altitude()
    assert lon == vec1.get_longitude()
    assert lat == vec1.get_latitude()
    assert alt == vec1.get_altitude()


class SimpleVec2TestCase(unittest.TestCase):
  """ This tests the methods on Vec2 (using HotSpot) """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    hotspot = factory.CreateHotSpot()
    # Test the Element methods
    assert kmldom.Type_hotSpot == hotspot.Type()
    assert hotspot.IsA(kmldom.Type_hotSpot)
    # Test the Vec2 methods
    assert not hotspot.has_x()
    assert not hotspot.has_xunits()
    assert not hotspot.has_y()
    assert not hotspot.has_yunits()
    x = 128
    xunits = kmldom.UNITS_PIXELS
    y = .7
    yunits = kmldom.UNITS_FRACTION
    # Test the setters
    hotspot.set_x(x)
    hotspot.set_xunits(xunits)
    hotspot.set_y(y)
    hotspot.set_yunits(yunits)
    # Test the has'ers
    assert hotspot.has_x()
    assert hotspot.has_xunits()
    assert hotspot.has_y()
    assert hotspot.has_yunits()
    # Test the getters
    assert x == hotspot.get_x()
    assert xunits == hotspot.get_xunits()
    assert y == hotspot.get_y()
    assert yunits == hotspot.get_yunits()
    # Test the clear'ers
    hotspot.clear_x()
    hotspot.clear_xunits()
    hotspot.clear_y()
    hotspot.clear_yunits()
    assert not hotspot.has_x()
    assert not hotspot.has_xunits()
    assert not hotspot.has_y()
    assert not hotspot.has_yunits()


class SimpleObjectTestCase(unittest.TestCase):
  """ This tests the Object methods (using Placemark) """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    # Element methods
    assert kmldom.Type_Placemark == placemark.Type()
    assert placemark.IsA(kmldom.Type_Object)
    assert placemark.IsA(kmldom.Type_Placemark)
    # Object methods
    # has_xxx()...
    assert not placemark.has_id()
    assert not placemark.has_targetid()
    id = 'placemark123'
    targetid ='placemark456'
    # set_xxx()...
    placemark.set_id(id)
    placemark.set_targetid(targetid)
    assert placemark.has_id()
    assert placemark.has_targetid()
    # xxx()...
    assert id == placemark.get_id()
    assert targetid == placemark.get_targetid()
    # clear_xxx()...
    placemark.clear_id()
    placemark.clear_targetid()
    assert not placemark.has_id()
    assert not placemark.has_targetid()


class SimpleFeatureTestCase(unittest.TestCase):
  """ This tests the Feature methods (using Folder) """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    folder = factory.CreateFolder()

    # Element methods
    assert kmldom.Type_Folder == folder.Type()
    assert folder.IsA(kmldom.Type_Object)
    assert folder.IsA(kmldom.Type_Feature)
    assert folder.IsA(kmldom.Type_Folder)

    # TODO: Object methods

    # Feature methods
    # TODO everything else
    assert not folder.has_snippet()
    folder.set_snippet(factory.CreateSnippet())
    assert folder.has_snippet()
    snippet = folder.get_snippet()
    # Test that the snippet() returns a SnippetPtr
    assert not snippet.has_maxlines()
    folder.clear_snippet()
    assert not folder.has_snippet()


class SimpleRegionTestCase(unittest.TestCase):
  """ This tests the methods on Region """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    region = factory.CreateRegion()

    # Element methods
    assert kmldom.Type_Region == region.Type()
    assert region.IsA(kmldom.Type_Object)
    assert region.IsA(kmldom.Type_Region)

    # Object methods
    # has_xxx()...
    assert not region.has_id()
    assert not region.has_targetid()
    id = 'region123'
    targetid ='region456'
    # set_xxx()...
    region.set_id(id)
    region.set_targetid(targetid)
    assert region.has_id()
    assert region.has_targetid()
    # xxx()...
    assert id == region.get_id()
    assert targetid == region.get_targetid()
    # clear_xxx()...
    region.clear_id()
    region.clear_targetid()
    assert not region.has_id()
    assert not region.has_targetid()

    # Region methods
    assert not region.has_lod()
    assert not region.has_latlonaltbox()
    region.set_lod(factory.CreateLod())
    region.set_latlonaltbox(factory.CreateLatLonAltBox())
    assert region.has_lod()
    assert region.has_latlonaltbox()
    region.clear_lod()
    region.clear_latlonaltbox()
    assert not region.has_lod()
    assert not region.has_latlonaltbox()


class SimpleParseTestCase(unittest.TestCase):
  """ This tests the Parse() function."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.root = kmldom.ParseKml('<kml>'
                                '<Placemark id=\"pm123\">'
                                '<name>hi</name>'
                                '</Placemark>'
                                '</kml>')

  def tearDown(self):
    """ KML DOM memory is managed with smart pointers no tear down is needed """

  def testBasic(self):
    assert self.root
    kml = kmldom.AsKml(self.root)
    assert kml
    assert not kml.has_hint()
    assert not kml.has_networklinkcontrol()
    assert kml.has_feature()
    placemark = kmldom.AsPlacemark(kml.get_feature())
    assert placemark
    assert placemark.has_id()
    assert 'pm123' == placemark.get_id()
    assert placemark.has_name()
    assert 'hi' == placemark.get_name()
    assert not placemark.has_geometry()


class SimpleSerializePrettyTestCase(unittest.TestCase):
  """ This tests the SerializePretty() function."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.kml = self.factory.CreateKml()
    self.folder = self.factory.CreateFolder()
    self.folder.set_name('folder')
    self.placemark = self.factory.CreatePlacemark()
    self.placemark.set_name('placemark')
    self.folder.add_feature(self.placemark)
    self.kml.set_feature(self.folder)
    self.xml = kmldom.SerializePretty(self.kml)

  def testBasic(self):
    assert self.xml
    assert '<kml>\n'\
           '  <Folder>\n'\
           '    <name>folder</name>\n'\
           '    <Placemark>\n'\
           '      <name>placemark</name>\n'\
           '    </Placemark>\n'\
           '  </Folder>\n'\
           '</kml>\n' == self.xml


class SimpleSerializeRawTestCase(unittest.TestCase):
  """ This tests the SerializeRaw() function."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.kml = self.factory.CreateKml()
    self.folder = self.factory.CreateFolder()
    self.folder.set_name('folder')
    self.placemark = self.factory.CreatePlacemark()
    self.placemark.set_name('placemark')
    self.folder.add_feature(self.placemark)
    self.kml.set_feature(self.folder)
    self.xml = kmldom.SerializeRaw(self.kml)

  def testBasic(self):
    assert self.xml
    assert '<kml>'\
           '<Folder>'\
           '<name>folder</name>'\
           '<Placemark>'\
           '<name>placemark</name>'\
           '</Placemark>'\
           '</Folder>'\
           '</kml>' == self.xml


class SimpleExtendedDataTestCase(unittest.TestCase):
  """ This tests the ExtendedData element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.extendeddata = self.factory.CreateExtendedData()

  def testDefault(self):
    assert kmldom.AsExtendedData(self.extendeddata)
    assert 0 == self.extendeddata.get_data_array_size()
    assert 0 == self.extendeddata.get_schemadata_array_size()

def TestColorStyle(colorstyle):
  """ This tests the ColorStyle elements of the given ColorStyle-derived element"""
  # ColorStyle fields
  # Verify default state of <color>
  assert not colorstyle.has_color()
  # Set <color> to a given value and verify get_ and has_
  color = 'f0f0c80f'
  colorstyle.set_color(color)
  assert colorstyle.has_color()
  assert color == colorstyle.get_color()
  # Clear <color> and verify has_
  colorstyle.clear_color()
  assert not colorstyle.has_color()

  assert not colorstyle.has_colormode()
  colormode = kmldom.COLORMODE_RANDOM
  colorstyle.set_colormode(colormode)
  assert colorstyle.has_colormode()
  assert colormode == colorstyle.get_colormode()
  colorstyle.clear_colormode()
  assert not colorstyle.has_colormode()

class SimpleIconStyleTestCase(unittest.TestCase):
  """ This tests the IconStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.iconstyle = self.factory.CreateIconStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.iconstyle)
    assert kmldom.AsSubStyle(self.iconstyle)
    assert kmldom.AsColorStyle(self.iconstyle)

    TestColorStyle(self.iconstyle)

class SimpleLabelStyleTestCase(unittest.TestCase):
  """ This tests the LabelStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.labelstyle = self.factory.CreateLabelStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.labelstyle)
    assert kmldom.AsSubStyle(self.labelstyle)
    assert kmldom.AsColorStyle(self.labelstyle)

    TestColorStyle(self.labelstyle)

class SimpleLineStyleTestCase(unittest.TestCase):
  """ This tests the LineStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.linestyle = self.factory.CreateLineStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.linestyle)
    assert kmldom.AsSubStyle(self.linestyle)
    assert kmldom.AsColorStyle(self.linestyle)

    TestColorStyle(self.linestyle)

    # LineStyle fields
    # Verify default state of <width>
    assert not self.linestyle.has_width()
    # Set <width> and verify get_ and has_
    width = 4
    self.linestyle.set_width(width)
    assert self.linestyle.has_width()
    assert width == self.linestyle.get_width()
    # Clear <width> and verify has_
    self.linestyle.clear_width()
    assert not self.linestyle.has_width()

class SimpleListStyleTestCase(unittest.TestCase):
  """ This tests the ListStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.liststyle = self.factory.CreateListStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.liststyle)
    assert kmldom.AsSubStyle(self.liststyle)
    assert kmldom.AsListStyle(self.liststyle)
    assert not self.liststyle.has_id()
    assert 0 == self.liststyle.get_itemicon_array_size()


class SimplePhotoOverlayTestCase(unittest.TestCase):
  """ This tests the PhotoOverlay element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.photooverlay = self.factory.CreatePhotoOverlay()

  def testDefault(self):
    assert kmldom.AsObject(self.photooverlay)
    assert kmldom.AsFeature(self.photooverlay)
    assert kmldom.AsOverlay(self.photooverlay)
    assert kmldom.AsPhotoOverlay(self.photooverlay)
    assert not self.photooverlay.has_id()  # Object-ness
    assert not self.photooverlay.has_name()  # Feature-ness
    assert not self.photooverlay.has_icon()  # Overlay-ness
    assert not self.photooverlay.has_rotation()  # PhotoOverlay-ness...
    assert not self.photooverlay.has_viewvolume()
    assert not self.photooverlay.has_imagepyramid()
    assert not self.photooverlay.has_point()
    assert not self.photooverlay.has_shape()


class SimplePlacemarkTestCase(unittest.TestCase):
  """ This tests the Placemark element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.placemark = self.factory.CreatePlacemark()

  def testDefault(self):
    assert kmldom.AsObject(self.placemark)
    assert kmldom.AsFeature(self.placemark)
    assert kmldom.AsPlacemark(self.placemark)
    assert not self.placemark.has_id()  # Object-ness
    assert not self.placemark.has_name()  # Feature-ness
    assert not self.placemark.has_geometry()  # Placemark-ness

  def testName(self):
    name = 'This is my name'
    self.placemark.set_name(name)
    assert self.placemark.has_name()
    assert name == self.placemark.get_name()
    self.placemark.clear_name()
    assert not self.placemark.has_name()


class SimpleLatLonBoxTestCase(unittest.TestCase):
  """ This tests the LatLonBox element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.latlonbox = self.factory.CreateLatLonBox()

  def testDefault(self):
    assert kmldom.AsObject(self.latlonbox)
    assert kmldom.AsLatLonBox(self.latlonbox)
    assert not self.latlonbox.has_id()  # Object-ness
    assert not self.latlonbox.has_north()
    assert 0.0 == self.latlonbox.get_north()
    assert not self.latlonbox.has_south()
    assert 0.0 == self.latlonbox.get_south()
    assert not self.latlonbox.has_east()
    assert 0.0 == self.latlonbox.get_east()
    assert not self.latlonbox.has_west()
    assert 0.0 == self.latlonbox.get_west()

  def testSetClear(self):
    north = 42.42
    south = -11.12
    east = -80.80
    west = -123.321
    self.latlonbox.set_north(north)
    self.latlonbox.set_south(south)
    self.latlonbox.set_east(east)
    self.latlonbox.set_west(west)
    assert self.latlonbox.has_north()
    assert north == self.latlonbox.get_north()
    assert self.latlonbox.has_south()
    assert south == self.latlonbox.get_south()
    assert self.latlonbox.has_east()
    assert east == self.latlonbox.get_east()
    assert self.latlonbox.has_west()
    assert west == self.latlonbox.get_west()
    self.latlonbox.clear_north()
    self.latlonbox.clear_south()
    self.latlonbox.clear_east()
    self.latlonbox.clear_west()
    # Clearing everything returns to default state
    self.testDefault()


class SimpleLinkTestCase(unittest.TestCase):
  """ This tests the Link element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.link = self.factory.CreateLink()

  def testDefault(self):
    assert kmldom.AsObject(self.link)
    assert kmldom.AsLink(self.link)
    assert not self.link.has_id()  # Object-ness
    assert not self.link.has_href()  # Link-ness

  def testSetClear(self):
    href = 'http://example.com/cool.kml'
    self.link.set_href(href)
    assert self.link.has_href()
    assert href == self.link.get_href()
    self.link.clear_href()
    # Clearing everything returns to default state
    self.testDefault()


class SimpleChangeTestCase(unittest.TestCase):
  """ This tests the Change element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.change = self.factory.CreateChange()

  def testDefault(self):
    assert kmldom.AsChange(self.change)
    assert 0 == self.change.get_object_array_size()

  def testAddOneObject(self):
    placemark = self.factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    self.change.add_object(placemark)
    assert 1 == self.change.get_object_array_size()
    placemark = kmldom.AsPlacemark(self.change.get_object_array_at(0))
    assert name == placemark.get_name()

  def testAddNObjects(self):
    placemark = self.factory.CreatePlacemark()
    placemark_id = 'pm123'
    placemark.set_id(placemark_id)
    self.change.add_object(placemark)

    latlonbox = self.factory.CreateLatLonBox()
    latlonbox_id = 'llb123'
    latlonbox.set_id(latlonbox_id)
    self.change.add_object(latlonbox)

    location = self.factory.CreateLocation()
    location_id = 'location123'
    location.set_id(location_id)
    self.change.add_object(location)

    """ Verify there are 3 items in the array in the proper order."""
    assert 3 == self.change.get_object_array_size()
    assert placemark_id == self.change.get_object_array_at(0).get_id()
    assert latlonbox_id == self.change.get_object_array_at(1).get_id()
    assert location_id == self.change.get_object_array_at(2).get_id()


class SimpleCreateTestCase(unittest.TestCase):
  """ This tests the Create element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.create = self.factory.CreateCreate()

  def testDefault(self):
    assert kmldom.AsCreate(self.create)
    assert 0 == self.create.get_container_array_size()

  def testAddOneContainer(self):
    folder = self.factory.CreateFolder()
    target_folder_id = 'target-folder'
    folder.set_targetid(target_folder_id)
    self.create.add_container(folder)
    assert 1 == self.create.get_container_array_size()
    folder = kmldom.AsFolder(self.create.get_container_array_at(0))
    assert target_folder_id == folder.get_targetid()

  def testAddNContainers(self):
    folder = self.factory.CreateFolder()
    folder_a_id = 'folder-a'
    folder.set_id(folder_a_id)
    self.create.add_container(folder)

    document = self.factory.CreateFolder()
    document_a_id = 'container-a'
    document.set_id(document_a_id)
    self.create.add_container(document)

    document = self.factory.CreateFolder()
    document_b_id = 'container-b'
    document.set_id(document_b_id)
    self.create.add_container(document)

    folder = self.factory.CreateFolder()
    folder_b_id = 'folder-b'
    folder.set_id(folder_b_id)
    self.create.add_container(folder)

    # Verify there are 4 items in the array in the proper order.
    assert 4 == self.create.get_container_array_size()
    assert folder_a_id == self.create.get_container_array_at(0).get_id()
    assert document_a_id == self.create.get_container_array_at(1).get_id()
    assert document_b_id == self.create.get_container_array_at(2).get_id()
    assert folder_b_id == self.create.get_container_array_at(3).get_id()


class SimpleDeleteTestCase(unittest.TestCase):
  """ This tests the Delete element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.delete = self.factory.CreateDelete()

  def testDefault(self):
    assert kmldom.AsDelete(self.delete)
    assert 0 == self.delete.get_feature_array_size()

  def testAddOneFeature(self):
    groundoverlay = self.factory.CreateGroundOverlay()
    target_groundoverlay_id = 'target-groundoverlay'
    groundoverlay.set_targetid(target_groundoverlay_id)
    self.delete.add_feature(groundoverlay)
    assert 1 == self.delete.get_feature_array_size()
    groundoverlay = kmldom.AsGroundOverlay(self.delete.get_feature_array_at(0))
    assert target_groundoverlay_id == groundoverlay.get_targetid()

  def testAddNFeatures(self):
    document = self.factory.CreateDocument()
    document_id = 'document-id'
    document.set_id(document_id)
    self.delete.add_feature(document)

    folder = self.factory.CreateFolder()
    folder_id = 'folder-id'
    folder.set_id(folder_id)
    self.delete.add_feature(folder)

    groundoverlay = self.factory.CreateGroundOverlay()
    groundoverlay_id = 'groundoverlay-id'
    groundoverlay.set_id(groundoverlay_id)
    self.delete.add_feature(groundoverlay)

    networklink = self.factory.CreateNetworkLink()
    networklink_id = 'networklink-id'
    networklink.set_id(networklink_id)
    self.delete.add_feature(networklink)

    placemark = self.factory.CreatePlacemark()
    placemark_id = 'placemark-id'
    placemark.set_id(placemark_id)
    self.delete.add_feature(placemark)

    photooverlay = self.factory.CreatePhotoOverlay()
    photooverlay_id = 'photooverlay-id'
    photooverlay.set_id(photooverlay_id)
    self.delete.add_feature(photooverlay)

    screenoverlay = self.factory.CreateScreenOverlay()
    screenoverlay_id = 'screenoverlay-id'
    screenoverlay.set_id(screenoverlay_id)
    self.delete.add_feature(screenoverlay)

    # Verify there are 7 items in the array in the proper order.
    assert 7 == self.delete.get_feature_array_size()
    assert document_id == self.delete.get_feature_array_at(0).get_id()
    assert folder_id == self.delete.get_feature_array_at(1).get_id()
    assert groundoverlay_id == self.delete.get_feature_array_at(2).get_id()
    assert networklink_id == self.delete.get_feature_array_at(3).get_id()
    assert placemark_id == self.delete.get_feature_array_at(4).get_id()
    assert photooverlay_id == self.delete.get_feature_array_at(5).get_id()
    assert screenoverlay_id == self.delete.get_feature_array_at(6).get_id()


class SimpleDocumentTestCase(unittest.TestCase):
  """ This tests the Document element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.document = self.factory.CreateDocument()

  def testDefault(self):
    assert kmldom.AsObject(self.document)
    assert kmldom.AsFeature(self.document)
    assert kmldom.AsContainer(self.document)
    assert kmldom.AsDocument(self.document)
    assert not self.document.has_id()  # Object-ness
    assert not self.document.has_name()  # Feature-ness
    assert 0 == self.document.get_feature_array_size()  # Container-ness
    assert 0 == self.document.get_schema_array_size()  # Document-ness
    assert 0 == self.document.get_styleselector_array_size()  # Document-ness


class SimpleMultiGeometryTestCase(unittest.TestCase):
  """  This tests the MultiGeometry element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.multigeometry = self.factory.CreateMultiGeometry()

  def testDefault(self):
    assert kmldom.AsObject(self.multigeometry)
    assert kmldom.AsGeometry(self.multigeometry)
    assert kmldom.AsMultiGeometry(self.multigeometry)
    assert not self.multigeometry.has_id()  # Object-ness
    # MultiGeometry-ness
    assert 0 == self.multigeometry.get_geometry_array_size()


class SimpleOuterBoundaryIsTestCase(unittest.TestCase):
  """  TODO: This tests the OuterBoundaryIs element."""


class SimpleInnerBoundaryIsTestCase(unittest.TestCase):
  """  TODO: This tests the InnerBoundaryIs element."""


class SimplePolygonTestCase(unittest.TestCase):
  """  TODO: This tests the Polygon element."""

class SimplePolyStyleTestCase(unittest.TestCase):
  """ This tests the PolyStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.polystyle = self.factory.CreatePolyStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.polystyle)
    assert kmldom.AsSubStyle(self.polystyle)
    assert kmldom.AsColorStyle(self.polystyle)

    TestColorStyle(self.polystyle)

    # PolyStyle fields
    # Verify default state of <fill>
    assert not self.polystyle.has_fill()
    # Set <fill> and verify get_ and has_
    fill = True
    self.polystyle.set_fill(fill)
    assert self.polystyle.has_fill()
    assert fill == self.polystyle.get_fill()
    # Clear <fill> and verify has_
    self.polystyle.clear_fill()
    assert not self.polystyle.has_fill()

    # Verify default state of <outline>
    assert not self.polystyle.has_outline()
    # Set <outline> and verify get_ and has_
    outline = True
    self.polystyle.set_outline(outline)
    assert self.polystyle.has_outline()
    assert outline == self.polystyle.get_outline()
    # Clear <outline> and verify has_
    self.polystyle.clear_outline()
    assert not self.polystyle.has_outline()

class SimpleResourceMapTestCase(unittest.TestCase):
  """ This tests the ResourceMap element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.resourcemap = self.factory.CreateResourceMap()

  def testDefault(self):
    assert kmldom.AsObject(self.resourcemap)
    assert kmldom.AsResourceMap(self.resourcemap)
    assert not self.resourcemap.has_id()
    assert 0 == self.resourcemap.get_alias_array_size()


class SimpleSchemaTestCase(unittest.TestCase):
  """ This tests the Schema element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.schema = self.factory.CreateSchema()

  def testDefault(self):
    assert kmldom.AsSchema(self.schema)
    assert not self.schema.has_name()
    assert not self.schema.has_id()
    assert 0 == self.schema.get_simplefield_array_size()


class SimpleSchemaDataTestCase(unittest.TestCase):
  """ This tests the SchemaData element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.schemadata = self.factory.CreateSchemaData()

  def testDefault(self):
    assert kmldom.AsObject(self.schemadata)
    assert kmldom.AsSchemaData(self.schemadata)
    assert not self.schemadata.has_id()
    assert 0 == self.schemadata.get_simpledata_array_size()


class SimpleSimpleFieldTestCase(unittest.TestCase):
  """ This tests the SimpleField element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.simplefield = self.factory.CreateSimpleField()

  def testDefault(self):
    assert kmldom.AsSimpleField(self.simplefield)
    assert not self.simplefield.has_type()
    assert not self.simplefield.has_name()
    assert not self.simplefield.has_displayname()


class SimpleStyleMapTestCase(unittest.TestCase):
  """  TODO: This tests the StyleMap element."""


class SimpleUpdateTestCase(unittest.TestCase):
  """ This tests the Update element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.update = self.factory.CreateUpdate()

  def testDefault(self):
    assert kmldom.AsUpdate(self.update)
    assert 0 == self.update.get_updateoperation_array_size()


class SimpleStyleMapTestCase(unittest.TestCase):
  """  TODO: This tests the StyleMap element."""

class SimpleIconStyleIconTestCase(unittest.TestCase):
  """ This tests the IconStyleIcon element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.iconstyleicon= self.factory.CreateIconStyleIcon()

  def testBasic(self):
    assert not self.iconstyleicon.has_href() # default state
    assert '' == self.iconstyleicon.get_href()
    href = 'http://blah.blah'
    self.iconstyleicon.set_href(href)
    assert self.iconstyleicon.has_href()
    assert href == self.iconstyleicon.get_href()
    self.iconstyleicon.clear_href()
    assert not self.iconstyleicon.has_href() # back in default state
    assert '' == self.iconstyleicon.get_href()


def suite():
  suite = unittest.TestSuite()
  suite.addTest(VerySimpleParseTestCase())
  suite.addTest(VerySimpleSerializeTestCase())
  suite.addTest(SimpleFactoryTestCase())
  suite.addTest(VerySimpleCastTestCase())
  suite.addTest(VerySimpleSimpleChildTestCase())
  suite.addTest(VerySimpleComplexChildTestCase())
  suite.addTest(SimpleCoordinatesTestCase())
  suite.addTest(SimpleVec2TestCase())
  suite.addTest(SimpleObjectTestCase())
  suite.addTest(SimpleFeatureTestCase())
  suite.addTest(SimpleRegionTestCase())
  suite.addTest(SimpleParseTestCase('testBasic'))
  suite.addTest(SimpleSerializePrettyTestCase('testBasic'))
  suite.addTest(SimpleSerializeRawTestCase('testBasic'))
  suite.addTest(SimpleExtendedDataTestCase('testDefault'))
  suite.addTest(SimpleIconStyleTestCase('testDefault'))
  suite.addTest(SimpleLabelStyleTestCase('testDefault'))
  suite.addTest(SimpleLineStyleTestCase('testDefault'))
  suite.addTest(SimpleListStyleTestCase('testDefault'))
  suite.addTest(SimplePhotoOverlayTestCase('testDefault'))
  suite.addTest(SimplePlacemarkTestCase('testDefault'))
  suite.addTest(SimplePlacemarkTestCase('testName'))
  suite.addTest(SimplePolyStyleTestCase('testDefault'))
  suite.addTest(SimpleLatLonBoxTestCase('testDefault'))
  suite.addTest(SimpleLatLonBoxTestCase('testSetClear'))
  suite.addTest(SimpleLinkTestCase('testDefault'))
  suite.addTest(SimpleLinkTestCase('testSetClear'))
  suite.addTest(SimpleChangeTestCase('testDefault'))
  suite.addTest(SimpleChangeTestCase('testAddOneObject'))
  suite.addTest(SimpleChangeTestCase('testAddNObjects'))
  suite.addTest(SimpleCreateTestCase('testDefault'))
  suite.addTest(SimpleCreateTestCase('testAddOneContainer'))
  suite.addTest(SimpleCreateTestCase('testAddNContainers'))
  suite.addTest(SimpleDeleteTestCase('testAddOneFeature'))
  suite.addTest(SimpleDeleteTestCase('testAddNFeatures'))
  suite.addTest(SimpleDocumentTestCase('testDefault'))
  suite.addTest(SimpleResourceMapTestCase('testDefault'))
  suite.addTest(SimpleMultiGeometryTestCase('testDefault'))
  suite.addTest(SimpleSchemaTestCase('testDefault'))
  suite.addTest(SimpleSchemaDataTestCase('testDefault'))
  suite.addTest(SimpleSimpleFieldTestCase('testDefault'))
  suite.addTest(SimpleUpdateTestCase('testDefault'))
  suite.addTest(SimpleIconStyleIconTestCase('testBasic'))
  return suite


runner = unittest.TextTestRunner()
runner.run(suite())
