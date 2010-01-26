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

// This file contains the implementation of the public XML serializing
// functions and the implementation of the Serialize class.

#include "kml/dom/serializer.h"
#include <string>
#include "kml/dom/element.h"
#include "kml/dom/xsd.h"

namespace kmldom {

Serializer::Serializer() : xsd_(*Xsd::GetSchema()) {
}

// Study the incoming string for chars that are invalid to represent in XML.
// Wrap the whole thing in a CDATA if any found.  Doesn't try to '&amp;'-ify.
// Avoids CDATA-ing any string that's already a CDATA.
// Returns a string that's legal for an XML value.
const std::string Serializer::MaybeQuoteString(const std::string& value) {
  if ((value.find("<![CDATA[",0) != 0) &&
      (value.find_first_of("&'<>\"", 0) != std::string::npos))
    return "<![CDATA[" + value + "]]>";
  else
    return value;
}

// This emits the string for the given enum and enum value.
// For example, type_id=Type_altitudeMode, enum_value=ALTITUDEMODE_ABSOLUTE.
// If enum_value is not valid for the given type_id nothing is emitted.
void Serializer::SaveEnum(int type_id, int enum_value) {
  std::string enum_string = xsd_.EnumValue(type_id, enum_value);
  if (!enum_string.empty()) {
    SaveFieldById(type_id, enum_string);
  }
}

// This emits the given Element.  This is a method of Serialize such that
// an Element's Serialize method need only friend class Serialize.
void Serializer::SaveElement(const ElementPtr& element) {
  element->Serialize(*this);
}

}  // namespace kmldom
