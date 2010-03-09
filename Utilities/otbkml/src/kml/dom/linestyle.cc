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

#include "kml/dom/linestyle.h"
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

LineStyle::LineStyle()
  : width_(1.0), has_width_(false) {
}

LineStyle::~LineStyle() {
}

void LineStyle::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_width:
      has_width_ = element->SetDouble(&width_);
      break;
    default:
      ColorStyle::AddElement(element);
      break;
  }
}

void LineStyle::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  ColorStyle::Serialize(serializer);
  if (has_width()) {
    serializer.SaveFieldById(Type_width, get_width());
  }
}

void LineStyle::Accept(Visitor* visitor) {
  visitor->VisitLineStyle(LineStylePtr(this));
}

}  // end namespace kmldom
