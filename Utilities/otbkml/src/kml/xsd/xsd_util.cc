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

// This file contains XSD convenience utilities.

#include "kml/xsd/xsd_util.h"
#include "kml/base/attributes.h"
#include "kml/xsd/xsd_complex_type.h"
#include "kml/xsd/xsd_schema.h"

using kmlbase::Attributes;

namespace kmlxsd {

const char kAbstract[] = "abstract";
const char kBase[] = "base";
const char kComplexType[] = "complexType";
const char kDefault[] = "default";
const char kElement[] = "element";
const char kExtension[] = "extension";
const char kEnumeration[] = "enumeration";
const char kName[] = "name";
const char kSchema[] = "schema";
const char kSimpleType[] = "simpleType";
const char kSubstitutionGroup[] = "substitutionGroup";
const char kTargetNamespace[] = "targetNamespace";
const char kRestriction[] = "restriction";
const char kType[] = "type";
const char kValue[] = "value";

// Test utility to create a <xs:complexType name="type_name">.
XsdComplexType* CreateXsdComplexType(const string& type_name) {
  Attributes attributes;
  attributes.SetString(kName, type_name);
  return XsdComplexType::Create(attributes);
}

XsdElement* CreateXsdElement(const string& name, const string& type) {
  Attributes attributes;
  attributes.SetString(kName, name);
  attributes.SetString(kType, type);
  return XsdElement::Create(attributes);
}

XsdSchema* CreateXsdSchema(const string& prefix,
                           const string& target_namespace) {
  Attributes attributes;
  attributes.SetString(string("xmlns:") + prefix, target_namespace);
  attributes.SetString("targetNamespace", target_namespace);
  return XsdSchema::Create(attributes);
}

}  // end namespace kmlxsd
