#!/usr/bin/python

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
    self.factory.DeleteElement(self.root)

  def testBasic(self):
    assert self.root
    kml = kmldom.AsKml(self.root)
    assert kml
    assert not kml.has_hint()
    assert not kml.has_networklinkcontrol()
    assert kml.has_feature()
    placemark = kmldom.AsPlacemark(kml.feature())
    assert placemark
    assert placemark.has_id()
    assert 'pm123' == placemark.id()
    assert placemark.has_name()
    assert 'hi' == placemark.name()
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

  def tearDown(self):
    self.factory.DeleteElement(self.kml)

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

  def tearDown(self):
    self.factory.DeleteElement(self.kml)

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

  def tearDown(self):
    self.factory.DeleteElement(self.extendeddata)

  def testDefault(self):
    assert kmldom.AsExtendedData(self.extendeddata)
    assert 0 == self.extendeddata.extendeddatamember_array_size()


class SimpleListStyleTestCase(unittest.TestCase):
  """ This tests the ListStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.liststyle = self.factory.CreateListStyle()

  def tearDown(self):
    self.factory.DeleteElement(self.liststyle)

  def testDefault(self):
    assert kmldom.AsObject(self.liststyle)
    assert kmldom.AsSubStyle(self.liststyle)
    assert kmldom.AsListStyle(self.liststyle)
    assert not self.liststyle.has_id()
    assert 0 == self.liststyle.itemicon_array_size()


class SimplePhotoOverlayTestCase(unittest.TestCase):
  """ This tests the PhotoOverlay element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.photooverlay = self.factory.CreatePhotoOverlay()

  def tearDown(self):
    self.factory.DeleteElement(self.photooverlay)

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

  def tearDown(self):
    self.factory.DeleteElement(self.placemark)

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
    assert name == self.placemark.name()
    self.placemark.clear_name()
    assert not self.placemark.has_name()


class SimpleLatLonBoxTestCase(unittest.TestCase):
  """ This tests the LatLonBox element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.latlonbox = self.factory.CreateLatLonBox()

  def tearDown(self):
    self.factory.DeleteElement(self.latlonbox)

  def testDefault(self):
    assert kmldom.AsObject(self.latlonbox)
    assert kmldom.AsLatLonBox(self.latlonbox)
    assert not self.latlonbox.has_id()  # Object-ness
    assert not self.latlonbox.has_north()
    assert 0.0 == self.latlonbox.north()
    assert not self.latlonbox.has_south()
    assert 0.0 == self.latlonbox.south()
    assert not self.latlonbox.has_east()
    assert 0.0 == self.latlonbox.east()
    assert not self.latlonbox.has_west()
    assert 0.0 == self.latlonbox.west()

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
    assert north == self.latlonbox.north()
    assert self.latlonbox.has_south()
    assert south == self.latlonbox.south()
    assert self.latlonbox.has_east()
    assert east == self.latlonbox.east()
    assert self.latlonbox.has_west()
    assert west == self.latlonbox.west()
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

  def tearDown(self):
    self.factory.DeleteElement(self.link)

  def testDefault(self):
    assert kmldom.AsObject(self.link)
    assert kmldom.AsLink(self.link)
    assert not self.link.has_id()  # Object-ness
    assert not self.link.has_href()  # Link-ness

  def testSetClear(self):
    href = 'http://example.com/cool.kml'
    self.link.set_href(href)
    assert self.link.has_href()
    assert href == self.link.href()
    self.link.clear_href()
    # Clearing everything returns to default state
    self.testDefault()


class SimpleChangeTestCase(unittest.TestCase):
  """ This tests the Change element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.change = self.factory.CreateChange()

  def tearDown(self):
    self.factory.DeleteElement(self.change)

  def testDefault(self):
    assert kmldom.AsChange(self.change)
    assert 0 == self.change.object_array_size()

  def testAddOneObject(self):
    placemark = self.factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    self.change.add_object(placemark)
    assert 1 == self.change.object_array_size()
    placemark = kmldom.AsPlacemark(self.change.object_array_at(0))
    assert name == placemark.name()

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
    assert 3 == self.change.object_array_size()
    assert placemark_id == self.change.object_array_at(0).id()
    assert latlonbox_id == self.change.object_array_at(1).id()
    assert location_id == self.change.object_array_at(2).id()


class SimpleCreateTestCase(unittest.TestCase):
  """ This tests the Create element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.create = self.factory.CreateCreate()

  def tearDown(self):
    self.factory.DeleteElement(self.create)

  def testDefault(self):
    assert kmldom.AsCreate(self.create)
    assert 0 == self.create.container_array_size()

  def testAddOneContainer(self):
    folder = self.factory.CreateFolder()
    target_folder_id = 'target-folder'
    folder.set_targetid(target_folder_id)
    self.create.add_container(folder)
    assert 1 == self.create.container_array_size()
    folder = kmldom.AsFolder(self.create.container_array_at(0))
    assert target_folder_id == folder.targetid()

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
    assert 4 == self.create.container_array_size()
    assert folder_a_id == self.create.container_array_at(0).id()
    assert document_a_id == self.create.container_array_at(1).id()
    assert document_b_id == self.create.container_array_at(2).id()
    assert folder_b_id == self.create.container_array_at(3).id()


class SimpleDeleteTestCase(unittest.TestCase):
  """ This tests the Delete element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.delete = self.factory.CreateDelete()

  def tearDown(self):
    self.factory.DeleteElement(self.delete)

  def testDefault(self):
    assert kmldom.AsDelete(self.delete)
    assert 0 == self.delete.feature_array_size()

  def testAddOneFeature(self):
    groundoverlay = self.factory.CreateGroundOverlay()
    target_groundoverlay_id = 'target-groundoverlay'
    groundoverlay.set_targetid(target_groundoverlay_id)
    self.delete.add_feature(groundoverlay)
    assert 1 == self.delete.feature_array_size()
    groundoverlay = kmldom.AsGroundOverlay(self.delete.feature_array_at(0))
    assert target_groundoverlay_id == groundoverlay.targetid()

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
    assert 7 == self.delete.feature_array_size()
    assert document_id == self.delete.feature_array_at(0).id()
    assert folder_id == self.delete.feature_array_at(1).id()
    assert groundoverlay_id == self.delete.feature_array_at(2).id()
    assert networklink_id == self.delete.feature_array_at(3).id()
    assert placemark_id == self.delete.feature_array_at(4).id()
    assert photooverlay_id == self.delete.feature_array_at(5).id()
    assert screenoverlay_id == self.delete.feature_array_at(6).id()


class SimpleDocumentTestCase(unittest.TestCase):
  """ This tests the Document element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.document = self.factory.CreateDocument()

  def tearDown(self):
    self.factory.DeleteElement(self.document)

  def testDefault(self):
    assert kmldom.AsObject(self.document)
    assert kmldom.AsFeature(self.document)
    assert kmldom.AsContainer(self.document)
    assert kmldom.AsDocument(self.document)
    assert not self.document.has_id()  # Object-ness
    assert not self.document.has_name()  # Feature-ness
    assert 0 == self.document.feature_array_size()  # Container-ness
    assert 0 == self.document.schema_array_size()  # Document-ness
    assert 0 == self.document.styleselector_array_size()  # Document-ness


class SimpleMultiGeometryTestCase(unittest.TestCase):
  """  This tests the MultiGeometry element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.multigeometry = self.factory.CreateMultiGeometry()

  def tearDown(self):
    self.factory.DeleteElement(self.multigeometry)

  def testDefault(self):
    assert kmldom.AsObject(self.multigeometry)
    assert kmldom.AsGeometry(self.multigeometry)
    assert kmldom.AsMultiGeometry(self.multigeometry)
    assert not self.multigeometry.has_id()  # Object-ness
    assert 0 == self.multigeometry.geometry_array_size()  # MultiGeometry-ness


class SimpleOuterBoundaryIsTestCase(unittest.TestCase):
  """  TODO: This tests the OuterBoundaryIs element."""


class SimpleInnerBoundaryIsTestCase(unittest.TestCase):
  """  TODO: This tests the InnerBoundaryIs element."""


class SimplePolygonTestCase(unittest.TestCase):
  """  TODO: This tests the Polygon element."""


class SimpleResourceMapTestCase(unittest.TestCase):
  """ This tests the ResourceMap element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.resourcemap = self.factory.CreateResourceMap()

  def tearDown(self):
    self.factory.DeleteElement(self.resourcemap)

  def testDefault(self):
    assert kmldom.AsObject(self.resourcemap)
    assert kmldom.AsResourceMap(self.resourcemap)
    assert not self.resourcemap.has_id()
    assert 0 == self.resourcemap.alias_array_size()


class SimpleSchemaTestCase(unittest.TestCase):
  """ This tests the Schema element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.schema = self.factory.CreateSchema()

  def tearDown(self):
    self.factory.DeleteElement(self.schema)

  def testDefault(self):
    assert kmldom.AsSchema(self.schema)
    assert not self.schema.has_name()
    assert not self.schema.has_id()
    assert 0 == self.schema.simplefield_array_size()


class SimpleSchemaDataTestCase(unittest.TestCase):
  """ This tests the SchemaData element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.schemadata = self.factory.CreateSchemaData()

  def tearDown(self):
    self.factory.DeleteElement(self.schemadata)

  def testDefault(self):
    assert kmldom.AsObject(self.schemadata)
    assert kmldom.AsSchemaData(self.schemadata)
    assert not self.schemadata.has_id()
    assert 0 == self.schemadata.simpledata_array_size()


class SimpleSimpleFieldTestCase(unittest.TestCase):
  """ This tests the SimpleField element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.simplefield = self.factory.CreateSimpleField()

  def tearDown(self):
    self.factory.DeleteElement(self.simplefield)

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

  def tearDown(self):
    self.factory.DeleteElement(self.update)

  def testDefault(self):
    assert kmldom.AsUpdate(self.update)
    assert 0 == self.update.updateoperation_array_size()


class SimpleStyleMapTestCase(unittest.TestCase):
  """  TODO: This tests the StyleMap element."""

class SimpleIconStyleIconTestCase(unittest.TestCase):
  """ This tests the IconStyleIcon element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.iconstyleicon= self.factory.CreateIconStyleIcon()

  def tearDown(self):
    self.factory.DeleteElement(self.iconstyleicon)

  def testBasic(self):
    assert not self.iconstyleicon.has_href() # default state
    assert '' == self.iconstyleicon.href()
    href = 'http://blah.blah'
    self.iconstyleicon.set_href(href)
    assert self.iconstyleicon.has_href()
    assert href == self.iconstyleicon.href()
    self.iconstyleicon.clear_href()
    assert not self.iconstyleicon.has_href() # back in default state
    assert '' == self.iconstyleicon.href()


def suite():
  suite = unittest.TestSuite()
  suite.addTest(SimpleParseTestCase('testBasic'))
  suite.addTest(SimpleSerializePrettyTestCase('testBasic'))
  suite.addTest(SimpleSerializeRawTestCase('testBasic'))
  suite.addTest(SimpleExtendedDataTestCase('testDefault'))
  suite.addTest(SimpleListStyleTestCase('testDefault'))
  suite.addTest(SimplePhotoOverlayTestCase('testDefault'))
  suite.addTest(SimplePlacemarkTestCase('testDefault'))
  suite.addTest(SimplePlacemarkTestCase('testName'))
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
