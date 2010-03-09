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
#include "kml/base/string_util.h"
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;
using kmlbase::Color32;

namespace kmldom {

// <ItemIcon>
ItemIcon::ItemIcon()
  : has_state_(false), has_href_(false) {
    state_array_.push_back(ITEMICONSTATE_OPEN);
}

ItemIcon::~ItemIcon() {}

void ItemIcon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_state:
      {
        clear_state();
        std::vector<string> v;
        kmlbase::SplitStringUsing(element->get_char_data(), " ", &v);
        std::vector<string>::const_iterator itr;
        for (itr = v.begin(); itr != v.end(); ++itr) {
          int val = Xsd::GetSchema()->EnumId(Type_state, *itr);
          if (val != -1) {
            add_state(val);
          }
        }
        has_state_ = true;
      }
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_state()) {
    string content;
    for (size_t i = 0; i != get_state_array_size(); ++i) {
      string s = Xsd::GetSchema()->EnumValue(Type_state,
                                                  get_state_array_at(i));
      content.append(s);
      if (i != get_state_array_size() - 1) {
        content.append(" ");
      }
    }
    serializer.SaveFieldById(Type_state, content);
  }
  if (has_href()) {
    serializer.SaveFieldById(Type_href, get_href());
  }
}

void ItemIcon::Accept(Visitor* visitor) {
  visitor->VisitItemIcon(ItemIconPtr(this));
}

// <ListStyle>
ListStyle::ListStyle()
  : listitemtype_(LISTITEMTYPE_CHECK), has_listitemtype_(false),
    bgcolor_(Color32(0xffffffff)), has_bgcolor_(false),
    maxsnippetlines_(2), has_maxsnippetlines_(false) {
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
      set_bgcolor(Color32(element->get_char_data()));
      break;
    case Type_ItemIcon:
      add_itemicon(AsItemIcon(element));
      break;
    case Type_maxSnippetLines:
      has_maxsnippetlines_ = element->SetInt(&maxsnippetlines_);
      break;
    default:
      SubStyle::AddElement(element);
      break;
  }
}

void ListStyle::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  SubStyle::Serialize(serializer);
  if (has_listitemtype()) {
    serializer.SaveEnum(Type_listItemType, get_listitemtype());
  }
  if (has_bgcolor()) {
    serializer.SaveColor(Type_bgColor, get_bgcolor());
  }
  serializer.SaveElementArray(itemicon_array_);
  if (has_maxsnippetlines()) {
    serializer.SaveFieldById(Type_maxSnippetLines, get_maxsnippetlines());
  }
}

void ListStyle::Accept(Visitor* visitor) {
  visitor->VisitListStyle(ListStylePtr(this));
}

void ListStyle::AcceptChildren(VisitorDriver* driver) {
  SubStyle::AcceptChildren(driver);
  Element::AcceptRepeated<ItemIconPtr>(&itemicon_array_, driver);
}

}  // end namespace kmldom
