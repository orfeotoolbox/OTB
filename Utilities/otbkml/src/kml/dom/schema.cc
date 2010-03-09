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

// This file contains the implementation of the SimpleField and Schema elements.

#include "kml/dom/schema.h"
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

// <SimpleField>
SimpleField::SimpleField()
  : has_type_(false),
    has_name_(false),
    has_displayname_(false) {
  set_xmlns(kmlbase::XMLNS_KML22);
}

SimpleField::~SimpleField() {}

static const char kSimpleFieldTypeAttr[] = "type";
static const char kSimpleFieldNameAttr[] = "name";

void SimpleField::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_type_ = attributes->CutValue(kSimpleFieldTypeAttr, &type_);
  has_name_ = attributes->CutValue(kSimpleFieldNameAttr, &name_);
  AddUnknownAttributes(attributes);
}

void SimpleField::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_type_) {
    attributes->SetValue(kSimpleFieldTypeAttr, type_);
  }
  if (has_name_) {
    attributes->SetValue(kSimpleFieldNameAttr, name_);
  }
}

void SimpleField::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_displayName) {
    has_displayname_ = element->SetString(&displayname_);
  } else {
    Element::AddElement(element);
  }
}

void SimpleField::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  if (has_displayname()) {
    serializer.SaveFieldById(Type_displayName, get_displayname());
  }
}

void SimpleField::Accept(Visitor* visitor) {
  visitor->VisitSimpleField(SimpleFieldPtr(this));
}

// <Schema>
Schema::Schema()
    : has_name_(false) {
}

Schema::~Schema() {}

static const char kSchemaNameAttr[] = "name";

void Schema::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_name_ = attributes->CutValue(kSchemaNameAttr, &name_);
  Object::ParseAttributes(attributes);
}

void Schema::SerializeAttributes(Attributes* attributes) const {
  Object::SerializeAttributes(attributes);
  if (has_name_) {
    attributes->SetValue(kSchemaNameAttr, name_);
  }
}

void Schema::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (SimpleFieldPtr simplefield = AsSimpleField(element)) {
    add_simplefield(simplefield);
  } else {
    Object::AddElement(element);
  }
}

void Schema::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  serializer.SaveElementArray(simplefield_array_);
}

void Schema::Accept(Visitor* visitor) {
  visitor->VisitSchema(SchemaPtr(this));
}

void Schema::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  Element::AcceptRepeated<SimpleFieldPtr>(&simplefield_array_, driver);
}

}  // end namespace kmldom
