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

// This program makes some basic use of the KML DOM API.

#include <assert.h>
#include <iostream>
#include "kml/dom.h"

using std::cout;
using std::endl;
using kmldom::CoordinatesPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

void HelloKml(bool verbose) {
  KmlFactory* factory(KmlFactory::GetFactory());
  // <coordinates>
  CoordinatesPtr coordinates(factory->CreateCoordinates());
  coordinates->add_latlngalt(37.123, -122.456, 314.159);
  // <Point><coordinates>...
  PointPtr point(factory->CreatePoint());
  point->set_coordinates(coordinates);
  // <Point><altitudeMode>...<coordinates>...
  point->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  assert(point->get_altitudemode() == kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  // <Placemark><Point><coordinates>...
  PlacemarkPtr placemark(factory->CreatePlacemark());
  placemark->set_geometry(point);

  // A Placemark is (duh) a Placemark
  assert (placemark->Type() == kmldom::Type_Placemark);
  // It's also a Feature.
  assert(placemark->IsA(kmldom::Type_Feature));
  placemark->set_name("point placemark");
  if (verbose) {
    cout << "Placemark's name is " << placemark->get_name() << endl;
  }
  // We know it has some geometry.
  assert(placemark->has_geometry());
  // And we can test to see if that geometry is a Point.
  assert(placemark->get_geometry()->IsA(kmldom::Type_Point));
  // If it is, we can make a point from it. (Yes, API should hide casting.)
  const PointPtr pt = kmldom::AsPoint(placemark->get_geometry());
  assert(pt->get_altitudemode() == kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  if (verbose) {
    cout.precision(6);
    cout << placemark->get_name() << " is located at: ";
    cout << pt->get_coordinates()->get_coordinates_array_at(0).get_latitude()
         << ", ";
    cout << pt->get_coordinates()->get_coordinates_array_at(0).get_longitude()
         << endl;
  }

  // All storage is freed by smart pointers as they go out of scope.
}

int main(int argc, char** argv) {
  HelloKml(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v');
  return 0;
}
