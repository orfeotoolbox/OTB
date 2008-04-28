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
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"

namespace kmldom {

// <SimpleField>
SimpleField::SimpleField()
  : has_type_(false),
    has_name_(false),
    has_displayname_(false) {
}

SimpleField::~SimpleField() {}

static const char kSimpleFieldTypeAttr[] = "type";
static const char kSimpleFieldNameAttr[] = "name";

void SimpleField::ParseAttributes(const Attributes& attributes) {
  has_type_ = attributes.GetString(kSimpleFieldTypeAttr, &type_);
  has_name_ = attributes.GetString(kSimpleFieldNameAttr, &name_);
  Element::ParseAttributes(attributes);
}

void SimpleField::GetAttributes(Attributes* attributes) const {
  if (has_type_) {
    attributes->SetString(kSimpleFieldTypeAttr, type_);
  }
  if (has_name_) {
    attributes->SetString(kSimpleFieldNameAttr, name_);
  }
}

void SimpleField::AddElement(Element* element) {
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
  Attributes attributes;
  Element::GetAttributes(&attributes);
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  if (has_displayname()) {
    serializer.SaveFieldById(Type_displayName, displayname());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

// <Schema>
Schema::Schema()
  : has_name_(false),
    has_id_(false) {
}

Schema::~Schema() {
  for (size_t i = 0; i < simplefield_array_.size(); ++i) {
    delete simplefield_array_[i];
  }
}

static const char kSchemaNameAttr[] = "name";
static const char kSchemaIdAttr[] = "id";

void Schema::ParseAttributes(const Attributes& attributes) {
  has_name_ = attributes.GetString(kSchemaNameAttr, &name_);
  has_id_ = attributes.GetString(kSchemaIdAttr, &id_);
}

void Schema::GetAttributes(Attributes* attributes) const {
  if (has_name_) {
    attributes->SetString(kSchemaNameAttr, name_);
  }
  if (has_id_) {
    attributes->SetString(kSchemaIdAttr, id_);
  }
}

void Schema::AddElement(Element* element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_SimpleField) {
    add_simplefield(static_cast<SimpleField*>(element));
  } else {
    Element::AddElement(element);
  }
}

void Schema::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  for (size_t i = 0; i < simplefield_array_.size(); ++i) {
    serializer.SaveElement(*simplefield_array_at(i));
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
