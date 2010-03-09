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

// This file implements the internal Xsd class specifically for KML 2.2.

#include "kml/dom/xsd.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml22.cc"

namespace kmldom {

Xsd* Xsd::schema_ = NULL;

Xsd* Xsd::GetSchema() {
  if (schema_ == NULL) {
    schema_ = new Xsd;
  }
  return schema_;
}

Xsd::Xsd() {
  for (int i = 0; i < Type_Invalid; ++i) {
    tag_to_id[kKml22Elements[i].element_name_] = i;
  }
}

int Xsd::ElementId(const string& element_name) const {
  tag_id_map_t::const_iterator iter = tag_to_id.find(element_name);
  if (iter == tag_to_id.end()) {
    return Type_Unknown;
  }
  return iter->second;
}

static bool is_valid(int id) {
  return id > Type_Unknown && id < Type_Invalid;
}

string Xsd::ElementName(int id) const {
  if (!is_valid(id)) {
    return string();
  }
  // This is the other side of the wart found in KmlHandler::StartElement.
  // TODO: factor this and kKml22 out of Xsd.
  if (id == Type_IconStyleIcon) {
    return "Icon";
  }
  XsdElement element = kKml22Elements[id];
  return element.element_name_;
}

XsdType Xsd::ElementType(int id) const {
  if (!is_valid(id)) {
    return XSD_UNKNOWN;
  }
  XsdElement element = kKml22Elements[id];
  return element.xsd_type_;
}

int Xsd::EnumId(int type_id, string enum_value) const {
  const int size = sizeof(kKml22Enums)/sizeof(XsdSimpleTypeEnum);
  for (int i = 0; i < size; ++i) {
    XsdSimpleTypeEnum* simple = kKml22Enums + i;
    if (simple->type_id == type_id) {
      for (const char** enum_value_item = simple->enum_value_list;
           *enum_value_item;
           ++enum_value_item) {
        if (*enum_value_item == enum_value) {
          // enum id is simple offset into enum_value_list;
          return static_cast<int>(enum_value_item - simple->enum_value_list);
        }
      }
    }
  }
  // KML fully ignores the whole element if the enum value is unknown.  Thus,
  // <altitudeMode>somethingNew</altitudeMode> is effectively
  // <altitudeMode>clampToGround</altitudeMode> given
  // that clampToGround is the default for <altitudeMode>.
  return -1;
}

string Xsd::EnumValue(int type_id, int enum_id) const {
  if (enum_id < 0) {
    return string();
  }
  for (XsdSimpleTypeEnum* simple = kKml22Enums; simple; ++simple) {
    if (simple->type_id == type_id) {
      return simple->enum_value_list[enum_id];
    }
  }
  return string();
}


}  // end namespace kmldom
