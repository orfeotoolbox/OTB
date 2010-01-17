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

#include "kml/dom/kml.h"
#include "kml/dom/attributes.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

namespace kmldom {

Kml::Kml() : has_hint_(false) {}

Kml::~Kml() {}

static const char kHint[] = "hint";

void Kml::ParseAttributes(const Attributes& attributes) {
  has_hint_ = attributes.GetString(kHint, &hint_);
  Element::ParseAttributes(attributes);
}

void Kml::GetAttributes(Attributes* attributes) const {
  Element::GetAttributes(attributes);
  if (has_hint_) {
    attributes->SetString(kHint, hint_);
  }
}

void Kml::AddElement(const ElementPtr& element) {
  if (element->IsA(Type_Feature)) {
    set_feature(AsFeature(element));
  } else if (element->Type() == Type_NetworkLinkControl) {
    set_networklinkcontrol(AsNetworkLinkControl(element));
  } else {
    Element::AddElement(element);
  }
}

void Kml::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  if (has_networklinkcontrol()) {
    serializer.SaveElement(get_networklinkcontrol());
  }
  if (has_feature()) {
    serializer.SaveElement(get_feature());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
