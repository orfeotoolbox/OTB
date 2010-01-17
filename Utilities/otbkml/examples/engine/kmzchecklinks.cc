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

// This program checks the KMZ archive for all files the KML references.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"

using boost::scoped_ptr;
using kmlengine::Href;
using kmlengine::KmzFile;

int PrintAndCheckLinks(const char* kmz_filename) {
  // Open the KMZ file.
  scoped_ptr<KmzFile> kmz_file(KmzFile::OpenFromFile(kmz_filename));
  if (!kmz_file.get()) {
    return -1;
  }

  // Dig out the KML.
  std::string kml;
  if (!kmz_file->ReadKml(&kml)) {
    return -1;
  }

  // Get the links.
  // TODO engine support for links in <description> and <BalloonStyle><text>
  kmlengine::href_vector_t href_vector;
  if (!kmlengine::GetLinks(kml, &href_vector)) {
    return -1;
  }

  // TODO Model/Alias/targetHref is relative to Model/Link/href.
  // TODO if Model/Link/href="dir/model.kml" then textures/foo.jpg is
  // TODO access with Model/ResourceMap/Alias/targetHref="../textures/foo.jpg"

  // Try to access each relative link within the KMZ.
  int ret = 0;
  for (size_t i = 0; i < href_vector.size(); ++i) {
    std::cout << href_vector[i] << " ... ";
    Href href(href_vector[i]);
    if (href.IsRelativePath()) {
      std::string content;
      if (!kmz_file->ReadFile(href.get_path().c_str(), &content)) {
        std::cout << "NO";
        ret = -1;
      } else {
        std::cout << "yes";
      }
    } else if (href.IsFragmentOnly()) {
      std::cout << "(fragment only)";
    } else {
      std::cout << "(absolute)";
    }
    std::cout << std::endl;
  }
  return ret;
}

int main(int argc, char** argv) {
 if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " file.kmz" << std::endl;
    return -1;
  }
  return PrintAndCheckLinks(argv[1]);
}
