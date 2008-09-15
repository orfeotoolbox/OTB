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

// This program makes some basic use of the KML Engine KmlFile class.
// KmlFile's GetObjectById() provides an efficient and convenient means to fine
// an element by its XML ID.

#include <iostream>
#include "kml/dom.h"
#include "kml/engine.h"

void HelloKmlFile() {
  const std::string kKml =
    "<kml xmlns=\"http://www.opengis.net/kml/2.2\">"
      "<Folder id=\"f0\">"
        "<name>Folder 0</name>"
        "<Placemark id=\"pm0\">"
          "<name>Placemark 0</name>"
        "</Placemark>"
        "<Placemark id=\"pm1\">"
          "<name>Placemark 1</name>"
        "</Placemark>"
      "</Folder>"
    "</kml>";

  // Parsing into a KmlFile creates an internal database of object id mappings.
  kmlengine::KmlFilePtr kml_file = kmlengine::KmlFile::CreateFromParse(kKml,
                                                                       NULL);

  // Access the 3 Features by their XML Id.  Note that GetObjectById() returns
  // a ObjectPtr, hence the use of the cast to access <name>.
  kmldom::FolderPtr folder0 = AsFolder(kml_file->GetObjectById("f0"));
  std::cout << "Folder 0 name: " << folder0->get_name() << std::endl;
  kmldom::PlacemarkPtr placemark0 = AsPlacemark(kml_file->GetObjectById("pm0"));
  std::cout << "Placemark 0 name: " << placemark0->get_name() << std::endl;
  kmldom::PlacemarkPtr placemark1 = AsPlacemark(kml_file->GetObjectById("pm1"));
  std::cout << "Placemark 1 name: " << placemark1->get_name() << std::endl;
}

int main(int argc, char** argv) {
  HelloKmlFile();
  return 0;
}
