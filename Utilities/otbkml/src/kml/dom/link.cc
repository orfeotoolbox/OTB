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
//

#include "kml/dom/link.h"
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"

namespace kmldom {

BasicLink::BasicLink()
  : has_href_(false) {
}

BasicLink::~BasicLink() {
}

// TODO: fix CDATA parsing in general.
static const char *kCdataOpen = "<![CDATA[";

static bool SetStringInsideCdata(ElementPtr element,
                                 const std::string& char_data,
                                 std::string* val) {
  if (!element) {
    return false;
  }
  std::string::size_type offset = strlen(kCdataOpen);
  if (char_data.compare(0, offset, kCdataOpen, offset) == 0) {
    *val = char_data.substr(offset, char_data.size() - offset - 3);
    return true;
  }
  return element->SetString(val);
}

void BasicLink::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_href) {
      // TODO: use a generalized approach
      // has_href_ = element->SetString(&href_);
      has_href_ = SetStringInsideCdata(element, element->get_char_data(),
                                       &href_);
  } else {
      Object::AddElement(element);
  }
}

void BasicLink::Serialize(Serializer& serializer) const {
  Object::Serialize(serializer);
  if (has_href()) {
    serializer.SaveFieldById(Type_href, get_href());
  }
}

// Construct with defaults as per KML standard.
AbstractLink::AbstractLink()
  : refreshmode_(REFRESHMODE_ONCHANGE),
    has_refreshmode_(false),
    refreshinterval_(4.0),
    has_refreshinterval_(false),
    viewrefreshmode_(VIEWREFRESHMODE_NEVER),
    has_viewrefreshmode_(false),
    viewrefreshtime_(4.0),
    has_viewrefreshtime_(false),
    viewboundscale_(1.0),
    has_viewboundscale_(false),
    has_viewformat_(false),
    has_httpquery_(false) {
}

AbstractLink::~AbstractLink() {
}

void AbstractLink::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_refreshMode:
      has_refreshmode_ = element->SetEnum(&refreshmode_);
      break;
    case Type_refreshInterval:
      has_refreshinterval_ = element->SetDouble(&refreshinterval_);
      break;
    case Type_viewRefreshMode:
      has_viewrefreshmode_ = element->SetEnum(&viewrefreshmode_);
      break;
    case Type_viewRefreshTime:
      has_viewrefreshtime_ = element->SetDouble(&viewrefreshtime_);
      break;
    case Type_viewBoundScale:
      has_viewboundscale_ = element->SetDouble(&viewboundscale_);
      break;
    case Type_viewFormat:
      has_viewformat_ = element->SetString(&viewformat_);
      break;
    case Type_httpQuery:
      has_httpquery_ = element->SetString(&httpquery_);
      break;
    default:
      BasicLink::AddElement(element);
      break;
  }
}

void AbstractLink::Serialize(Serializer& serializer) const {
  Attributes attributes;
  AbstractLink::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  BasicLink::Serialize(serializer);
  if (has_refreshmode()) {
    serializer.SaveEnum(Type_refreshMode, get_refreshmode());
  }
  if (has_refreshinterval()) {
    serializer.SaveFieldById(Type_refreshInterval, get_refreshinterval());
  }
  if (has_viewrefreshmode()) {
    serializer.SaveEnum(Type_viewRefreshMode, get_viewrefreshmode());
  }
  if (has_viewrefreshtime()) {
    serializer.SaveFieldById(Type_viewRefreshTime, get_viewrefreshtime());
  }
  if (has_viewboundscale()) {
    serializer.SaveFieldById(Type_viewBoundScale, get_viewboundscale());
  }
  if (has_viewformat()) {
    serializer.SaveFieldById(Type_viewFormat, get_viewformat());
  }
  if (has_httpquery()) {
    serializer.SaveFieldById(Type_httpQuery, get_httpquery());
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

Link::Link() {}

Link::~Link() {}

Icon::Icon() {}

Icon::~Icon() {}

Url::Url() {}

Url::~Url() {}

IconStyleIcon::IconStyleIcon() {}

IconStyleIcon::~IconStyleIcon() {}

void IconStyleIcon::Serialize(Serializer& serializer) const {
  Attributes attributes;
  BasicLink::GetAttributes(&attributes);
  // IconStyleIcon is still called Icon.
  serializer.BeginById(Type_Icon, attributes);
  BasicLink::Serialize(serializer);
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
