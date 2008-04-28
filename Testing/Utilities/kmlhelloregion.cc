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
REGISTER_TEST(kmlhelloregion);
}


// This program generates GroundOverlays with Regions.  The two overlays have
// exclusive Lods on their Regions, hence they swap as the viewpoint changes.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/kmlprint.h"

using std::cout;
using std::string;
using kmldom::Folder;
using kmldom::GroundOverlay;
using kmldom::Kml;
using kmldom::KmlFactory;
using kmldom::LatLonAltBox;
using kmldom::LatLonBox;
using kmldom::Lod;
using kmldom::Region;

// This creates a Region at the given bounding box with the given Lod range.
static Region* CreateRegion(double north, double south,
                            double east, double west,
                            double minlodpixels, double maxlodpixels) {
  KmlFactory* factory = KmlFactory::GetFactory();
  Region* region = factory->CreateRegion();
  LatLonAltBox* latlonaltbox = factory->CreateLatLonAltBox();
  latlonaltbox->set_north(north);
  latlonaltbox->set_south(south);
  latlonaltbox->set_east(east);
  latlonaltbox->set_west(west);
  Lod* lod = factory->CreateLod();
  lod->set_minlodpixels(minlodpixels);
  lod->set_maxlodpixels(maxlodpixels);
  region->set_latlonaltbox(latlonaltbox);
  region->set_lod(lod);
  return region;
}

// This creates a GroundOverlay at the given bounding box.
// Since there is no Icon (image) a polygon of the given color is drawn.
static GroundOverlay* CreateGroundOverlay(double north, double south,
                                          double east, double west,
                                          string name, string color) {
  KmlFactory* factory = KmlFactory::GetFactory();
  GroundOverlay* groundoverlay = factory->CreateGroundOverlay();
  groundoverlay->set_name(name);
  LatLonBox* latlonbox = factory->CreateLatLonBox();
  latlonbox->set_north(north);
  latlonbox->set_south(south);
  latlonbox->set_east(east);
  latlonbox->set_west(west);
  groundoverlay->set_latlonbox(latlonbox);
  groundoverlay->set_color(color);
  return groundoverlay;
}

// This uses Regions to make a clean swap between overlays.
// Set minLodPixels of one Region to the same as maxLodPixels of the other.
void SwapOverlays() {
  KmlFactory* factory = KmlFactory::GetFactory();
  double north = 10;
  double south = -10;
  double east = 10;
  double west = -10;
  double lod_a = 128;  // 128 x 128 pixels
  double lod_b = 512;
  double lod_c = 1024;
  string solid_red("ff0000ff");  // aabbggrr
  string solid_blue("ffff0000");

  // Create a solid red GroundOverlay.
  GroundOverlay* red =
    CreateGroundOverlay(north, south, east, west, "Red", solid_red);
  // Give it a Region with lod range a - b
  red->set_region(CreateRegion(north, south, east, west, lod_a, lod_b));
  // Create a solid blue GroundOverlay.
  GroundOverlay* blue =
    CreateGroundOverlay(north, south, east, west, "Blue", solid_blue);
  // Give it a Region with lod range b - c
  blue->set_region(CreateRegion(north, south, east, west, lod_b, lod_c));

  Folder* folder = factory->CreateFolder();
  folder->set_name("Swap Overlays");
  folder->add_feature(red);
  folder->add_feature(blue);

  Kml* kml = factory->CreateKml();
  kml->set_feature(folder);

  G_kmlprint.flux << kmldom::SerializePretty(*kml);

  delete kml;  // Deletes kml and all children we attached above.
}


int kmlhelloregion(int argc, char* argv[]) {
G_kmlprint.flux.open(argv[1]);
  SwapOverlays();
G_kmlprint.flux.close();
  return EXIT_SUCCESS;
}
