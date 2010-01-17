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

// This example program that takes as its only argument a KML or KMZ file,
// then walks through all contained Features, computing the markup required
// to display the description balloon for each feature.

#include <iostream>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"

using kmldom::BalloonStylePtr;
using kmldom::DocumentPtr;
using kmldom::ElementPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::SerializePretty;
using kmldom::SnippetPtr;
using kmldom::StylePtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;
using std::cout;
using std::cerr;
using std::endl;

// The FeatureVisitor class implements the mechanism to walk a Feature
// hierarchy. This subclass calls CreateBalloonText on each Feature and prints
// the composited text to stdout.
class FeatureBalloonPrinter : public kmlengine::FeatureVisitor {
 public:
  FeatureBalloonPrinter(const KmlFilePtr& kml_file) :
    kml_file_(kml_file) {}
  // The callback from VisitFeatureHierarchy.
  virtual void VisitFeature(const kmldom::FeaturePtr& f) {
    std::string fname = f->has_name() ? f->get_name() : "Unnamed feature";
    cout << "Feature balloon text for " << fname << endl;
    cout << kmlengine::CreateBalloonText(kml_file_, f) << endl << endl;
  }
 private:
  const KmlFilePtr kml_file_;
};

void VisitFeatureBalloons(const KmlFilePtr& kml_file) {
  FeatureBalloonPrinter feature_balloon_printer(kml_file);
  kmlengine::VisitFeatureHierarchy(kmlengine::GetRootFeature(kml_file->root()),
                                   feature_balloon_printer);
}

int HandleFile(const char* filename) {
  std::string file_data;
  if (!kmlbase::File::ReadFileToString(filename, &file_data)) {
    cerr << "error: read of " << filename << " failed" << endl;
    return 1;
  }
  std::string errors;
  KmlFilePtr kml_file = KmlFile::CreateFromParse(file_data, &errors);
  if (!kml_file || !errors.empty()) {
    cerr << "parse failed: " << errors << endl;;
    return 1;
  }

  VisitFeatureBalloons(kml_file);
  return 0;
}


int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " kmlfile" << endl;
    return -1;
  }

  return HandleFile(argv[1]);
}
