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

// This file contains the implementation of the Atom elements.

#include "kml/dom/atom.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"
#include "kml/base/attributes.h"

using kmlbase::Attributes;

namespace kmldom {

// Attributes.
static const char kHref[] = "href";
static const char kHrefLang[] = "hreflang";
static const char kLabel[] = "label";
static const char kLength[] = "length";
static const char kRel[] = "rel";
static const char kScheme[] = "scheme";
static const char kSrc[] = "src";
static const char kTerm[] = "term";
static const char kTitle[] = "title";
static const char kType[] = "type";

// <atom:author>
AtomAuthor::AtomAuthor()
  : has_name_(false),
    has_uri_(false),
    has_email_(false) {
  set_xmlns(kmlbase::XMLNS_ATOM);
}

AtomAuthor::~AtomAuthor() {}

void AtomAuthor::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }

  // Explicit child elements.
  switch(element->Type()) {
    case Type_atomEmail:
      has_email_ = element->SetString(&email_);
      break;
    case Type_atomName:
      has_name_ = element->SetString(&name_);
      break;
    case Type_atomUri:
      has_uri_ = element->SetString(&uri_);
      break;
    default:
      Element::AddElement(element);
  }
}

void AtomAuthor::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  // In order of http://schemas.opengis.net/kml/2.2.0/atom-author-link.xsd
  // although no order is specified (this is not an XSD sequence, for example).
  if (has_name()) {
    serializer.SaveFieldById(Type_atomName, get_name());
  } 
  if (has_uri()) {
    serializer.SaveFieldById(Type_atomUri, get_uri());
  }
  if (has_email()) {
    serializer.SaveFieldById(Type_atomEmail, get_email());
  }
} 

// <atom:category>
AtomCategory::AtomCategory()
  : has_term_(false),
    has_scheme_(false),
    has_label_(false) {
  set_xmlns(kmlbase::XMLNS_ATOM);
}

AtomCategory::~AtomCategory() {}

void AtomCategory::AddElement(const ElementPtr& element) {
  // Any element passed in here is unknown.
  Element::AddElement(element);
}

void AtomCategory::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_term_ = attributes->CutValue(kTerm, &term_);
  has_scheme_ = attributes->CutValue(kScheme, &scheme_);
  has_label_ = attributes->CutValue(kLabel, &label_);
  AddUnknownAttributes(attributes);
}

void AtomCategory::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_scheme()) {
    attributes->SetValue(kScheme, get_scheme());
  }
  if (has_term()) {
    attributes->SetValue(kTerm, get_term());
  }
  if (has_label()) {
    attributes->SetValue(kLabel, get_label());
  }
}

void AtomCategory::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
}


// <atom:content>
AtomContent::AtomContent()
  : has_src_(false),
    has_type_(false) {
  set_xmlns(kmlbase::XMLNS_ATOM);
}

AtomContent::~AtomContent() {}

void AtomContent::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_src_ = attributes->CutValue(kSrc, &src_);
  has_type_ = attributes->CutValue(kType, &type_);
  AddUnknownAttributes(attributes);
}

void AtomContent::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_src()) {
    attributes->SetValue(kSrc, get_src());
  }
  if (has_type()) {
    attributes->SetValue(kType, get_type());
  }
}

void AtomContent::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
}

// Common children of <atom:feed> and <atom:entry>.
AtomCommon::AtomCommon()
  : has_id_(false),
    has_title_(false),
    has_updated_(false) {
}

void AtomCommon::add_category(const AtomCategoryPtr& category) {
  AddComplexChild(category, &category_array_);
}

void AtomCommon::add_link(const AtomLinkPtr& link) {
  AddComplexChild(link, &link_array_);
}

void AtomCommon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }

  // Explicit child elements.
  switch(element->Type()) {
    case Type_atomId:
      has_id_ = element->SetString(&id_);
      break;
    case Type_atomTitle:
      has_title_ = element->SetString(&title_);
      break;
    case Type_atomUpdated:
      has_updated_ = element->SetString(&updated_);
      break;
    case Type_AtomCategory:
      add_category(AsAtomCategory(element));
      break;
    case Type_AtomLink:
      add_link(AsAtomLink(element));
      break;
    default:
      Element::AddElement(element);
  }
}

void AtomCommon::Serialize(Serializer& serializer) const {
  Element::Serialize(serializer);
  if (has_id()) {
    serializer.SaveFieldById(Type_atomId, get_id());
  }
  if (has_title()) {
    serializer.SaveFieldById(Type_atomTitle, get_title());
  }
  if (has_updated()) {
    serializer.SaveFieldById(Type_atomUpdated, get_updated());
  }
  serializer.SaveElementArray(category_array_);
  serializer.SaveElementArray(link_array_);
}

// <atom:entry>
AtomEntry::AtomEntry()
  : has_summary_(false) {
  set_xmlns(kmlbase::XMLNS_ATOM);
}

AtomEntry::~AtomEntry() {}

void AtomEntry::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch(element->Type()) {
    case Type_atomSummary:
      has_summary_ = element->SetString(&summary_);
      break;
    case Type_AtomContent:
      set_content(AsAtomContent(element));
      break;
    default:
      AtomCommon::AddElement(element);
  }
}

void AtomEntry::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  AtomCommon::Serialize(serializer);
  if (has_summary()) {
    serializer.SaveFieldById(Type_atomSummary, get_summary());
  }
  if (has_content()) {
    serializer.SaveElement(get_content());
  }
}

// <atom:feed>
AtomFeed::AtomFeed() {
  set_xmlns(kmlbase::XMLNS_ATOM);
}

AtomFeed::~AtomFeed() {}

void AtomFeed::add_entry(const AtomEntryPtr& atom_entry) {
  AddComplexChild(atom_entry, &entry_array_);
}

void AtomFeed::AddElement(const ElementPtr& element) {
  if (AtomEntryPtr entry = AsAtomEntry(element)) {
    add_entry(entry);
  } else {
    AtomCommon::AddElement(element);
  }
}

void AtomFeed::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  AtomCommon::Serialize(serializer);
  serializer.SaveElementArray(entry_array_);
}

// <atom:link>
AtomLink::AtomLink()
  : has_href_(false),
    has_rel_(false),
    has_type_(false),
    has_hreflang_(false),
    has_title_(false),
    has_length_(false),
    length_(0) {
  set_xmlns(kmlbase::XMLNS_ATOM);
}

AtomLink::~AtomLink() {}

void AtomLink::AddElement(const ElementPtr& element) {
  // Any element passed in here is by definition unknown, or "undefinedContent"
  // in the atom standard.
  Element::AddElement(element);
}

void AtomLink::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_href_ = attributes->CutValue(kHref, &href_);
  has_rel_ = attributes->CutValue(kRel, &rel_);
  has_type_ = attributes->CutValue(kType, &type_);
  has_hreflang_ = attributes->CutValue(kHrefLang, &hreflang_);
  has_title_ = attributes->CutValue(kTitle, &title_);
  has_length_ = attributes->CutValue(kLength, &length_);
  AddUnknownAttributes(attributes);
}

void AtomLink::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_href()) {
    attributes->SetValue(kHref, get_href());
  }
  if (has_rel()) {
    attributes->SetValue(kRel, get_rel());
  }
  if (has_type()) {
    attributes->SetValue(kType, get_type());
  }
  if (has_hreflang()) {
    attributes->SetValue(kHrefLang, get_hreflang());
  }
  if (has_title()) {
    attributes->SetValue(kTitle, get_title());
  }
  if (has_length()) {
    attributes->SetValue(kLength, get_length());
  }
}

void AtomLink::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
}

}  // end namespace kmldom
