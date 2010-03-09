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

#include "kml/dom/stylemap.h"
#include "kml/base/attributes.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

// <Pair>
Pair::Pair()
  : key_(STYLESTATE_NORMAL), has_key_(false), has_styleurl_(false) {
}

Pair::~Pair() {}

void Pair::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_StyleSelector)) {
    set_styleselector(AsStyleSelector(element));
    return;
  }
  switch (element->Type()) {
    case Type_key:
      has_key_ = element->SetEnum(&key_);
      break;
    case Type_styleUrl:
      has_styleurl_ = element->SetString(&styleurl_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void Pair::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_key()) {
    serializer.SaveEnum(Type_key, get_key());
  }
  if (has_styleurl()) {
    serializer.SaveFieldById(Type_styleUrl, get_styleurl());
  }
  if (has_styleselector()) {
    serializer.SaveElementGroup(get_styleselector(), Type_StyleSelector);
  }
}

void Pair::Accept(Visitor* visitor) {
  visitor->VisitPair(PairPtr(this));
}

void Pair::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  if (has_styleselector()) {
    driver->Visit(get_styleselector());
  }
}

// <StyleMap>
StyleMap::StyleMap() {}

StyleMap::~StyleMap() {}

void StyleMap::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_Pair) {
    add_pair(AsPair(element));
  } else {
    StyleSelector::AddElement(element);
  }
}

void StyleMap::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  StyleSelector::Serialize(serializer);
  serializer.SaveElementArray(pair_array_);
}

void StyleMap::Accept(Visitor* visitor) {
  visitor->VisitStyleMap(StyleMapPtr(this));
}

void StyleMap::AcceptChildren(VisitorDriver* driver) {
  StyleSelector::AcceptChildren(driver);
  Element::AcceptRepeated<PairPtr>(&pair_array_, driver);
}

}  // end namespace kmldom
