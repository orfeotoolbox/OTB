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
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

SnippetCommon::SnippetCommon()
  : has_text_(false),
    maxlines_(2),
    has_maxlines_(false) {
  set_xmlns(kmlbase::XMLNS_KML22);
}

SnippetCommon::~SnippetCommon() {}

static const char kMaxLines[] = "maxLines";

void SnippetCommon::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_maxlines_ = attributes->CutValue(kMaxLines, &maxlines_);
  AddUnknownAttributes(attributes);
}

void SnippetCommon::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_maxlines_) {
    attributes->SetValue(kMaxLines, static_cast<double>(maxlines_));
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
  ElementSerializer element_serializer(*this, serializer);
  serializer.SaveContent(text_, true);
}

Snippet::Snippet() {}

Snippet::~Snippet() {}

void Snippet::Accept(Visitor* visitor) {
  visitor->VisitSnippet(SnippetPtr(this));
}

LinkSnippet::LinkSnippet() {}

LinkSnippet::~LinkSnippet() {}

void LinkSnippet::Accept(Visitor* visitor) {
  visitor->VisitLinkSnippet(LinkSnippetPtr(this));
}

}  // end namespace kmldom
