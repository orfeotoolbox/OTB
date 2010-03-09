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
#include "kml/dom/kml22.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kmldom.h"

namespace kmldom {

KmlFactory* KmlFactory::factory_ = NULL;

// static
KmlFactory* KmlFactory::GetFactory() {
  if (factory_ == NULL) {
    factory_ = new KmlFactory();
  }
  return factory_;
}

ElementPtr KmlFactory::CreateElementById(KmlDomType id) const {
  switch (id) {
  case Type_Alias: return CreateAlias();
  case Type_AtomAuthor: return CreateAtomAuthor();
  case Type_AtomCategory: return CreateAtomCategory();
  case Type_AtomContent: return CreateAtomContent();
  case Type_AtomEntry: return CreateAtomEntry();
  case Type_AtomFeed: return CreateAtomFeed();
  case Type_AtomLink: return CreateAtomLink();
  case Type_BalloonStyle: return CreateBalloonStyle();
  case Type_Camera: return CreateCamera();
  case Type_Change: return CreateChange();
  case Type_Create: return CreateCreate();
  case Type_Data: return CreateData();
  case Type_Delete: return CreateDelete();
  case Type_Document: return CreateDocument();
  case Type_ExtendedData: return CreateExtendedData();
  case Type_Folder: return CreateFolder();
  case Type_GroundOverlay: return CreateGroundOverlay();
  case Type_Icon: return CreateIcon();
  case Type_IconStyle: return CreateIconStyle();
  case Type_IconStyleIcon: return CreateIconStyleIcon();
  case Type_ImagePyramid: return CreateImagePyramid();
  case Type_ItemIcon: return CreateItemIcon();
  case Type_LabelStyle: return CreateLabelStyle();
  case Type_LatLonBox: return CreateLatLonBox();
  case Type_LatLonAltBox: return CreateLatLonAltBox();
  case Type_LinearRing: return CreateLinearRing();
  case Type_LineString: return CreateLineString();
  case Type_LineStyle: return CreateLineStyle();
  case Type_Link: return CreateLink();
  case Type_ListStyle: return CreateListStyle();
  case Type_Location: return CreateLocation();
  case Type_Lod: return CreateLod();
  case Type_LookAt: return CreateLookAt();
  case Type_Metadata: return CreateMetadata();
  case Type_Model: return CreateModel();
  case Type_MultiGeometry: return CreateMultiGeometry();
  case Type_NetworkLink: return CreateNetworkLink();
  case Type_NetworkLinkControl: return CreateNetworkLinkControl();
  case Type_Orientation: return CreateOrientation();
  case Type_Pair: return CreatePair();
  case Type_PhotoOverlay: return CreatePhotoOverlay();
  case Type_Placemark: return CreatePlacemark();
  case Type_PolyStyle: return CreatePolyStyle();
  case Type_Point: return CreatePoint();
  case Type_Polygon: return CreatePolygon();
  case Type_Region: return CreateRegion();
  case Type_ResourceMap: return CreateResourceMap();
  case Type_Scale: return CreateScale();
  case Type_Schema: return CreateSchema();
  case Type_SchemaData: return CreateSchemaData();
  case Type_ScreenOverlay: return CreateScreenOverlay();
  case Type_SimpleData: return CreateSimpleData();
  case Type_SimpleField: return CreateSimpleField();
  case Type_Snippet: return CreateSnippet();
  case Type_Style: return CreateStyle();
  case Type_StyleMap: return CreateStyleMap();
  case Type_TimeSpan: return CreateTimeSpan();
  case Type_TimeStamp: return CreateTimeStamp();
  case Type_ViewVolume: return CreateViewVolume();
  case Type_Update: return CreateUpdate();
  case Type_Url: return CreateUrl();
  case Type_coordinates: return CreateCoordinates();
  case Type_hotSpot: return CreateHotSpot();
  case Type_innerBoundaryIs: return CreateInnerBoundaryIs();
  case Type_kml: return CreateKml();
  case Type_linkSnippet: return CreateLinkSnippet();
  case Type_overlayXY: return CreateOverlayXY();
  case Type_outerBoundaryIs: return CreateOuterBoundaryIs();
  case Type_rotationXY: return CreateRotationXY();
  case Type_screenXY: return CreateScreenXY();
  case Type_size: return CreateSize();
  case Type_XalAddressDetails: return CreateXalAddressDetails();
  case Type_XalAdministrativeArea: return CreateXalAdministrativeArea();
  case Type_XalCountry: return CreateXalCountry();
  case Type_XalLocality: return CreateXalLocality();
  case Type_XalPostalCode: return CreateXalPostalCode();
  case Type_XalSubAdministrativeArea: return CreateXalSubAdministrativeArea();
  case Type_XalThoroughfare: return CreateXalThoroughfare();

  case Type_GxAnimatedUpdate: return CreateGxAnimatedUpdate();
  case Type_GxFlyTo: return CreateGxFlyTo();
  case Type_GxLatLonQuad: return CreateGxLatLonQuad();
  case Type_GxPlaylist: return CreateGxPlaylist();
  case Type_GxSoundCue: return CreateGxSoundCue();
  case Type_GxTimeSpan: return CreateGxTimeSpan();
  case Type_GxTimeStamp: return CreateGxTimeStamp();
  case Type_GxTour: return CreateGxTour();
  case Type_GxTourControl: return CreateGxTourControl();
  case Type_GxWait: return CreateGxWait();

  default: return NULL;
  }
}

Field* KmlFactory::CreateFieldById(KmlDomType type_id) const {
  return new Field(type_id);
}

Alias* KmlFactory::CreateAlias() const {
  return new Alias();
}

AtomAuthor* KmlFactory::CreateAtomAuthor() const {
  return new AtomAuthor();
}

AtomCategory* KmlFactory::CreateAtomCategory() const {
  return new AtomCategory();
}

AtomContent* KmlFactory::CreateAtomContent() const {
  return new AtomContent();
}

AtomEntry* KmlFactory::CreateAtomEntry() const {
  return new AtomEntry();
}

AtomFeed* KmlFactory::CreateAtomFeed() const {
  return new AtomFeed();
}

AtomLink* KmlFactory::CreateAtomLink() const {
  return new AtomLink();
}

BalloonStyle* KmlFactory::CreateBalloonStyle() const {
  return new BalloonStyle();
}

Camera* KmlFactory::CreateCamera() const {
  return new Camera();
}

Change* KmlFactory::CreateChange() const {
  return new Change();
}

Create* KmlFactory::CreateCreate() const {
  return new Create();
}

Data* KmlFactory::CreateData() const {
  return new Data();
}

Delete* KmlFactory::CreateDelete() const {
  return new Delete();
}

Document* KmlFactory::CreateDocument() const {
  return new Document();
}

ExtendedData* KmlFactory::CreateExtendedData() const {
  return new ExtendedData();
}

Folder* KmlFactory::CreateFolder() const {
  return new Folder();
}

GroundOverlay* KmlFactory::CreateGroundOverlay() const {
  return new GroundOverlay();
}

HotSpot* KmlFactory::CreateHotSpot() const {
  return new HotSpot();
}

Icon* KmlFactory::CreateIcon() const {
  return new Icon();
}

IconStyle* KmlFactory::CreateIconStyle() const {
  return new IconStyle();
}

IconStyleIcon* KmlFactory::CreateIconStyleIcon() const {
  return new IconStyleIcon();
}

ImagePyramid* KmlFactory::CreateImagePyramid() const {
  return new ImagePyramid();
}

InnerBoundaryIs* KmlFactory::CreateInnerBoundaryIs() const {
  return new InnerBoundaryIs();
}

ItemIcon* KmlFactory::CreateItemIcon() const {
  return new ItemIcon();
}

Kml* KmlFactory::CreateKml() const {
  return new Kml();
}

LabelStyle* KmlFactory::CreateLabelStyle() const {
  return new LabelStyle();
}

LatLonBox* KmlFactory::CreateLatLonBox() const {
  return new LatLonBox();
}

LatLonAltBox* KmlFactory::CreateLatLonAltBox() const {
  return new LatLonAltBox();
}

LinearRing* KmlFactory::CreateLinearRing() const {
  return new LinearRing();
}

LineString* KmlFactory::CreateLineString() const {
  return new LineString();
}

LineStyle* KmlFactory::CreateLineStyle() const {
  return new LineStyle();
}

Link* KmlFactory::CreateLink() const {
  return new Link();
}

LinkSnippet* KmlFactory::CreateLinkSnippet() const {
  return new LinkSnippet();
}

ListStyle* KmlFactory::CreateListStyle() const {
  return new ListStyle();
}

Location* KmlFactory::CreateLocation() const {
  return new Location();
}

Lod* KmlFactory::CreateLod() const {
  return new Lod();
}

LookAt* KmlFactory::CreateLookAt() const {
  return new LookAt();
}

Metadata* KmlFactory::CreateMetadata() const {
  return new Metadata();
}

Model* KmlFactory::CreateModel() const {
  return new Model();
}

MultiGeometry* KmlFactory::CreateMultiGeometry() const {
  return new MultiGeometry();
}

NetworkLink* KmlFactory::CreateNetworkLink() const {
  return new NetworkLink();
}

NetworkLinkControl* KmlFactory::CreateNetworkLinkControl() const {
  return new NetworkLinkControl();
}

Orientation* KmlFactory::CreateOrientation() const {
  return new Orientation();
}

OuterBoundaryIs* KmlFactory::CreateOuterBoundaryIs() const {
  return new OuterBoundaryIs();
}

OverlayXY* KmlFactory::CreateOverlayXY() const {
  return new OverlayXY();
}

Pair* KmlFactory::CreatePair() const {
  return new Pair();
}

PhotoOverlay* KmlFactory::CreatePhotoOverlay() const {
  return new PhotoOverlay();
}

Placemark* KmlFactory::CreatePlacemark() const {
  return new Placemark();
}

Point* KmlFactory::CreatePoint() const {
  return new Point();
}

Polygon* KmlFactory::CreatePolygon() const {
  return new Polygon();
}

PolyStyle* KmlFactory::CreatePolyStyle() const {
  return new PolyStyle();
}

Region* KmlFactory::CreateRegion() const {
  return new Region();
}

ResourceMap* KmlFactory::CreateResourceMap() const {
  return new ResourceMap();
}

RotationXY* KmlFactory::CreateRotationXY() const {
  return new RotationXY();
}

Scale* KmlFactory::CreateScale() const {
  return new Scale();
}

ScreenOverlay* KmlFactory::CreateScreenOverlay() const {
  return new ScreenOverlay();
}

Coordinates* KmlFactory::CreateCoordinates() const {
  return new Coordinates();
}

ScreenXY* KmlFactory::CreateScreenXY() const {
  return new ScreenXY();
}

Schema* KmlFactory::CreateSchema() const {
  return new Schema();
}

SchemaData* KmlFactory::CreateSchemaData() const {
  return new SchemaData();
}

Update* KmlFactory::CreateUpdate() const {
  return new Update();
}

Url* KmlFactory::CreateUrl() const {
  return new Url();
}

SimpleData* KmlFactory::CreateSimpleData() const {
  return new SimpleData();
}

SimpleField* KmlFactory::CreateSimpleField() const {
  return new SimpleField();
}

Size* KmlFactory::CreateSize() const {
  return new Size();
}

Snippet* KmlFactory::CreateSnippet() const {
  return new Snippet();
}

Style* KmlFactory::CreateStyle() const {
  return new Style();
}

StyleMap* KmlFactory::CreateStyleMap() const {
  return new StyleMap();
}

TimeSpan* KmlFactory::CreateTimeSpan() const {
  return new TimeSpan();
}

TimeStamp* KmlFactory::CreateTimeStamp() const {
  return new TimeStamp();
}

ViewVolume* KmlFactory::CreateViewVolume() const {
  return new ViewVolume();
}

XalAddressDetails* KmlFactory::CreateXalAddressDetails() const {
  return new XalAddressDetails();
}

XalAdministrativeArea* KmlFactory::CreateXalAdministrativeArea() const {
  return new XalAdministrativeArea();
}

XalCountry* KmlFactory::CreateXalCountry() const {
  return new XalCountry();
}

XalLocality* KmlFactory::CreateXalLocality() const {
  return new XalLocality();
}

XalPostalCode* KmlFactory::CreateXalPostalCode() const {
  return new XalPostalCode();
}

XalSubAdministrativeArea* KmlFactory::CreateXalSubAdministrativeArea() const {
  return new XalSubAdministrativeArea();
}

XalThoroughfare* KmlFactory::CreateXalThoroughfare() const {
  return new XalThoroughfare();
}

GxAnimatedUpdate* KmlFactory::CreateGxAnimatedUpdate() const {
  return new GxAnimatedUpdate();
}

GxFlyTo* KmlFactory::CreateGxFlyTo() const {
  return new GxFlyTo();
}

GxLatLonQuad* KmlFactory::CreateGxLatLonQuad() const {
  return new GxLatLonQuad();
}

GxPlaylist* KmlFactory::CreateGxPlaylist() const {
  return new GxPlaylist();
}

GxSoundCue* KmlFactory::CreateGxSoundCue() const {
  return new GxSoundCue();
}

GxTimeSpan* KmlFactory::CreateGxTimeSpan() const {
  return new GxTimeSpan();
}

GxTimeStamp* KmlFactory::CreateGxTimeStamp() const {
  return new GxTimeStamp();
}

GxTour* KmlFactory::CreateGxTour() const {
  return new GxTour();
}

GxTourControl* KmlFactory::CreateGxTourControl() const {
  return new GxTourControl();
}

GxWait* KmlFactory::CreateGxWait() const {
  return new GxWait();
}

}  // namespace kmldom
