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

// This file contains the declarations of the Pair and StyleMap elements.

#ifndef KML_DOM_STYLEMAP_H__
#define KML_DOM_STYLEMAP_H__

#include <string>
#include <vector>
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"
#include "kml/dom/styleselector.h"

namespace kmldom {

class Serializer;

// <Pair>
class Pair : public Object {
 public:
  virtual ~Pair();
  virtual KmlDomType Type() const { return Type_Pair; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Pair || Object::IsA(type);
  }

  // <key>
  int key() const {
    return key_;
  }
  bool has_key() const {
    return has_key_;
  }
  void set_key(int key) {
    key_ = key;
    has_key_ = true;
  }
  void clear_key() {
    key_ = STYLESTATE_NORMAL;
    has_key_ = false;
  }

  // <styleUrl>
  std::string styleurl() const {
    return styleurl_;
  }
  bool has_styleurl() const {
    return has_styleurl_;
  }
  void set_styleurl(const std::string& styleurl) {
    styleurl_ = styleurl;
    has_styleurl_ = true;
  }
  void clear_styleurl() {
    styleurl_.clear();
    has_styleurl_ = false;
  }

  // StyleSelector
  const StyleSelector* styleselector() const { return styleselector_; }
  bool has_styleselector() const { return styleselector_ != NULL; }
  void set_styleselector(StyleSelector* styleselector) {
    if (styleselector == NULL) {
      clear_styleselector();
    } else if (styleselector->set_parent(this)) {
      delete styleselector_;
      styleselector_ = styleselector;
    }
  }
  void clear_styleselector() {
    delete styleselector_;
    styleselector_ = NULL;
  }

 private:
  friend class KmlFactory;
  Pair ();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  int key_;
  bool has_key_;
  std::string styleurl_;
  bool has_styleurl_;
  StyleSelector* styleselector_;
  DISALLOW_EVIL_CONSTRUCTORS(Pair);
};

// <StyleMap>
class StyleMap : public StyleSelector {
 public:
  virtual ~StyleMap();
  virtual KmlDomType Type() const { return Type_StyleMap; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_StyleMap || StyleSelector::IsA(type);
  }

  void add_pair(Pair* pair) {
    pair_array_.push_back(pair);
  }

  const size_t pair_array_size() const {
    return pair_array_.size();
  }

  const Pair* pair_array_at(unsigned int index) const {
    return pair_array_[index];
  }

private:
  friend class KmlFactory;
  StyleMap ();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<Pair*> pair_array_;
  DISALLOW_EVIL_CONSTRUCTORS(StyleMap);
};

}  // end namespace kmldom

#endif  // KML_DOM_STYLEMAP_H__
