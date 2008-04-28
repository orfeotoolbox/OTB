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
REGISTER_TEST(kmlhelloattrs);
}


// Sample of parse and dom access of attributes.

//#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using kmldom::Element;
using kmldom::HotSpot;
using kmldom::IconStyle;
using kmldom::Placemark;

#include "kml/util/kmlprint.h"

void CheckHotSpot() {
  const std::string kml(
      "<IconStyle id=\"is42\">"
      "<hotSpot x=\"0.5\" y=\"123\" xunits=\"fraction\" yunits=\"pixels\"/>"
      "</IconStyle>");
  Element* root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  const IconStyle* iconstyle = kmldom::AsIconStyle(root);
  assert(true == iconstyle->has_hotspot());

  const HotSpot* hotspot = iconstyle->hotspot();
  assert(true == hotspot->has_x());
  assert(0.5 == hotspot->x());
  assert(true == hotspot->has_y());
  assert(123 == hotspot->y());
  assert(true == hotspot->has_xunits());
  assert(kmldom::UNITS_FRACTION == hotspot->xunits());
  assert(true == hotspot->has_yunits());
  assert(kmldom::UNITS_PIXELS == hotspot->yunits());

  // Serialize the sample.
  std::string parsed_kml = kmldom::SerializePretty(*hotspot);
  G_kmlprint.flux <<"parsed_kml: " <<parsed_kml<<std::endl;
//  std::cout << parsed_kml << std::endl;

  // Free all resources allocated by the parser.
  delete root;
}

void CheckId() {
  // Parse some KML with an attribute.
  const std::string kml(
      "<Placemark id=\"placemark123\"><name>x</name></Placemark>");
  Element* root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  const Placemark* placemark = kmldom::AsPlacemark(root);

  assert((bool)("placemark123" == placemark->id()));

  // Serialize the sample.
  std::string parsed_kml = kmldom::SerializePretty(*placemark);
  G_kmlprint.flux <<"parsed_kml: " <<parsed_kml<<std::endl;
//  std::cout << parsed_kml << std::endl;

  // Free all resources allocated by the parser.
  delete root;
}

int kmlhelloattrs(int argc, char* argv[]) {
  G_kmlprint.flux.open(argv[1]);
  CheckId();
  CheckHotSpot();
  G_kmlprint.flux.close();
  return EXIT_SUCCESS;  // All is well if we got this far.
}
