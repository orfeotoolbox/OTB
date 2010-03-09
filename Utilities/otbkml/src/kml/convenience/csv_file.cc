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

// This file contains the implementation of the Csv class for parsing CSV
// files and generating Point Placemark KML.

#include "kml/convenience/csv_file.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "kml/base/string_util.h"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/feature_list.h"

namespace kmlconvenience {

using kmldom::FeaturePtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmlconvenience::FeatureList;
using std::vector;

void CsvFile::ParseCsvLine(const string& csv_line) {
  vector<string> csv_parts;
  kmlbase::SplitStringUsing(csv_line, "|", &csv_parts);
  if (csv_parts.size() < 5) {
    return;
  }
  PlacemarkPtr placemark = kmlconvenience::CreatePointPlacemark(
      csv_parts[3], strtod(csv_parts[1].c_str(), NULL),
      strtod(csv_parts[2].c_str(), NULL));
  placemark->set_description(csv_parts[4]);
  if (csv_parts.size() > 5) {
    placemark->set_styleurl(csv_parts[5]);
  }
  kmlconvenience::SetFeatureScore(csv_parts[0], placemark);
  feature_list_->PushBack(placemark);
}
 
// TODO: create a File line getter in kml/base/file.cc
void CsvFile::ParseCsvFile(const char* filename) {
  std::ifstream csv_file;
  csv_file.open(filename);
  while (csv_file.good()) {
    string csv_line;
    getline(csv_file, csv_line);
    if (csv_line.empty()) {
      return;
    }
    ParseCsvLine(csv_line);
  }
}

}  // end namespace kmlconvenience
