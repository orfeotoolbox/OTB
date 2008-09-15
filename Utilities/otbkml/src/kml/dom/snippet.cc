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

#include "kml/dom/snippet.h"
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

namespace kmldom {

SnippetCommon::SnippetCommon()
  : has_text_(false),
    maxlines_(2),
    has_maxlines_(false) {
}

SnippetCommon::~SnippetCommon() {}

static const char kMaxLines[] = "maxLines";

void SnippetCommon::ParseAttributes(const Attributes& attributes) {
  double maxlines_attr = 0.0;
  has_maxlines_ = attributes.GetDouble(kMaxLines, &maxlines_attr);
  if (has_maxlines_) {
    set_maxlines(static_cast<unsigned int>(maxlines_attr));
  }
  Element::ParseAttributes(attributes);
}

void SnippetCommon::GetAttributes(Attributes* attributes) const {
  Element::GetAttributes(attributes);
  if (has_maxlines_) {
    attributes->SetDouble(kMaxLines, static_cast<double>(maxlines_));
  }
}

// Snippet needs to parse its own character data.
void SnippetCommon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  set_text(get_char_data());
}

void SnippetCommon::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Element::SerializeUnknown(serializer);
  serializer.SaveContent(text_, true);
  serializer.End();
}

Snippet::Snippet() {}

Snippet::~Snippet() {}

LinkSnippet::LinkSnippet() {}

LinkSnippet::~LinkSnippet() {}

}  // end namespace kmldom
