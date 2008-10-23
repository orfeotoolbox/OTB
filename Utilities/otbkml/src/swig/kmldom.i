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

// This is the SWIG interface file for the KML DOM.
// This was developed with SWIG version 1.3.35.

%module kmldom

// Include the headers needed to build kmldom_wrap.cc.
%{
#include "kml/dom.h"
%}

// Tell SWIG about boost::intrusive_ptr and the inheritance hierarchy
// of objects using intrusive_ptr.  Without this SWIG does not know
// that PlacemarkPtr "is a" FeaturePtr "is a" ObjectPtr "is a" ElementPtr.
// (SWIG _does_ know that a Placemark "is a" Feature, but pointers directly
// to these types are never used directly in libkml).
// TODO: push these changes into a future version of SWIG as there
//       are no libkml-specific enhancements these intrusive_ptr .i files.
#ifdef SWIGPYTHON
%include "python/boost_intrusive_ptr.i"
#elif SWIGJAVA
%include "java/boost_intrusive_ptr.i"
#endif

// Classes for abstract elements and internal convenience classes.
SWIG_INTRUSIVE_PTR(Referent, kmlbase::Referent)
SWIG_INTRUSIVE_PTR_DERIVED(Element, kmlbase::Referent, kmldom::Element)
SWIG_INTRUSIVE_PTR_DERIVED(AbstractLatLonBox, kmldom::Object,
                           kmldom::AbstractLatLonBox)
SWIG_INTRUSIVE_PTR_DERIVED(AbstractView, kmldom::Object, kmldom::AbstractView)
SWIG_INTRUSIVE_PTR_DERIVED(BasicLink, kmldom::Object, kmldom::BasicLink)
SWIG_INTRUSIVE_PTR_DERIVED(Container, kmldom::Feature, kmldom::Container)
SWIG_INTRUSIVE_PTR_DERIVED(Coordinates, kmldom::Element, kmldom::Coordinates)
SWIG_INTRUSIVE_PTR_DERIVED(ColorStyle, kmldom::SubStyle, kmldom::ColorStyle)
SWIG_INTRUSIVE_PTR_DERIVED(Feature, kmldom::Object, kmldom::Feature)
SWIG_INTRUSIVE_PTR_DERIVED(Geometry, kmldom::Object, kmldom::Geometry)
SWIG_INTRUSIVE_PTR_DERIVED(Object, kmldom::Element, kmldom::Object)
SWIG_INTRUSIVE_PTR_DERIVED(Overlay, kmldom::Feature, kmldom::Overlay)
SWIG_INTRUSIVE_PTR_DERIVED(SnippetCommon, kmldom::Element,
                           kmldom::SnippetCommon)
SWIG_INTRUSIVE_PTR_DERIVED(StyleSelector, kmldom::Object,
                           kmldom::StyleSelector)
SWIG_INTRUSIVE_PTR_DERIVED(SubStyle, kmldom::Object, kmldom::SubStyle)
SWIG_INTRUSIVE_PTR_DERIVED(TimePrimitive, kmldom::Object,
                           kmldom::TimePrimitive)
SWIG_INTRUSIVE_PTR_DERIVED(UpdateOperation, kmldom::Element,
                           kmldom::UpdateOperation)
SWIG_INTRUSIVE_PTR_DERIVED(Vec2, kmldom::Element, kmldom::Vec2)

// Classes for concrete elements.
SWIG_INTRUSIVE_PTR_DERIVED(Alias, kmldom::Object, kmldom::Alias)
SWIG_INTRUSIVE_PTR_DERIVED(BalloonStyle, kmldom::SubStyle,
                           kmldom::BalloonStyle)
SWIG_INTRUSIVE_PTR_DERIVED(Camera, kmldom::AbstractView, kmldom::Camera)
SWIG_INTRUSIVE_PTR_DERIVED(Change, kmldom::UpdateOperation, kmldom::Change)
SWIG_INTRUSIVE_PTR_DERIVED(Coordinates, kmldom::Element, kmldom::Coordinates)
SWIG_INTRUSIVE_PTR_DERIVED(Create, kmldom::UpdateOperation, kmldom::Create)
SWIG_INTRUSIVE_PTR_DERIVED(Data, kmldom::Object, kmldom::Data)
SWIG_INTRUSIVE_PTR_DERIVED(Delete, kmldom::UpdateOperation, kmldom::Delete)
SWIG_INTRUSIVE_PTR_DERIVED(Document, kmldom::Container, kmldom::Document)
SWIG_INTRUSIVE_PTR_DERIVED(ExtendedData, kmldom::Element, kmldom::ExtendedData)
SWIG_INTRUSIVE_PTR_DERIVED(Folder, kmldom::Container, kmldom::Folder)
SWIG_INTRUSIVE_PTR_DERIVED(GroundOverlay, kmldom::Overlay,
                           kmldom::GroundOverlay)
SWIG_INTRUSIVE_PTR_DERIVED(HotSpot, kmldom::Vec2, kmldom::HotSpot)
SWIG_INTRUSIVE_PTR_DERIVED(Icon, kmldom::BasicLink, kmldom::Icon)
SWIG_INTRUSIVE_PTR_DERIVED(IconStyle, kmldom::ColorStyle, kmldom::IconStyle)
SWIG_INTRUSIVE_PTR_DERIVED(IconStyleIcon, kmldom::BasicLink,
                           kmldom::IconStyleIcon)
SWIG_INTRUSIVE_PTR_DERIVED(ImagePyramid, kmldom::Object, kmldom::ImagePyramid)
SWIG_INTRUSIVE_PTR_DERIVED(InnerBoundaryIs, kmldom::Element,
                           kmldom::InnerBoundaryIs)
SWIG_INTRUSIVE_PTR_DERIVED(ItemIcon, kmldom::Object, kmldom::ItemIcon)
SWIG_INTRUSIVE_PTR_DERIVED(Kml, kmldom::Element, kmldom::Kml)
SWIG_INTRUSIVE_PTR_DERIVED(LabelStyle, kmldom::ColorStyle, kmldom::LabelStyle)
SWIG_INTRUSIVE_PTR_DERIVED(LatLonBox, kmldom::AbstractLatLonBox,
                           kmldom::LatLonBox)
SWIG_INTRUSIVE_PTR_DERIVED(LatLonAltBox, kmldom::AbstractLatLonBox,
                           kmldom::LatLonAltBox)
SWIG_INTRUSIVE_PTR_DERIVED(LinearRing, kmldom::Geometry, kmldom::LinearRing)
SWIG_INTRUSIVE_PTR_DERIVED(LineString, kmldom::Geometry, kmldom::LineString)
SWIG_INTRUSIVE_PTR_DERIVED(LineStyle, kmldom::ColorStyle, kmldom::LineStyle)
SWIG_INTRUSIVE_PTR_DERIVED(Link, kmldom::BasicLink, kmldom::Link)
SWIG_INTRUSIVE_PTR_DERIVED(LinkSnippet, kmldom::SnippetCommon,
                           kmldom::LinkSnippet)
SWIG_INTRUSIVE_PTR_DERIVED(ListStyle, kmldom::SubStyle, kmldom::ListStyle)
SWIG_INTRUSIVE_PTR_DERIVED(Location, kmldom::Object, kmldom::Location)
SWIG_INTRUSIVE_PTR_DERIVED(Lod, kmldom::Object, kmldom::Lod)
SWIG_INTRUSIVE_PTR_DERIVED(LookAt, kmldom::AbstractView, kmldom::LookAt)
SWIG_INTRUSIVE_PTR_DERIVED(Model, kmldom::Geometry, kmldom::Model)
SWIG_INTRUSIVE_PTR_DERIVED(MultiGeometry, kmldom::Geometry,
                           kmldom::MultiGeometry)
SWIG_INTRUSIVE_PTR_DERIVED(NetworkLink, kmldom::Feature, kmldom::NetworkLink)
SWIG_INTRUSIVE_PTR_DERIVED(Orientation, kmldom::Object, kmldom::Orientation)
SWIG_INTRUSIVE_PTR_DERIVED(NetworkLinkControl, kmldom::Element,
                           kmldom::NetworkLinkControl)
SWIG_INTRUSIVE_PTR_DERIVED(OuterBoundaryIs, kmldom::Element,
                           kmldom::OuterBoundaryIs)
SWIG_INTRUSIVE_PTR_DERIVED(OverlayXY, kmldom::Vec2, kmldom::OverlayXY)
SWIG_INTRUSIVE_PTR_DERIVED(Pair, kmldom::Object, kmldom::Pair)
SWIG_INTRUSIVE_PTR_DERIVED(PhotoOverlay, kmldom::Overlay, kmldom::PhotoOverlay)
SWIG_INTRUSIVE_PTR_DERIVED(Placemark, kmldom::Feature, kmldom::Placemark)
SWIG_INTRUSIVE_PTR_DERIVED(Polygon, kmldom::Geometry, kmldom::Polygon)
SWIG_INTRUSIVE_PTR_DERIVED(Point, kmldom::Geometry, kmldom::Point)
SWIG_INTRUSIVE_PTR_DERIVED(PolyStyle, kmldom::ColorStyle, kmldom::PolyStyle)
SWIG_INTRUSIVE_PTR_DERIVED(Region, kmldom::Object, kmldom::Region)
SWIG_INTRUSIVE_PTR_DERIVED(ResourceMap, kmldom::Object, kmldom::ResourceMap)
SWIG_INTRUSIVE_PTR_DERIVED(RotationXY, kmldom::Vec2, kmldom::RotationXY)
SWIG_INTRUSIVE_PTR_DERIVED(Scale, kmldom::Object, kmldom::Scale)
SWIG_INTRUSIVE_PTR_DERIVED(Schema, kmldom::Object, kmldom::Schema)
SWIG_INTRUSIVE_PTR_DERIVED(SchemaData, kmldom::Object, kmldom::SchemaData)
SWIG_INTRUSIVE_PTR_DERIVED(ScreenOverlay, kmldom::Overlay,
                           kmldom::ScreenOverlay)
SWIG_INTRUSIVE_PTR_DERIVED(ScreenXY, kmldom::Vec2, kmldom::ScreenXY)
SWIG_INTRUSIVE_PTR_DERIVED(Size, kmldom::Vec2, kmldom::Size)
SWIG_INTRUSIVE_PTR_DERIVED(SimpleData, kmldom::Element, kmldom::SimpleData)
SWIG_INTRUSIVE_PTR_DERIVED(SimpleField, kmldom::Element, kmldom::SimpleField)
SWIG_INTRUSIVE_PTR_DERIVED(Snippet, kmldom::SnippetCommon, kmldom::Snippet)
SWIG_INTRUSIVE_PTR_DERIVED(Style, kmldom::StyleSelector, kmldom::Style)
SWIG_INTRUSIVE_PTR_DERIVED(StyleMap, kmldom::StyleSelector, kmldom::StyleMap)
SWIG_INTRUSIVE_PTR_DERIVED(TimeSpan, kmldom::TimePrimitive, kmldom::TimeSpan)
SWIG_INTRUSIVE_PTR_DERIVED(TimeStamp, kmldom::TimePrimitive, kmldom::TimeStamp)
SWIG_INTRUSIVE_PTR_DERIVED(ViewVolume, kmldom::Object, kmldom::ViewVolume)
SWIG_INTRUSIVE_PTR_DERIVED(Update, kmldom::Element, kmldom::Update)

// Tell SWIG about C++ Standard Library std::string.
%include "std_string.i"

// Tell SWIG about the kmldom type ids (Type_Placemark, etc).
%include "kml/dom/kml22.h"

// Tell SWIG about the intrusive_ptr typedefs.
%include "kml/dom/kml_ptr.h"

namespace kmlbase {

%nodefaultctor Referent;
class Referent {
};

}  // end namespace kmlbase

namespace kmldom {

// All complex elements in KML are derived from Element.  Element-derived
// classes are constructed using KmlFactory thus no constructor is available.
// All elements are "%nodefaultctor" given that the constructor is private
// in the underlying class since only the factory creates an element instance.

%nodefaultctor Element;
class Element : public kmlbase::Referent{
public:
  virtual KmlDomType Type();
  virtual bool IsA(KmlDomType type_id);
};

class Vec3 {
public:
  double get_longitude();
  double get_latitude();
  double get_altitude();
};

// This is <coordinates> in the KML 2.2 XSD.
%nodefaultctor Coordinates;
class Coordinates : public Element {
public:
  void add_latlng(double latitude, double longitude);
  void add_latlngalt(double latitude, double longitude, double altitude);
  size_t get_coordinates_array_size();
  const Vec3 get_coordinates_array_at(unsigned int index);
};

// This is vec2Type in the KML 2.2 XSD.
%nodefaultctor Vec2;
class Vec2 : public Element {
public:
  // x=
  double get_x();
  void set_x(double value);
  bool has_x();
  void clear_x();

  // y=
  double get_y();
  void set_y(double value);
  bool has_y();
  void clear_y();

  // xunits=
  int get_xunits();
  void set_xunits(int value);
  bool has_xunits();
  void clear_xunits();

  // yunits=
  int get_yunits();
  void set_yunits(int value);
  bool has_yunits();
  void clear_yunits();
};

%include "kmlelements.i"

%nodefaultctor KmlFactory;
class KmlFactory {
public:
  static KmlFactory* GetFactory();

  // Factory functions to create all KML complex elements.
  AliasPtr CreateAlias() const;
  BalloonStylePtr CreateBalloonStyle() const;
  CameraPtr CreateCamera() const;
  ChangePtr CreateChange() const;
  CoordinatesPtr CreateCoordinates() const;
  CreatePtr CreateCreate() const;
  DataPtr CreateData() const;
  DeletePtr CreateDelete() const;
  DocumentPtr CreateDocument() const;
  ExtendedDataPtr CreateExtendedData() const;
  FolderPtr CreateFolder() const;
  GroundOverlayPtr CreateGroundOverlay() const;
  HotSpotPtr CreateHotSpot() const;
  IconPtr CreateIcon() const;
  IconStylePtr CreateIconStyle() const;
  IconStyleIconPtr CreateIconStyleIcon() const;
  ImagePyramidPtr CreateImagePyramid() const;
  InnerBoundaryIsPtr CreateInnerBoundaryIs() const;
  ItemIconPtr CreateItemIcon() const;
  KmlPtr CreateKml() const;
  LabelStylePtr CreateLabelStyle() const;
  LatLonBoxPtr CreateLatLonBox() const;
  LatLonAltBoxPtr CreateLatLonAltBox() const;
  LinearRingPtr CreateLinearRing() const;
  LineStringPtr CreateLineString() const;
  LineStylePtr CreateLineStyle() const;
  LinkPtr CreateLink() const;
  LinkSnippetPtr CreateLinkSnippet() const;
  ListStylePtr CreateListStyle() const;
  LocationPtr CreateLocation() const;
  LodPtr CreateLod() const;
  LookAtPtr CreateLookAt() const;
  ModelPtr CreateModel() const;
  MultiGeometryPtr CreateMultiGeometry() const;
  NetworkLinkPtr CreateNetworkLink() const;
  OrientationPtr CreateOrientation() const;
  NetworkLinkControlPtr CreateNetworkLinkControl() const;
  OuterBoundaryIsPtr CreateOuterBoundaryIs() const;
  OverlayXYPtr CreateOverlayXY() const;
  PairPtr CreatePair() const;
  PhotoOverlayPtr CreatePhotoOverlay() const;
  PlacemarkPtr CreatePlacemark() const;
  PolygonPtr CreatePolygon() const;
  PointPtr CreatePoint() const;
  PolyStylePtr CreatePolyStyle() const;
  RegionPtr CreateRegion() const;
  ResourceMapPtr CreateResourceMap() const;
  RotationXYPtr CreateRotationXY() const;
  ScalePtr CreateScale() const;
  SchemaPtr CreateSchema() const;
  SchemaDataPtr CreateSchemaData() const;
  ScreenOverlayPtr CreateScreenOverlay() const;
  ScreenXYPtr CreateScreenXY() const;
  SizePtr CreateSize() const;
  SimpleDataPtr CreateSimpleData() const;
  SimpleFieldPtr CreateSimpleField() const;
  SnippetPtr CreateSnippet() const;
  StylePtr CreateStyle() const;
  StyleMapPtr CreateStyleMap() const;
  TimeSpanPtr CreateTimeSpan() const;
  TimeStampPtr CreateTimeStamp() const;
  ViewVolumePtr CreateViewVolume() const;
  UpdatePtr CreateUpdate() const;
};

// Parse KML from memory buffer to Element hierarchy.
ElementPtr ParseKml(const std::string& kml);

// Serialize "pretty": newlines and indentation.
std::string SerializePretty(const ElementPtr& element);

// Serialize "raw": no newlines or indentation added.
std::string SerializeRaw(const ElementPtr& element);

}  // end namespace kmldom

// This must come after the elements declared above in kmlelements.i.
%include "kml/dom/kml_cast.h"
