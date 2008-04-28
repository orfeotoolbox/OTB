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

// This file contains the declaration of the NetworkLinkControl element.

#ifndef KML_DOM_NETWORKLINKCONTROL_H__
#define KML_DOM_NETWORKLINKCONTROL_H__

#include <vector>
#include "kml/dom/abstractview.h"
#include "kml/dom/container.h"
#include "kml/dom/element.h"
#include "kml/dom/feature.h"
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"
#include "kml/util/util.h"

namespace kmldom {

// UpdateOperation
// An internal class from which <Create>, <Delete> and <Change> derive. The
// KML XSD uses a choice here which is not readily modeled in C++.
class UpdateOperation : public Element {
 public:
  virtual ~UpdateOperation();

 protected:
  // UpdateOperation is abstract.
  UpdateOperation();

 private:
  DISALLOW_EVIL_CONSTRUCTORS(UpdateOperation);
};

// <Create>
class Create : public UpdateOperation {
 public:
  virtual ~Create();
  virtual KmlDomType Type() const { return Type_Create; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Create;
  }

  // Create targets containers.
  void add_container(Container* container) {
    container_array_.push_back(container);
  }

  const size_t container_array_size() const {
    return container_array_.size();
  }

  const Container* container_array_at(unsigned int index) const {
    return container_array_[index];
  }

 private:
  friend class KmlFactory;
  Create();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<Container*> container_array_;
  DISALLOW_EVIL_CONSTRUCTORS(Create);
};

// <Delete>
class Delete : public UpdateOperation {
 public:
  virtual ~Delete();
  virtual KmlDomType Type() const { return Type_Delete; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Delete || Element::IsA(type);
  }

  // Delete targets Features.
  void add_feature(Feature* feature) {
    feature_array_.push_back(feature);
  }

  const size_t feature_array_size() const {
    return feature_array_.size();
  }

  const Feature* feature_array_at(unsigned int index) const {
    return feature_array_[index];
  }

 private:
  friend class KmlFactory;
  Delete();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<Feature*> feature_array_;
  DISALLOW_EVIL_CONSTRUCTORS(Delete);
};

// <Change>
class Change : public UpdateOperation {
 public:
  virtual ~Change();
  virtual KmlDomType Type() const { return Type_Change; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Change || Element::IsA(type);
  }

  // Change targets Objects.
  void add_object(Object* object) {
    object_array_.push_back(object);
  }

  const size_t object_array_size() const {
    return object_array_.size();
  }

  const Object* object_array_at(unsigned int index) const {
    return object_array_[index];
  }

 private:
  friend class KmlFactory;
  Change();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<Object*> object_array_;
  DISALLOW_EVIL_CONSTRUCTORS(Change);
};

// <Update>
class Update : public Element {
 public:
  virtual ~Update();
  virtual KmlDomType Type() const { return Type_Update; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Update || Element::IsA(type);
  }

  // <targetHref>
  const std::string& targethref() const { return targethref_; }
  bool has_targethref() const { return has_targethref_; }
  void set_targethref(const std::string& targethref) {
    targethref_ = targethref;
    has_targethref_ = true;
  }
  void clear_targethref() {
    targethref_.clear();
    has_targethref_ = false;
  }

  // <Create>, <Delete> and <Change> elements.
  void add_updateoperation(UpdateOperation* updateoperation) {
    updateoperation_array_.push_back(updateoperation);
  }

  const size_t updateoperation_array_size() const {
    return updateoperation_array_.size();
  }

  const UpdateOperation* updateoperation_array_at(unsigned int index) const {
    return updateoperation_array_[index];
  }

 private:
  friend class KmlFactory;
  Update();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::string targethref_;
  bool has_targethref_;
  std::vector<UpdateOperation*> updateoperation_array_;
  DISALLOW_EVIL_CONSTRUCTORS(Update);
};

// <NetworkLinkControl>
class NetworkLinkControl : public Element {
 public:
  virtual ~NetworkLinkControl();
  virtual KmlDomType Type() const { return Type_NetworkLinkControl; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_NetworkLinkControl || Element::IsA(type);
  }

  // <minRefreshPeriod>
  double minrefreshperiod() const { return minrefreshperiod_; }
  bool has_minrefreshperiod() const { return has_minrefreshperiod_; }
  void set_minrefreshperiod(double value) {
    minrefreshperiod_ = value;
    has_minrefreshperiod_ = true;
  }
  void clear_minrefreshperiod() {
    minrefreshperiod_ = 0.0;
    has_minrefreshperiod_ = false;
  }

  // <maxSessionLength>
  double maxsessionlength() const { return maxsessionlength_; }
  bool has_maxsessionlength() const { return has_maxsessionlength_; }
  void set_maxsessionlength(double value) {
    maxsessionlength_ = value;
    has_maxsessionlength_ = true;
  }
  void clear_maxsessionlength() {
    maxsessionlength_ = 0.0;
    has_maxsessionlength_ = false;
  }

  // <cookie>
  const std::string& cookie() const { return cookie_; }
  bool has_cookie() const { return has_cookie_; }
  void set_cookie(const std::string& cookie) {
    cookie_ = cookie;
    has_cookie_ = true;
  }
  void clear_cookie() {
    cookie_.clear();
    has_cookie_ = false;
  }

  // <message>
  const std::string& message() const { return message_; }
  bool has_message() const { return has_message_; }
  void set_message(const std::string& message) {
    message_ = message;
    has_message_ = true;
  }
  void clear_message() {
    message_.clear();
    has_message_ = false;
  }

  // <linkName>
  const std::string& linkname() const { return linkname_; }
  bool has_linkname() const { return has_linkname_; }
  void set_linkname(const std::string& linkname) {
    linkname_ = linkname;
    has_linkname_ = true;
  }
  void clear_linkname() {
    linkname_.clear();
    has_linkname_ = false;
  }

  // <linkDescription>
  const std::string& linkdescription() const { return linkdescription_; }
  bool has_linkdescription() const { return has_linkdescription_; }
  void set_linkdescription(const std::string& linkdescription) {
    linkdescription_ = linkdescription;
    has_linkdescription_ = true;
  }
  void clear_linkdescription() {
    linkdescription_.clear();
    has_linkdescription_ = false;
  }

  // <linkSnippet>
  const LinkSnippet* linksnippet() const { return linksnippet_; }
  bool has_linksnippet() const { return linksnippet_ != NULL; }
  void set_linksnippet(LinkSnippet* linksnippet) {
    if (linksnippet == NULL) {
      clear_linksnippet();
    } else if (linksnippet->set_parent(this)) {
      delete linksnippet_;  // last one wins
      linksnippet_ = linksnippet;
    }
  }
  void clear_linksnippet() {
    delete linksnippet_;
    linksnippet_ = NULL;
  }

  // <expires>
  const std::string& expires() const { return expires_; }
  bool has_expires() const { return has_expires_; }
  void set_expires(const std::string& expires) {
    expires_ = expires;
    has_expires_ = true;
  }
  void clear_expires() {
    expires_.clear();
    has_expires_ = false;
  }

  // <Update>
  const Update* update() const { return update_; }
  bool has_update() const { return update_ != NULL; }
  void set_update(Update* update) {
    if (update == NULL) {
      clear_update();
    } else if (update->set_parent(this)) {
      delete update_;  // last one wins
      update_ = update;
    }
  }
  void clear_update() {
    delete update_;
    update_ = NULL;
  }

  // AbstractView
  const AbstractView* abstractview() const { return abstractview_; }
  bool has_abstractview() const { return abstractview_ != NULL; }
  void set_abstractview(AbstractView* abstractview) {
    if (abstractview == NULL) {
      clear_abstractview();
    } else if (abstractview->set_parent(this)) {
      delete abstractview_;  // note: "last one wins".
      abstractview_ = abstractview;
    }
  }
  void clear_abstractview() {
    delete abstractview_;
    abstractview_ = NULL;
  }

 private:
  friend class KmlFactory;
  NetworkLinkControl();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double minrefreshperiod_;
  bool has_minrefreshperiod_;
  double maxsessionlength_;
  bool has_maxsessionlength_;
  std::string cookie_;
  bool has_cookie_;
  std::string message_;
  bool has_message_;
  std::string linkname_;
  bool has_linkname_;
  std::string linkdescription_;
  bool has_linkdescription_;
  LinkSnippet* linksnippet_;
  std::string expires_;
  bool has_expires_;
  Update* update_;
  AbstractView* abstractview_;
  DISALLOW_EVIL_CONSTRUCTORS(NetworkLinkControl);
};

}  // namespace kmldom

#endif  // KML_DOM_NETWORKLINKCONTROL_H__
