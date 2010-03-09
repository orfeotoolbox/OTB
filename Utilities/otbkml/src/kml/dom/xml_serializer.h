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

// This file contains the declaration of the internal XmlSerializer class.

#ifndef KML_DOM_XML_SERIALIZER_H__
#define KML_DOM_XML_SERIALIZER_H__

#include <stack>
#include <vector>
#include "kml/dom/serializer.h"
#include "kml/dom.h"

namespace kmlbase {
class Attributes;
}

namespace kmldom {

// The XmlSerializer class is internal to the KML DOM and is used by each
// Element to save its tag name, fields (attributes and simple elements),
// character data content and/or complex child elements.
class XmlSerializer : public Serializer {
 public:
  // Construct a serializer with the given strings for line breaks and
  // indentation.  The indent string is used once for each level of
  // indentation.  For no line break and/or indent whitespace use "".
  XmlSerializer(const char* newline, const char* indent);

  virtual ~XmlSerializer() {}

  // Emit the start tag of the given element: <Placemark id="pm123">.
  virtual void BeginById(int type_id, const kmlbase::Attributes& attributes);

  // Emit the end tag of the given element: </Placemark>.
  virtual void End();

  // Emit the XML for the field of the given type with the given content
  // as its character data.  If value is empty a nil element is emitted.
  virtual void SaveStringFieldById(int type_id, string value);

  // Save out character data.
  virtual void SaveContent(const string& content, bool maybe_quote);

  // Save a Color32 value as its AABBGGRR representation.
  virtual void SaveColor(int type_id, const kmlbase::Color32& color);

  // Emit one level of indentation.
  virtual void Indent();

  // Write the state of this serializer to the given string.
  void WriteString(string* output);

 private:
  const char* newline_;
  const char* indent_;
  std::vector<string> xml_;
  std::stack<string> tag_stack_;
};

}  // end namespace kmldom

#endif  // KML_DOM_XML_SERIALIZER_H__
