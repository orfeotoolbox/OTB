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

#ifndef KML_DOM_KML_H__
#define KML_DOM_KML_H__

#include "kml/dom/element.h"
#include "kml/dom/feature.h"
#include "kml/dom/kml22.h"
#include "kml/dom/networklinkcontrol.h"

namespace kmldom {

class Serializer;

// <kml>
class Kml : public Element {
 public:
  virtual ~Kml();
  virtual KmlDomType Type() const { return Type_kml; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_kml;
  }

  const std::string& hint() { return hint_; }
  bool has_hint() const { return has_hint_; }
  void set_hint(const std::string& hint) {
    hint_ = hint;
    has_hint_ = true;
  }
  void clear_hint() {
    hint_.clear();
    has_hint_ = false;
  }

  const NetworkLinkControl* networklinkcontrol() const {
    return networklinkcontrol_;
  }
  bool has_networklinkcontrol() const { return networklinkcontrol_ != NULL; }
  void set_networklinkcontrol(NetworkLinkControl* networklinkcontrol) {
    if (networklinkcontrol == NULL) {
      clear_networklinkcontrol();
    } else if (networklinkcontrol->set_parent(this)) {
      delete networklinkcontrol_;  // "Last one wins"
      networklinkcontrol_ = networklinkcontrol;
    }
  }
  void clear_networklinkcontrol() {
    delete networklinkcontrol_;
    networklinkcontrol_ = NULL;
  }

  const Feature* feature() const { return feature_; }
  bool has_feature() const { return feature_ != NULL; }
  void set_feature(Feature* feature) {
    if (feature == NULL) {
      clear_feature();
    } else if (feature->set_parent(this)) {
      delete feature_;  // "Last one wins"
      feature_ = feature;
    }
  }
  void clear_feature() {
    delete feature_;
    feature_ = NULL;
  }

 private:
  friend class KmlFactory;
  Kml();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  virtual void ParseAttributes(const Attributes& attributes);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string hint_;
  bool has_hint_;
  NetworkLinkControl* networklinkcontrol_;
  Feature* feature_;
  DISALLOW_EVIL_CONSTRUCTORS(Kml);
};

}  // end namespace kmldom

#endif  // KML_DOM_KML_H__
