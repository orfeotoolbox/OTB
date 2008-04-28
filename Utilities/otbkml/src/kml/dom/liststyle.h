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

// This file contains the declaration of the ItemIcon and ListStyle elements.

#ifndef KML_DOM_LISTSTYLE_H__
#define KML_DOM_LISTSTYLE_H__

#include <vector>
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"
#include "kml/dom/substyle.h"
#include "kml/util/util.h"

namespace kmldom {

// <ItemIcon>
class ItemIcon : public Object {
 public:
  virtual ~ItemIcon();
  virtual KmlDomType Type() const { return Type_ItemIcon; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_ItemIcon || Object::IsA(type);
  }

  // <state>
  int state() const {
    return state_;
  }
  bool has_state() const {
    return has_state_;
  }
  void set_state(int state) {
    state_ = state;
    has_state_ = true;
  }
  void clear_state() {
    state_ = ITEMICONSTATE_OPEN;
    has_state_ = false;
  }

  // <href>
  std::string href() const {
    return href_;
  }
  bool has_href() const {
    return has_href_;
  }
  void set_href(const std::string& href) {
    href_ = href;
    has_href_ = true;
  }
  void clear_href() {
    href_.clear();
    has_href_ = false;
  }

 private:
  friend class KmlFactory;
  ItemIcon();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serialize) const;
  int state_;
  bool has_state_;
  std::string href_;
  bool has_href_;
  DISALLOW_EVIL_CONSTRUCTORS(ItemIcon);
};

// <ListStyle>
class ListStyle : public SubStyle {
 public:
  virtual ~ListStyle();
  virtual KmlDomType Type() const { return Type_ListStyle; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_ListStyle || SubStyle::IsA(type);
  }

  // <listItemType>
  int listitemtype() const {
    return listitemtype_;
  }
  bool has_listitemtype() const {
    return has_listitemtype_;
  }
  void set_listitemtype(int listitemtype) {
    listitemtype_ = listitemtype;
    has_listitemtype_ = true;
  }
  void clear_listitemtype() {
    listitemtype_ = LISTITEMTYPE_CHECK;
    has_listitemtype_ = false;
  }

  // <bgColor>
  std::string bgcolor() const {
    return bgcolor_;
  }
  bool has_bgcolor() const {
    return has_bgcolor_;
  }
  void set_bgcolor(const std::string& bgcolor) {
    bgcolor_ = bgcolor;
    has_bgcolor_ = true;
  }
  void clear_bgcolor() {
    bgcolor_ = "ffffffff";
    has_bgcolor_ = false;
  }

  // <ItemIcon>
  void add_itemicon(ItemIcon* itemicon) {
    itemicon_array_.push_back(itemicon);
  }

  const size_t itemicon_array_size() const {
    return itemicon_array_.size();
  }

  const ItemIcon* itemicon_array_at(unsigned int index) const {
    return itemicon_array_[index];
  }

 private:
  friend class KmlFactory;
  ListStyle();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serialize) const;
  int listitemtype_;
  bool has_listitemtype_;
  std::string bgcolor_;
  bool has_bgcolor_;
  std::vector<ItemIcon*> itemicon_array_;
  DISALLOW_EVIL_CONSTRUCTORS(ListStyle);
};

}  // end namespace kmldom

#endif // KML_DOM_LISTSTYLE_H__
