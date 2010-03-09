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
#include "gtest/gtest.h"

namespace kmldom {

class KmlCastTest : public testing::Test {
};

TEST_F(KmlCastTest, TestCasts) {
  KmlFactory* factory = KmlFactory::GetFactory();

  // The temporary ElementPtr holds a reference to the created Element
  // which is released when the As*() goes out of scope.
  ASSERT_TRUE(AsAbstractLatLonBox(factory->CreateElementById(Type_LatLonBox)));
  ASSERT_TRUE(AsAbstractLatLonBox(
      factory->CreateElementById(Type_LatLonAltBox)));
  ASSERT_TRUE(AsAbstractView(factory->CreateElementById(Type_LookAt)));
  ASSERT_TRUE(AsColorStyle(factory->CreateElementById(Type_IconStyle)));
  ASSERT_TRUE(AsContainer(factory->CreateElementById(Type_Folder)));
  ASSERT_TRUE(AsFeature(factory->CreateElementById(Type_Placemark)));
  ASSERT_TRUE(AsGeometry(factory->CreateElementById(Type_Point)));
  ASSERT_TRUE(AsObject(factory->CreateElementById(Type_Placemark)));
  ASSERT_TRUE(AsOverlay(factory->CreateElementById(Type_GroundOverlay)));
  ASSERT_TRUE(AsStyleSelector(factory->CreateElementById(Type_Style)));
  ASSERT_TRUE(AsSubStyle(factory->CreateElementById(Type_BalloonStyle)));
  ASSERT_TRUE(AsTimePrimitive(factory->CreateElementById(Type_TimeSpan)));
  ASSERT_TRUE(AsAlias(factory->CreateElementById(Type_Alias)));
  ASSERT_TRUE(AsAtomAuthor(factory->CreateElementById(Type_AtomAuthor)));
  ASSERT_TRUE(AsAtomContent(factory->CreateElementById(Type_AtomContent)));
  ASSERT_TRUE(AsAtomEntry(factory->CreateElementById(Type_AtomEntry)));
  ASSERT_TRUE(AsAtomFeed(factory->CreateElementById(Type_AtomFeed)));
  ASSERT_TRUE(AsAtomLink(factory->CreateElementById(Type_AtomLink)));
  ASSERT_TRUE(AsBalloonStyle(factory->CreateElementById(Type_BalloonStyle)));
  ASSERT_TRUE(AsCamera(factory->CreateElementById(Type_Camera)));
  ASSERT_TRUE(AsChange(factory->CreateElementById(Type_Change)));
  ASSERT_TRUE(AsCoordinates(factory->CreateElementById(Type_coordinates)));
  ASSERT_TRUE(AsCreate(factory->CreateElementById(Type_Create)));
  ASSERT_TRUE(AsData(factory->CreateElementById(Type_Data)));
  ASSERT_TRUE(AsDelete(factory->CreateElementById(Type_Delete)));
  ASSERT_TRUE(AsDocument(factory->CreateElementById(Type_Document)));
  ASSERT_TRUE(AsExtendedData(factory->CreateElementById(Type_ExtendedData)));
  ASSERT_TRUE(AsFolder(factory->CreateElementById(Type_Folder)));
  ASSERT_TRUE(AsGroundOverlay(
      factory->CreateElementById(Type_GroundOverlay)));
  ASSERT_TRUE(AsHotSpot(factory->CreateElementById(Type_hotSpot)));
  ASSERT_TRUE(AsIcon(factory->CreateElementById(Type_Icon)));
  ASSERT_TRUE(AsIconStyle(factory->CreateElementById(Type_IconStyle)));
  ASSERT_TRUE(AsImagePyramid(factory->CreateElementById(Type_ImagePyramid)));
  ASSERT_TRUE(AsInnerBoundaryIs(
      factory->CreateElementById(Type_innerBoundaryIs)));
  ASSERT_TRUE(AsItemIcon(factory->CreateElementById(Type_ItemIcon)));
  ASSERT_TRUE(AsLabelStyle(factory->CreateElementById(Type_LabelStyle)));
  ASSERT_TRUE(AsLatLonAltBox(factory->CreateElementById(Type_LatLonAltBox)));
  ASSERT_TRUE(AsLatLonBox(factory->CreateElementById(Type_LatLonBox)));
  ASSERT_TRUE(AsLineString(factory->CreateElementById(Type_LineString)));
  ASSERT_TRUE(AsLineStyle(factory->CreateElementById(Type_LineStyle)));
  ASSERT_TRUE(AsLinearRing(factory->CreateElementById(Type_LinearRing)));
  ASSERT_TRUE(AsLink(factory->CreateElementById(Type_Link)));
  ASSERT_TRUE(AsLinkSnippet(factory->CreateElementById(Type_linkSnippet)));
  ASSERT_TRUE(AsListStyle(factory->CreateElementById(Type_ListStyle)));
  ASSERT_TRUE(AsLocation(factory->CreateElementById(Type_Location)));
  ASSERT_TRUE(AsLod(factory->CreateElementById(Type_Lod)));
  ASSERT_TRUE(AsLookAt(factory->CreateElementById(Type_LookAt)));
  ASSERT_TRUE(AsKml(factory->CreateElementById(Type_kml)));
  ASSERT_TRUE(AsModel(factory->CreateElementById(Type_Model)));
  ASSERT_TRUE(AsMultiGeometry(
      factory->CreateElementById(Type_MultiGeometry)));
  ASSERT_TRUE(AsNetworkLink(factory->CreateElementById(Type_NetworkLink)));
  ASSERT_TRUE(AsNetworkLinkControl(
      factory->CreateElementById(Type_NetworkLinkControl)));
  ASSERT_TRUE(AsOrientation(factory->CreateElementById(Type_Orientation)));
  ASSERT_TRUE(AsOuterBoundaryIs(
      factory->CreateElementById(Type_outerBoundaryIs)));
  ASSERT_TRUE(AsOverlayXY(factory->CreateElementById(Type_overlayXY)));
  ASSERT_TRUE(AsPair(factory->CreateElementById(Type_Pair)));
  ASSERT_TRUE(AsPhotoOverlay(factory->CreateElementById(Type_PhotoOverlay)));
  ASSERT_TRUE(AsPlacemark(factory->CreateElementById(Type_Placemark)));
  ASSERT_TRUE(AsPoint(factory->CreateElementById(Type_Point)));
  ASSERT_TRUE(AsPolyStyle(factory->CreateElementById(Type_PolyStyle)));
  ASSERT_TRUE(AsPolygon(factory->CreateElementById(Type_Polygon)));
  ASSERT_TRUE(AsRegion(factory->CreateElementById(Type_Region)));
  ASSERT_TRUE(AsResourceMap(factory->CreateElementById(Type_ResourceMap)));
  ASSERT_TRUE(AsRotationXY(factory->CreateElementById(Type_rotationXY)));
  ASSERT_TRUE(AsScale(factory->CreateElementById(Type_Scale)));
  ASSERT_TRUE(AsSchema(factory->CreateElementById(Type_Schema)));
  ASSERT_TRUE(AsSchemaData(factory->CreateElementById(Type_SchemaData)));
  ASSERT_TRUE(AsScreenOverlay(
      factory->CreateElementById(Type_ScreenOverlay)));
  ASSERT_TRUE(AsScreenXY(factory->CreateElementById(Type_screenXY)));
  ASSERT_TRUE(AsSimpleData(factory->CreateElementById(Type_SimpleData)));
  ASSERT_TRUE(AsSimpleField(factory->CreateElementById(Type_SimpleField)));
  ASSERT_TRUE(AsSize(factory->CreateElementById(Type_size)));
  ASSERT_TRUE(AsSnippet(factory->CreateElementById(Type_Snippet)));
  ASSERT_TRUE(AsStyle(factory->CreateElementById(Type_Style)));
  ASSERT_TRUE(AsStyleMap(factory->CreateElementById(Type_StyleMap)));
  ASSERT_TRUE(AsTimeSpan(factory->CreateElementById(Type_TimeSpan)));
  ASSERT_TRUE(AsTimeStamp(factory->CreateElementById(Type_TimeStamp)));
  ASSERT_TRUE(AsUpdate(factory->CreateElementById(Type_Update)));
  ASSERT_TRUE(AsViewVolume(factory->CreateElementById(Type_ViewVolume)));

  ASSERT_TRUE(AsXalAddressDetails(
      factory->CreateElementById(Type_XalAddressDetails)));
  ASSERT_TRUE(AsXalAdministrativeArea(
      factory->CreateElementById(Type_XalAdministrativeArea)));
  ASSERT_TRUE(AsXalCountry(factory->CreateElementById(Type_XalCountry)));
  ASSERT_TRUE(AsXalLocality(factory->CreateElementById(Type_XalLocality)));
  ASSERT_TRUE(AsXalPostalCode(factory->CreateElementById(Type_XalPostalCode)));
  ASSERT_TRUE(AsXalSubAdministrativeArea(
      factory->CreateElementById(Type_XalSubAdministrativeArea)));
  ASSERT_TRUE(AsXalThoroughfare(
      factory->CreateElementById(Type_XalThoroughfare)));

  ASSERT_TRUE(AsGxAnimatedUpdate(
      factory->CreateElementById(Type_GxAnimatedUpdate)));
  ASSERT_TRUE(AsGxFlyTo(
      factory->CreateElementById(Type_GxFlyTo)));
  ASSERT_TRUE(AsGxLatLonQuad(
      factory->CreateElementById(Type_GxLatLonQuad)));
  ASSERT_TRUE(AsGxPlaylist(
      factory->CreateElementById(Type_GxPlaylist)));
  ASSERT_TRUE(AsGxSoundCue(
      factory->CreateElementById(Type_GxSoundCue)));
  ASSERT_TRUE(AsGxTimeSpan(
      factory->CreateElementById(Type_GxTimeSpan)));
  ASSERT_TRUE(AsGxTimeStamp(
      factory->CreateElementById(Type_GxTimeStamp)));
  ASSERT_TRUE(AsTimePrimitive(
      factory->CreateElementById(Type_GxTimeSpan)));
  ASSERT_TRUE(AsTimePrimitive(
      factory->CreateElementById(Type_GxTimeStamp)));
  ASSERT_TRUE(AsGxTour(
      factory->CreateElementById(Type_GxTour)));
  ASSERT_TRUE(AsGxTourControl(
      factory->CreateElementById(Type_GxTourControl)));
  ASSERT_TRUE(AsGxTourPrimitive(
      factory->CreateElementById(Type_GxFlyTo)));
  ASSERT_TRUE(AsGxWait(
      factory->CreateElementById(Type_GxWait)));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
