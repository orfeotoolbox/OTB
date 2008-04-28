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
#include "kml/dom/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/serializer.h"

namespace kmldom {

class IconStyleIcon;
class HotSpot;

IconStyle::IconStyle() :
  scale_(1.0),
  has_scale_(false),
  heading_(0.0),
  has_heading_(false),
  icon_(NULL),
  hotspot_(NULL) {
}

IconStyle::~IconStyle() {
  delete icon_;
  delete hotspot_;
}

void IconStyle::AddElement(Element* element) {
  switch (element->Type()) {
    case Type_scale:
      has_scale_ = element->SetDouble(&scale_);
      break;
    case Type_heading:
      has_heading_ = element->SetDouble(&heading_);
      break;
    case Type_IconStyleIcon:
      set_icon(static_cast<IconStyleIcon*>(element));
      break;
    case Type_hotSpot:
      set_hotspot(static_cast<HotSpot*>(element));
      break;
    default:
      ColorStyle::AddElement(element);
      break;
  }
}

void IconStyle::Serialize(Serializer& serializer) const {
  Attributes attributes;
  ColorStyle::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  ColorStyle::Serialize(serializer);
  if (has_scale()) {
    serializer.SaveFieldById(Type_scale, scale());
  }
  if (has_heading()) {
    serializer.SaveFieldById(Type_heading, heading());
  }
  if (has_icon()) {
    serializer.SaveElement(*icon());
  }
  if (has_hotspot()) {
    serializer.SaveElement(*hotspot());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
