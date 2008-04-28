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

// This file contains the declaration of the Element and Field classes.
// The Element class is the base class for the KML Object Model.  All
// KML complex elements are derived from this class.  As the Element class
// members wnd methods indicate an Element always has a given type id, and
// a single parent Element.  Element itself holds all unknown XML for a given
// KML element including completely unknown XML, misplaced KML,
// and unknown attributes.  During parse a simple element is held for
// a short time in the Field specialization of Element.

#ifndef KML_DOM_ELEMENT_H__
#define KML_DOM_ELEMENT_H__

#include <string>
#include <vector>
#include "kml/dom/kml22.h"
#include "kml/util/util.h"

namespace kmldom {

class Attributes;
class Serializer;
class Xsd;

class Element {
 public:
  virtual ~Element();
  virtual KmlDomType Type() const { return type_id_; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Unknown;
  }

  // This sets this element's parent element.  Returns false if the element
  // already has a parent.  Returns true if the parent is set successfully.
  // To directly mirror XML each element has exactly one parent.
  bool set_parent(Element* parent) {
    if (parent_ || (parent == this)) {
      return false;
    }
    parent_ = parent;
    return true;
  }

  // This is the concatenation of all character data found parsing this element.
  const std::string& char_data() const {
    return char_data_;
  }
  void set_char_data(const std::string& char_data) {
    char_data_ = char_data;
  }

  // A derived class implements this to use with parsing.  Element is
  // either a complex or simple element which the given concrete element
  // can accept.  If the given element is a valid child the concrete element
  // takes ownership.  The given element is attached to the concrete parent
  // if it is a valid complex child.  If the element is a simple element
  // the character data is converted to the appropropriate simple type
  // and the passed element is discarded.  If the passed element is not
  // a valid child of the given concrete element the AddElement method
  // there should pass this up to its parent for consideration.  A misplaced
  // element will ultimately be attached to Element itself.
  virtual void AddElement(Element* element);

  // A derived class implements this to use with parsing.  A given concrete
  // element examines the passed attributes for any it is aware of and
  // passes the attributes to its parent class and ultimately to Element
  // itself to preserve unknown attributes.
  virtual void ParseAttributes(const Attributes& attributes);

  // A derived class implements this to use with serialization.  See
  // class Serializer for more information.
  virtual void Serialize(Serializer& serialize) const {}

  // A derived class uses this to use with serialization.  The derived
  // class adds its attributes to the given set and passes attributes
  // along to the parent and utlimately to Element itself to preserve
  // unknown attributes.
  virtual void GetAttributes(Attributes* attributes) const;

  // Each fully unknown element (and its children) is saved in raw XML form.
  void AddUnknownElement(const std::string& s);

  // Called by concrete elements to serialize unknown and/or misplaced
  // elements discovered at parse time.
  void SerializeUnknown(Serializer& serializer) const;

  // Returns the unknown elements.
  const std::vector<std::string>& unknown_elements_array() const {
    return unknown_elements_array_;
  }

  // Returns the unknown legal (misplaced) elements.
  const std::vector<Element*>& unknown_legal_elements_array() const {
    return unknown_legal_elements_array_;
  }

  // Permits polymorphic use of Field methods.
  virtual bool SetBool(bool* val) { delete this; return false; }
  virtual bool SetDouble(double* val) { delete this; return false; }
  virtual bool SetInt(int* val) { delete this; return false; }
  virtual bool SetEnum(int* val) { delete this; return false; }
  virtual bool SetString(std::string* val) { delete this; return false; }

 protected:
  // Element is an abstract base class and is never created directly.
  Element();
  Element(KmlDomType type_id);

 private:
  Element* parent_;
  KmlDomType type_id_;
  std::string char_data_;
  // A vector of strings to contain unknown non-KML elements discovered during
  // parse.
  std::vector<std::string> unknown_elements_array_;
  // A vector of Element*'s to contain known KML elements found during parse
  // to be in illegal positions, e.g. <Placemark><Document>.
  std::vector<Element*> unknown_legal_elements_array_;
  // Unknown attributes found during parse are copied out and a pointer is
  // stored. The object is dynamically allocated so every element is not
  // burdened with an unnecessary Attributes object.
  Attributes* unknown_attributes_;
  DISALLOW_EVIL_CONSTRUCTORS(Element);
};

class Field : public Element {
 public:
  Field(KmlDomType type_id);

  // Sets the given bool from the character data.  If no val pointer is
  // supplied false is returned, else true is returned and the val is set.
  // Element is ALWAYS DELETED.
  bool SetBool(bool* val);

  // Sets the given double from the character data.  If no val pointer is
  // supplied false is returned, else true is returned and the val is set.
  // Element is ALWAYS DELETED.
  bool SetDouble(double *val);

  // Sets the given int from the character data.  If no val pointer is
  // supplied false is returned, else true is returned and the val is set.
  // Element is ALWAYS DELETED.
  bool SetInt(int* val);

  // Sets the given enum from the character data.  If no val pointer is
  // supplied false is returned, else true is returned and the val is set.
  // Element is ALWAYS DELETED.
  bool SetEnum(int* enum_val);

  bool SetString(std::string* val);

 private:
  Xsd* xsd_;
  DISALLOW_EVIL_CONSTRUCTORS(Field);
};

}  // namespace kmldom

#endif  // KML_DOM_ELEMENT_H__
