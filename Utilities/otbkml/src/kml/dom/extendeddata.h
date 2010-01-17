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

// This file contains the declarations of the SimpleData, SchemaData,
// Data, and ExtendedData elements.

#ifndef KML_DOM_EXTENDEDDATA_H__
#define KML_DOM_EXTENDEDDATA_H__

#include <string>
#include <vector>
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/object.h"
#include "kml/base/util.h"

namespace kmldom {

// <SimpleData>
class SimpleData : public Element {
 public:
  virtual ~SimpleData();
  virtual KmlDomType Type() const { return Type_SimpleData; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_SimpleData;
  }

  // name=
  const std::string& get_name() const { return name_; }
  bool has_name() const { return has_name_; }
  void set_name(const std::string& value) {
    name_ = value;
    has_name_ = true;
  }
  void clear_name() {
    name_.clear();
    has_name_ = false;
  }

  // char data
  const std::string& get_text() const { return text_; }
  bool has_text() const { return has_text_; }
  void set_text(const std::string& value) {
    text_ = value;
    has_text_ = true;
  }
  void clear_text() {
    text_.clear();
    has_text_ = false;
  }

 private:
  friend class KmlFactory;
  SimpleData();
  friend class KmlHandler;
  virtual void ParseAttributes(const Attributes& attributes);
  virtual void AddElement(const ElementPtr& child);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string name_;
  bool has_name_;
  std::string text_;
  bool has_text_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(SimpleData);
};

// <SchemaData>
class SchemaData : public Object {
 public:
  virtual ~SchemaData();
  virtual KmlDomType Type() const { return Type_SchemaData; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_SchemaData || Object::IsA(type);
  }

  // schemaUrl=
  const std::string& get_schemaurl() const { return schemaurl_; }
  bool has_schemaurl() const { return has_schemaurl_; }
  void set_schemaurl(const std::string& value) {
    schemaurl_ = value;
    has_schemaurl_ = true;
  }
  void clear_schemaurl() {
    schemaurl_.clear();
    has_schemaurl_ = false;
  }

  void add_simpledata(const SimpleDataPtr& simpledata) {
    AddComplexChild(simpledata, &simpledata_array_);
  }

  const size_t get_simpledata_array_size() const {
    return simpledata_array_.size();
  }

  const SimpleDataPtr& get_simpledata_array_at(size_t index) const {
    return simpledata_array_[index];
  }

 private:
  friend class KmlFactory;
  SchemaData();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  virtual void ParseAttributes(const Attributes& attributes);
  friend class ExtendedData;
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string schemaurl_;
  bool has_schemaurl_;
  std::vector<SimpleDataPtr> simpledata_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(SchemaData);
};

// <Data>
class Data : public Object {
 public:
  virtual ~Data();
  virtual KmlDomType Type() const { return Type_Data; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Data || Object::IsA(type);
  }

  // name=
  const std::string& get_name() const { return name_; }
  bool has_name() const { return has_name_; }
  void set_name(const std::string& value) {
    name_ = value;
    has_name_ = true;
  }
  void clear_name() {
    name_.clear();
    has_name_ = false;
  }

  // <displayname>
  const std::string& get_displayname() const { return displayname_; }
  bool has_displayname() const { return has_displayname_; }
  void set_displayname(const std::string& value) {
    displayname_ = value;
    has_displayname_ = true;
  }
  void clear_displayname() {
    displayname_.clear();
    has_displayname_ = false;
  }

  // <value>
  const std::string& get_value() const { return value_; }
  bool has_value() const { return has_value_; }
  void set_value(const std::string& value) {
    value_ = value;
    has_value_ = true;
  }
  void clear_value() {
    value_.clear();
    has_value_ = false;
  }

 private:
  friend class KmlFactory;
  Data();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  virtual void ParseAttributes(const Attributes& attributes);
  friend class ExtendedData;
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string name_;
  bool has_name_;
  std::string displayname_;
  bool has_displayname_;
  std::string value_;
  bool has_value_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Data);
};

// <ExtendedData>
class ExtendedData : public Element {
 public:
  virtual ~ExtendedData();
  virtual KmlDomType Type() const { return Type_ExtendedData; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_ExtendedData;
  }

  // <Data>.
  void add_data(const DataPtr& data) {
    AddComplexChild(data, &data_array_);
  }

  const size_t get_data_array_size() const {
    return data_array_.size();
  }

  const DataPtr& get_data_array_at(size_t index) const {
    return data_array_[index];
  }

  // <SchemaData>.
  void add_schemadata(const SchemaDataPtr& schemadata) {
    AddComplexChild(schemadata, &schemadata_array_);
  }

  const size_t get_schemadata_array_size() const {
    return schemadata_array_.size();
  }

  const SchemaDataPtr& get_schemadata_array_at(size_t index) const {
    return schemadata_array_[index];
  }

 private:
  friend class KmlFactory;
  ExtendedData();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<DataPtr> data_array_;
  std::vector<SchemaDataPtr> schemadata_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(ExtendedData);
};

}  // end namespace kmldom

#endif  // KML_DOM_EXTENDEDDATA_H__
