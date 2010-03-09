// Copyright 2009, Google Inc. All rights reserved.
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

// WARNING: THE VISITOR API IMPLEMENTED IN THIS CLASS IS EXPERIMENTAL AND
// SUBJECT TO CHANGE WITHOUT WARNING.

#include "kml/dom/visitor.h"

#include "kml/dom/kmldom.h"
#include "kml/dom/kml_cast.h"

namespace kmldom {

Visitor::Visitor() { }

Visitor::~Visitor() { }

void Visitor::VisitElement(const ElementPtr& element) {
  /* Top of element class hierarchy: Do nothing. */
}

void Visitor::VisitAbstractLatLonBox(
    const AbstractLatLonBoxPtr& element) {
  VisitObject(element);
}

void Visitor::VisitAbstractLink(
    const AbstractLinkPtr& element) {
  VisitBasicLink(element);
}

void Visitor::VisitAbstractView(
    const AbstractViewPtr& element) {
  VisitObject(element);
}

void Visitor::VisitAlias(
    const AliasPtr& element) {
  VisitObject(element);
}

void Visitor::VisitBalloonStyle(
    const BalloonStylePtr& element) {
  VisitSubStyle(element);
}

void Visitor::VisitBasicLink(
    const BasicLinkPtr& element) {
  VisitObject(element);
}

void Visitor::VisitCamera(
    const CameraPtr& element) {
  VisitAbstractView(element);
}

void Visitor::VisitChange(
    const ChangePtr& element) {
  VisitUpdateOperation(element);
}

void Visitor::VisitColorStyle(
    const ColorStylePtr& element) {
  VisitSubStyle(element);
}

void Visitor::VisitContainer(
    const ContainerPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitCoordinates(
    const CoordinatesPtr& element) {
  VisitElement(element);
}

void Visitor::VisitCreate(
    const CreatePtr& element) {
  VisitUpdateOperation(element);
}

void Visitor::VisitData(
    const DataPtr& element) {
  VisitObject(element);
}

void Visitor::VisitDelete(
    const DeletePtr& element) {
  VisitUpdateOperation(element);
}

void Visitor::VisitDocument(
    const DocumentPtr& element) {
  VisitContainer(element);
}

void Visitor::VisitExtendedData(
    const ExtendedDataPtr& element) {
  VisitElement(element);
}

void Visitor::VisitFeature(
    const FeaturePtr& element) {
  VisitObject(element);
}

void Visitor::VisitField(
    const FieldPtr& element) {
  VisitElement(element);
}

void Visitor::VisitFolder(
    const FolderPtr& element) {
  VisitContainer(element);
}

void Visitor::VisitGeometry(
    const GeometryPtr& element) {
  VisitObject(element);
}

void Visitor::VisitGroundOverlay(
    const GroundOverlayPtr& element) {
  VisitOverlay(element);
}

void Visitor::VisitGxAnimatedUpdate(
    const GxAnimatedUpdatePtr& element) {
  VisitGxTourPrimitive(element);
}

void Visitor::VisitGxFlyTo(
    const GxFlyToPtr& element) {
  VisitGxTourPrimitive(element);
}

void Visitor::VisitGxLatLonQuad(
    const GxLatLonQuadPtr& element) {
  VisitObject(element);
}

void Visitor::VisitGxPlaylist(
    const GxPlaylistPtr& element) {
  VisitObject(element);
}

void Visitor::VisitGxSoundCue(
    const GxSoundCuePtr& element) {
  VisitGxTourPrimitive(element);
}

void Visitor::VisitGxTimeSpan(
    const GxTimeSpanPtr& element) {
  VisitTimeSpan(element);
}

void Visitor::VisitGxTimeStamp(
    const GxTimeStampPtr& element) {
  VisitTimeStamp(element);
}

void Visitor::VisitGxTour(
    const GxTourPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitGxTourControl(
    const GxTourControlPtr& element) {
  VisitGxTourPrimitive(element);
}

void Visitor::VisitGxTourPrimitive(
    const GxTourPrimitivePtr& element) {
  VisitObject(element);
}

void Visitor::VisitGxWait(
    const GxWaitPtr& element) {
  VisitGxTourPrimitive(element);
}

void Visitor::VisitHotSpot(
    const HotSpotPtr& element) {
  VisitVec2(element);
}

void Visitor::VisitIcon(
    const IconPtr& element) {
  VisitAbstractLink(element);
}

void Visitor::VisitIconStyle(
    const IconStylePtr& element) {
  VisitColorStyle(element);
}

void Visitor::VisitIconStyleIcon(
    const IconStyleIconPtr& element) {
  VisitBasicLink(element);
}

void Visitor::VisitImagePyramid(
    const ImagePyramidPtr& element) {
  VisitObject(element);
}

void Visitor::VisitInnerBoundaryIs(
    const InnerBoundaryIsPtr& element) {
  VisitElement(element);
}

void Visitor::VisitItemIcon(
    const ItemIconPtr& element) {
  VisitObject(element);
}

void Visitor::VisitKml(
    const KmlPtr& element) {
  VisitElement(element);
}

void Visitor::VisitLabelStyle(
    const LabelStylePtr& element) {
  VisitColorStyle(element);
}

void Visitor::VisitLatLonAltBox(
    const LatLonAltBoxPtr& element) {
  VisitAbstractLatLonBox(element);
}

void Visitor::VisitLatLonBox(
    const LatLonBoxPtr& element) {
  VisitAbstractLatLonBox(element);
}

void Visitor::VisitLineString(
    const LineStringPtr& element) {
  VisitGeometry(element);
}

void Visitor::VisitLineStyle(
    const LineStylePtr& element) {
  VisitColorStyle(element);
}

void Visitor::VisitLinearRing(
    const LinearRingPtr& element) {
  VisitGeometry(element);
}

void Visitor::VisitLink(
    const LinkPtr& element) {
  VisitAbstractLink(element);
}

void Visitor::VisitLinkSnippet(
    const LinkSnippetPtr& element) {
  VisitElement(element);
}

void Visitor::VisitListStyle(
    const ListStylePtr& element) {
  VisitSubStyle(element);
}

void Visitor::VisitLocation(
    const LocationPtr& element) {
  VisitObject(element);
}

void Visitor::VisitLod(
    const LodPtr& element) {
  VisitObject(element);
}

void Visitor::VisitLookAt(
    const LookAtPtr& element) {
  VisitAbstractView(element);
}

void Visitor::VisitMetadata(
    const MetadataPtr& element) {
  VisitElement(element);
}

void Visitor::VisitModel(
    const ModelPtr& element) {
  VisitGeometry(element);
}

void Visitor::VisitMultiGeometry(
    const MultiGeometryPtr& element) {
  VisitGeometry(element);
}

void Visitor::VisitNetworkLink(
    const NetworkLinkPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitNetworkLinkControl(
    const NetworkLinkControlPtr& element) {
  VisitElement(element);
}

void Visitor::VisitObject(
    const ObjectPtr& element) {
  VisitElement(element);
}

void Visitor::VisitOrientation(
    const OrientationPtr& element) {
  VisitObject(element);
}

void Visitor::VisitOuterBoundaryIs(
    const OuterBoundaryIsPtr& element) {
  VisitElement(element);
}

void Visitor::VisitOverlay(
    const OverlayPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitOverlayXY(
    const OverlayXYPtr& element) {
  VisitVec2(element);
}

void Visitor::VisitPair(
    const PairPtr& element) {
  VisitObject(element);
}

void Visitor::VisitPhotoOverlay(
    const PhotoOverlayPtr& element) {
  VisitOverlay(element);
}

void Visitor::VisitPlacemark(
    const PlacemarkPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitPoint(
    const PointPtr& element) {
  VisitGeometry(element);
}

void Visitor::VisitPolyStyle(
    const PolyStylePtr& element) {
  VisitColorStyle(element);
}

void Visitor::VisitPolygon(
    const PolygonPtr& element) {
  VisitGeometry(element);
}

void Visitor::VisitRegion(
    const RegionPtr& element) {
  VisitObject(element);
}

void Visitor::VisitResourceMap(
    const ResourceMapPtr& element) {
  VisitObject(element);
}

void Visitor::VisitRotationXY(
    const RotationXYPtr& element) {
  VisitVec2(element);
}

void Visitor::VisitScale(
    const ScalePtr& element) {
  VisitObject(element);
}

void Visitor::VisitSchema(
    const SchemaPtr& element) {
  VisitObject(element);
}

void Visitor::VisitSchemaData(
    const SchemaDataPtr& element) {
  VisitObject(element);
}

void Visitor::VisitScreenOverlay(
    const ScreenOverlayPtr& element) {
  VisitOverlay(element);
}

void Visitor::VisitScreenXY(
    const ScreenXYPtr& element) {
  VisitVec2(element);
}

void Visitor::VisitSimpleData(
    const SimpleDataPtr& element) {
  VisitElement(element);
}

void Visitor::VisitSimpleField(
    const SimpleFieldPtr& element) {
  VisitElement(element);
}

void Visitor::VisitSize(
    const SizePtr& element) {
  VisitVec2(element);
}

void Visitor::VisitSnippet(
    const SnippetPtr& element) {
  VisitElement(element);
}

void Visitor::VisitStyle(
    const StylePtr& element) {
  VisitStyleSelector(element);
}

void Visitor::VisitStyleMap(
    const StyleMapPtr& element) {
  VisitStyleSelector(element);
}

void Visitor::VisitStyleSelector(
    const StyleSelectorPtr& element) {
  VisitObject(element);
}

void Visitor::VisitSubStyle(
    const SubStylePtr& element) {
  VisitObject(element);
}

void Visitor::VisitTimePrimitive(
    const TimePrimitivePtr& element) {
  VisitObject(element);
}

void Visitor::VisitTimeSpan(
    const TimeSpanPtr& element) {
  VisitTimePrimitive(element);
}

void Visitor::VisitTimeStamp(
    const TimeStampPtr& element) {
  VisitTimePrimitive(element);
}

void Visitor::VisitUpdate(
    const UpdatePtr& element) {
  VisitElement(element);
}

void Visitor::VisitUpdateOperation(
    const UpdateOperationPtr& element) {
  VisitElement(element);
}

void Visitor::VisitUrl(
    const UrlPtr& element) {
  VisitAbstractLink(element);
}

void Visitor::VisitVec2(
    const Vec2Ptr& element) {
  VisitElement(element);
}

void Visitor::VisitViewVolume(
    const ViewVolumePtr& element) {
  VisitObject(element);
}

}  // end namespace kmldom
