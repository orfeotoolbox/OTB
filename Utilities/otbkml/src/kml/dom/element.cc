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

// This file contains the implementations of the Element and Field classes.

#include "kml/dom/element.h"
#include <stdlib.h>
#include "kml/base/attributes.h"
#include "kml/base/string_util.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;
using kmlbase::XmlElement;

namespace kmldom {

Element::Element()
 : type_id_(Type_Unknown) {
}

Element::Element(KmlDomType type_id)
  : type_id_(type_id) {
}

Element::~Element() {
}

// Anything reaching this level is an known (KML) element found in an illegal
// position during parse. We will store it for later serialiation.
void Element::AddElement(const ElementPtr& element) {
  unknown_legal_elements_array_.push_back(element);
}

// Anything that reaches this level of the hierarchy is an unknown (non-KML)
// element found during parse.
void Element::AddUnknownElement(const string& s) {
  unknown_elements_array_.push_back(s);
}

// Serialize at this level is expected to handle only the unknown elements
// we discovered during parse.
void Element::SerializeUnknown(Serializer& serializer) const {
  // First serialize the misplaced elements:
  for (size_t i = 0; i < unknown_legal_elements_array_.size(); ++i) {
    // Anything derived from Element implements a Serialize() method.
    unknown_legal_elements_array_[i]->Serialize(serializer);
  }
  // Now serialize unknown elements:
  // Announce to the Serializer that the next N SaveContent() are each
  // unparsed xml.
  size_t unknown_size = unknown_elements_array_.size();
  if (unknown_size > 0) {
    serializer.BeginElementArray(Type_Unknown, unknown_size);
    for (size_t i = 0; i < unknown_size; ++i) {
      serializer.Indent();
      // This is raw XML do not try to CDATA escape it.
      serializer.SaveContent(unknown_elements_array_[i], false);
    }
    serializer.EndElementArray(Type_Unknown);
  }
}

// Handling of unknown attributes found during parse.  Split out
// xmlns attributes.  Take ownership of the passed attributes object.
void Element::AddUnknownAttributes(Attributes* attributes) {
  if (attributes) {
    // Split out any attribute of the form xmlns:PREFIX=.
    if (Attributes* xmlns = attributes->SplitByPrefix("xmlns")) {
      if (xmlns_.get()) {
        xmlns_->MergeAttributes(*xmlns);
        delete xmlns;
      } else {
        xmlns_.reset(xmlns);
      }
    }
    // Split out xmlns= itself.
    string xmlns;
    if (attributes->CutValue("xmlns", &xmlns)) {
      if (!xmlns_.get()) {
        xmlns_.reset(new Attributes);
      }
      // "xmlns" can never be an xml namespace prefix.
      xmlns_->SetValue("xmlns", xmlns);
    }
    // Anything left is saved as fully unknown.
    if (attributes->GetSize() == 0) {
      delete attributes;  // Nothing left so delete it.
    } else {
      unknown_attributes_.reset(attributes);
    }
  }
}

// This default implementation for elements that otherwise have no attributes
// to parse simply add all attributes to the unknown attributes held in Element
// for later retreival with GetUnknownAttributes().
void Element::ParseAttributes(Attributes* attributes) {
  AddUnknownAttributes(attributes);
}

// This is the reverse of ParseAttributes().
void Element::SerializeAttributes(Attributes* attributes) const {
  if (attributes) {
    if (unknown_attributes_.get()) {
      attributes->MergeAttributes(*unknown_attributes_);
    }
    if (xmlns_.get()) {
      kmlbase::StringMapIterator iter = xmlns_->CreateIterator();
      for (; !iter.AtEnd(); iter.Advance()) {
        string key = iter.Data().first == "xmlns" ? iter.Data().first :
                          string("xmlns:") + iter.Data().first;
        attributes->SetValue(key, iter.Data().second);
      }
    }
  }
}

ElementPtr Element::GetParent() const {
  return AsElement(const_cast<XmlElement*>(XmlElement::GetParent()));
}

void Element::MergeXmlns(const Attributes& xmlns) {
  if (!xmlns_.get()) {
    xmlns_.reset(new Attributes);
  }
  xmlns_->MergeAttributes(xmlns);
}

ElementSerializer::ElementSerializer(const Element& element,
                                     Serializer& serializer)
    : element_(element), serializer_(serializer) {
  Attributes attributes;
  element_.SerializeAttributes(&attributes);
  serializer.BeginById(element_.Type(), attributes);
}

ElementSerializer::~ElementSerializer() {
  element_.SerializeUnknown(serializer_);
  serializer_.End();
}

void Element::Accept(Visitor* visitor) {
  visitor->VisitElement(ElementPtr(this));
}

Field::Field(KmlDomType type_id)
  : Element(type_id), xsd_(*Xsd::GetSchema()) {
}

void Field::Serialize(Serializer& serializer) const {
  serializer.SaveFieldById(Type(), get_char_data());
}

bool Field::SetBool(bool* val) {
  bool ret = false;
  if (val) {
    kmlbase::FromString(get_char_data(), val);
    ret = true;
  }
  return ret;
}

bool Field::SetDouble(double* val) {
  bool ret = false;
  if (val) {
    kmlbase::FromString(get_char_data(), val);
    ret = true;
  }
  return ret;
}

bool Field::SetInt(int* val) {
  bool ret = false;
  if (val) {
    kmlbase::FromString(get_char_data(), val);
    ret = true;
  }
  return ret;
}

bool Field::SetEnum(int* enum_val) {
  bool ret = false;
  if (enum_val) {
    int val = xsd_.EnumId(Type(), get_char_data());
    if (val != -1) {
      *enum_val = val;
      ret = true;
    }
  }
  return ret;
}

bool Field::SetString(string* val) {
  bool ret = false;
  if (val) {
    *val = get_char_data();
    ret = true;
  }
  return ret;
}

}  // namespace kmldom
