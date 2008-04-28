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
// This was developed with SWIG version 1.3.33.

%module kmldom

%{
#include "kml/dom.h"
%}

%include "std_string.i"

// These declare the kmldom namespace
%include "kml/dom/kml22.h"

namespace kmldom {

// All complex elements in KML are derived from Element.  Element-derived
// classes are constructed using KmlFactory thus no constructor is available.
// All Element-derived classes are destroyed with KmlFactory::DeleteElement().

%nodefaultctor Element;
class Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

class Vec3 {
public:
  double longitude();
  double latitude();
  double altitude();
};

%nodefaultctor Coordinates;
class Coordinates : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  void add_point2(double longitude, double latitude);
  void add_point3(double longitude, double latitude, double altitude);
  size_t coordinates_array_size() const;
  const Vec3 coordinates_array_at(unsigned int index);
};

%nodefaultctor Vec2;
class Vec2 : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(KmlDomType type);

  // x=
  double x() const;
  void set_x(double value);
  bool has_x() const;
  void clear_x();

  // y=
  double y() const;
  void set_y(double value);
  bool has_y() const;
  void clear_y();

  // xunits=
  int xunits() const;
  void set_xunits(int value);
  bool has_xunits() const;
  void clear_xunits();

  // yunits=
  int yunits() const;
  void set_yunits(int value);
  bool has_yunits() const;
  void clear_yunits();
};

%include "kmlelements.i"

%nodefaultctor KmlFactory;
class KmlFactory {
public:
  static kmldom::KmlFactory* GetFactory();
  void DeleteElement(kmldom::Element* element);

  // Factory functions to create all KML complex elements.
  Alias* CreateAlias() const;
  BalloonStyle* CreateBalloonStyle() const;
  Coordinates* CreateCoordinates() const;
  Camera* CreateCamera() const;
  Change* CreateChange() const;
  Create* CreateCreate() const;
  Data* CreateData() const;
  Delete* CreateDelete() const;
  Document* CreateDocument() const;
  ExtendedData* CreateExtendedData() const;
  Folder* CreateFolder() const;
  GroundOverlay* CreateGroundOverlay() const;
  HotSpot* CreateHotSpot() const;
  Icon* CreateIcon() const;
  IconStyle* CreateIconStyle() const;
  IconStyleIcon* CreateIconStyleIcon() const;
  ImagePyramid* CreateImagePyramid() const;
  InnerBoundaryIs* CreateInnerBoundaryIs() const;
  ItemIcon* CreateItemIcon() const;
  Kml* CreateKml() const;
  LabelStyle* CreateLabelStyle() const;
  LatLonBox* CreateLatLonBox() const;
  LatLonAltBox* CreateLatLonAltBox() const;
  LinearRing* CreateLinearRing() const;
  LineString* CreateLineString() const;
  LineStyle* CreateLineStyle() const;
  Link* CreateLink() const;
  ListStyle* CreateListStyle() const;
  Location* CreateLocation() const;
  Lod* CreateLod() const;
  LookAt* CreateLookAt() const;
  Model* CreateModel() const;
  MultiGeometry* CreateMultiGeometry() const;
  NetworkLink* CreateNetworkLink() const;
  Orientation* CreateOrientation() const;
  NetworkLinkControl* CreateNetworkLinkControl() const;
  OuterBoundaryIs* CreateOuterBoundaryIs() const;
  OverlayXY* CreateOverlayXY() const;
  Pair* CreatePair() const;
  PhotoOverlay* CreatePhotoOverlay() const;
  Placemark* CreatePlacemark() const;
  Polygon* CreatePolygon() const;
  Point* CreatePoint() const;
  PolyStyle* CreatePolyStyle() const;
  Region* CreateRegion() const;
  ResourceMap* CreateResourceMap() const;
  RotationXY* CreateRotationXY() const;
  Scale* CreateScale() const;
  Schema* CreateSchema() const;
  SchemaData* CreateSchemaData() const;
  ScreenOverlay* CreateScreenOverlay() const;
  ScreenXY* CreateScreenXY() const;
  Size* CreateSize() const;
  SimpleData* CreateSimpleData() const;
  SimpleField* CreateSimpleField() const;
  Snippet* CreateSnippet() const;
  Style* CreateStyle() const;
  StyleMap* CreateStyleMap() const;
  TimeSpan* CreateTimeSpan() const;
  TimeStamp* CreateTimeStamp() const;
  ViewVolume* CreateViewVolume() const;
  Update* CreateUpdate() const;
};

// Parse KML from memory buffer to Element hierarchy.
kmldom::Element* ParseKml(const std::string& kml);

// Serialize "pretty": newlines and indentation.
std::string SerializePretty(const kmldom::Element& element);

// Serialize "raw": no newlines or indentation added.
std::string SerializeRaw(const kmldom::Element& element);

}  // end namespace kmldom

// This must come after the elements declared above.
%include "kml/dom/kml_cast.h"

