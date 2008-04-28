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
REGISTER_TEST(kmlhellofolder);
}


#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using std::cout;
using std::endl;
using std::string;
using kmldom::Coordinates;
using kmldom::Feature;
using kmldom::Folder;
using kmldom::KmlFactory;
using kmldom::Placemark;
using kmldom::Point;

#include "kml/util/kmlprint.h"


Placemark* CreatePlacemark(kmldom::KmlFactory* factory,
                           const string& name,
                           double lat, double lon) {
  Placemark* placemark(factory->CreatePlacemark());
  placemark->set_name(name);

  Coordinates* coordinates(factory->CreateCoordinates());
  coordinates->add_point2(lon, lat);

  Point* point(factory->CreatePoint());
  point->set_coordinates(coordinates);

  placemark->set_geometry(point);

  return placemark;
}

static const unsigned int kHowManyPoints = 1001;

int kmlhellofolder(int argc, char* argv[]) {
  KmlFactory* factory(KmlFactory::GetFactory());

G_kmlprint.flux.open(argv[1]);

  // Create a Folder, fill it with Placemarks.
  G_kmlprint.flux << "Creating " << kHowManyPoints << " Placemarks:" << endl;
  Folder* folder = factory->CreateFolder();
  for (int i = 0 ; i < kHowManyPoints ; ++i) {
    folder->add_feature(CreatePlacemark(factory, "hi", 1.1, 2.2));
  }

  // Read back all the Placemarks in the Folder.
  G_kmlprint.flux << "Iterating " << kHowManyPoints << " Placemarks:" << endl;
  assert(kHowManyPoints == folder->feature_array_size());
  for (int i = 0; i < folder->feature_array_size(); ++i) {
    assert((bool)("hi" == folder->feature_array_at(i)->name()));
    assert(kmldom::Type_Placemark == folder->feature_array_at(i)->Type());
  }

  // Deletes everything within.
  G_kmlprint.flux << "Deleting Folder" << endl;
  delete folder;
G_kmlprint.flux.close();

  return EXIT_SUCCESS;
}
