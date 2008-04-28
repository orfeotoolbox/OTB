/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    Some parts of this code are derived from kml library examples. See KMLCopyright.txt
    for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(kmlprintgeometry);
}


// Walks a Feature hierarchy, looking for Placemarks with Geometry, printing
// the type of Geometry encountered.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/fileio.h"
#include "kml/util/kmlprint.h"

using kmldom::Container;
using kmldom::Element;
using kmldom::Feature;
using kmldom::Geometry;
using kmldom::Kml;
using kmldom::MultiGeometry;
using kmldom::Placemark;
using std::endl;

static void WalkGeometry(const Geometry* geometry);
static void WalkFeature(const Feature* feature);
static void WalkContainer(const Container& container);
static const Feature* GetRootFeature(const Element* root);

static void WalkGeometry(const Geometry* geometry) {
  if (!geometry) {
    return;
  }
  // Print the Geometry type.
  G_kmlprint.flux << "Found a";
  switch(geometry->Type()) {
    case kmldom::Type_Point:
      G_kmlprint.flux << " Point";
      break;
    case kmldom::Type_LineString:
      G_kmlprint.flux << " LineString";
      break;
    case kmldom::Type_LinearRing:
      G_kmlprint.flux << " LinearRing";
      break;
    case kmldom::Type_Polygon:
      G_kmlprint.flux << " Polygon";
      break;
    case kmldom::Type_MultiGeometry:
      G_kmlprint.flux << " MultiGeometry";
      break;
    case kmldom::Type_Model:
      G_kmlprint.flux << " Model";
      break;
    default:  // KML has 6 types of Geometry.
      break;
  }
  G_kmlprint.flux << endl;
  // Recurse into <MultiGeometry>.
  if (const MultiGeometry* multigeometry = kmldom::AsMultiGeometry(geometry)) {
    for (size_t i = 0; i < multigeometry->geometry_array_size(); ++i) {
      WalkGeometry(multigeometry->geometry_array_at(i));
    }
  }
}

static void WalkFeature(const Feature* feature) {
  if (feature) {
    if (const Container* container = kmldom::AsContainer(feature)) {
      WalkContainer(*container);
    } else if (const Placemark* placemark = kmldom::AsPlacemark(feature)) {
      WalkGeometry(placemark->geometry());
    }
  }
}

static void WalkContainer(const Container& container) {
  for (size_t i = 0; i < container.feature_array_size(); ++i) {
    WalkFeature(container.feature_array_at(i));
  }
}

static const Feature* GetRootFeature(const Element* root) {
  if (root) {
    const Kml* kml = kmldom::AsKml(root);
    if (kml && kml->has_feature()) {
      return kml->feature();
    }
    return kmldom::AsFeature(root);
  }
  return NULL;
}

int kmlprintgeometry(int argc, char* argv[]) {
  std::string kml;
G_kmlprint.flux.open(argv[2]);
  ReadFileToString(argv[1], &kml);
  std::string errors;
  Element* root = kmldom::Parse(kml, &errors);
  if (root) {
    WalkFeature(GetRootFeature(root));
    delete root;
  }
G_kmlprint.flux.close();
  return EXIT_SUCCESS;
}

