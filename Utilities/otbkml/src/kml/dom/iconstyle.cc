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

// This file contains the implementation of the IconStyle element.

#include "kml/dom/iconstyle.h"
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

class IconStyleIcon;
class HotSpot;

IconStyle::IconStyle() :
  scale_(1.0),
  has_scale_(false),
  heading_(0.0),
  has_heading_(false) {
}

IconStyle::~IconStyle() {}

void IconStyle::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_scale:
      has_scale_ = element->SetDouble(&scale_);
      break;
    case Type_heading:
      has_heading_ = element->SetDouble(&heading_);
      break;
    case Type_IconStyleIcon:
      set_icon(AsIconStyleIcon(element));
      break;
    case Type_hotSpot:
      set_hotspot(AsHotSpot(element));
      break;
    default:
      ColorStyle::AddElement(element);
      break;
  }
}

void IconStyle::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  ColorStyle::Serialize(serializer);
  if (has_scale()) {
    serializer.SaveFieldById(Type_scale, get_scale());
  }
  if (has_heading()) {
    serializer.SaveFieldById(Type_heading, get_heading());
  }
  if (has_icon()) {
    serializer.SaveElement(get_icon());
  }
  if (has_hotspot()) {
    serializer.SaveElement(get_hotspot());
  }
}

void IconStyle::Accept(Visitor* visitor) {
  visitor->VisitIconStyle(IconStylePtr(this));
}

void IconStyle::AcceptChildren(VisitorDriver* driver) {
  ColorStyle::AcceptChildren(driver);
  if (has_icon()) {
    driver->Visit(get_icon());
  }
  if (has_hotspot()) {
    driver->Visit(get_hotspot());
  }
}

}  // end namespace kmldom
