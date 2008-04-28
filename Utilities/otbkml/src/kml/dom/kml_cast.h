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

namespace kmldom {

// Abstract element groups.
const AbstractView* AsAbstractView(const Element* element);
const ColorStyle* AsColorStyle(const Element* element);
const Container* AsContainer(const Element* element);
const Feature* AsFeature(const Element* element);
const Geometry* AsGeometry(const Element* element);
const Object* AsObject(const Element* element);
const Overlay* AsOverlay(const Element* element);
const StyleSelector* AsStyleSelector(const Element* element);
const SubStyle* AsSubStyle(const Element* element);
const TimePrimitive* AsTimePrimitive(const Element* element);

// Concrete elements.
const Alias* AsAlias(const Element* element);
const BalloonStyle* AsBalloonStyle(const Element* element);
const Camera* AsCamera(const Element* element);
const Change* AsChange(const Element* element);
const Create* AsCreate(const Element* element);
const Data* AsData(const Element* element);
const Delete* AsDelete(const Element* element);
const Document* AsDocument(const Element* element);
const ExtendedData* AsExtendedData(const Element* element);
const Folder* AsFolder(const Element* element);
const GroundOverlay* AsGroundOverlay(const Element* element);
const HotSpot* AsHotSpot(const Element* element);
const Icon* AsIcon(const Element* element);
const IconStyle* AsIconStyle(const Element* element);
const ImagePyramid* AsImagePyramid(const Element* element);
const InnerBoundaryIs* AsInnerBoundaryIs(const Element* element);
const ItemIcon* AsItemIcon(const Element* element);
const Kml* AsKml(const Element* element);
const LabelStyle* AsLabelStyle(const Element* element);
const LatLonAltBox* AsLatLonAltBox(const Element* element);
const LatLonBox* AsLatLonBox(const Element* element);
const LineString* AsLineString(const Element* element);
const LineStyle* AsLineStyle(const Element* element);
const LinearRing* AsLinearRing(const Element* element);
const Link* AsLink(const Element* element);
const LinkSnippet* AsLinkSnippet(const Element* element);
const ListStyle* AsListStyle(const Element* element);
const Location* AsLocation(const Element* element);
const Lod* AsLod(const Element* element);
const LookAt* AsLookAt(const Element* element);
const Model* AsModel(const Element* element);
const MultiGeometry* AsMultiGeometry(const Element* element);
const NetworkLink* AsNetworkLink(const Element* element);
const NetworkLinkControl* AsNetworkLinkControl(const Element* element);
const Orientation* AsOrientation(const Element* element);
const OuterBoundaryIs* AsOuterBoundaryIs(const Element* element);
const OverlayXY* AsOverlayXY(const Element* element);
const Pair* AsPair(const Element* element);
const PhotoOverlay* AsPhotoOverlay(const Element* element);
const Placemark* AsPlacemark(const Element* element);
const Point* AsPoint(const Element* element);
const PolyStyle* AsPolyStyle(const Element* element);
const Polygon* AsPolygon(const Element* element);
const Region* AsRegion(const Element* element);
const ResourceMap* AsResourceMap(const Element* element);
const RotationXY* AsRotationXY(const Element* element);
const Scale* AsScale(const Element* element);
const Schema* AsSchema(const Element* element);
const SchemaData* AsSchemaData(const Element* element);
const ScreenOverlay* AsScreenOverlay(const Element* element);
const ScreenXY* AsScreenXY(const Element* element);
const SimpleData* AsSimpleData(const Element* element);
const SimpleField* AsSimpleField(const Element* element);
const Size* AsSize(const Element* element);
const Snippet* AsSnippet(const Element* element);
const Style* AsStyle(const Element* element);
const StyleMap* AsStyleMap(const Element* element);
const TimeSpan* AsTimeSpan(const Element* element);
const TimeStamp* AsTimeStamp(const Element* element);
const Update* AsUpdate(const Element* element);
const ViewVolume* AsViewVolume(const Element* element);

}  // end namespace kmldom

#endif  // KML_DOM_KML_CAST_H__
