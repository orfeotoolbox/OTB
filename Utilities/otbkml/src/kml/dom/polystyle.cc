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

#include "kml/dom/polystyle.h"
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

PolyStyle::PolyStyle()
  : fill_(true), has_fill_(false), outline_(true), has_outline_(false) {
}

PolyStyle::~PolyStyle() {
}

void PolyStyle::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_fill:
      has_fill_ = element->SetBool(&fill_);
      break;
    case Type_outline:
      has_outline_ = element->SetBool(&outline_);
      break;
    default:
      ColorStyle::AddElement(element);
      break;
  }
}

void PolyStyle::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  ColorStyle::Serialize(serializer);
  if (has_fill()) {
    serializer.SaveFieldById(Type_fill, get_fill());
  }
  if (has_outline()) {
    serializer.SaveFieldById(Type_outline, get_outline());
  }
}

void PolyStyle::Accept(Visitor* visitor) {
  visitor->VisitPolyStyle(PolyStylePtr(this));
}

}  // end namespace kmldom
