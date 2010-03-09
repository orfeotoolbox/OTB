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
#include "kml_ptr.h"
#include "boost/intrusive_ptr.hpp"

namespace kmldom {

const AbstractLatLonBoxPtr AsAbstractLatLonBox(const ElementPtr element) {
  if (element && element->IsA(Type_AbstractLatLonBox)) {
    return boost::static_pointer_cast<AbstractLatLonBox>(element);
  }
  return NULL;
}

const AbstractViewPtr AsAbstractView(const ElementPtr element) {
  if (element && element->IsA(Type_AbstractView)) {
    return boost::static_pointer_cast<AbstractView>(element);
  }
  return NULL;
}

const ColorStylePtr AsColorStyle(const ElementPtr element) {
  if (element && element->IsA(Type_ColorStyle)) {
    return boost::static_pointer_cast<ColorStyle>(element);
  }
  return NULL;
}

const ContainerPtr AsContainer(const ElementPtr element) {
  if (element && element->IsA(Type_Container)) {
    return boost::static_pointer_cast<Container>(element);
  }
  return NULL;
}

const FeaturePtr AsFeature(const ElementPtr element) {
  if (element && element->IsA(Type_Feature)) {
    return boost::static_pointer_cast<Feature>(element);
  }
  return NULL;
}

const GeometryPtr AsGeometry(const ElementPtr element) {
  if (element && element->IsA(Type_Geometry)) {
    return boost::static_pointer_cast<Geometry>(element);
  }
  return NULL;
}

const ObjectPtr AsObject(const ElementPtr element) {
  if (element && element->IsA(Type_Object)) {
    return boost::static_pointer_cast<Object>(element);
  }
  return NULL;
}

const OverlayPtr AsOverlay(const ElementPtr element) {
  if (element && element->IsA(Type_Overlay)) {
    return boost::static_pointer_cast<Overlay>(element);
  }
  return NULL;
}

const StyleSelectorPtr AsStyleSelector(const ElementPtr element) {
  if (element && element->IsA(Type_StyleSelector)) {
    return boost::static_pointer_cast<StyleSelector>(element);
  }
  return NULL;
}

const SubStylePtr AsSubStyle(const ElementPtr element) {
  if (element && element->IsA(Type_SubStyle)) {
    return boost::static_pointer_cast<SubStyle>(element);
  }
  return NULL;
}

const TimePrimitivePtr AsTimePrimitive(const ElementPtr element) {
  if (element && element->IsA(Type_TimePrimitive)) {
    return boost::static_pointer_cast<TimePrimitive>(element);
  }
  return NULL;
}

const AliasPtr AsAlias(const ElementPtr element) {
  if (element && element->Type() == Type_Alias) {
    return boost::static_pointer_cast<Alias>(element);
  }
  return NULL;
}

const BalloonStylePtr AsBalloonStyle(const ElementPtr element) {
  if (element && element->Type() == Type_BalloonStyle) {
    return boost::static_pointer_cast<BalloonStyle>(element);
  }
  return NULL;
}

const CameraPtr AsCamera(const ElementPtr element) {
  if (element && element->Type() == Type_Camera) {
    return boost::static_pointer_cast<Camera>(element);
  }
  return NULL;
}

const ChangePtr AsChange(const ElementPtr element) {
  if (element && element->Type() == Type_Change) {
    return boost::static_pointer_cast<Change>(element);
  }
  return NULL;
}

const CreatePtr AsCreate(const ElementPtr element) {
  if (element && element->Type() == Type_Create) {
    return boost::static_pointer_cast<Create>(element);
  }
  return NULL;
}

const DataPtr AsData(const ElementPtr element) {
  if (element && element->Type() == Type_Data) {
    return boost::static_pointer_cast<Data>(element);
  }
  return NULL;
}

const DeletePtr AsDelete(const ElementPtr element) {
  if (element && element->Type() == Type_Delete) {
    return boost::static_pointer_cast<Delete>(element);
  }
  return NULL;
}

const DocumentPtr AsDocument(const ElementPtr element) {
  if (element && element->Type() == Type_Document) {
    return boost::static_pointer_cast<Document>(element);
  }
  return NULL;
}

const FolderPtr AsFolder(const ElementPtr element) {
  if (element && element->Type() == Type_Folder) {
    return boost::static_pointer_cast<Folder>(element);
  }
  return NULL;
}

const GroundOverlayPtr AsGroundOverlay(const ElementPtr element) {
  if (element && element->Type() == Type_GroundOverlay) {
    return boost::static_pointer_cast<GroundOverlay>(element);
  }
  return NULL;
}

const HotSpotPtr AsHotSpot(const ElementPtr element) {
  if (element && element->Type() == Type_hotSpot) {
    return boost::static_pointer_cast<HotSpot>(element);
  }
  return NULL;
}

const IconPtr AsIcon(const ElementPtr element) {
  if (element && element->Type() == Type_Icon) {
    return boost::static_pointer_cast<Icon>(element);
  }
  return NULL;
}

const IconStylePtr AsIconStyle(const ElementPtr element) {
  if (element && element->Type() == Type_IconStyle) {
    return boost::static_pointer_cast<IconStyle>(element);
  }
  return NULL;
}

const IconStyleIconPtr AsIconStyleIcon(const ElementPtr element) {
  if (element && element->Type() == Type_IconStyleIcon) {
    return boost::static_pointer_cast<IconStyleIcon>(element);
  }
  return NULL;
}

const ImagePyramidPtr AsImagePyramid(const ElementPtr element) {
  if (element && element->Type() == Type_ImagePyramid) {
    return boost::static_pointer_cast<ImagePyramid>(element);
  }
  return NULL;
}

const InnerBoundaryIsPtr AsInnerBoundaryIs(const ElementPtr element) {
  if (element && element->Type() == Type_innerBoundaryIs) {
    return boost::static_pointer_cast<InnerBoundaryIs>(element);
  }
  return NULL;
}

const ItemIconPtr AsItemIcon(const ElementPtr element) {
  if (element && element->Type() == Type_ItemIcon) {
    return boost::static_pointer_cast<ItemIcon>(element);
  }
  return NULL;
}

const LabelStylePtr AsLabelStyle(const ElementPtr element) {
  if (element && element->Type() == Type_LabelStyle) {
    return boost::static_pointer_cast<LabelStyle>(element);
  }
  return NULL;
}

const LatLonAltBoxPtr AsLatLonAltBox(const ElementPtr element) {
  if (element && element->Type() == Type_LatLonAltBox) {
    return boost::static_pointer_cast<LatLonAltBox>(element);
  }
  return NULL;
}

const LatLonBoxPtr AsLatLonBox(const ElementPtr element) {
  if (element && element->Type() == Type_LatLonBox) {
    return boost::static_pointer_cast<LatLonBox>(element);
  }
  return NULL;
}

const LineStringPtr AsLineString(const ElementPtr element) {
  if (element && element->Type() == Type_LineString) {
    return boost::static_pointer_cast<LineString>(element);
  }
  return NULL;
}

const LineStylePtr AsLineStyle(const ElementPtr element) {
  if (element && element->Type() == Type_LineStyle) {
    return boost::static_pointer_cast<LineStyle>(element);
  }
  return NULL;
}

const LinearRingPtr AsLinearRing(const ElementPtr element) {
  if (element && element->Type() == Type_LinearRing) {
    return boost::static_pointer_cast<LinearRing>(element);
  }
  return NULL;
}

const LinkPtr AsLink(const ElementPtr element) {
  if (element && element->Type() == Type_Link) {
    return boost::static_pointer_cast<Link>(element);
  }
  return NULL;
}

const LinkSnippetPtr AsLinkSnippet(const ElementPtr element) {
  if (element && element->Type() == Type_linkSnippet) {
    return boost::static_pointer_cast<LinkSnippet>(element);
  }
  return NULL;
}

const ListStylePtr AsListStyle(const ElementPtr element) {
  if (element && element->Type() == Type_ListStyle) {
    return boost::static_pointer_cast<ListStyle>(element);
  }
  return NULL;
}

const LocationPtr AsLocation(const ElementPtr element) {
  if (element && element->Type() == Type_Location) {
    return boost::static_pointer_cast<Location>(element);
  }
  return NULL;
}

const LodPtr AsLod(const ElementPtr element) {
  if (element && element->Type() == Type_Lod) {
    return boost::static_pointer_cast<Lod>(element);
  }
  return NULL;
}

const LookAtPtr AsLookAt(const ElementPtr element) {
  if (element && element->Type() == Type_LookAt) {
    return boost::static_pointer_cast<LookAt>(element);
  }
  return NULL;
}

const ModelPtr AsModel(const ElementPtr element) {
  if (element && element->Type() == Type_Model) {
    return boost::static_pointer_cast<Model>(element);
  }
  return NULL;
}

const MultiGeometryPtr AsMultiGeometry(const ElementPtr element) {
  if (element && element->Type() == Type_MultiGeometry) {
    return boost::static_pointer_cast<MultiGeometry>(element);
  }
  return NULL;
}

const NetworkLinkPtr AsNetworkLink(const ElementPtr element) {
  if (element && element->Type() == Type_NetworkLink) {
    return boost::static_pointer_cast<NetworkLink>(element);
  }
  return NULL;
}

const OrientationPtr AsOrientation(const ElementPtr element) {
  if (element && element->Type() == Type_Orientation) {
    return boost::static_pointer_cast<Orientation>(element);
  }
  return NULL;
}

const OuterBoundaryIsPtr AsOuterBoundaryIs(const ElementPtr element) {
  if (element && element->Type() == Type_outerBoundaryIs) {
    return boost::static_pointer_cast<OuterBoundaryIs>(element);
  }
  return NULL;
}

const OverlayXYPtr AsOverlayXY(const ElementPtr element) {
  if (element && element->Type() == Type_overlayXY) {
    return boost::static_pointer_cast<OverlayXY>(element);
  }
  return NULL;
}

const PairPtr AsPair(const ElementPtr element) {
  if (element && element->Type() == Type_Pair) {
    return boost::static_pointer_cast<Pair>(element);
  }
  return NULL;
}

const PhotoOverlayPtr AsPhotoOverlay(const ElementPtr element) {
  if (element && element->Type() == Type_PhotoOverlay) {
    return boost::static_pointer_cast<PhotoOverlay>(element);
  }
  return NULL;
}

const PlacemarkPtr AsPlacemark(const ElementPtr element) {
  if (element && element->Type() == Type_Placemark) {
    return boost::static_pointer_cast<Placemark>(element);
  }
  return NULL;
}

const PointPtr AsPoint(const ElementPtr element) {
  if (element && element->Type() == Type_Point) {
    return boost::static_pointer_cast<Point>(element);
  }
  return NULL;
}

const PolyStylePtr AsPolyStyle(const ElementPtr element) {
  if (element && element->Type() == Type_PolyStyle) {
    return boost::static_pointer_cast<PolyStyle>(element);
  }
  return NULL;
}

const PolygonPtr AsPolygon(const ElementPtr element) {
  if (element && element->Type() == Type_Polygon) {
    return boost::static_pointer_cast<Polygon>(element);
  }
  return NULL;
}

const RegionPtr AsRegion(const ElementPtr element) {
  if (element && element->Type() == Type_Region) {
    return boost::static_pointer_cast<Region>(element);
  }
  return NULL;
}

const ResourceMapPtr AsResourceMap(const ElementPtr element) {
  if (element && element->Type() == Type_ResourceMap) {
    return boost::static_pointer_cast<ResourceMap>(element);
  }
  return NULL;
}

const RotationXYPtr AsRotationXY(const ElementPtr element) {
  if (element && element->Type() == Type_rotationXY) {
    return boost::static_pointer_cast<RotationXY>(element);
  }
  return NULL;
}

const ScalePtr AsScale(const ElementPtr element) {
  if (element && element->Type() == Type_Scale) {
    return boost::static_pointer_cast<Scale>(element);
  }
  return NULL;
}

const SchemaPtr AsSchema(const ElementPtr element) {
  if (element && element->Type() == Type_Schema) {
    return boost::static_pointer_cast<Schema>(element);
  }
  return NULL;
}

const SchemaDataPtr AsSchemaData(const ElementPtr element) {
  if (element && element->Type() == Type_SchemaData) {
    return boost::static_pointer_cast<SchemaData>(element);
  }
  return NULL;
}

const ScreenOverlayPtr AsScreenOverlay(const ElementPtr element) {
  if (element && element->Type() == Type_ScreenOverlay) {
    return boost::static_pointer_cast<ScreenOverlay>(element);
  }
  return NULL;
}

const ScreenXYPtr AsScreenXY(const ElementPtr element) {
  if (element && element->Type() == Type_screenXY) {
    return boost::static_pointer_cast<ScreenXY>(element);
  }
  return NULL;
}

const SizePtr AsSize(const ElementPtr element) {
  if (element && element->Type() == Type_size) {
    return boost::static_pointer_cast<Size>(element);
  }
  return NULL;
}

const SnippetPtr AsSnippet(const ElementPtr element) {
  if (element && element->Type() == Type_Snippet) {
    return boost::static_pointer_cast<Snippet>(element);
  }
  return NULL;
}

const StylePtr AsStyle(const ElementPtr element) {
  if (element && element->Type() == Type_Style) {
    return boost::static_pointer_cast<Style>(element);
  }
  return NULL;
}

const StyleMapPtr AsStyleMap(const ElementPtr element) {
  if (element && element->Type() == Type_StyleMap) {
    return boost::static_pointer_cast<StyleMap>(element);
  }
  return NULL;
}

const TimeSpanPtr AsTimeSpan(const ElementPtr element) {
  if (element && element->Type() == Type_TimeSpan) {
    return boost::static_pointer_cast<TimeSpan>(element);
  }
  return NULL;
}

const TimeStampPtr AsTimeStamp(const ElementPtr element) {
  if (element && element->Type() == Type_TimeStamp) {
    return boost::static_pointer_cast<TimeStamp>(element);
  }
  return NULL;
}

const UpdatePtr AsUpdate(const ElementPtr element) {
  if (element && element->Type() == Type_Update) {
    return boost::static_pointer_cast<Update>(element);
  }
  return NULL;
}

const ViewVolumePtr AsViewVolume(const ElementPtr element) {
  if (element && element->Type() == Type_ViewVolume) {
    return boost::static_pointer_cast<ViewVolume>(element);
  }
  return NULL;
}

}  // end namespace kmldom
