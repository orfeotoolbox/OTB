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

// This file contains the implementation of the Attributes class.

#include "kml/base/attributes.h"
#include <map>
#include <vector>

namespace kmlbase {

// static
Attributes* Attributes::Create(const char** attrs) {
  Attributes* attributes = new Attributes;
  if (attributes->Parse(attrs)) {
    return attributes;
  }
  delete attributes;
  return NULL;
}

Attributes* Attributes::Create(const kmlbase::StringVector& attrs) {
  Attributes* attributes = new Attributes;
  if (attributes->Parse(attrs)) {
    return attributes;
  }
  delete attributes;
  return NULL;
}

// private
bool Attributes::Parse(const char** attrs) {
  while (*attrs && *(attrs+1)) {  // Quietly ignore unpaired last item.
    const char* attr_name = *attrs++;
    const char* attr_val = *attrs++;
    attributes_map_[attr_name] = attr_val;
  }
  return true;
}

bool Attributes::Parse(const kmlbase::StringVector& attrs) {
  for (unsigned int i = 0; i < attrs.size() ; i += 2) {
    if (attrs.size() - i < 1)
      break;
    string attr_name = attrs.at(i);
    string attr_val = attrs.at(i+1);
    attributes_map_[attr_name] = attr_val;
  }
  return true;
}

void Attributes::Serialize(string* output) const {
  if (output) {
    StringMapIterator iter = CreateIterator();
    for (; !iter.AtEnd(); iter.Advance()) {
      *output += " ";
      *output += iter.Data().first;
      *output += "=\"";
      *output += iter.Data().second;
      *output += "\"";
    }
  }
}

Attributes* Attributes::Clone() const { 
  Attributes* clone = new Attributes();
  clone->attributes_map_ = attributes_map_;
  return clone;
}

void Attributes::MergeAttributes(const Attributes& input) {
  StringMapIterator iter = input.CreateIterator();
  for (; !iter.AtEnd(); iter.Advance()) {
    attributes_map_[iter.Data().first] = iter.Data().second;
  }
}

bool Attributes::FindValue(const string& key, string* value) const {
  StringMap::const_iterator entry = attributes_map_.find(key); 
  if (entry != attributes_map_.end()) {
    if (value) {
      *value = entry->second;
    } 
    return true;
  } 
  return false;
} 

bool Attributes::FindKey(const string& value, string* key) const {
  StringMapIterator iter = CreateIterator();
  for (; !iter.AtEnd(); iter.Advance()) {
    if (value == iter.Data().second) {
      if (key) {
        *key = iter.Data().first;
      }
      return true;
    }
  }
  return false;
}

void Attributes::GetAttrNames(std::vector<string>* string_vector) const {
  if (string_vector) {
    StringMapIterator iter = CreateIterator();
    for (; !iter.AtEnd(); iter.Advance()) {
      string_vector->push_back(iter.Data().first);
    }
  }
}

Attributes* Attributes::SplitByPrefix(const string& prefix) {
  size_t prefix_size = prefix.size() + 1;  // +1 for the ":"
  Attributes* split = new Attributes();
  std::vector<string> keys_to_erase;
  for (StringMapIterator iter = CreateIterator();
       !iter.AtEnd(); iter.Advance()) {
    const string& key = iter.Data().first;
    if (key.compare(0, prefix_size, prefix + ":") == 0) {
      split->SetValue(key.substr(prefix_size), iter.Data().second);
      // Can't erase() while iterating so save the key.
      keys_to_erase.push_back(key);
    }
  }
  // Nothing was split out so just return now.
  if (split->attributes_map_.empty()) {
    delete split;
    return NULL;
  }
  // Go back and remove all entries added to the split map.
  for (size_t i = 0; i < keys_to_erase.size(); ++i) {
    attributes_map_.erase(keys_to_erase[i]);
  }
  return split;
}

}  // end namespace kmlbase
