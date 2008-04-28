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

#include "kml/dom/style.h"
#include "kml/dom/attributes.h"
#include "kml/dom/kml22.h"
#include "kml/dom/serializer.h"

namespace kmldom {

Style::Style() :
  iconstyle_(NULL),
  labelstyle_(NULL),
  linestyle_(NULL),
  polystyle_(NULL),
  balloonstyle_(NULL),
  liststyle_(NULL) {
}

Style::~Style() {
  delete iconstyle_;
  delete labelstyle_;
  delete linestyle_;
  delete polystyle_;
  delete balloonstyle_;
  delete liststyle_;
}

void Style::AddElement(Element* element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_IconStyle:
      set_iconstyle(static_cast<IconStyle*>(element));
      break;
    case Type_LabelStyle:
      set_labelstyle(static_cast<LabelStyle*>(element));
      break;
    case Type_LineStyle:
      set_linestyle(static_cast<LineStyle*>(element));
      break;
    case Type_PolyStyle:
      set_polystyle(static_cast<PolyStyle*>(element));
      break;
    case Type_BalloonStyle:
      set_balloonstyle(static_cast<BalloonStyle*>(element));
      break;
    case Type_ListStyle:
      set_liststyle(static_cast<ListStyle*>(element));
      break;
    default:
      StyleSelector::AddElement(element);
      break;
  }
}

void Style::Serialize(Serializer& serializer) const {
  Attributes attributes;
  StyleSelector::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  StyleSelector::Serialize(serializer);
  if (has_iconstyle()) {
    serializer.SaveElement(*iconstyle());
  }
  if (has_labelstyle()) {
    serializer.SaveElement(*labelstyle());
  }
  if (has_linestyle()) {
    serializer.SaveElement(*linestyle());
  }
  if (has_polystyle()) {
    serializer.SaveElement(*polystyle());
  }
  if (has_balloonstyle()) {
    serializer.SaveElement(*balloonstyle());
  }
  if (has_liststyle()) {
    serializer.SaveElement(*liststyle());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
