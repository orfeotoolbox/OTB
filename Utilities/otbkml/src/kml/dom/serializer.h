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
#include <string>
#include "kml/dom/kml_ptr.h"

namespace kmldom {

class Attributes;
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

  Serializer();

  virtual ~Serializer() {}

  // Emit the start tag of the given element: <Placemark id="pm123">.
  virtual void BeginById(int type_id, const Attributes& attributes) {};

  // Emit the end tag of the given element: </Placemark>.
  virtual void End() {};

  // Emit a complex element.
  virtual void SaveElement(const ElementPtr& element);

  // Emit a simple element.
  virtual void SaveStringFieldById(int type_id, std::string value) {}

  // Save out raw text.  If maybe_quote is true the content is examined
  // for non-XML-valid characters and if so the content is CDATA escaped.
  // If maybe_quote is false the content is emitted directly.
  virtual void SaveContent(const std::string& content, bool maybe_quote) {};

  // Emit indent.
  virtual void Indent() {}

  // If value contains any non-XML valid characters a CDATA-escaped
  // string is returned, else the original string is returned.
  const std::string MaybeQuoteString(const std::string& value);

  // Save the given value out as the enum element identified by type_id.
  void SaveEnum(int type_id, int enum_value);

  // Save the given value out as the simple element identified by type_id.
  template<typename T>
  void SaveFieldById(int type_id, T value) {
    SaveStringFieldById(type_id, ToString(value));
  }

 protected:
   const Xsd& xsd_;
};

}  // end namespace kmldom

#endif  // KML_DOM_SERIALIZER_H__
