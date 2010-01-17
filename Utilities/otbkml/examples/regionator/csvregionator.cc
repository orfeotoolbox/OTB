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

// Build a Region-based NetworkLink hierarchy from a CSV file.
// This is a C++ version of this:
//   http://regionator.googlecode.com/svn/trunk/kml/csvregionator.py
// Each line of the input.csv is in this format:
//   score|lat|lon|name|description[|style_url]
// A data set for use with this program is here:
//   http://regionator.googlecode.com/svn/trunk/ch-sbb/ch-stations.csv

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/csv_file.h"
#include "kml/engine.h"
#include "kml/regionator/feature_list_region_handler.h"
#include "kml/regionator/regionator.h"

using kmldom::RegionPtr;
using kmlconvenience::CsvFile;
using kmlconvenience::FeatureList;
using kmlengine::Bbox;
using kmlregionator::FeatureListRegionHandler;
using kmlregionator::RegionHandler;
using kmlregionator::Regionator;

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " input.csv output_directory" <<
      std::endl;
    return 1;
  }
  const char* csv_filename = argv[1];
  const char* output_dir = argv[2];

  // Parse the CSV file into a FeatureList of Point Placemarks sorted by score.
  FeatureList feature_list;
  CsvFile placemarks(&feature_list);
  placemarks.ParseCsvFile(csv_filename);
  feature_list.Sort();

  // Give the FeatureList to the FeatureListRegionHandler.
  FeatureListRegionHandler feature_list_region_handler(&feature_list);

  // Create a root Region based on the bounding box of the FeatureList.
  Bbox bbox;
  feature_list.ComputeBoundingBox(&bbox);
  // TODO snap
  RegionPtr root = kmlconvenience::CreateRegion2d(bbox.get_north(),
                                                  bbox.get_south(),
                                                  bbox.get_east(),
                                                  bbox.get_west(),
                                                  256, -1);

  // Create a Regionator instance and walk the hierarchy starting at root.
  Regionator regionator(feature_list_region_handler, root);
  regionator.Regionate(output_dir);
}
