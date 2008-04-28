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

#include "kml_cast.h"

namespace kmldom {

const AbstractView* AsAbstractView(const Element* element) {
  if (element && element->IsA(Type_AbstractView)) {
    return static_cast<const AbstractView*>(element);
  }
  return NULL;
}

const ColorStyle* AsColorStyle(const Element* element) {
  if (element && element->IsA(Type_ColorStyle)) {
    return static_cast<const ColorStyle*>(element);
  }
  return NULL;
}

const Container* AsContainer(const Element* element) {
  if (element && element->IsA(Type_Container)) {
    return static_cast<const Container*>(element);
  }
  return NULL;
}

const Feature* AsFeature(const Element* element) {
  if (element && element->IsA(Type_Feature)) {
    return static_cast<const Feature*>(element);
  }
  return NULL;
}

const Geometry* AsGeometry(const Element* element) {
  if (element && element->IsA(Type_Geometry)) {
    return static_cast<const Geometry*>(element);
  }
  return NULL;
}

const Object* AsObject(const Element* element) {
  if (element && element->IsA(Type_Object)) {
    return static_cast<const Object*>(element);
  }
  return NULL;
}

const Overlay* AsOverlay(const Element* element) {
  if (element && element->IsA(Type_Overlay)) {
    return static_cast<const Overlay*>(element);
  }
  return NULL;
}

const StyleSelector* AsStyleSelector(const Element* element) {
  if (element && element->IsA(Type_StyleSelector)) {
    return static_cast<const StyleSelector*>(element);
  }
  return NULL;
}

const SubStyle* AsSubStyle(const Element* element) {
  if (element && element->IsA(Type_SubStyle)) {
    return static_cast<const SubStyle*>(element);
  }
  return NULL;
}

const TimePrimitive* AsTimePrimitive(const Element* element) {
  if (element && element->IsA(Type_TimePrimitive)) {
    return static_cast<const TimePrimitive*>(element);
  }
  return NULL;
}

const Alias* AsAlias(const Element* element) {
  if (element && element->Type() == Type_Alias) {
    return static_cast<const Alias*>(element);
  }
  return NULL;
}

const BalloonStyle* AsBalloonStyle(const Element* element) {
  if (element && element->Type() == Type_BalloonStyle) {
    return static_cast<const BalloonStyle*>(element);
  }
  return NULL;
}

const Camera* AsCamera(const Element* element) {
  if (element && element->Type() == Type_Camera) {
    return static_cast<const Camera*>(element);
  }
  return NULL;
}

const Change* AsChange(const Element* element) {
  if (element && element->Type() == Type_Change) {
    return static_cast<const Change*>(element);
  }
  return NULL;
}

const Create* AsCreate(const Element* element) {
  if (element && element->Type() == Type_Create) {
    return static_cast<const Create*>(element);
  }
  return NULL;
}

const Data* AsData(const Element* element) {
  if (element && element->Type() == Type_Data) {
    return static_cast<const Data*>(element);
  }
  return NULL;
}

const Delete* AsDelete(const Element* element) {
  if (element && element->Type() == Type_Delete) {
    return static_cast<const Delete*>(element);
  }
  return NULL;
}

const Document* AsDocument(const Element* element) {
  if (element && element->Type() == Type_Document) {
    return static_cast<const Document*>(element);
  }
  return NULL;
}

const ExtendedData* AsExtendedData(const Element* element) {
  if (element && element->Type() == Type_ExtendedData) {
    return static_cast<const ExtendedData*>(element);
  }
  return NULL;
}

const Folder* AsFolder(const Element* element) {
  if (element && element->Type() == Type_Folder) {
    return static_cast<const Folder*>(element);
  }
  return NULL;
}

const GroundOverlay* AsGroundOverlay(const Element* element) {
  if (element && element->Type() == Type_GroundOverlay) {
    return static_cast<const GroundOverlay*>(element);
  }
  return NULL;
}

const HotSpot* AsHotSpot(const Element* element) {
  if (element && element->Type() == Type_hotSpot) {
    return static_cast<const HotSpot*>(element);
  }
  return NULL;
}

const Icon* AsIcon(const Element* element) {
  if (element && element->Type() == Type_Icon) {
    return static_cast<const Icon*>(element);
  }
  return NULL;
}

const IconStyle* AsIconStyle(const Element* element) {
  if (element && element->Type() == Type_IconStyle) {
    return static_cast<const IconStyle*>(element);
  }
  return NULL;
}

const ImagePyramid* AsImagePyramid(const Element* element) {
  if (element && element->Type() == Type_ImagePyramid) {
    return static_cast<const ImagePyramid*>(element);
  }
  return NULL;
}

const InnerBoundaryIs* AsInnerBoundaryIs(const Element* element) {
  if (element && element->Type() == Type_innerBoundaryIs) {
    return static_cast<const InnerBoundaryIs*>(element);
  }
  return NULL;
}

const ItemIcon* AsItemIcon(const Element* element) {
  if (element && element->Type() == Type_ItemIcon) {
    return static_cast<const ItemIcon*>(element);
  }
  return NULL;
}

const LabelStyle* AsLabelStyle(const Element* element) {
  if (element && element->Type() == Type_LabelStyle) {
    return static_cast<const LabelStyle*>(element);
  }
  return NULL;
}

const LatLonAltBox* AsLatLonAltBox(const Element* element) {
  if (element && element->Type() == Type_LatLonAltBox) {
    return static_cast<const LatLonAltBox*>(element);
  }
  return NULL;
}

const LatLonBox* AsLatLonBox(const Element* element) {
  if (element && element->Type() == Type_LatLonBox) {
    return static_cast<const LatLonBox*>(element);
  }
  return NULL;
}

const LineString* AsLineString(const Element* element) {
  if (element && element->Type() == Type_LineString) {
    return static_cast<const LineString*>(element);
  }
  return NULL;
}

const LineStyle* AsLineStyle(const Element* element) {
  if (element && element->Type() == Type_LineStyle) {
    return static_cast<const LineStyle*>(element);
  }
  return NULL;
}

const LinearRing* AsLinearRing(const Element* element) {
  if (element && element->Type() == Type_LinearRing) {
    return static_cast<const LinearRing*>(element);
  }
  return NULL;
}

const Link* AsLink(const Element* element) {
  if (element && element->Type() == Type_Link) {
    return static_cast<const Link*>(element);
  }
  return NULL;
}

const LinkSnippet* AsLinkSnippet(const Element* element) {
  if (element && element->Type() == Type_linkSnippet) {
    return static_cast<const LinkSnippet*>(element);
  }
  return NULL;
}

const ListStyle* AsListStyle(const Element* element) {
  if (element && element->Type() == Type_ListStyle) {
    return static_cast<const ListStyle*>(element);
  }
  return NULL;
}

const Location* AsLocation(const Element* element) {
  if (element && element->Type() == Type_Location) {
    return static_cast<const Location*>(element);
  }
  return NULL;
}

const Lod* AsLod(const Element* element) {
  if (element && element->Type() == Type_Lod) {
    return static_cast<const Lod*>(element);
  }
  return NULL;
}

const LookAt* AsLookAt(const Element* element) {
  if (element && element->Type() == Type_LookAt) {
    return static_cast<const LookAt*>(element);
  }
  return NULL;
}

const Kml* AsKml(const Element* element) {
  if (element && element->Type() == Type_kml) {
    return static_cast<const Kml*>(element);
  }
  return NULL;
}

const Model* AsModel(const Element* element) {
  if (element && element->Type() == Type_Model) {
    return static_cast<const Model*>(element);
  }
  return NULL;
}

const MultiGeometry* AsMultiGeometry(const Element* element) {
  if (element && element->Type() == Type_MultiGeometry) {
    return static_cast<const MultiGeometry*>(element);
  }
  return NULL;
}

const NetworkLink* AsNetworkLink(const Element* element) {
  if (element && element->Type() == Type_NetworkLink) {
    return static_cast<const NetworkLink*>(element);
  }
  return NULL;
}

const NetworkLinkControl* AsNetworkLinkControl(const Element* element) {
  if (element && element->Type() == Type_NetworkLinkControl) {
    return static_cast<const NetworkLinkControl*>(element);
  }
  return NULL;
}

const Orientation* AsOrientation(const Element* element) {
  if (element && element->Type() == Type_Orientation) {
    return static_cast<const Orientation*>(element);
  }
  return NULL;
}

const OuterBoundaryIs* AsOuterBoundaryIs(const Element* element) {
  if (element && element->Type() == Type_outerBoundaryIs) {
    return static_cast<const OuterBoundaryIs*>(element);
  }
  return NULL;
}

const OverlayXY* AsOverlayXY(const Element* element) {
  if (element && element->Type() == Type_overlayXY) {
    return static_cast<const OverlayXY*>(element);
  }
  return NULL;
}

const Pair* AsPair(const Element* element) {
  if (element && element->Type() == Type_Pair) {
    return static_cast<const Pair*>(element);
  }
  return NULL;
}

const PhotoOverlay* AsPhotoOverlay(const Element* element) {
  if (element && element->Type() == Type_PhotoOverlay) {
    return static_cast<const PhotoOverlay*>(element);
  }
  return NULL;
}

const Placemark* AsPlacemark(const Element* element) {
  if (element && element->Type() == Type_Placemark) {
    return static_cast<const Placemark*>(element);
  }
  return NULL;
}

const Point* AsPoint(const Element* element) {
  if (element && element->Type() == Type_Point) {
    return static_cast<const Point*>(element);
  }
  return NULL;
}

const PolyStyle* AsPolyStyle(const Element* element) {
  if (element && element->Type() == Type_PolyStyle) {
    return static_cast<const PolyStyle*>(element);
  }
  return NULL;
}

const Polygon* AsPolygon(const Element* element) {
  if (element && element->Type() == Type_Polygon) {
    return static_cast<const Polygon*>(element);
  }
  return NULL;
}

const Region* AsRegion(const Element* element) {
  if (element && element->Type() == Type_Region) {
    return static_cast<const Region*>(element);
  }
  return NULL;
}

const ResourceMap* AsResourceMap(const Element* element) {
  if (element && element->Type() == Type_ResourceMap) {
    return static_cast<const ResourceMap*>(element);
  }
  return NULL;
}

const RotationXY* AsRotationXY(const Element* element) {
  if (element && element->Type() == Type_rotationXY) {
    return static_cast<const RotationXY*>(element);
  }
  return NULL;
}

const Scale* AsScale(const Element* element) {
  if (element && element->Type() == Type_Scale) {
    return static_cast<const Scale*>(element);
  }
  return NULL;
}

const Schema* AsSchema(const Element* element) {
  if (element && element->Type() == Type_Schema) {
    return static_cast<const Schema*>(element);
  }
  return NULL;
}

const SchemaData* AsSchemaData(const Element* element) {
  if (element && element->Type() == Type_SchemaData) {
    return static_cast<const SchemaData*>(element);
  }
  return NULL;
}

const ScreenOverlay* AsScreenOverlay(const Element* element) {
  if (element && element->Type() == Type_ScreenOverlay) {
    return static_cast<const ScreenOverlay*>(element);
  }
  return NULL;
}

const ScreenXY* AsScreenXY(const Element* element) {
  if (element && element->Type() == Type_screenXY) {
    return static_cast<const ScreenXY*>(element);
  }
  return NULL;
}

const SimpleData* AsSimpleData(const Element* element) {
  if (element && element->Type() == Type_SimpleData) {
    return static_cast<const SimpleData*>(element);
  }
  return NULL;
}

const SimpleField* AsSimpleField(const Element* element) {
  if (element && element->Type() == Type_SimpleField) {
    return static_cast<const SimpleField*>(element);
  }
  return NULL;
}

const Size* AsSize(const Element* element) {
  if (element && element->Type() == Type_size) {
    return static_cast<const Size*>(element);
  }
  return NULL;
}

const Snippet* AsSnippet(const Element* element) {
  if (element && element->Type() == Type_Snippet) {
    return static_cast<const Snippet*>(element);
  }
  return NULL;
}

const Style* AsStyle(const Element* element) {
  if (element && element->Type() == Type_Style) {
    return static_cast<const Style*>(element);
  }
  return NULL;
}

const StyleMap* AsStyleMap(const Element* element) {
  if (element && element->Type() == Type_StyleMap) {
    return static_cast<const StyleMap*>(element);
  }
  return NULL;
}

const TimeSpan* AsTimeSpan(const Element* element) {
  if (element && element->Type() == Type_TimeSpan) {
    return static_cast<const TimeSpan*>(element);
  }
  return NULL;
}

const TimeStamp* AsTimeStamp(const Element* element) {
  if (element && element->Type() == Type_TimeStamp) {
    return static_cast<const TimeStamp*>(element);
  }
  return NULL;
}

const Update* AsUpdate(const Element* element) {
  if (element && element->Type() == Type_Update) {
    return static_cast<const Update*>(element);
  }
  return NULL;
}

const ViewVolume* AsViewVolume(const Element* element) {
  if (element && element->Type() == Type_ViewVolume) {
    return static_cast<const ViewVolume*>(element);
  }
  return NULL;
}

}  // end namespace kmldom
