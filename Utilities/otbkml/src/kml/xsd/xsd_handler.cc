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

// This file contains the implementation of the XsdHandler class.

#include "kml/xsd/xsd_handler.h"
#include <cstring>  // strcmp
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "kml/xsd/xsd_element.h"
#include "kml/xsd/xsd_file.h"
#include "kml/xsd/xsd_schema.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::Attributes;

namespace kmlxsd {

// <xs:complexType name="..." abstract="...">
void XsdHandler::StartComplexType(const Attributes& attributes) {
  if (!current_type_) {  // <xs:complexType> and/or <xs:simpleType>
    current_type_ = XsdComplexType::Create(attributes);
  }
}

// </xs:complexType">
// </xs:simpleType">
void XsdHandler::EndType() {
  xsd_file_->add_type(current_type_);
  current_type_ = NULL;
}

// <xs:extension base="...">
void XsdHandler::StartExtension(const Attributes& attributes) {
  if (XsdComplexTypePtr complex_type =
          XsdComplexType::AsComplexType(current_type_)) {
    string base;
    if (attributes.GetString(kBase, &base)) {
      complex_type->set_extension_base(base);
    }
  }
}

// <xs:simpleType name="..."/>
void XsdHandler::StartSimpleType(const Attributes& attributes) {
  if (!current_type_) {  // <xs:simpleType is never nested...
    current_type_ = XsdSimpleType::Create(attributes);
  }
}

// <xs:restriction base="...">
void XsdHandler::StartRestriction(const Attributes& attributes) {
  if (XsdSimpleTypePtr simple_type =
          XsdSimpleType::AsSimpleType(current_type_)) {
    string base;
    if (attributes.GetString(kBase, &base)) {
      simple_type->set_restriction_base(base);
    }
  }
}

// <xs:enumeration value="...">
void XsdHandler::StartEnumeration(const Attributes& attributes) {
  if (XsdSimpleTypePtr simple_type =
          XsdSimpleType::AsSimpleType(current_type_)) {
    string value;
    if (attributes.GetString(kValue, &value)) {
      simple_type->add_enumeration(value);
    }
  }
}

// <xs:element name="..." type="..." default=".."/>
// <xs:element ref="..." minOccurs="..." maxOccurs="..."/>
void XsdHandler::StartXsElement(const Attributes& attributes) {
  XsdElement* element = XsdElement::Create(attributes);
  if (!element) {
    return;
  }

  // Is this a global element?
  if (parse_.top() == kSchema) {
    xsd_file_->add_element(element);
  } else if (XsdComplexTypePtr complex_type =
                 XsdComplexType::AsComplexType(current_type_)) {
     // Is it a child of <xs:complexType>?
    complex_type->add_element(element);
  }
}

// ExpatHandler::StartElement
void XsdHandler::StartElement(const string& xs_element_name,
                              const kmlbase::StringVector& atts) {
  boost::scoped_ptr<Attributes> attributes(Attributes::Create(atts));

  if (xs_element_name.compare(kSchema) == 0) {
    xsd_file_->set_schema(XsdSchema::Create(*attributes));
  } else if (xs_element_name.compare(kElement) == 0) {
    StartXsElement(*attributes);
  } else if (xs_element_name.compare(kComplexType) == 0) {
    StartComplexType(*attributes);
  } else if (xs_element_name.compare(kExtension) == 0) {
    StartExtension(*attributes);
  } else if (xs_element_name.compare(kSimpleType) == 0) {
    StartSimpleType(*attributes);
  } else if (xs_element_name.compare(kRestriction) == 0) {
    StartRestriction(*attributes);
  } else if (xs_element_name.compare(kEnumeration) == 0) {
    StartEnumeration(*attributes);
  }

  // Always push, and we always pop in EndElement().
  parse_.push(xs_element_name);
}

// ExpatHandler::EndElement
void XsdHandler::EndElement(const string& xs_element_name) {
  // Always pop, because we always push in StartElement().
  parse_.pop();
  if (xs_element_name.compare(kComplexType) == 0) {
    EndType();
  } else if (xs_element_name.compare(kSimpleType) == 0) {
    EndType();
  }
}

}  // end namespace kmlxsd
