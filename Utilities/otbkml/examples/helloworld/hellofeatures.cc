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

// Walks the container hierarchy of the KML file printing about each Feature.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/base/file.h"
#include "print.h"

using std::cout;
using std::endl;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlPtr;

static const FeaturePtr GetRootFeature(const ElementPtr& root) {
  const KmlPtr kml = kmldom::AsKml(root);
  if (kml && kml->has_feature()) {
    return kml->get_feature();
  }
  return kmldom::AsFeature(root);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " kmlfile" << endl;
    return 1;
  }

  // Read it.
  std::string kml;
  if (!kmlbase::File::ReadFileToString(argv[1], &kml)) {
    cout << argv[1] << " read failed" << endl;
    return 1;
  }

  // Parse it.
  std::string errors;
  ElementPtr root = kmldom::Parse(kml, &errors);
  if (!root) {
    cout << errors;
    return 1;
  }

  // Print it.
  const FeaturePtr feature = GetRootFeature(root);
  if (feature) {
    PrintFeature(feature, 0);
  } else {
    cout << "No root feature" << endl;
  }
}
