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

// This file declares the C++ ExpatHandler interface to the otherwise C expat.

#ifndef KML_BASE_EXPAT_HANDLER_H__
#define KML_BASE_EXPAT_HANDLER_H__

#include "expat.h" // XML_Char
#include "string_util.h"  // StringVector

namespace kmlbase {
class Attributes;
// This declares the pure virtual ExpatHandler interface.
class ExpatHandler {
public:
  virtual ~ExpatHandler() {}
  virtual void StartElement(const string& name,
                            const StringVector& atts) = 0;
  virtual void EndElement(const string& name) = 0;
  virtual void CharData(const string&) = 0;

  // Namespace handlers with an empty default implementation.
  virtual void StartNamespace(const string& prefix,
                              const string& uri) {}
  virtual void EndNamespace(const string& prefix) {}

  void set_parser(XML_Parser parser) {
    parser_ = parser;
  }
  XML_Parser get_parser() {
    return parser_;
  }

private:
  XML_Parser parser_;
};

}  // end namespace kmlbase

inline string xml_char_to_string(const XML_Char *input) {
  string output;

  // This is technically wrong, but only for cases uninteresting for KML as
  // we only ever have single-byte encodings.
  for (const XML_Char *p = input; input && *p; p++) {
    output.append((const char *)p, 1);
  }
  return output;
}

inline void xml_char_to_string_vec(const XML_Char **input,
                                   kmlbase::StringVector *ovec) {
  if (!ovec)
    return;
  while (input && *input) {
    ovec->push_back(xml_char_to_string(*input++));
    ovec->push_back(xml_char_to_string(*input++));
  }
}

inline string xml_char_to_string_n(const XML_Char *input, size_t length) {
  string output;
  while (length--) {
    output.append((const char *)input++, 1);
  }
  return output;
}

#endif  // KML_BASE_EXPAT_HANDLER_H__
