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
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"

using kmlbase::Attributes;

namespace kmldom {

Object::Object()
  : has_id_(false),
     has_targetid_(false) {
  set_xmlns(kmlbase::XMLNS_KML22);
}

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

void Object::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_id_ = attributes->CutValue(kId, &id_);
  has_targetid_ = attributes->CutValue(kTargetId, &targetid_);
  AddUnknownAttributes(attributes);
}

void Object::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  // If the id or targetId have been explictly set via API calls, we overwrite
  // the values stored in the attributes object.
  if (has_id_) {
    attributes->SetValue(kId, id_);
  }
  if (has_targetid_) {
    attributes->SetValue(kTargetId, targetid_);
  }
}


}  // namespace kmldom
