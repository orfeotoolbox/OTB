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

// This sample program creates a polygon representation of a great circle.
// You can define the circle either by a point and a radius from that point,
// or by a central point and a point on the circumference.

#include <iostream>
#include <vector>
#include "kml/dom.h"
#include "kml/base/math_util.h"
#include "kml/base/vec3.h"
#include "kml/convenience/convenience.h"

using kmldom::KmlFactory;
using kmldom::LinearRingPtr;
using kmldom::CoordinatesPtr;
using kmldom::PlacemarkPtr;
using std::cout;
using std::cerr;
using std::endl;

// The number of lines composing the circumference of the circle.
static const size_t kSegments = 360;

// Creates a LinearRing that is the great circle described by a constant
// radius from lat, lng.
static LinearRingPtr CircleFromPointAndRadius(double lat, double lng,
                                              double radius) {
  CoordinatesPtr coords = kmlconvenience::CreateCoordinatesCircle(
      lat, lng, radius, kSegments);                                        
  LinearRingPtr lr = KmlFactory::GetFactory()->CreateLinearRing();
  lr->set_coordinates(coords);
  return lr;
}

// Creates a LinearRing that is the great circle described by a circumfrential
// point (circum_lat, circum_lng) from a central point (center_lat,
// center_lng).
static LinearRingPtr CircleFromTwoPoints(double center_lat, double center_lng,
                                         double circum_lat, double circum_lng) {
  double radius = kmlbase::DistanceBetweenPoints(center_lat, center_lng,
                                                 circum_lat, circum_lng);
  return CircleFromPointAndRadius(center_lat, center_lng, radius);
}

// Generate the Polygon circle and print to stdout.
static int PrintPlacemarkCircle(LinearRingPtr lr) {
  PlacemarkPtr p = kmlconvenience::CreateBasicPolygonPlacemark(lr);
  cout << kmldom::SerializePretty(p);
  return 0;
}

static int DoPointRadius(double lat, double lng, double radius) {
  LinearRingPtr lr = CircleFromPointAndRadius(lat, lng, radius);
  return PrintPlacemarkCircle(lr);
}

static int DoTwoPoints(double lat1, double lng1, double lat2, double lng2) {
  LinearRingPtr lr = CircleFromTwoPoints(lat1, lng1, lat2, lng2);
  return PrintPlacemarkCircle(lr);
}

int kmlcirclegen(int argc, char* argv[]) {
  if (argc !=4 && argc !=5 ) {
    cerr << "Supply either a central point (lat, lng in decimal degrees) " <<
      "and a radius (in meters) OR a central point and a point on " <<
      "the circumference" << endl;
    cerr << "usage: " << argv[0] << " center-lat center-lng radius" << endl;
    cerr << "usage: " << argv[0] << " center-lat center-lng circum-lat " <<
      "circum-lng" << endl;
    cerr << endl;
    return 1;
  }

  double center_lat = strtod(argv[1], NULL);
  double center_lng = strtod(argv[2], NULL);
  if (argc == 4) {
    double radius = strtod(argv[3], NULL);
    return DoPointRadius(center_lat, center_lng, radius);
  }
  if (argc == 5) {
    double circum_lat = strtod(argv[3], NULL);
    double circum_lng = strtod(argv[4], NULL);
    return DoTwoPoints(center_lat, center_lng, circum_lat, circum_lng);
  }
  return EXIT_SUCCESS;
}
