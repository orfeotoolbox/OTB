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
#include "gtest/gtest.h"

namespace kmldom {

TEST(KmlFactoryTest, TestFactory) {
  KmlFactory* factory(KmlFactory::GetFactory());

  // CreateElementById returns an ElementPtr which automatically
  // releases the underlying Element storage when it goes
  // out of scope.

  // Verify functions used by parser to create any KML element.
  KmlDomType id = Type_kml;

  // CreateElementById() new's a Element and returns an ElementPtr
  // dereferenced here to access Type().  When this expression
  // goes out of scope ElementPtr's destructor delete's the Element.
  ASSERT_EQ(Type_kml, factory->CreateElementById(id)->Type());

  ElementPtr e;
  e = factory->CreateFieldById(id);
  ASSERT_EQ(Type_kml, e->Type());

  // Verify all factory methods return correct type.
  e = factory->CreateAlias();
  ASSERT_EQ(Type_Alias, e->Type());

  e = factory->CreateAtomAuthor();
  ASSERT_EQ(Type_AtomAuthor, e->Type());

  e = factory->CreateAtomCategory();
  ASSERT_EQ(Type_AtomCategory, e->Type());

  e = factory->CreateAtomContent();
  ASSERT_EQ(Type_AtomContent, e->Type());

  e = factory->CreateAtomEntry();
  ASSERT_EQ(Type_AtomEntry, e->Type());

  e = factory->CreateAtomFeed();
  ASSERT_EQ(Type_AtomFeed, e->Type());

  e = factory->CreateAtomLink();
  ASSERT_EQ(Type_AtomLink, e->Type());

  e = factory->CreateBalloonStyle();
  ASSERT_EQ(Type_BalloonStyle, e->Type());

  e = factory->CreateCamera();
  ASSERT_EQ(Type_Camera, e->Type());

  e = factory->CreateChange();
  ASSERT_EQ(Type_Change, e->Type());

  e = factory->CreateCreate();
  ASSERT_EQ(Type_Create, e->Type());

  e = factory->CreateCoordinates();
  ASSERT_EQ(Type_coordinates, e->Type());

  e = factory->CreateData();
  ASSERT_EQ(Type_Data, e->Type());

  e = factory->CreateDelete();
  ASSERT_EQ(Type_Delete, e->Type());

  e = factory->CreateDocument();
  ASSERT_EQ(Type_Document, e->Type());

  e = factory->CreateExtendedData();
  ASSERT_EQ(Type_ExtendedData, e->Type());

  e = factory->CreateFolder();
  ASSERT_EQ(Type_Folder, e->Type());

  e = factory->CreateGroundOverlay();
  ASSERT_EQ(Type_GroundOverlay, e->Type());

  e = factory->CreateHotSpot();
  ASSERT_EQ(Type_hotSpot, e->Type());

  e = factory->CreateIcon();
  ASSERT_EQ(Type_Icon, e->Type());

  e = factory->CreateIconStyle();
  ASSERT_EQ(Type_IconStyle, e->Type());

  e = factory->CreateIconStyleIcon();
  ASSERT_EQ(Type_IconStyleIcon, e->Type());

  e = factory->CreateImagePyramid();
  ASSERT_EQ(Type_ImagePyramid, e->Type());

  e = factory->CreateItemIcon();
  ASSERT_EQ(Type_ItemIcon, e->Type());

  e = factory->CreateKml();
  ASSERT_EQ(Type_kml, e->Type());

  e = factory->CreateLabelStyle();
  ASSERT_EQ(Type_LabelStyle, e->Type());

  e = factory->CreateLatLonBox();
  ASSERT_EQ(Type_LatLonBox, e->Type());

  e = factory->CreateLatLonAltBox();
  ASSERT_EQ(Type_LatLonAltBox, e->Type());

  e = factory->CreateLinearRing();
  ASSERT_EQ(Type_LinearRing, e->Type());

  e = factory->CreateLineString();
  ASSERT_EQ(Type_LineString, e->Type());

  e = factory->CreateLineStyle();
  ASSERT_EQ(Type_LineStyle, e->Type());

  e = factory->CreateLink();
  ASSERT_EQ(Type_Link, e->Type());

  e = factory->CreateLinkSnippet();
  ASSERT_EQ(Type_linkSnippet, e->Type());

  e = factory->CreateListStyle();
  ASSERT_EQ(Type_ListStyle, e->Type());

  e = factory->CreateLocation();
  ASSERT_EQ(Type_Location, e->Type());

  e = factory->CreateLod();
  ASSERT_EQ(Type_Lod, e->Type());

  e = factory->CreateLookAt();
  ASSERT_EQ(Type_LookAt, e->Type());

  e = factory->CreateMetadata();
  ASSERT_EQ(Type_Metadata, e->Type());

  e = factory->CreateModel();
  ASSERT_EQ(Type_Model, e->Type());

  e = factory->CreateMultiGeometry();
  ASSERT_EQ(Type_MultiGeometry, e->Type());

  e = factory->CreateNetworkLink();
  ASSERT_EQ(Type_NetworkLink, e->Type());

  e = factory->CreateNetworkLinkControl();
  ASSERT_EQ(Type_NetworkLinkControl, e->Type());

  e = factory->CreateOrientation();
  ASSERT_EQ(Type_Orientation, e->Type());

  e = factory->CreateOuterBoundaryIs();
  ASSERT_EQ(Type_outerBoundaryIs, e->Type());

  e = factory->CreateOverlayXY();
  ASSERT_EQ(Type_overlayXY, e->Type());

  e = factory->CreatePair();
  ASSERT_EQ(Type_Pair, e->Type());

  e = factory->CreatePhotoOverlay();
  ASSERT_EQ(Type_PhotoOverlay, e->Type());

  e = factory->CreatePlacemark();
  ASSERT_EQ(Type_Placemark, e->Type());

  e = factory->CreatePolygon();
  ASSERT_EQ(Type_Polygon, e->Type());

  e = factory->CreatePoint();
  ASSERT_EQ(Type_Point, e->Type());

  e = factory->CreatePolyStyle();
  ASSERT_EQ(Type_PolyStyle, e->Type());

  e = factory->CreateRegion();
  ASSERT_EQ(Type_Region, e->Type());

  e = factory->CreateResourceMap();
  ASSERT_EQ(Type_ResourceMap, e->Type());

  e = factory->CreateRotationXY();
  ASSERT_EQ(Type_rotationXY, e->Type());

  e = factory->CreateScale();
  ASSERT_EQ(Type_Scale, e->Type());

  e = factory->CreateSchema();
  ASSERT_EQ(Type_Schema, e->Type());

  e = factory->CreateSchemaData();
  ASSERT_EQ(Type_SchemaData, e->Type());

  e = factory->CreateScreenOverlay();
  ASSERT_EQ(Type_ScreenOverlay, e->Type());

  e = factory->CreateScreenXY();
  ASSERT_EQ(Type_screenXY, e->Type());

  e = factory->CreateSimpleData();
  ASSERT_EQ(Type_SimpleData, e->Type());

  e = factory->CreateSimpleField();
  ASSERT_EQ(Type_SimpleField, e->Type());

  e = factory->CreateSize();
  ASSERT_EQ(Type_size, e->Type());

  e = factory->CreateSnippet();
  ASSERT_EQ(Type_Snippet, e->Type());

  e = factory->CreateStyle();
  ASSERT_EQ(Type_Style, e->Type());

  e = factory->CreateStyleMap();
  ASSERT_EQ(Type_StyleMap, e->Type());

  e = factory->CreateTimeSpan();
  ASSERT_EQ(Type_TimeSpan, e->Type());

  e = factory->CreateTimeStamp();
  ASSERT_EQ(Type_TimeStamp, e->Type());

  e = factory->CreateViewVolume();
  ASSERT_EQ(Type_ViewVolume, e->Type());

  e = factory->CreateUpdate();
  ASSERT_EQ(Type_Update, e->Type());

  e = factory->CreateUrl();
  ASSERT_EQ(Type_Url, e->Type());

  e = factory->CreateXalAddressDetails();
  ASSERT_EQ(Type_XalAddressDetails, e->Type());

  e = factory->CreateXalAdministrativeArea();
  ASSERT_EQ(Type_XalAdministrativeArea, e->Type());

  e = factory->CreateXalCountry();
  ASSERT_EQ(Type_XalCountry, e->Type());

  e = factory->CreateXalSubAdministrativeArea();
  ASSERT_EQ(Type_XalSubAdministrativeArea, e->Type());

  e = factory->CreateGxAnimatedUpdate();
  ASSERT_EQ(Type_GxAnimatedUpdate, e->Type());

  e = factory->CreateGxFlyTo();
  ASSERT_EQ(Type_GxFlyTo, e->Type());

  e = factory->CreateGxLatLonQuad();
  ASSERT_EQ(Type_GxLatLonQuad, e->Type());

  e = factory->CreateGxPlaylist();
  ASSERT_EQ(Type_GxPlaylist, e->Type());

  e = factory->CreateGxSoundCue();
  ASSERT_EQ(Type_GxSoundCue, e->Type());

  e = factory->CreateGxTimeSpan();
  ASSERT_EQ(Type_GxTimeSpan, e->Type());

  e = factory->CreateGxTimeStamp();
  ASSERT_EQ(Type_GxTimeStamp, e->Type());

  e = factory->CreateGxTour();
  ASSERT_EQ(Type_GxTour, e->Type());

  e = factory->CreateGxTourControl();
  ASSERT_EQ(Type_GxTourControl, e->Type());

  e = factory->CreateGxWait();
  ASSERT_EQ(Type_GxWait, e->Type());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
