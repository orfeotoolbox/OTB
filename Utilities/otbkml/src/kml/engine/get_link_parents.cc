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

#include "kml/engine/get_link_parents.h"
#include "kml/dom/parser.h"
#include "kml/engine/engine_types.h"

using kmldom::ElementPtr;
using kmldom::Parser;

namespace kmlengine {

bool IsIconParent(const ElementPtr& element) {
  switch (element->Type()) {
    default:
      return false;
    case kmldom::Type_PhotoOverlay:
    case kmldom::Type_GroundOverlay:
    case kmldom::Type_ScreenOverlay:
    case kmldom::Type_ItemIcon:
    case kmldom::Type_IconStyle:
      return true;
  }
}

bool IsLinkParent(const ElementPtr& element) {
  switch (element->Type()) {
    default:
      return false;
    case kmldom::Type_NetworkLink:
    case kmldom::Type_Model:
     return true;
  }
}

bool GetLinkParents(const string& kml,
                    ElementVector* link_parent_vector) {
  if (!link_parent_vector) {
    return false;
  }
  GetLinkParentsParserObserver get_link_parents(link_parent_vector);
  Parser parser;
  parser.AddObserver(&get_link_parents);
  return parser.Parse(kml, NULL) != NULL;
}

}  // end namespace kmlengine
