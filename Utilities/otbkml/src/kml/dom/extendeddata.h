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

#include <vector>
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"
#include "kml/util/util.h"

namespace kmldom {

class serializer;

// <SimpleData>
class SimpleData : public Element {
 public:
  virtual ~SimpleData();
  virtual KmlDomType Type() const { return Type_SimpleData; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_SimpleData;
  }

  // name=
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

  // char data
  const std::string& text() const { return text_; }
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
  virtual void AddElement(Element* child);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string name_;
  bool has_name_;
  std::string text_;
  bool has_text_;
  DISALLOW_EVIL_CONSTRUCTORS(SimpleData);
};

// ExtendedDataMember
// An internal class from which both Data and SchemaData derive.
// It is derived from Object such that Serialize etc. are passed on to
// inherited classes.
class ExtendedDataMember : public Object {
 public:
  virtual ~ExtendedDataMember();

 protected:
  ExtendedDataMember();

 private:
  DISALLOW_EVIL_CONSTRUCTORS(ExtendedDataMember);
};

// <SchemaData>
class SchemaData : public ExtendedDataMember {
 public:
  virtual ~SchemaData();
  virtual KmlDomType Type() const { return Type_SchemaData; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_SchemaData || Object::IsA(type);
  }

  // schemaUrl=
  const std::string& schemaurl() const { return schemaurl_; }
  bool has_schemaurl() const { return has_schemaurl_; }
  void set_schemaurl(const std::string& value) {
    schemaurl_ = value;
    has_schemaurl_ = true;
  }
  void clear_schemaurl() {
    schemaurl_.clear();
    has_schemaurl_ = false;
  }

  void add_simpledata(SimpleData* simpledata) {
    if (simpledata && simpledata->set_parent(this)) {
      simpledata_array_.push_back(simpledata);
    }
  }

  const size_t simpledata_array_size() const {
    return simpledata_array_.size();
  }

  const SimpleData* simpledata_array_at(unsigned int index) const {
    return simpledata_array_[index];
  }

 private:
  friend class KmlFactory;
  SchemaData();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  virtual void ParseAttributes(const Attributes& attributes);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string schemaurl_;
  bool has_schemaurl_;
  std::vector<SimpleData*> simpledata_array_;
  DISALLOW_EVIL_CONSTRUCTORS(SchemaData);
};

// <Data>
class Data : public ExtendedDataMember {
 public:
  virtual ~Data();
  virtual KmlDomType Type() const { return Type_Data; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Data || Object::IsA(type);
  }

  // name=
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

  // <displayname>
  const std::string& displayname() const { return displayname_; }
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
  const std::string& value() const { return value_; }
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
  virtual void AddElement(Element* element);
  virtual void ParseAttributes(const Attributes& attributes);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  virtual void GetAttributes(Attributes* attributes) const;
  std::string name_;
  bool has_name_;
  std::string displayname_;
  bool has_displayname_;
  std::string value_;
  bool has_value_;
  DISALLOW_EVIL_CONSTRUCTORS(Data);
};

// <ExtendedData>
class ExtendedData : public Element {
 public:
  virtual ~ExtendedData();
  virtual KmlDomType Type() const { return Type_ExtendedData; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_ExtendedData;
  }

  void add_extendeddatamember(ExtendedDataMember* extendeddatamember) {
    if (extendeddatamember && extendeddatamember->set_parent(this)) {
      extendeddatamember_array_.push_back(extendeddatamember);
    }
  }

  const size_t extendeddatamember_array_size() const {
    return extendeddatamember_array_.size();
  }

  const ExtendedDataMember* extendeddatamember_array_at(unsigned int index)
    const {
    return extendeddatamember_array_[index];
  }

 private:
  friend class KmlFactory;
  ExtendedData();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<ExtendedDataMember*> extendeddatamember_array_;
  DISALLOW_EVIL_CONSTRUCTORS(ExtendedData);
};

}  // namespace kmldom

#endif  // KML_DOM_EXTENDEDDATA_H__
