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

// This file contains the definition of the NetworkLink element.

#include "kml/dom/networklink.h"
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"

namespace kmldom {

NetworkLink::NetworkLink() :
  refreshvisibility_(false),
  has_refreshvisibility_(false),
  flytoview_(false),
  has_flytoview_(false),
  link_(NULL) {
}

NetworkLink::~NetworkLink() {
  delete link_;
}

void NetworkLink::AddElement(Element* element) {
  switch (element->Type()) {
    case Type_refreshVisibility:
      has_refreshvisibility_ = element->SetBool(&refreshvisibility_);
      break;
    case Type_flyToView:
      has_flytoview_ = element->SetBool(&flytoview_);
      break;
    case Type_Url:
      // <Url> is deprecated.  This permits it in the parser.
      // Fall through and set as Link.
    case Type_Link:
      set_link(static_cast<Link*>(element));
      break;
    default:
      Feature::AddElement(element);
      break;
  }
}

void NetworkLink::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  Feature::Serialize(serializer);
  if (has_refreshvisibility()) {
    serializer.SaveFieldById(Type_refreshVisibility, refreshvisibility());
  }
  if (has_flytoview()) {
    serializer.SaveFieldById(Type_flyToView, flytoview());
  }
  if (has_link()) {
    // If this is <Url> it will serialize as such.
    serializer.SaveElement(*link());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
