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

// This file contains the implementation of the BalloonStyle element.

#include "kml/dom/balloonstyle.h"
#include "kml/dom/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/serializer.h"

namespace kmldom {

BalloonStyle::BalloonStyle() :
  bgcolor_("ffffffff"),
  has_bgcolor_(false),
  textcolor_("ff000000"),
  has_textcolor_(false),
  has_text_(false),
  displaymode_(DISPLAYMODE_DEFAULT),
  has_displaymode_(false) {
}

BalloonStyle::~BalloonStyle() {
}

void BalloonStyle::AddElement(Element* element) {
  switch (element->Type()) {
    case Type_bgColor:
      has_bgcolor_ = element->SetString(&bgcolor_);
      break;
    case Type_textColor:
      has_textcolor_ = element->SetString(&textcolor_);
      break;
    case Type_text:
      has_text_ = element->SetString(&text_);
      break;
    case Type_displayMode:
      has_displaymode_ = element->SetEnum(&displaymode_);
      break;
    default:
      SubStyle::AddElement(element);
      break;
  }
}

void BalloonStyle::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  SubStyle::Serialize(serializer);
  if (has_bgcolor()) {
    serializer.SaveFieldById(Type_bgColor, bgcolor());
  }
  if (has_textcolor()) {
    serializer.SaveFieldById(Type_textColor, textcolor());
  }
  if (has_text()) {
    serializer.SaveFieldById(Type_text, text());
  }
  if (has_displaymode()) {
    serializer.SaveEnum(Type_displayMode, displaymode());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
