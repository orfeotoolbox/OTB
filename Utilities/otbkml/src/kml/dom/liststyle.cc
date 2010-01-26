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

#include "kml/dom/liststyle.h"
#include "kml/dom/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

namespace kmldom {

// <ItemIcon>
ItemIcon::ItemIcon()
  : state_(ITEMICONSTATE_OPEN), has_state_(false), has_href_(false) {
}

ItemIcon::~ItemIcon() {}

void ItemIcon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_state:
      // TODO: multiple space-separated enums.
      has_state_ = element->SetEnum(&state_);
      break;
    case Type_href:
      has_href_ = element->SetString(&href_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void ItemIcon::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_state()) {
    serializer.SaveEnum(Type_state, get_state());
  }
  if (has_href()) {
    serializer.SaveFieldById(Type_href, get_href());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

// <ListStyle>
ListStyle::ListStyle()
  : listitemtype_(LISTITEMTYPE_CHECK), has_listitemtype_(false),
    bgcolor_("ffffffff"), has_bgcolor_(false) {
}

ListStyle::~ListStyle() {}

void ListStyle::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_listItemType:
      has_listitemtype_ = element->SetEnum(&listitemtype_);
      break;
    case Type_bgColor:
      has_bgcolor_ = element->SetString(&bgcolor_);
      break;
    case Type_ItemIcon:
      add_itemicon(AsItemIcon(element));
      break;
    default:
      SubStyle::AddElement(element);
      break;
  }
}

void ListStyle::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  SubStyle::Serialize(serializer);
  if (has_listitemtype()) {
    serializer.SaveEnum(Type_listItemType, get_listitemtype());
  }
  if (has_bgcolor()) {
    serializer.SaveFieldById(Type_bgColor, get_bgcolor());
  }
  for (size_t i = 0; i < get_itemicon_array_size(); ++i) {
    serializer.SaveElement(get_itemicon_array_at(i));
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
