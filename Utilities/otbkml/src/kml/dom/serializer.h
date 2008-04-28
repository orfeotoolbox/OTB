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

// This file contains the declaration of the internal Serializer class.

#ifndef KML_DOM_SERIALIZER_H__
#define KML_DOM_SERIALIZER_H__

#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include "kml/dom/xsd.h"

namespace kmldom {

class Attributes;
class Element;
class Xsd;

template<typename T>
inline std::string ToString(T value) {
  std::stringstream ss;
  ss.precision(15);
  ss << value;
  return ss.str();
}

// The Serializer class is internal to the KML DOM and is used by each
// Element to save its tag name, fields (attributes and simple elements),
// character data content and/or complex child elements.
class Serializer {
 public:
  // Construct a serializer with the given strings for line breaks and
  // indentation.  The indent string is used once for each level of
  // indentation.  For no line break and/or indent whitespace use "".
  Serializer(const char* newline, const char* indent);

  // Emit the start tag of the given element: <Placemark id="pm123">.
  void BeginById(int type_id, const Attributes& attributes);
  //
  // Emit the end tag of the given element: </Placemark>.
  void End();

  // Handles the case of a complex element with character data. Used by
  // <Snippet> and <SimpleData>.
  void SaveComplexStringFieldByName(std::string tag_name,
                                    const Attributes& attributes,
                                    std::string value);

  // Save the given value out as the simple element identified by type_id.
  template<typename T>
  void SaveFieldById(int type_id, T value) {
    SaveStringFieldByName(Xsd::GetSchema()->ElementName(type_id),
                          ToString(value));
  }

  // Save the given value out as the enum element identified by type_id.
  void SaveEnum(int type_id, int enum_value);

  // Save the given complex element.
  void SaveElement(const Element& element);

  // Save out raw text.
  void SaveContent(std::string content);

  void Indent();

  // Write the state of this serializer to the given string.
  void WriteString(std::string* output);

 private:
  void BeginByName(const std::string& tag_name, const Attributes& attributes);
  void SaveStringFieldByName(std::string tag_name, std::string value);
  Xsd* xsd_;
  const char* newline_;
  const char* indent_;
  std::vector<std::string> xml_;
  std::stack<std::string> tag_stack_;
  const std::string MaybeQuoteString(const std::string& value);
};

}  // end namespace kmldom

#endif  // KML_DOM_SERIALIZER_H__
