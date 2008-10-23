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

#include "print.h"
#include <iostream>
#include <string>
#include "kml/dom.h"

using std::cout;
using std::endl;
using std::string;

using kmldom::ContainerPtr;
using kmldom::FeaturePtr;

void PrintIndented(string item, int depth) {
  while (depth--) {
    cout << "  ";
  }
  cout << item;
}

void PrintFeature(const FeaturePtr& feature, int depth) {
  switch (feature->Type()) {
    case kmldom::Type_Document:
      PrintIndented("Document", depth);
      break;
    case kmldom::Type_Folder:
      PrintIndented("Folder", depth);
      break;
    case kmldom::Type_GroundOverlay:
      PrintIndented("GroundOverlay", depth);
      break;
    case kmldom::Type_NetworkLink:
      PrintIndented("NetworkLink", depth);
      break;
    case kmldom::Type_PhotoOverlay:
      PrintIndented("PhotoOverlay", depth);
      break;
    case kmldom::Type_Placemark:
      PrintIndented("Placemark", depth);
      break;
    case kmldom::Type_ScreenOverlay:
      PrintIndented("ScreenOverlay", depth);
      break;
    default:
      PrintIndented("other", depth);
      break;
  }

  if (feature->has_name()) {
    cout << " " << feature->get_name();
  }
  cout << endl;

  if (const ContainerPtr container = kmldom::AsContainer(feature)) {
    PrintContainer(container, depth+1);
  }
}

void PrintContainer(const ContainerPtr& container, int depth) {
  for (size_t i = 0; i < container->get_feature_array_size(); ++i) {
    PrintFeature(container->get_feature_array_at(i), depth);
  }
}
