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

// This program makes some basic use of the KML Engine CreateResolvedStyle()
// function.  For example:
//   printstyle ../../../examples/kml/polygons2d.kml pm300

#include <iostream>
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"

// This prints out the normal <Style> for the feature of the given id in the
// given KML file.
void HelloCreateResolvedStyle(const std::string& kml_filename,
                              const std::string& id) {
  // Read the KML file.
  std::string kml_content;
  if (!kmlbase::File::ReadFileToString(kml_filename, &kml_content)) {
    std::cerr << "read error: " << kml_filename << std::endl;
    return;
  }

  // Parse using KmlFile.
  std::string errors;
  kmlengine::KmlFilePtr kml_file =
      kmlengine::KmlFile::CreateFromParse(kml_content, &errors);
  if (!kml_file) {
    std::cerr << errors;
    return;
  }

  // Look up the Feature
  kmldom::FeaturePtr feature = kmldom::AsFeature(kml_file->GetObjectById(id));
  if (!feature) {
    std::cerr << "no feature with id: " << id << std::endl;
    return;
  }

  // Create a resolved style for the Feature and print it.
  kmldom::StylePtr style = CreateResolvedStyle(feature, kml_file,
                                               kmldom::STYLESTATE_NORMAL);
  std::cout << kmldom::SerializePretty(style);
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << "kmlfile feature-id" << std::endl;
    return -1;
  }

  HelloCreateResolvedStyle(argv[1], argv[2]);
  return 0;
}
