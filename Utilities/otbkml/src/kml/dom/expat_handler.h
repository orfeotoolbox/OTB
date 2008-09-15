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

#ifndef KML_PARSER_EXPAT_HANDLER_H__
#define KML_PARSER_EXPAT_HANDLER_H__

// OTB Modifications
//#include "expat.h" // XML_Char
#include "otb_expat.h" // XML_Char

namespace kmldom {

// This declares the pure virtual ExpatHandler interface.  The ExpatParser()
// function takes an implementation class with this interface.
class ExpatHandler {
public:
  virtual ~ExpatHandler() {}
  virtual void StartElement(const char *name, const char **atts) = 0;
  virtual void EndElement(const char *name) = 0;
  virtual void CharData(const XML_Char *s, int len) = 0;

  void set_parser(XML_Parser parser) {
    parser_ = parser;
  }
  XML_Parser get_parser() {
    return parser_;
  }

private:
  XML_Parser parser_;
};

}  // end namespace kmldom

#endif  // KML_PARSER_EXPAT_HANDLER_H__
