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
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

namespace kmldom {

Element::Element()
  : parent_(NULL), unknown_attributes_(NULL) {
}

Element::Element(KmlDomType type_id)
  : parent_(NULL), type_id_(type_id), unknown_attributes_(NULL) {
}

Element::~Element() {
  delete unknown_attributes_;
}

// Anything reaching this level is an known (KML) element found in an illegal
// position during parse. We will store it for later serialiation.
void Element::AddElement(const ElementPtr& element) {
  unknown_legal_elements_array_.push_back(element);
}

// Anything that reaches this level of the hierarchy is an unknown (non-KML)
// element found during parse.
void Element::AddUnknownElement(const std::string& s) {
  unknown_elements_array_.push_back(s);
}

// Serialize at this level is expected to handle only the unknown elements
// we discovered during parse.
void Element::SerializeUnknown(Serializer& serializer) const {
  // First serialize the misplaced elements:
  for (size_t i = 0; i < unknown_legal_elements_array_.size(); ++i) {
    // We need to serialize each according to its type.
    KmlDomType type_id = unknown_legal_elements_array_[i]->Type();
    XsdType xsd_type = Xsd::GetSchema()->ElementType(type_id);
    if (xsd_type == XSD_COMPLEX_TYPE) {
      // Can serialize itself.
      unknown_legal_elements_array_[i]->Serialize(serializer);
    } else {
      // Is field, serialize directly.
      serializer.SaveFieldById(
          type_id, unknown_legal_elements_array_[i]->get_char_data());
    }
  }
  // Now serialize unknown elements:
  for (size_t i = 0; i < unknown_elements_array_.size(); ++i) {
    serializer.Indent();
    // This is raw XML do not try to CDATA escape it.
    serializer.SaveContent(unknown_elements_array_[i], false);
  }
}

// Handling of unknown attributes found during parse. We make a copy of the
// Attributes object and store a pointer.
void Element::ParseAttributes(const Attributes& attributes) {
  unknown_attributes_ = attributes.Clone();
}

// Passes the stored Attributes object to the caller which performs a merge
// of the unknown attributes with the knowns. See attributes.h for how
// conflicts are resolved.
void Element::GetAttributes(Attributes* attributes) const {
  if (attributes && unknown_attributes_) {
    attributes->MergeAttributes(*unknown_attributes_);
  }
}

Field::Field(KmlDomType type_id)
  : Element(type_id), xsd_(*Xsd::GetSchema()) {
}

bool Field::SetBool(bool* val) {
  bool ret = false;
  if (val) {
    *val = get_char_data() == "1" || get_char_data() == "true";
    ret = true;
  }
  return ret;
}

bool Field::SetDouble(double* val) {
  bool ret = false;
  if (val) {
    *val = strtod(get_char_data().c_str(), NULL);
    ret = true;
  }
  return ret;
}

bool Field::SetInt(int* val) {
  bool ret = false;
  if (val) {
    *val = atoi(get_char_data().c_str());
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

bool Field::SetString(std::string* val) {
  bool ret = false;
  if (val) {
    *val = get_char_data();
    ret = true;
  }
  return ret;
}

}  // namespace kmldom
