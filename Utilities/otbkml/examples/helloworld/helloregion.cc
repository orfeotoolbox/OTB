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

// This program generates GroundOverlays with Regions.  The two overlays have
// exclusive Lods on their Regions, hence they swap as the viewpoint changes.

#include <iostream>
#include <string>
#include "kml/dom.h"

using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::LatLonAltBoxPtr;
using kmldom::LatLonBoxPtr;
using kmldom::LodPtr;
using kmldom::RegionPtr;

// Declare functions defined in this file.
static GroundOverlayPtr CreateGroundOverlay(double north, double south,
                                            double east, double west,
                                            const std::string& name,
                                            const std::string& color);
static RegionPtr CreateRegion(double north, double south,
                              double east, double west,
                              double minlodpixels, double maxlodpixels);
void SwapOverlays();

// This creates a Region at the given bounding box with the given Lod range.
static RegionPtr CreateRegion(double north, double south,
                              double east, double west,
                              double minlodpixels, double maxlodpixels) {
  KmlFactory* factory = KmlFactory::GetFactory();
  RegionPtr region = factory->CreateRegion();
  LatLonAltBoxPtr latlonaltbox = factory->CreateLatLonAltBox();
  latlonaltbox->set_north(north);
  latlonaltbox->set_south(south);
  latlonaltbox->set_east(east);
  latlonaltbox->set_west(west);
  LodPtr lod = factory->CreateLod();
  lod->set_minlodpixels(minlodpixels);
  lod->set_maxlodpixels(maxlodpixels);
  region->set_latlonaltbox(latlonaltbox);
  region->set_lod(lod);
  return region;
}

// This creates a GroundOverlay at the given bounding box.
// Since there is no Icon (image) a polygon of the given color is drawn.
static GroundOverlayPtr CreateGroundOverlay(double north, double south,
                                            double east, double west,
                                            const std::string& name,
                                            const std::string& color) {
  KmlFactory* factory = KmlFactory::GetFactory();
  GroundOverlayPtr groundoverlay = factory->CreateGroundOverlay();
  groundoverlay->set_name(name);
  LatLonBoxPtr latlonbox = factory->CreateLatLonBox();
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
  std::string solid_red("ff0000ff");  // aabbggrr
  std::string solid_blue("ffff0000");

  // Create a solid red GroundOverlay.
  GroundOverlayPtr red =
    CreateGroundOverlay(north, south, east, west, "Red", solid_red);
  // Give it a Region with lod range a - b
  red->set_region(CreateRegion(north, south, east, west, lod_a, lod_b));
  // Create a solid blue GroundOverlay.
  GroundOverlayPtr blue =
    CreateGroundOverlay(north, south, east, west, "Blue", solid_blue);
  // Give it a Region with lod range b - c
  blue->set_region(CreateRegion(north, south, east, west, lod_b, lod_c));

  FolderPtr folder = factory->CreateFolder();
  folder->set_name("Swap Overlays");
  folder->add_feature(red);
  folder->add_feature(blue);

  KmlPtr kml = factory->CreateKml();
  kml->set_feature(folder);

  std::cout << kmldom::SerializePretty(kml);
}


int main(int argc, char** argv) {
  SwapOverlays();
}
