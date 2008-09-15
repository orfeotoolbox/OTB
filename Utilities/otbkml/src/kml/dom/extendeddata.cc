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

// This file contains the implementation of the ExtendedData, SimpleData,
// SchemaData and Data element.

#include "kml/dom/extendeddata.h"
#include "kml/dom/attributes.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

namespace kmldom {

// <SimpleData>
SimpleData::SimpleData()
  : has_name_(false),
    has_text_(false) {
}

SimpleData::~SimpleData() {}

static const char kSimpleDataName[] = "name";

void SimpleData::ParseAttributes(const Attributes& attributes) {
  has_name_ = attributes.GetString(kSimpleDataName, &name_);
  Element::ParseAttributes(attributes);
}

void SimpleData::GetAttributes(Attributes* attributes) const {
  Element::GetAttributes(attributes);
  if (has_name_) {
    attributes->SetString(kSimpleDataName, name_);
  }
}

// SimpleData needs to parse its own character data (like Snippet).
void SimpleData::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_SimpleData) {
    set_text(get_char_data());
  } else {
    // We have a known KML element inside <SimpleData> and need to store it.
    Element::AddElement(element);
  }
}

void SimpleData::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  if (has_text()) {
    serializer.SaveContent(text_, true);
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

// <SchemaData>
SchemaData::SchemaData()
  : has_schemaurl_(false) {
}

SchemaData::~SchemaData() {
  // simpledata_array_'s destructor calls the destructor of each SimpleDataPtr
  // releasing the reference and potentially freeing the SimpleData storage.
}

static const char kSchemaUrl[] = "schemaUrl";

void SchemaData::ParseAttributes(const Attributes& attributes) {
  has_schemaurl_ = attributes.GetString(kSchemaUrl, &schemaurl_);
  Object::ParseAttributes(attributes);
}

void SchemaData::GetAttributes(Attributes* attributes) const {
  if (has_schemaurl_) {
    attributes->SetString(kSchemaUrl, schemaurl_);
  }
}

void SchemaData::AddElement(const ElementPtr& element) {
  if (SimpleDataPtr simpledata = AsSimpleData(element)) {
    add_simpledata(simpledata);
  } else {
    Object::AddElement(element);
  }
}

void SchemaData::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  for (size_t i = 0; i < get_simpledata_array_size(); ++i) {
    serializer.SaveElement(get_simpledata_array_at(i));
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

// <Data>
Data::Data()
  : has_name_(false),
    has_displayname_(false),
    has_value_(false) {
}

Data::~Data() {}

static const char kDataName[] = "name";

void Data::ParseAttributes(const Attributes& attributes) {
  has_name_ = attributes.GetString(kDataName, &name_);
  Object::ParseAttributes(attributes);
}

void Data::GetAttributes(Attributes* attributes) const {
  if (has_name_) {
    attributes->SetString(kDataName, name_);
  }
}

void Data::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_displayName) {
    has_displayname_ = element->SetString(&displayname_);
  } else if (element->Type() == Type_value) {
    has_value_ = element->SetString(&value_);
  } else {
    Object::AddElement(element);
  }
}

void Data::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_displayname()) {
    serializer.SaveFieldById(Type_displayName, get_displayname());
  }
  if (has_value()) {
    serializer.SaveFieldById(Type_value, get_value());
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

// <ExtendedData>
ExtendedData::ExtendedData() {}

ExtendedData::~ExtendedData() {
  // data_array_'s and schemadata_array_'s destructors call the destructor of
  // each DataPtr and SchemaDataPtr, releasing the references and potentially
  // freeing the SchemaData and Data storage.
}

void ExtendedData::AddElement(const ElementPtr& element) {
  if (DataPtr data = AsData(element)) {
    add_data(data);
  } else if (SchemaDataPtr schemadata = AsSchemaData(element)) {
    add_schemadata(schemadata);
  } else {
    Element::AddElement(element);
  }
}

void ExtendedData::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  for (size_t i = 0; i < data_array_.size(); i++) {
    data_array_[i]->Serialize(serializer);
  }
  for (size_t i = 0; i < schemadata_array_.size(); i++) {
    schemadata_array_[i]->Serialize(serializer);
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
