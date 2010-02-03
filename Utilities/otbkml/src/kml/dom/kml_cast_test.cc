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

#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class KmlCastTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlCastTest);
  CPPUNIT_TEST(TestCasts);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestCasts();
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlCastTest);

void KmlCastTest::TestCasts() {
  KmlFactory* factory = KmlFactory::GetFactory();

  // The temporary ElementPtr holds a reference to the created Element
  // which is released when the As*() goes out of scope.
  CPPUNIT_ASSERT(AsAbstractLatLonBox(
      factory->CreateElementById(Type_LatLonBox)));
  CPPUNIT_ASSERT(AsAbstractLatLonBox(
      factory->CreateElementById(Type_LatLonAltBox)));
  CPPUNIT_ASSERT(AsAbstractView(factory->CreateElementById(Type_LookAt)));
  CPPUNIT_ASSERT(AsColorStyle(factory->CreateElementById(Type_IconStyle)));
  CPPUNIT_ASSERT(AsContainer(factory->CreateElementById(Type_Folder)));
  CPPUNIT_ASSERT(AsFeature(factory->CreateElementById(Type_Placemark)));
  CPPUNIT_ASSERT(AsGeometry(factory->CreateElementById(Type_Point)));
  CPPUNIT_ASSERT(AsObject(factory->CreateElementById(Type_Placemark)));
  CPPUNIT_ASSERT(AsOverlay(factory->CreateElementById(Type_GroundOverlay)));
  CPPUNIT_ASSERT(AsStyleSelector(factory->CreateElementById(Type_Style)));
  CPPUNIT_ASSERT(AsSubStyle(factory->CreateElementById(Type_BalloonStyle)));
  CPPUNIT_ASSERT(AsTimePrimitive(factory->CreateElementById(Type_TimeSpan)));
  CPPUNIT_ASSERT(AsAlias(factory->CreateElementById(Type_Alias)));
  CPPUNIT_ASSERT(AsBalloonStyle(factory->CreateElementById(Type_BalloonStyle)));
  CPPUNIT_ASSERT(AsCamera(factory->CreateElementById(Type_Camera)));
  CPPUNIT_ASSERT(AsChange(factory->CreateElementById(Type_Change)));
  CPPUNIT_ASSERT(AsCoordinates(factory->CreateElementById(Type_coordinates)));
  CPPUNIT_ASSERT(AsCreate(factory->CreateElementById(Type_Create)));
  CPPUNIT_ASSERT(AsData(factory->CreateElementById(Type_Data)));
  CPPUNIT_ASSERT(AsDelete(factory->CreateElementById(Type_Delete)));
  CPPUNIT_ASSERT(AsDocument(factory->CreateElementById(Type_Document)));
  CPPUNIT_ASSERT(AsExtendedData(factory->CreateElementById(Type_ExtendedData)));
  CPPUNIT_ASSERT(AsFolder(factory->CreateElementById(Type_Folder)));
  CPPUNIT_ASSERT(AsGroundOverlay(
      factory->CreateElementById(Type_GroundOverlay)));
  CPPUNIT_ASSERT(AsHotSpot(factory->CreateElementById(Type_hotSpot)));
  CPPUNIT_ASSERT(AsIcon(factory->CreateElementById(Type_Icon)));
  CPPUNIT_ASSERT(AsIconStyle(factory->CreateElementById(Type_IconStyle)));
  CPPUNIT_ASSERT(AsImagePyramid(factory->CreateElementById(Type_ImagePyramid)));
  CPPUNIT_ASSERT(AsInnerBoundaryIs(
      factory->CreateElementById(Type_innerBoundaryIs)));
  CPPUNIT_ASSERT(AsItemIcon(factory->CreateElementById(Type_ItemIcon)));
  CPPUNIT_ASSERT(AsLabelStyle(factory->CreateElementById(Type_LabelStyle)));
  CPPUNIT_ASSERT(AsLatLonAltBox(factory->CreateElementById(Type_LatLonAltBox)));
  CPPUNIT_ASSERT(AsLatLonBox(factory->CreateElementById(Type_LatLonBox)));
  CPPUNIT_ASSERT(AsLineString(factory->CreateElementById(Type_LineString)));
  CPPUNIT_ASSERT(AsLineStyle(factory->CreateElementById(Type_LineStyle)));
  CPPUNIT_ASSERT(AsLinearRing(factory->CreateElementById(Type_LinearRing)));
  CPPUNIT_ASSERT(AsLink(factory->CreateElementById(Type_Link)));
  CPPUNIT_ASSERT(AsLinkSnippet(factory->CreateElementById(Type_linkSnippet)));
  CPPUNIT_ASSERT(AsListStyle(factory->CreateElementById(Type_ListStyle)));
  CPPUNIT_ASSERT(AsLocation(factory->CreateElementById(Type_Location)));
  CPPUNIT_ASSERT(AsLod(factory->CreateElementById(Type_Lod)));
  CPPUNIT_ASSERT(AsLookAt(factory->CreateElementById(Type_LookAt)));
  CPPUNIT_ASSERT(AsKml(factory->CreateElementById(Type_kml)));
  CPPUNIT_ASSERT(AsModel(factory->CreateElementById(Type_Model)));
  CPPUNIT_ASSERT(AsMultiGeometry(
      factory->CreateElementById(Type_MultiGeometry)));
  CPPUNIT_ASSERT(AsNetworkLink(factory->CreateElementById(Type_NetworkLink)));
  CPPUNIT_ASSERT(AsNetworkLinkControl(
          factory->CreateElementById(Type_NetworkLinkControl)));
  CPPUNIT_ASSERT(AsOrientation(factory->CreateElementById(Type_Orientation)));
  CPPUNIT_ASSERT(AsOuterBoundaryIs(
      factory->CreateElementById(Type_outerBoundaryIs)));
  CPPUNIT_ASSERT(AsOverlayXY(factory->CreateElementById(Type_overlayXY)));
  CPPUNIT_ASSERT(AsPair(factory->CreateElementById(Type_Pair)));
  CPPUNIT_ASSERT(AsPhotoOverlay(factory->CreateElementById(Type_PhotoOverlay)));
  CPPUNIT_ASSERT(AsPlacemark(factory->CreateElementById(Type_Placemark)));
  CPPUNIT_ASSERT(AsPoint(factory->CreateElementById(Type_Point)));
  CPPUNIT_ASSERT(AsPolyStyle(factory->CreateElementById(Type_PolyStyle)));
  CPPUNIT_ASSERT(AsPolygon(factory->CreateElementById(Type_Polygon)));
  CPPUNIT_ASSERT(AsRegion(factory->CreateElementById(Type_Region)));
  CPPUNIT_ASSERT(AsResourceMap(factory->CreateElementById(Type_ResourceMap)));
  CPPUNIT_ASSERT(AsRotationXY(factory->CreateElementById(Type_rotationXY)));
  CPPUNIT_ASSERT(AsScale(factory->CreateElementById(Type_Scale)));
  CPPUNIT_ASSERT(AsSchema(factory->CreateElementById(Type_Schema)));
  CPPUNIT_ASSERT(AsSchemaData(factory->CreateElementById(Type_SchemaData)));
  CPPUNIT_ASSERT(AsScreenOverlay(
     factory->CreateElementById(Type_ScreenOverlay)));
  CPPUNIT_ASSERT(AsScreenXY(factory->CreateElementById(Type_screenXY)));
  CPPUNIT_ASSERT(AsSimpleData(factory->CreateElementById(Type_SimpleData)));
  CPPUNIT_ASSERT(AsSimpleField(factory->CreateElementById(Type_SimpleField)));
  CPPUNIT_ASSERT(AsSize(factory->CreateElementById(Type_size)));
  CPPUNIT_ASSERT(AsSnippet(factory->CreateElementById(Type_Snippet)));
  CPPUNIT_ASSERT(AsStyle(factory->CreateElementById(Type_Style)));
  CPPUNIT_ASSERT(AsStyleMap(factory->CreateElementById(Type_StyleMap)));
  CPPUNIT_ASSERT(AsTimeSpan(factory->CreateElementById(Type_TimeSpan)));
  CPPUNIT_ASSERT(AsTimeStamp(factory->CreateElementById(Type_TimeStamp)));
  CPPUNIT_ASSERT(AsUpdate(factory->CreateElementById(Type_Update)));
  CPPUNIT_ASSERT(AsViewVolume(factory->CreateElementById(Type_ViewVolume)));
}

}  // end namespace kmldom

TEST_MAIN
