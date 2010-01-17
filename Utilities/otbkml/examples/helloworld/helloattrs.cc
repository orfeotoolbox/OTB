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

// Sample of parse and DOM access of attributes.

#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using kmldom::ElementPtr;
using kmldom::HotSpotPtr;
using kmldom::IconStylePtr;
using kmldom::PlacemarkPtr;
using std::cout;
using std::endl;

void CheckHotSpot() {
  const std::string kml(
      "<IconStyle id=\"is42\">"
      "<hotSpot x=\"0.5\" y=\"123\" xunits=\"fraction\" yunits=\"pixels\"/>"
      "</IconStyle>");
  ElementPtr root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  const IconStylePtr iconstyle = kmldom::AsIconStyle(root);
  assert(true == iconstyle->has_hotspot());

  const HotSpotPtr hotspot = iconstyle->get_hotspot();
  assert(true == hotspot->has_x());
  assert(0.5 == hotspot->get_x());
  assert(true == hotspot->has_y());
  assert(123 == hotspot->get_y());
  assert(true == hotspot->has_xunits());
  assert(kmldom::UNITS_FRACTION == hotspot->get_xunits());
  assert(true == hotspot->has_yunits());
  assert(kmldom::UNITS_PIXELS == hotspot->get_yunits());

  // Serialize the sample.
  std::string parsed_kml = kmldom::SerializePretty(hotspot);
  cout << parsed_kml << endl;
}

void CheckId() {
  // Parse some KML with an attribute.
  const std::string kml(
      "<Placemark id=\"placemark123\"><name>x</name></Placemark>");
  ElementPtr root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  const PlacemarkPtr placemark = kmldom::AsPlacemark(root);

  assert("placemark123" == placemark->get_id());

  // Serialize the sample.
  std::string parsed_kml = kmldom::SerializePretty(placemark);
  cout << parsed_kml << endl;
}

int main(int argc, char** argv) {
  CheckId();
  CheckHotSpot();
  return 0;  // All is well if we got this far.
}
