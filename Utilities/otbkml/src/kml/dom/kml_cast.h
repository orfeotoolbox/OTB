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

#ifndef KML_DOM_KML_CAST_H__
#define KML_DOM_KML_CAST_H__

#include "kml/dom/kmldom.h"
#include "kml/dom/kml_ptr.h"

namespace kmldom {

// Abstract element groups.
const AbstractLatLonBoxPtr AsAbstractLatLonBox(const ElementPtr element);
const AbstractViewPtr AsAbstractView(const ElementPtr element);
const ColorStylePtr AsColorStyle(const ElementPtr element);
const ContainerPtr AsContainer(const ElementPtr element);
const FeaturePtr AsFeature(const ElementPtr element);
const GeometryPtr AsGeometry(const ElementPtr element);
const ObjectPtr AsObject(const ElementPtr element);
const OverlayPtr AsOverlay(const ElementPtr element);
const StyleSelectorPtr AsStyleSelector(const ElementPtr element);
const SubStylePtr AsSubStyle(const ElementPtr element);
const TimePrimitivePtr AsTimePrimitive(const ElementPtr element);

// Concrete elements.
const AliasPtr AsAlias(const ElementPtr element);
const BalloonStylePtr AsBalloonStyle(const ElementPtr element);
const CameraPtr AsCamera(const ElementPtr element);
const ChangePtr AsChange(const ElementPtr element);
const CoordinatesPtr AsCoordinates(const ElementPtr element);
const CreatePtr AsCreate(const ElementPtr element);
const DataPtr AsData(const ElementPtr element);
const DeletePtr AsDelete(const ElementPtr element);
const DocumentPtr AsDocument(const ElementPtr element);
const ExtendedDataPtr AsExtendedData(const ElementPtr element);
const FolderPtr AsFolder(const ElementPtr element);
const GroundOverlayPtr AsGroundOverlay(const ElementPtr element);
const HotSpotPtr AsHotSpot(const ElementPtr element);
const IconPtr AsIcon(const ElementPtr element);
const IconStylePtr AsIconStyle(const ElementPtr element);
const IconStyleIconPtr AsIconStyleIcon(const ElementPtr element);
const ImagePyramidPtr AsImagePyramid(const ElementPtr element);
const InnerBoundaryIsPtr AsInnerBoundaryIs(const ElementPtr element);
const ItemIconPtr AsItemIcon(const ElementPtr element);
const KmlPtr AsKml(const ElementPtr element);
const LabelStylePtr AsLabelStyle(const ElementPtr element);
const LatLonAltBoxPtr AsLatLonAltBox(const ElementPtr element);
const LatLonBoxPtr AsLatLonBox(const ElementPtr element);
const LineStringPtr AsLineString(const ElementPtr element);
const LineStylePtr AsLineStyle(const ElementPtr element);
const LinearRingPtr AsLinearRing(const ElementPtr element);
const LinkPtr AsLink(const ElementPtr element);
const LinkSnippetPtr AsLinkSnippet(const ElementPtr element);
const ListStylePtr AsListStyle(const ElementPtr element);
const LocationPtr AsLocation(const ElementPtr element);
const LodPtr AsLod(const ElementPtr element);
const LookAtPtr AsLookAt(const ElementPtr element);
const ModelPtr AsModel(const ElementPtr element);
const MultiGeometryPtr AsMultiGeometry(const ElementPtr element);
const NetworkLinkPtr AsNetworkLink(const ElementPtr element);
const NetworkLinkControlPtr AsNetworkLinkControl(const ElementPtr element);
const OrientationPtr AsOrientation(const ElementPtr element);
const OuterBoundaryIsPtr AsOuterBoundaryIs(const ElementPtr element);
const OverlayXYPtr AsOverlayXY(const ElementPtr element);
const PairPtr AsPair(const ElementPtr element);
const PhotoOverlayPtr AsPhotoOverlay(const ElementPtr element);
const PlacemarkPtr AsPlacemark(const ElementPtr element);
const PointPtr AsPoint(const ElementPtr element);
const PolyStylePtr AsPolyStyle(const ElementPtr element);
const PolygonPtr AsPolygon(const ElementPtr element);
const RegionPtr AsRegion(const ElementPtr element);
const ResourceMapPtr AsResourceMap(const ElementPtr element);
const RotationXYPtr AsRotationXY(const ElementPtr element);
const ScalePtr AsScale(const ElementPtr element);
const SchemaPtr AsSchema(const ElementPtr element);
const SchemaDataPtr AsSchemaData(const ElementPtr element);
const ScreenOverlayPtr AsScreenOverlay(const ElementPtr element);
const ScreenXYPtr AsScreenXY(const ElementPtr element);
const SimpleDataPtr AsSimpleData(const ElementPtr element);
const SimpleFieldPtr AsSimpleField(const ElementPtr element);
const SizePtr AsSize(const ElementPtr element);
const SnippetPtr AsSnippet(const ElementPtr element);
const StylePtr AsStyle(const ElementPtr element);
const StyleMapPtr AsStyleMap(const ElementPtr element);
const TimeSpanPtr AsTimeSpan(const ElementPtr element);
const TimeStampPtr AsTimeStamp(const ElementPtr element);
const UpdatePtr AsUpdate(const ElementPtr element);
const ViewVolumePtr AsViewVolume(const ElementPtr element);

}  // end namespace kmldom

#endif  // KML_DOM_KML_CAST_H__
