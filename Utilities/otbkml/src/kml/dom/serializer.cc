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
#include "kml/base/string_util.h"
#include "kml/base/vec3.h"
#include "kml/dom/element.h"
#include "kml/dom/xsd.h"

using kmlbase::ToString;

namespace kmldom {

static string EnityEscapeXml(const string& xml) {
  kmlbase::StringMap map;
  map["&"] = "&amp;";
  map["<"] = "&lt;";
  map[">"] = "&gt;";
  map["'"] = "&apos;";
  map["\""] = "&quot;";
  return kmlbase::CreateExpandedStrings(xml, map, "", "");
}

Serializer::Serializer() : xsd_(*Xsd::GetSchema()) {
}

// Study the incoming string for chars that are invalid to represent in XML.
// Wrap the whole thing in a CDATA if any found. Avoids CDATA-ing any string
// that's already a CDATA, and will entity-escape any reserved XML chars in
// the process. Returns a string that's legal for an XML value.
const string Serializer::MaybeQuoteString(const string& value) {
  // If there's a CDATA anywhere in this string, it must have been set through
  // the API (since the underlying XML parser will strip it out). We need to
  // entity-replace any reserved characters.
  if (value.find("<![CDATA[") != string::npos) {
    return EnityEscapeXml(value);
  }
  // If the string contains any reserved characters (but does not have any
  // raw CDATA as checked above), wrap it in CDATA.
  if (value.find_first_of("&'<>\"") != string::npos) {
    return "<![CDATA[" + value + "]]>";
  }
  return value;
}

// This emits the string for the given enum and enum value.
// For example, type_id=Type_altitudeMode, enum_value=ALTITUDEMODE_ABSOLUTE.
// If enum_value is not valid for the given type_id nothing is emitted.
void Serializer::SaveEnum(int type_id, int enum_value) {
  string enum_string = xsd_.EnumValue(type_id, enum_value);
  if (!enum_string.empty()) {
    SaveFieldById(type_id, enum_string);
  }
}

// This emits the given Element.  This is a method of Serialize such that
// an Element's Serialize method need only friend class Serialize.
void Serializer::SaveElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  element->Serialize(*this);
}

// This default implementation turns the tuple into an indented string and
// emits it as generic content.  This is entirely adequate for any text-based
// serializer such as XML.
void Serializer::SaveVec3(const kmlbase::Vec3& vec3) {
  Indent();
  SaveContent(ToString(vec3.get_longitude()) + "," +
              ToString(vec3.get_latitude()) + "," +
              ToString(vec3.get_altitude()) + "\n", false); 
  // TODO: here's where we can use has_altitude() to avoid emitting that.
}

}  // namespace kmldom
