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

#include "kml/xsd/xsd_primitive_type.h"

namespace kmlxsd {

// This exactly matches the XsdPrimitiveType enum.
static const char* XsdPrimitiveTypeName[] = {
  NULL,  // XSD_INVALID
  "string",  // 3.2.1
  "boolean",  // 3.2.2
  "decimal",  // 3.2.3
  "float",  // 3.2.4
  "double",  // 3.2.5
  "duration",  // 3.2.6
  "dateTime",  // 3.2.7
  "time",  // 3.2.8
  "date",  // 3.2.9
  "gYearMonth",  // 3.2.10
  "gYear",  // 3.2.11
  "gMonthDay",  // 3.2.12
  "gDay",  // 3.2.13
  "gMonth",  // 3.2.14
  "hexBinary",  // 3.2.15
  "base64Binary",  // 3.2.16
  "anyURI",  // 3.2.17
  "QNAME",  // 3.2.18
  "NOTATION",  // 3.2.19
  // NOTE: this includes only the types involved in the xsd:int derivation.
  "integer",  // 3.3.13.  Is-a XSD_DECIMAL.
  "long",  // 3.3.16.  Is-a XSD_INTEGER.
  "int",  // 3.3.17.  Is-a XSD_LONG.
};

const string XsdPrimitiveType::GetTypeName(TypeId type_id) {
  return XsdPrimitiveTypeName[type_id];
}

XsdPrimitiveType::TypeId XsdPrimitiveType::GetTypeId(
    const string& type_name) {
  // TODO: make a map and save it somewhere...
  size_t size = sizeof(XsdPrimitiveTypeName)/sizeof(XsdPrimitiveTypeName[0]);
  for (size_t i = 1; i < size; ++i) {
    if (type_name.compare(XsdPrimitiveTypeName[i]) == 0) {
      return static_cast<TypeId>(i);
    }
  }
  return XSD_INVALID;
}

}  // end namespace kmlxsd
