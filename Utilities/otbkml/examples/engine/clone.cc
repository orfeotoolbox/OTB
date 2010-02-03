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

// This program makes some basic use of the KML Engine Clone() function.
// Since element is shared in XML the Clone() function provides away to use the
// same element data with multiple parents.  Note that Clone() is a "deep"
// clone: all simple and complex elements are cloned.

#include <iostream>
#include "kml/dom.h"
#include "kml/engine.h"

// This function creates a Region with a LatLonAltBox of the specified bounds
// and a Lod with the specified minLodPixels.
kmldom::RegionPtr CreateRegionLatLonAltBoxLod(double north, double south,
                                              double east, double west,
                                              double minlodpixels) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::LatLonAltBoxPtr latlonaltbox = factory->CreateLatLonAltBox();
  latlonaltbox->set_north(north);
  latlonaltbox->set_south(south);
  latlonaltbox->set_east(east);
  latlonaltbox->set_west(west);
  kmldom::LodPtr lod = factory->CreateLod();
  lod->set_minlodpixels(minlodpixels);
  kmldom::RegionPtr region = factory->CreateRegion();
  region->set_latlonaltbox(latlonaltbox);
  region->set_lod(lod);
  return region;
}

void HelloClone() {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();

  // Create a Container and some Features.
  kmldom::FolderPtr folder = factory->CreateFolder();
  kmldom::PlacemarkPtr pm0 = factory->CreatePlacemark();
  kmldom::PlacemarkPtr pm1 = factory->CreatePlacemark();

  // Create a Region with LatLonAltBox and Lod.
  kmldom::RegionPtr region = CreateRegionLatLonAltBoxLod(1, 2, 3, 4, 100);

  // Create a clone of this Region for each Placemark.
  pm0->set_region(kmldom::AsRegion(kmlengine::Clone(region)));
  pm1->set_region(kmldom::AsRegion(kmlengine::Clone(region)));
  folder->add_feature(pm0);
  folder->add_feature(pm1);

  // Print the resulting XML.
  std::cout << kmldom::SerializePretty(folder);
}

int main(int argc, char** argv) {
  HelloClone();
  return 0;
}
