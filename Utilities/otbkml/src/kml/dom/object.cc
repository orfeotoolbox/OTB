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

#include "kml/dom/object.h"
#include "kml/dom/attributes.h"

namespace kmldom {

Object::Object()
  : has_id_(false), has_targetid_(false) {}

Object::~Object() {}

static const char kId[] = "id";
static const char kTargetId[] = "targetId";

void Object::AddElement(const ElementPtr& element) {
  // Any element passed in here is by definition unknown. Specificially,
  // it will handle a "known" element appearing in the wrong place, e.g.
  // <Placemark><Document>. We pass everything straight to Element for
  // storage.
  Element::AddElement(element);
}

void Object::ParseAttributes(const Attributes& attributes) {
  has_id_ = attributes.GetString(kId, &id_);
  has_targetid_ = attributes.GetString(kTargetId, &targetid_);
  Element::ParseAttributes(attributes);
}

void Object::GetAttributes(Attributes* attributes) const {
  Element::GetAttributes(attributes);
  // If the id or targetId have been explictly set via API calls, we overwrite
  // the values stored in the attibutes object.
  if (has_id_) {
    attributes->SetString(kId, id_);
  }
  if (has_targetid_) {
    attributes->SetString(kTargetId, targetid_);
  }
}

}  // namespace kmldom
