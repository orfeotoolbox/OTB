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

#include "kml/dom/placemark.h"
#include "kml/base/attributes.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/element.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

Placemark::Placemark() {}

Placemark::~Placemark() {}

void Placemark::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_Geometry)) {
    set_geometry(AsGeometry(element));
    return;
  }
  Feature::AddElement(element);
}

void Placemark::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Feature::Serialize(serializer);
  if (has_geometry()) {
    serializer.SaveElementGroup(get_geometry(), Type_Geometry);
  }
}

void Placemark::Accept(Visitor* visitor) {
  visitor->VisitPlacemark(PlacemarkPtr(this));
}

void Placemark::AcceptChildren(VisitorDriver* driver) {
  Feature::AcceptChildren(driver);
  if (has_geometry()) {
    driver->Visit(get_geometry());
  }
}

}  // namespace kmldom
