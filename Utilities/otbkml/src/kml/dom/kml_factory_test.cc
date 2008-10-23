// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "kml/dom/kml_factory.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class KmlFactoryTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlFactoryTest);
  CPPUNIT_TEST(TestFactory);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestFactory();
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlFactoryTest);

void KmlFactoryTest::TestFactory() {
  KmlFactory* factory(KmlFactory::GetFactory());

  // CreateElementById returns an ElementPtr which automatically
  // releases the underlying Element storage when it goes
  // out of scope.

  // Verify functions used by parser to create any KML element.
  KmlDomType id = Type_kml;

  // CreateElementById() new's a Element and returns an ElementPtr
  // dereferenced here to access Type().  When this expression
  // goes out of scope ElementPtr's destructor delete's the Element.
  CPPUNIT_ASSERT(Type_kml == factory->CreateElementById(id)->Type());

  ElementPtr e;
  e = factory->CreateFieldById(id);
  CPPUNIT_ASSERT(Type_kml == e->Type());

  // Verify all factory methods return correct type.
  e = factory->CreateAlias();
  CPPUNIT_ASSERT(Type_Alias == e->Type());

  e = factory->CreateBalloonStyle();
  CPPUNIT_ASSERT(Type_BalloonStyle == e->Type());

  e = factory->CreateCamera();
  CPPUNIT_ASSERT(Type_Camera == e->Type());

  e = factory->CreateChange();
  CPPUNIT_ASSERT(Type_Change == e->Type());

  e = factory->CreateCreate();
  CPPUNIT_ASSERT(Type_Create == e->Type());

  e = factory->CreateCoordinates();
  CPPUNIT_ASSERT(Type_coordinates == e->Type());

  e = factory->CreateData();
  CPPUNIT_ASSERT(Type_Data == e->Type());

  e = factory->CreateDelete();
  CPPUNIT_ASSERT(Type_Delete == e->Type());

  e = factory->CreateDocument();
  CPPUNIT_ASSERT(Type_Document == e->Type());

  e = factory->CreateExtendedData();
  CPPUNIT_ASSERT(Type_ExtendedData == e->Type());

  e = factory->CreateFolder();
  CPPUNIT_ASSERT(Type_Folder == e->Type());

  e = factory->CreateGroundOverlay();
  CPPUNIT_ASSERT(Type_GroundOverlay == e->Type());

  e = factory->CreateHotSpot();
  CPPUNIT_ASSERT(Type_hotSpot == e->Type());

  e = factory->CreateIcon();
  CPPUNIT_ASSERT(Type_Icon == e->Type());

  e = factory->CreateIconStyle();
  CPPUNIT_ASSERT(Type_IconStyle == e->Type());

  e = factory->CreateIconStyleIcon();
  CPPUNIT_ASSERT(Type_IconStyleIcon == e->Type());

  e = factory->CreateImagePyramid();
  CPPUNIT_ASSERT(Type_ImagePyramid == e->Type());

  e = factory->CreateItemIcon();
  CPPUNIT_ASSERT(Type_ItemIcon == e->Type());

  e = factory->CreateKml();
  CPPUNIT_ASSERT(Type_kml == e->Type());

  e = factory->CreateLabelStyle();
  CPPUNIT_ASSERT(Type_LabelStyle == e->Type());

  e = factory->CreateLatLonBox();
  CPPUNIT_ASSERT(Type_LatLonBox == e->Type());

  e = factory->CreateLatLonAltBox();
  CPPUNIT_ASSERT(Type_LatLonAltBox == e->Type());

  e = factory->CreateLinearRing();
  CPPUNIT_ASSERT(Type_LinearRing == e->Type());

  e = factory->CreateLineString();
  CPPUNIT_ASSERT(Type_LineString == e->Type());

  e = factory->CreateLineStyle();
  CPPUNIT_ASSERT(Type_LineStyle == e->Type());

  e = factory->CreateLink();
  CPPUNIT_ASSERT(Type_Link == e->Type());

  e = factory->CreateLinkSnippet();
  CPPUNIT_ASSERT(Type_linkSnippet == e->Type());

  e = factory->CreateListStyle();
  CPPUNIT_ASSERT(Type_ListStyle == e->Type());

  e = factory->CreateLocation();
  CPPUNIT_ASSERT(Type_Location == e->Type());

  e = factory->CreateLod();
  CPPUNIT_ASSERT(Type_Lod == e->Type());

  e = factory->CreateLookAt();
  CPPUNIT_ASSERT(Type_LookAt == e->Type());

  e = factory->CreateModel();
  CPPUNIT_ASSERT(Type_Model == e->Type());

  e = factory->CreateMultiGeometry();
  CPPUNIT_ASSERT(Type_MultiGeometry == e->Type());

  e = factory->CreateNetworkLink();
  CPPUNIT_ASSERT(Type_NetworkLink == e->Type());

  e = factory->CreateNetworkLinkControl();
  CPPUNIT_ASSERT(Type_NetworkLinkControl == e->Type());

  e = factory->CreateOrientation();
  CPPUNIT_ASSERT(Type_Orientation == e->Type());

  e = factory->CreateOuterBoundaryIs();
  CPPUNIT_ASSERT(Type_outerBoundaryIs == e->Type());

  e = factory->CreateOverlayXY();
  CPPUNIT_ASSERT(Type_overlayXY == e->Type());

  e = factory->CreatePair();
  CPPUNIT_ASSERT(Type_Pair == e->Type());

  e = factory->CreatePhotoOverlay();
  CPPUNIT_ASSERT(Type_PhotoOverlay == e->Type());

  e = factory->CreatePlacemark();
  CPPUNIT_ASSERT(Type_Placemark == e->Type());

  e = factory->CreatePolygon();
  CPPUNIT_ASSERT(Type_Polygon == e->Type());

  e = factory->CreatePoint();
  CPPUNIT_ASSERT(Type_Point == e->Type());

  e = factory->CreatePolyStyle();
  CPPUNIT_ASSERT(Type_PolyStyle == e->Type());

  e = factory->CreateRegion();
  CPPUNIT_ASSERT(Type_Region == e->Type());

  e = factory->CreateResourceMap();
  CPPUNIT_ASSERT(Type_ResourceMap == e->Type());

  e = factory->CreateRotationXY();
  CPPUNIT_ASSERT(Type_rotationXY == e->Type());

  e = factory->CreateScale();
  CPPUNIT_ASSERT(Type_Scale == e->Type());

  e = factory->CreateSchema();
  CPPUNIT_ASSERT(Type_Schema == e->Type());

  e = factory->CreateSchemaData();
  CPPUNIT_ASSERT(Type_SchemaData == e->Type());

  e = factory->CreateScreenOverlay();
  CPPUNIT_ASSERT(Type_ScreenOverlay == e->Type());

  e = factory->CreateScreenXY();
  CPPUNIT_ASSERT(Type_screenXY == e->Type());

  e = factory->CreateSimpleData();
  CPPUNIT_ASSERT(Type_SimpleData == e->Type());

  e = factory->CreateSimpleField();
  CPPUNIT_ASSERT(Type_SimpleField == e->Type());

  e = factory->CreateSize();
  CPPUNIT_ASSERT(Type_size == e->Type());

  e = factory->CreateSnippet();
  CPPUNIT_ASSERT(Type_Snippet == e->Type());

  e = factory->CreateStyle();
  CPPUNIT_ASSERT(Type_Style == e->Type());

  e = factory->CreateStyleMap();
  CPPUNIT_ASSERT(Type_StyleMap == e->Type());

  e = factory->CreateTimeSpan();
  CPPUNIT_ASSERT(Type_TimeSpan == e->Type());

  e = factory->CreateTimeStamp();
  CPPUNIT_ASSERT(Type_TimeStamp == e->Type());

  e = factory->CreateViewVolume();
  CPPUNIT_ASSERT(Type_ViewVolume == e->Type());

  e = factory->CreateUpdate();
  CPPUNIT_ASSERT(Type_Update == e->Type());

  e = factory->CreateUrl();
  CPPUNIT_ASSERT(Type_Url == e->Type());
}

}  // end namespace kmldom

TEST_MAIN
