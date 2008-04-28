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

// This file contains the declaration and definition of the internal
// Attributes class.

#ifndef KML_DOM_ATTRIBUTES_H__
#define KML_DOM_ATTRIBUTES_H__

#include <stdlib.h>
#include <map>
#include <sstream>
#include <string>
#include "kml/util/util.h"

namespace kmldom {

class Attributes {
 public:
  // Construct the Attributes instance from a list of name-value pairs
  // as is used in expat's startElement.
  Attributes(const char** attrs) {
   while (*attrs) {
     const char* attr_name = *attrs++;
     const char* attr_val = *attrs++;
     attributes_[attr_name] = attr_val;
   }
  }

  // Construct the Attributes instance with no initial name-value pairs.
  Attributes() {}

  // Get the value of the given attribute as a string.  Returns true if an
  // attribute with this name exits.  If no attribute by this name exists
  // false is returned and the string is untouched.  If no result string
  // pointer is supplied false is returned.
  bool GetString(const std::string attr_name, std::string* attr_val) const {
    if (!attr_val) {
      return false;
    }
    attributes_map_t::const_iterator entry = attributes_.find(attr_name);
    if (entry == attributes_.end()) {
      return false;
    }
    *attr_val = entry->second;
    return true;
  }

  // Set the value of the given attribute.  Any previous value for this
  // attribute is overwritten.
  void SetString(const std::string attr_name, const std::string attr_val) {
    attributes_[attr_name] = attr_val;
  }

  // Get the value of the given attribute as a double.  If the attribute value
  // is not a valid double the output is 0.  This returns true if an attribute
  // with this name exists.  If no attribute by this name exists false is
  // returned and the result pointer is untouched.  If no result pointer is
  // supplied false is returned.
  bool GetDouble(const std::string attr_name, double* attr_val) const {
    if (!attr_val) {
      return false;
    }
    std::string string_value;
    if (GetString(attr_name, &string_value)) {
      *attr_val = strtod(string_value.c_str(), NULL);
      return true;
    }
    return false;
  }

  // Set the value of the given attribute from a double.  Any previous value
  // for this attribute is overwritten.
  void SetDouble(const std::string attr_name, double attr_val) {
    std::stringstream ss;
    ss.precision(15);
    ss << attr_val;
    SetString(attr_name, ss.str());
  }

  // Serialize the current state of the Attributes instance into the
  // passed string.  This appends to any content previously in the string.
  // If no string pointer is supplied this method does nothing.
  void Serialize(std::string* output) const {
    if (!output) {
      return;
    }
    attributes_map_t::const_iterator entry;
    for (entry = attributes_.begin(); entry != attributes_.end(); ++entry) {
      *output += " ";
      *output += entry->first;
      *output += "=\"";
      *output += entry->second;
      *output += "\"";
    }
  }

  // Creates an exact copy of the Attributes object. Called by
  // Element::ParseAttributes().
  Attributes* Clone() const {
    Attributes* attributes = new Attributes();
    for (attributes_map_t::const_iterator itr = attributes_.begin();
         itr != attributes_.end(); ++itr) {
      attributes->SetString(itr->first, itr->second);
    }
    return attributes;
  }

  // This sets each attribute from the passed Attributes instance.
  // Any conflicting attributes are overridden from the input.
  void MergeAttributes(const Attributes& attrs) {
    for (attributes_map_t::const_iterator itr = attrs.attributes_.begin();
         itr != attrs.attributes_.end(); ++itr) {
      attributes_[itr->first] = itr->second;
    }
  }

 private:
  // XML attributes have no order and are unique.  The attribute name is
  // preserved to properly save unknown attributes.
  typedef std::map<std::string,std::string> attributes_map_t;
  attributes_map_t attributes_;
  DISALLOW_EVIL_CONSTRUCTORS(Attributes);
};

}  // end namespace kmldom

#endif  // KML_DOM_ATTRIBUTES_H__
