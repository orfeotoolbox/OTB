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

// Walks a Feature hierarchy, looking for Placemarks with Geometry, printing
// the type of Geometry encountered.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/base/file.h"

using kmldom::ContainerPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::GeometryPtr;
using kmldom::KmlPtr;
using kmldom::MultiGeometryPtr;
using kmldom::PlacemarkPtr;
using std::cout;
using std::endl;

static void WalkGeometry(const GeometryPtr& geometry);
static void WalkFeature(const FeaturePtr& feature);
static void WalkContainer(const ContainerPtr& container);
static const FeaturePtr GetRootFeature(const ElementPtr& root);

static void WalkGeometry(const GeometryPtr& geometry) {
  if (!geometry) {
    return;
  }
  // Print the Geometry type.
  cout << "Found a";
  switch(geometry->Type()) {
    case kmldom::Type_Point:
      cout << " Point";
      break;
    case kmldom::Type_LineString:
      cout << " LineString";
      break;
    case kmldom::Type_LinearRing:
      cout << " LinearRing";
      break;
    case kmldom::Type_Polygon:
      cout << " Polygon";
      break;
    case kmldom::Type_MultiGeometry:
      cout << " MultiGeometry";
      break;
    case kmldom::Type_Model:
      cout << " Model";
      break;
    default:  // KML has 6 types of Geometry.
      break;
  }
  cout << endl;
  // Recurse into <MultiGeometry>.
  if (const MultiGeometryPtr multigeometry =
      kmldom::AsMultiGeometry(geometry)) {
    for (size_t i = 0; i < multigeometry->get_geometry_array_size(); ++i) {
      WalkGeometry(multigeometry->get_geometry_array_at(i));
    }
  }
}

static void WalkFeature(const FeaturePtr& feature) {
  if (feature) {
    if (const ContainerPtr container = kmldom::AsContainer(feature)) {
      WalkContainer(container);
    } else if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature)) {
      WalkGeometry(placemark->get_geometry());
    }
  }
}

static void WalkContainer(const ContainerPtr& container) {
  for (size_t i = 0; i < container->get_feature_array_size(); ++i) {
    WalkFeature(container->get_feature_array_at(i));
  }
}

static const FeaturePtr GetRootFeature(const ElementPtr& root) {
  const KmlPtr kml = kmldom::AsKml(root);
  if (kml && kml->has_feature()) {
    return kml->get_feature();
  }
  return kmldom::AsFeature(root);
}

int main(int argc, char** argv) {
  std::string kml;
  kmlbase::File::ReadFileToString(argv[1], &kml);
  std::string errors;
  WalkFeature(GetRootFeature(kmldom::Parse(kml, &errors)));
  if (!errors.empty()) {
    cout << argv[1] << ": parse error" << endl;
    cout << errors << endl;
    return 1;
  }
  return 0;
}

