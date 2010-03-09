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

// This file contains the implementation of the XsdElement class.

#include "kml/xsd/xsd_element.h"
#include "kml/xsd/xsd_primitive_type.h"
#include "kml/base/attributes.h"

namespace kmlxsd {

// private
XsdElement::XsdElement()
  : abstract_(false),  // XSD default
    ref_(false),
    min_occurs_(1),  // XSD default
    max_occurs_(1), // XSD default
    type_id_(XsdPrimitiveType::XSD_INVALID) {
}

// private
bool XsdElement::ParseAttributes(const kmlbase::Attributes& attributes) {
  // <xs:element> comes in one of two forms:
  // <xs:element name=".." type=".." [default=".."] [substitutionGroup=".."]/>
  if (attributes.GetString("name", &name_)) {
    attributes.GetBool("abstract", &abstract_);
    attributes.GetString("default", &default_);
    attributes.GetString("type", &type_);
    type_id_ = XsdPrimitiveType::GetTypeId(type_);
    attributes.GetString("substitutionGroup", &substitution_group_);
    return true;
  }
  // <xs:element ref=".."/>
  if (attributes.GetString("ref", &name_)) {
    ref_ = true;
    return true;
  }
  return false;
}

// static
XsdElement* XsdElement::Create(const kmlbase::Attributes& attributes) {
  XsdElement* xsd_element = new XsdElement;
  if (xsd_element->ParseAttributes(attributes)) {
    return xsd_element;
  }
  delete xsd_element;
  return NULL;
}

}  // end namespace kmlxsd
