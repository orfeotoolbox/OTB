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

// This file contains the declaration of the abstract Feature element.

#ifndef KML_DOM_FEATURE_H__
#define KML_DOM_FEATURE_H__

#include "kml/dom/abstractview.h"
#include "kml/dom/extendeddata.h"
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"
#include "kml/dom/region.h"
#include "kml/dom/snippet.h"
#include "kml/dom/styleselector.h"
#include "kml/dom/timeprimitive.h"
#include "kml/util/util.h"

namespace kmldom {

class Feature : public Object {
 public:
  virtual ~Feature();
  virtual KmlDomType Type() const { return Type_Feature; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Feature || Object::IsA(type);
  }

  // <name>
  const std::string& name() const { return name_; }
  bool has_name() const { return has_name_; }
  void set_name(const std::string& value) {
    name_ = value;
    has_name_ = true;
  }
  void clear_name() {
    name_.clear();
    has_name_ = false;
  }

  // <visibility>
  bool visibility() const { return visibility_; }
  bool has_visibility() const { return has_visibility_; }
  void set_visibility(bool value) {
    visibility_ = value;
    has_visibility_ = true;
  }
  void clear_visibility() {
    visibility_ = true;  // Default <visibility> is true.
    has_visibility_ = false;
  }

  // <open>
  bool open() const { return open_; }
  bool has_open() const { return has_open_; }
  void set_open(bool value) {
    open_ = value;
    has_open_ = true;
  }
  void clear_open() {
    open_ = false;
    has_open_ = false;
  }

  // <address>
  const std::string& address() const { return address_; }
  bool has_address() const { return has_address_; }
  void set_address(const std::string& value) {
    address_ = value;
    has_address_ = true;
  }
  void clear_address() {
    address_.clear();
    has_address_ = false;
  }

  // <phoneNumber>
  const std::string& phonenumber() const { return phonenumber_; }
  bool has_phonenumber() const { return has_phonenumber_; }
  void set_phonenumber(const std::string& value) {
    phonenumber_ = value;
    has_phonenumber_ = true;
  }
  void clear_phonenumber() {
    phonenumber_.clear();
    has_phonenumber_ = false;
  }

  // <Snippet>
  const Snippet* snippet() const { return snippet_; }
  bool has_snippet() const { return snippet_ != NULL; }
  void set_snippet(Snippet* snippet) {
    if (snippet == NULL) {
      clear_snippet();
    } else if (snippet->set_parent(this)) {
      delete snippet_;  // last one wins
      snippet_ = snippet;
    }
  }
  void clear_snippet() {
    delete snippet_;
    snippet_ = NULL;
  }

  // <description>
  const std::string& description() const { return description_; }
  bool has_description() const { return has_description_; }
  void set_description(const std::string& value) {
    description_ = value;
    has_description_ = true;
  }
  void clear_description() {
    description_.clear();
    has_description_ = false;
  }

  // AbstractView
  const AbstractView* abstractview() const { return abstractview_; }
  bool has_abstractview() const { return abstractview_ != NULL; }
  void set_abstractview(AbstractView* abstractview) {
    if (abstractview == NULL) {
      clear_abstractview();
    } else if (abstractview->set_parent(this)) {
      delete abstractview_;
      abstractview_ = abstractview;
    }
  }
  void clear_abstractview() {
    delete abstractview_;
    abstractview_ = NULL;
  }

  // TimePrimitive
  const TimePrimitive* timeprimitive() const { return timeprimitive_; }
  bool has_timeprimitive() const { return timeprimitive_ != NULL; }
  void set_timeprimitive(TimePrimitive* timeprimitive) {
    if (timeprimitive == NULL) {
      clear_timeprimitive();
    } else if (timeprimitive->set_parent(this)) {
      delete timeprimitive_;
      timeprimitive_ = timeprimitive;
    }
  }
  void clear_timeprimitive() {
    delete timeprimitive_;
    timeprimitive_ = NULL;
  }

  // <styleUrl>
  const std::string& styleurl() const { return styleurl_; }
  std::string& styleurl() { return styleurl_; }
  bool has_styleurl() const { return has_styleurl_; }
  void set_styleurl(const std::string& value) {
    styleurl_ = value;
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

  // <Region>
  const Region* region() const { return region_; }
  bool has_region() const { return region_ != NULL; }
  void set_region(Region* region) {
    if (region == NULL) {
      clear_region();
    } else if (region->set_parent(this)) {
      delete region_;
      region_ = region;
    }
  }
  void clear_region() {
    delete region_;
    region_ = NULL;
  }

  // <ExtendedData>
  const ExtendedData* extendeddata() const { return extendeddata_; }
  bool has_extendeddata() const { return extendeddata_ != NULL; }
  void set_extendeddata(ExtendedData* extendeddata) {
    if (extendeddata == NULL) {
      clear_extendeddata();
    } else if (extendeddata->set_parent(this)) {
      delete extendeddata_;
      extendeddata_ = extendeddata;
    }
  }
  void clear_extendeddata() {
    delete extendeddata_;
    extendeddata_ = NULL;
  }

 protected:
  // Feature is abstract.
  Feature();
  virtual void AddElement(Element* element);
  virtual void Serialize(Serializer& serialize) const;

 private:
  std::string name_;
  bool has_name_;
  bool visibility_;
  bool has_visibility_;
  bool open_;
  bool has_open_;
  std::string address_;
  bool has_address_;
  std::string phonenumber_;
  bool has_phonenumber_;
  Snippet* snippet_;
  std::string description_;
  bool has_description_;
  AbstractView* abstractview_;
  TimePrimitive* timeprimitive_;
  std::string styleurl_;
  bool has_styleurl_;
  StyleSelector* styleselector_;
  Region* region_;
  ExtendedData* extendeddata_;
  DISALLOW_EVIL_CONSTRUCTORS(Feature);
};

}  // namespace kmldom

#endif  // KML_DOM_FEATURE_H__
