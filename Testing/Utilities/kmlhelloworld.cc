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
REGISTER_TEST(kmlhelloworld);
}


// This program makes some basic use of the KML DOM API.

#include <assert.h>
#include <iostream>
#include "kml/dom.h"
#include "kml/util/kmlprint.h"

using std::cout;
using std::endl;
using kmldom::Coordinates;
using kmldom::KmlFactory;
using kmldom::Placemark;
using kmldom::Point;

void HelloKml(bool verbose) {
  KmlFactory* factory(KmlFactory::GetFactory());
  // <coordinates>
  Coordinates* coordinates(factory->CreateCoordinates());
  coordinates->add_point3(-122.456, 37.123, 314.159);
  // <Point><coordinates>...
  Point* point(factory->CreatePoint());
  point->set_coordinates(coordinates);
  // <Point><altitudeMode>...<coordinates>...
  point->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  assert(point->altitudemode() == kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  // <Placemark><Point><coordinates>...
  Placemark* placemark(factory->CreatePlacemark());
  placemark->set_geometry(point);

  // A Placemark is (duh) a Placemark
  assert (placemark->Type() == kmldom::Type_Placemark);
  // It's also a Feature.
  assert(placemark->IsA(kmldom::Type_Feature));
  placemark->set_name("point placemark");
  if (verbose) {
    G_kmlprint.flux << "Placemark's name is " << placemark->name() << endl;
  }
  // We know it has some geometry.
  assert(placemark->has_geometry());
  // And we can test to see if that geometry is a Point.
  assert(placemark->geometry()->IsA(kmldom::Type_Point));
  // If it is, we can make a point from it. (Yes, API should hide casting.)
  const Point* pt = kmldom::AsPoint(placemark->geometry());
  assert(pt->altitudemode() == kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  if (verbose) {
    G_kmlprint.flux.precision(6);
    G_kmlprint.flux << placemark->name() << " is located at: ";
    G_kmlprint.flux << pt->coordinates()->coordinates_array_at(0).latitude() << ", ";
    G_kmlprint.flux << pt->coordinates()->coordinates_array_at(0).longitude() << endl;
  }

  // Free everything.
  delete placemark;
}

int kmlhelloworld(int argc, char* argv []) {
G_kmlprint.flux.open(argv[1]);
  HelloKml(true/*argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v'*/);
G_kmlprint.flux.close();
  return EXIT_SUCCESS;
}
