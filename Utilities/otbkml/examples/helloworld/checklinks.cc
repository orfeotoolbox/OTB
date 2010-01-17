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

// This program uses ParserObserver::AddChild() to discover all URLs
// in a KML file.  A potentially fetchable URL is found in all <href>'s,
// Model/Alias/targetHref, Feature/styleUrl, and SchemaData/schemaUrl=.
// TODO: balloons have HTML <img src=".."> and <a href="..."> links.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"

using std::cout;
using std::endl;
using kmldom::ElementPtr;
using kmlengine::KmzFile;

class PrintLinks : public kmldom::ParserObserver {
 public:
  virtual bool AddChild(const ElementPtr& parent, const ElementPtr& child) {
    switch (child->Type()) {
      case kmldom::Type_href:
        // NetworkLink/Link/href, Overlay/Icon/href, ItemIcon/href
        // Model/Link/href, IconStyle/Icon/href
        cout << "href=" << child->get_char_data() << endl;
        break;
      case kmldom::Type_targetHref:
        if (kmldom::Type_Alias == parent->Type()) {
          cout << "Alias/targetHref=" << child->get_char_data() << endl;
        }
        break;
      case kmldom::Type_styleUrl:
        cout << "styleUrl=" << child->get_char_data() << endl;
        break;
      case kmldom::Type_SchemaData:
        {  // Avoid 'jump to case label croses initialization' compiler warning.
          kmldom::SchemaDataPtr schemadata = kmldom::AsSchemaData(child);
          if (schemadata->has_schemaurl()) {
            cout << "schemaUrl=" << schemadata->get_schemaurl() << endl;
          }
        }
        break;
      // TODO: HTML links in description and BalloonStyle/text
      default: break;
    }
    return true;
  }
};

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " input.kml" << endl;
    return 1;
  }
  const char* kmlfile = argv[1];

  // Read the file.
  std::string file_data;
  if (!kmlbase::File::ReadFileToString(kmlfile, &file_data)) {
    cout << kmlfile << " read failed" << endl;
    return 1;
  }

  // If the file was KMZ, extract the KML file.
  std::string kml;
  if (KmzFile::IsKmz(file_data)) {
    boost::scoped_ptr<KmzFile> kmz_file(KmzFile::OpenFromString(kmlfile));
    if (!kmz_file.get()) {
      cout << "Failed opening KMZ file" << endl;
      return 1;
    }
    if (!kmz_file->ReadKml(&kml)) {
      cout << "Failed to read KML from KMZ" << endl;
      return 1;
    }
  } else {
    kml = file_data;
  }

  // Parse it.
  kmldom::Parser parser;
  PrintLinks print_links;
  parser.AddObserver(&print_links);
  std::string errors;
  ElementPtr root = parser.Parse(kml, &errors);
  if (!root) {
    cout << errors << endl;
    return 1;
  }
  return 0;
}
