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
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

// <SimpleData>
SimpleData::SimpleData()
  : has_name_(false),
    has_text_(false) {
  set_xmlns(kmlbase::XMLNS_KML22);
}

SimpleData::~SimpleData() {}

static const char kSimpleDataName[] = "name";

void SimpleData::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_name_ = attributes->CutValue(kSimpleDataName, &name_);
  AddUnknownAttributes(attributes);
}

void SimpleData::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_name_) {
    attributes->SetValue(kSimpleDataName, name_);
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
  ElementSerializer element_serializer(*this, serializer);
  if (has_text()) {
    serializer.SaveContent(text_, true);
  }
}

void SimpleData::Accept(Visitor* visitor) {
  visitor->VisitSimpleData(SimpleDataPtr(this));
}

// <SchemaData>
SchemaData::SchemaData()
  : has_schemaurl_(false) {
  set_xmlns(kmlbase::XMLNS_KML22);
}

SchemaData::~SchemaData() {
  // simpledata_array_'s destructor calls the destructor of each SimpleDataPtr
  // releasing the reference and potentially freeing the SimpleData storage.
}

static const char kSchemaUrl[] = "schemaUrl";

void SchemaData::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_schemaurl_ = attributes->CutValue(kSchemaUrl, &schemaurl_);
  Object::ParseAttributes(attributes);
}

void SchemaData::SerializeAttributes(Attributes* attributes) const {
  Object::SerializeAttributes(attributes);
  if (has_schemaurl_) {
    attributes->SetValue(kSchemaUrl, schemaurl_);
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  serializer.SaveElementArray(simpledata_array_);
}

void SchemaData::Accept(Visitor* visitor) {
  visitor->VisitSchemaData(SchemaDataPtr(this));
}

void SchemaData::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  Element::AcceptRepeated<SimpleDataPtr>(&simpledata_array_, driver);
}

// <Data>
Data::Data()
  : has_name_(false),
    has_displayname_(false),
    has_value_(false) {
  set_xmlns(kmlbase::XMLNS_KML22);
}

Data::~Data() {}

static const char kDataName[] = "name";

void Data::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_name_ = attributes->CutValue(kDataName, &name_);
  Object::ParseAttributes(attributes);
}

void Data::SerializeAttributes(Attributes* attributes) const {
  Object::SerializeAttributes(attributes);
  if (has_name_) {
    attributes->SetValue(kDataName, name_);
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_displayname()) {
    serializer.SaveFieldById(Type_displayName, get_displayname());
  }
  if (has_value()) {
    serializer.SaveFieldById(Type_value, get_value());
  }
}

void Data::Accept(Visitor* visitor) {
  visitor->VisitData(DataPtr(this));
}

// <ExtendedData>
ExtendedData::ExtendedData() {
  set_xmlns(kmlbase::XMLNS_KML22);
}

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
  ElementSerializer element_serializer(*this, serializer);
  serializer.SaveElementArray(data_array_);
  serializer.SaveElementArray(schemadata_array_);
}

void ExtendedData::Accept(Visitor* visitor) {
  visitor->VisitExtendedData(ExtendedDataPtr(this));
}

void ExtendedData::AcceptChildren(VisitorDriver* driver) {
  Element::AcceptChildren(driver);
  Element::AcceptRepeated<DataPtr>(&data_array_, driver);
  Element::AcceptRepeated<SchemaDataPtr>(&schemadata_array_, driver);
}

// <Metadata>
Metadata::Metadata() {
  set_xmlns(kmlbase::XMLNS_KML22);
}

Metadata::~Metadata() {
}

void Metadata::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
}

void Metadata::Accept(Visitor* visitor) {
  visitor->VisitMetadata(MetadataPtr(this));
}

}  // end namespace kmldom
