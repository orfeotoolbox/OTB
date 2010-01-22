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

// This file contains some sample code for Geometry elements:
// Point, LineString, LinearRing, Polygon, MultiGeometry

#include <iostream>
#include <sstream>
#include <string>
#include "kml/dom.h"

using std::stringstream;
using kmldom::CoordinatesPtr;
using kmldom::FolderPtr;
using kmldom::InnerBoundaryIsPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::LinearRingPtr;
using kmldom::LineStringPtr;
using kmldom::MultiGeometryPtr;
using kmldom::OuterBoundaryIsPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::PolygonPtr;

static void CreateGeometry(bool verbose);
template<int N> static LinearRingPtr CreateBoundary(const double (&corners)[N]);
static PlacemarkPtr CreateLineStringPlacemark();
static PlacemarkPtr CreateMultiGeometryPlacemark();
static PointPtr Create2dPoint(int id, double longitude, double latitude);
static PlacemarkPtr CreatePointPlacemark();
static PlacemarkPtr CreateSimplePolygon();
static PlacemarkPtr CreateTwoHolePolygon();

static PlacemarkPtr CreatePointPlacemark() {
  KmlFactory* factory = KmlFactory::GetFactory();

  CoordinatesPtr coordinates = factory->CreateCoordinates();
  coordinates->add_latlng(36.36, -123.32);

  PointPtr point = factory->CreatePoint();
  point->set_coordinates(coordinates);

  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_name("Point");
  placemark->set_geometry(point);

  return placemark;
}

static PlacemarkPtr CreateLineStringPlacemark() {
  KmlFactory* factory = KmlFactory::GetFactory();

  CoordinatesPtr coordinates = factory->CreateCoordinates();
  coordinates->add_latlngalt(38.38, -121.12, 123.456);
  coordinates->add_latlngalt(37.37, -122.22, 122.345);
  coordinates->add_latlngalt(36.36, -123.32, 121.234);
  coordinates->add_latlng(36.36, -123.32);

  LineStringPtr linestring = factory->CreateLineString();
  linestring->set_extrude(true);
  linestring->set_tessellate(true);
  linestring->set_altitudemode(kmldom::ALTITUDEMODE_ABSOLUTE);
  linestring->set_coordinates(coordinates);

  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_geometry(linestring);
  placemark->set_name("LineString");

  return placemark;
}

static PlacemarkPtr CreateSimplePolygon() {
  KmlFactory* factory = KmlFactory::GetFactory();

  CoordinatesPtr coordinates = factory->CreateCoordinates();
  coordinates->add_latlngalt(38.38, -121.12, 123.456);
  coordinates->add_latlngalt(37.37, -122.22, 122.345);
  coordinates->add_latlngalt(36.36, -123.32, 121.234);
  coordinates->add_latlngalt(38.38, -121.12, 123.456);

  LinearRingPtr linearring = factory->CreateLinearRing();
  linearring->set_coordinates(coordinates);

  OuterBoundaryIsPtr outerboundaryis = factory->CreateOuterBoundaryIs();
  outerboundaryis->set_linearring(linearring);

  PolygonPtr polygon = factory->CreatePolygon();
  polygon->set_tessellate(true);
  polygon->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  polygon->set_outerboundaryis(outerboundaryis);

  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_name("Simple Polygon");
  placemark->set_geometry(polygon);

  return placemark;
}

// Corners presumed to be the corners of the ring.
template<int N>
static LinearRingPtr CreateBoundary(const double (&corners)[N]) {
  KmlFactory* factory = KmlFactory::GetFactory();
  CoordinatesPtr coordinates = factory->CreateCoordinates();
  for (int i = 0; i < N; i+=2) {
    coordinates->add_latlng(corners[i], corners[i+1]);
  }
  // Last must be the same as first in a LinearRing.
  coordinates->add_latlng(corners[0], corners[1]);

  LinearRingPtr linearring = factory->CreateLinearRing();
  linearring->set_coordinates(coordinates);
  return linearring;
}

static PlacemarkPtr CreateTwoHolePolygon() {
  KmlFactory* factory = KmlFactory::GetFactory();
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_name("Polygon with 2 holes");

  PolygonPtr polygon = factory->CreatePolygon();

  const double outer_corners[] = {
    37.80198570954779,-122.4319382787491,
    37.80166118304026,-122.4318730681021,
    37.8017138829201,-122.4314979385389,
    37.80202995372478,-122.4315644851293
  };

  const double inner_west_corners[] = {
    37.80193182195953,-122.4318860160652,
    37.80172085332186,-122.4318421173957,
    37.80173660441158,-122.4317194993788,
    37.8019475540673,-122.4317632243884
  };

  const double inner_east_corners[] = {
    37.80195363606838,-122.4317133795781,
    37.80174288373532,-122.4316718851426,
    37.8017586463012,-122.4315492963515,
    37.80196848593953,-122.4315926416269
  };
  OuterBoundaryIsPtr outerboundaryis = factory->CreateOuterBoundaryIs();
  outerboundaryis->set_linearring(CreateBoundary(outer_corners));
  polygon->set_outerboundaryis(outerboundaryis);

  InnerBoundaryIsPtr innerboundaryis = factory->CreateInnerBoundaryIs();
  innerboundaryis->set_linearring(CreateBoundary(inner_west_corners));
  polygon->add_innerboundaryis(innerboundaryis);

  innerboundaryis = factory->CreateInnerBoundaryIs();
  innerboundaryis->set_linearring(CreateBoundary(inner_east_corners));
  polygon->add_innerboundaryis(innerboundaryis);

  placemark->set_geometry(polygon);

  return placemark;
}

static PointPtr Create2dPoint(int id, double longitude, double latitude) {
  KmlFactory* factory = KmlFactory::GetFactory();

  CoordinatesPtr coordinates = factory->CreateCoordinates();
  coordinates->add_latlng(longitude, latitude);

  PointPtr point = factory->CreatePoint();
  stringstream ss;
  ss << "pt_" << id;
  point->set_id(ss.str());
  point->set_coordinates(coordinates);
  return point;
}

static PlacemarkPtr CreateMultiGeometryPlacemark() {
  KmlFactory* factory = KmlFactory::GetFactory();

  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_name("MultiGeometry");
  placemark->set_id("pmmg");
  MultiGeometryPtr multigeometry = factory->CreateMultiGeometry();
  placemark->set_geometry(multigeometry);

  const int num_points = 3;
  for (int i = 0; i < num_points; ++i) {
    multigeometry->add_geometry(Create2dPoint(-120 + i, i, 37 + i));
  }

  return placemark;
}

static void CreateGeometry(bool verbose) {
  KmlFactory* factory = KmlFactory::GetFactory();

  FolderPtr folder = factory->CreateFolder();
  folder->add_feature(CreatePointPlacemark());
  folder->add_feature(CreateLineStringPlacemark());
  folder->add_feature(CreateSimplePolygon());
  folder->add_feature(CreateTwoHolePolygon());
  folder->add_feature(CreateMultiGeometryPlacemark());

  KmlPtr kml = factory->CreateKml();
  kml->set_feature(folder);

  if (verbose) {
    std::cout << kmldom::SerializePretty(kml);
  }
}

int main(int argc, char** argv) {
  // Silent by default.
  bool verbose = argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v';
  CreateGeometry(verbose);
  return 0;  // All is well if we got this far.
}
