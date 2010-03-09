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

// This file contains the implementation of the abstract element Geometry
// and the concrete elements coordinates, Point, LineString, LinearRing,
// outerBoundaryIs, innerBoundaryIs and Polygon.

#include "kml/dom/geometry.h"
#include <ctype.h>
#include <stdlib.h>
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;
using kmlbase::Vec3;

namespace kmldom {

Coordinates::Coordinates() {
  set_xmlns(kmlbase::XMLNS_KML22);
}

Coordinates::~Coordinates() {}

// This parses off a Vec3 from the given string and returns a pointer
// to the end of chars consumed.  The purpose is for use in the inner loop
// of the overall parse of a <coordinates> string.  This handles both 2d and
// 3d points and handles any form of whitespace around the commas between
// coordinates.  The general formula is commas between 2 or 3 coordinates
// with any form of whitespace permitted around the commas and whitespace
// between tuples including before the first and after the last.
// Here are some example 3d cstr's which all set the Vec3(1.1, 2.2, 3.3)
// Comma separated coords, space only between tuples:
// "1.1,2.2,3.3 4.4,5.5,6.6"
// Comma separated coords, leading whitespace:
// " 1.1,2.2,3.3 4.4,5.5,6.6"
// "\n 1.1,2.2,3.3 4.4,5.5,6.6"
// Whitespace between coordinates:
// "\t1.1 , 2.2 , 3.3  4.4 , 5.5,6.6"
// Here are some 2d cstr's which all parse to Vec3(1.1, 2.2, 0.0).
// Note that lack of altitude is represented as altitude = 0.0.
// (Actual use of the altitude typeically depends on altitudeMode).
// No spaces. Comma separated as might be common for Point:
// "1.1,2.2"
// A couple of tuples with comma-separated coordinates and single space
// separatting the tuples as might be used in LineString:
// "1.1,2.2 4.4,5.5"
// Leading newlines and tabs as might created by a pretty printer:
// "\n\t1.1, 2.2\t\t4.4, 5.5\n"
// Bad separators are simply discarded and we move to the next comma. A string
// like this: "1.1*2.2,3,3" will become "1.1,3.3,0.0". This precisely matches
// the precent for parsing of bad coordinate strings set by Google Earth.
bool Coordinates::ParseVec3(const char* cstr, char** nextp, Vec3* vec) {
  if (!cstr || !vec) {  // Not much to do w/o input or output.
    return false;
  }
  bool done = false;
  char* endp = const_cast<char*>(cstr);

  // Ignore any commas at the start of our scan. This will cause this:
  // <coordinates>1,2,3,4,5</coordinates> to be treated as:
  // <coordinates>1,2,3 4,5</coordinates>, which is how Google Earth treats
  // the misuse of commas as separators.
  if (*endp == ',') {
    ++endp;
  }

  // Longitude first.  strtod() eats leading whitespace.
  vec->set(0, strtod(endp, &endp));
  if (endp) {

    // Latitude next.
    while (isspace(*endp) || *endp != ',') {
      // We check here to make sure the parse is sane. If we've been passed
      // an invalid coordinate string, this loop will reach the null
      // terminator. If we see it, we set the nextp pointer to the end and
      // return which will let Coordinates::Parse know that it's finished.
      if (*endp == '\0') {
        *nextp = endp;
        return done;
      }
      // Eat whitespace between double and comma.
      ++endp;
    }
    vec->set(1, strtod(endp+1, &endp));
    done = true;  // Need at least lon,lat to be valid.

    // If no altitude set to 0
    while (isspace(*endp)) {  // Eat whitespace between double and comma.
      ++endp;
    }
    if (*endp == ',') {
      // Note that this sets altitude only if an altitude is supplied.
      vec->set(2, strtod(endp+1, &endp));
    }
  }
  if (nextp) {
    while (isspace(*endp)) {  // Eat the remaining whitespace before return.
      ++endp;
    }
    *nextp = endp;
  }
  return done;
}

// The char_data is everything between <coordinates> elements including
// leading and trailing whitespace.
void Coordinates::Parse(const string& char_data) {
  const char* cstr = char_data.c_str();
  const char* endp = cstr + char_data.size();
  char* next = const_cast<char*>(cstr);
  while (next != endp) {
    Vec3 vec;
    if (ParseVec3(next, &next, &vec)) {
      coordinates_array_.push_back(vec);
    }
  }
}

// Coordinates essentially parses itself.
void Coordinates::AddElement(const ElementPtr& element) {
  Parse(get_char_data());
}

void Coordinates::Serialize(Serializer& serializer) const {
  Attributes dummy;
  serializer.BeginById(Type(), dummy);
  serializer.BeginElementArray(Type(), coordinates_array_.size());
  for (size_t i = 0; i < coordinates_array_.size(); ++i) {
    serializer.SaveVec3(coordinates_array_[i]);
  }
  serializer.EndElementArray(Type_coordinates);
  serializer.End();
}

void Coordinates::Accept(Visitor* visitor) {
  visitor->VisitCoordinates(CoordinatesPtr(this));
}

Geometry::Geometry() {}

Geometry::~Geometry() {}

AltitudeGeometryCommon::AltitudeGeometryCommon()
  : altitudemode_(ALTITUDEMODE_CLAMPTOGROUND),
    has_altitudemode_(false),
    gx_altitudemode_(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR),
    has_gx_altitudemode_(false) {
}

AltitudeGeometryCommon::~AltitudeGeometryCommon() {
}

void AltitudeGeometryCommon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_altitudeMode:
      has_altitudemode_ = element->SetEnum(&altitudemode_);
      return;
    case Type_GxAltitudeMode:
      has_gx_altitudemode_ = element->SetEnum(&gx_altitudemode_);
      return;
    default:
      Geometry::AddElement(element);
  }
}

ExtrudeGeometryCommon::ExtrudeGeometryCommon()
  : extrude_(false),
    has_extrude_(false) {
}

ExtrudeGeometryCommon::~ExtrudeGeometryCommon() {
}

void ExtrudeGeometryCommon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_extrude) {
    has_extrude_ = element->SetBool(&extrude_);
    return;
  }
  AltitudeGeometryCommon::AddElement(element);
}

CoordinatesGeometryCommon::CoordinatesGeometryCommon() {}

CoordinatesGeometryCommon::~CoordinatesGeometryCommon() {}

void CoordinatesGeometryCommon::AddElement(const ElementPtr& element) {
  if (CoordinatesPtr coordinates = AsCoordinates(element)) {
    set_coordinates(coordinates);
  } else {
    ExtrudeGeometryCommon::AddElement(element);
  }
}


void CoordinatesGeometryCommon::AcceptChildren(VisitorDriver* driver) {
  ExtrudeGeometryCommon::AcceptChildren(driver);
  if (has_coordinates()) {
    driver->Visit(get_coordinates());
  }
}

Point::Point() {}

Point::~Point() {}

void Point::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Geometry::Serialize(serializer);
  if (has_extrude()) {
    serializer.SaveFieldById(Type_extrude, get_extrude());
  }
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  if (has_gx_altitudemode()) {
    serializer.SaveEnum(Type_GxAltitudeMode, get_gx_altitudemode());
  }
  if (has_coordinates()) {
    serializer.SaveElement(get_coordinates());
  }
}

void Point::Accept(Visitor* visitor) {
  visitor->VisitPoint(PointPtr(this));
}

LineCommon::LineCommon()
  : tessellate_(false),
    has_tessellate_(false) {
}

LineCommon::~LineCommon() {}

void LineCommon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_tessellate) {
    has_tessellate_ = element->SetBool(&tessellate_);
    return;
  }
  CoordinatesGeometryCommon::AddElement(element);
}

void LineCommon::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Geometry::Serialize(serializer);
  if (has_extrude()) {
    serializer.SaveFieldById(Type_extrude, get_extrude());
  }
  if (has_tessellate()) {
    serializer.SaveFieldById(Type_tessellate, get_tessellate());
  }
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  if (has_gx_altitudemode()) {
    serializer.SaveEnum(Type_GxAltitudeMode, get_gx_altitudemode());
  }
  if (has_coordinates()) {
    serializer.SaveElement(get_coordinates());
  }
}

LineString::LineString() {}

LineString::~LineString() {}

void LineString::Accept(Visitor* visitor) {
  visitor->VisitLineString(LineStringPtr(this));
}

LinearRing::LinearRing() {}

LinearRing::~LinearRing() {}

void LinearRing::Accept(Visitor* visitor) {
  visitor->VisitLinearRing(LinearRingPtr(this));
}

BoundaryCommon::BoundaryCommon() {}

BoundaryCommon::~BoundaryCommon() {}

void BoundaryCommon::AddElement(const ElementPtr& element) {
  if (LinearRingPtr linearring = AsLinearRing(element)) {
    set_linearring(linearring);
  } else {
    Element::AddElement(element);
  }
}

void BoundaryCommon::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  if (has_linearring()) {
    serializer.SaveElement(get_linearring());
  }
}


void BoundaryCommon::AcceptChildren(VisitorDriver* driver) {
  Element::AcceptChildren(driver);
  if (has_linearring()) {
    driver->Visit(get_linearring());
  }
}

OuterBoundaryIs::OuterBoundaryIs() {}

OuterBoundaryIs::~OuterBoundaryIs() {}

void OuterBoundaryIs::Accept(Visitor* visitor) {
  visitor->VisitOuterBoundaryIs(OuterBoundaryIsPtr(this));
}

InnerBoundaryIs::InnerBoundaryIs() {}

InnerBoundaryIs::~InnerBoundaryIs() {}

void InnerBoundaryIs::Accept(Visitor* visitor) {
  visitor->VisitInnerBoundaryIs(InnerBoundaryIsPtr(this));
}

Polygon::Polygon()
  : tessellate_(false),
    has_tessellate_(false) {
}

Polygon::~Polygon() {}

void Polygon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_tessellate:
      has_tessellate_ = element->SetBool(&tessellate_);
      break;
    case Type_outerBoundaryIs:
      set_outerboundaryis(AsOuterBoundaryIs(element));
      break;
    case Type_innerBoundaryIs:
      add_innerboundaryis(AsInnerBoundaryIs(element));
      break;
    default:
      ExtrudeGeometryCommon::AddElement(element);
  }
}

void Polygon::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Geometry::Serialize(serializer);
  if (has_extrude()) {
    serializer.SaveFieldById(Type_extrude, get_extrude());
  }
  if (has_tessellate()) {
    serializer.SaveFieldById(Type_tessellate, get_tessellate());
  }
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  if (has_gx_altitudemode()) {
    serializer.SaveEnum(Type_GxAltitudeMode, get_gx_altitudemode());
  }
  if (has_outerboundaryis()) {
    serializer.SaveElement(get_outerboundaryis());
  }
  serializer.SaveElementArray(innerboundaryis_array_);
}

void Polygon::Accept(Visitor* visitor) {
  visitor->VisitPolygon(PolygonPtr(this));
}

void Polygon::AcceptChildren(VisitorDriver* driver) {
  ExtrudeGeometryCommon::AcceptChildren(driver);
  if (has_outerboundaryis()) {
    driver->Visit(get_outerboundaryis());
  }
  Element::AcceptRepeated<InnerBoundaryIsPtr>(&innerboundaryis_array_, driver);
}

MultiGeometry::MultiGeometry() {}

MultiGeometry::~MultiGeometry() {}

void MultiGeometry::add_geometry(const GeometryPtr& geometry) {
  AddComplexChild(geometry, &geometry_array_);
}

void MultiGeometry::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_Geometry)) {
    add_geometry(AsGeometry(element));
    return;
  }
  Geometry::AddElement(element);
}

void MultiGeometry::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Geometry::Serialize(serializer);
  serializer.SaveElementGroupArray(geometry_array_, Type_Geometry);
}

void MultiGeometry::Accept(Visitor* visitor) {
  visitor->VisitMultiGeometry(MultiGeometryPtr(this));
}

void MultiGeometry::AcceptChildren(VisitorDriver* driver) {
  Geometry::AcceptChildren(driver);
  Element::AcceptRepeated<GeometryPtr>(&geometry_array_, driver);
}

}  // end namespace kmldom
