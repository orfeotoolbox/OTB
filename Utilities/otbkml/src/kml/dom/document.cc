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

#include "kml/dom/document.h"
#include "kml/base/attributes.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

Document::Document() {}

Document::~Document() {}

void Document::AddElement(const ElementPtr& element) {
  if (SchemaPtr schema = AsSchema(element)) {
    add_schema(schema);
  } else if (StyleSelectorPtr styleselector = AsStyleSelector(element)) {
    add_styleselector(styleselector);
  } else {
    Container::AddElement(element);
  }
}

// Due to Document being the only Feature with a StyleSelectorArray we have to
// take some matters into our own hands here and reach up into Feature and
// Container to serialize in XSD order.
void Document::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Feature::SerializeBeforeStyleSelector(serializer);
  serializer.SaveElementGroupArray(styleselector_array_, Type_StyleSelector);
  Feature::SerializeAfterStyleSelector(serializer);
  serializer.SaveElementArray(schema_array_);
  Container::SerializeFeatureArray(serializer);
}

void Document::Accept(Visitor* visitor) {
  visitor->VisitDocument(DocumentPtr(this));
}

void Document::AcceptChildren(VisitorDriver* driver) {
  Container::AcceptChildren(driver);
  Element::AcceptRepeated<SchemaPtr>(&schema_array_, driver);
  Element::AcceptRepeated<StyleSelectorPtr>(&styleselector_array_, driver);
}

}  // end namespace kmldom
