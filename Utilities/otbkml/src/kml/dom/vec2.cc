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

// This file contains the implementation of the abstract Vec2 element.

#include "kml/dom/vec2.h"
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

namespace kmldom {

Vec2::Vec2()
  : x_(1.0),
    has_x_(false),
    y_(1.0),
    has_y_(false),
    xunits_(UNITS_FRACTION),
    has_xunits_(false),
    yunits_(UNITS_FRACTION),
    has_yunits_(false) {
}

Vec2::~Vec2() {}

static const char kX[] = "x";
static const char kY[] = "y";
static const char kXUnits[] = "xunits";
static const char kYUnits[] = "yunits";

// Look up the given attribute and convert it to an enum val of the given
// enum type.  For example, attr_name="xunits" and enum_type=Type_units.
static bool GetEnumAttr(const Attributes& attributes,
                        const std::string attr_name,
                        int enum_type, int* enum_val) {
  std::string attr_val;
  if (attributes.GetString(attr_name, &attr_val)) {
    *enum_val = Xsd::GetSchema()->EnumId(enum_type, attr_val);
    return true;
  }
  return false;
}

void Vec2::ParseAttributes(const Attributes& attributes) {
  Element::ParseAttributes(attributes);
  has_x_ = attributes.GetDouble(kX, &x_);
  has_y_ = attributes.GetDouble(kY, &y_);
  has_xunits_ = GetEnumAttr(attributes, kXUnits, Type_units, &xunits_);
  has_yunits_ = GetEnumAttr(attributes, kYUnits, Type_units, &yunits_);
}

void Vec2::GetAttributes(Attributes* attributes) const {
  Element::GetAttributes(attributes);
  if (has_x_) {
    attributes->SetDouble(kX, x_);
  }
  if (has_y_) {
    attributes->SetDouble(kY, y_);
  }
  if (has_xunits_) {
    attributes->SetString(kXUnits,
                          Xsd::GetSchema()->EnumValue(Type_units, xunits_));
  }
  if (has_yunits_) {
    attributes->SetString(kYUnits,
                          Xsd::GetSchema()->EnumValue(Type_units, yunits_));
  }
}

void Vec2::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
