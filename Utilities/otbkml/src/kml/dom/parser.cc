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

// This file maps the C-based expat to a general purpose C++ ExpatHandler class.
// KmlHandler is the only such handler at present, hence the static nature
// of the ExpatParser() function.

#include "kml/dom/kml_funcs.h"
#include <cstring>
#include <sstream>
#include <string>
#include "expat.h"
#include "kml/dom/expat_handler.h"
#include "kml/dom/kml_handler.h"

namespace kmldom {

class Element;

static void XMLCALL
startElement(void *userData, const char *name, const char **atts) {
  ((ExpatHandler*)userData)->StartElement(name, atts);
}

static void XMLCALL
endElement(void *userData, const char *name) {
  ((ExpatHandler*)userData)->EndElement(name);
}

// startCdata and endCdata insert the <![CDATA[  ]]> tags, stripped by expat
// during its parse. The intent is that we exactly preserve the author's
// markup and avoid superfluous entity escaping.
static void XMLCALL
startCdata(void *userData) {
  const char* s = "<![CDATA[";
  ((ExpatHandler*)userData)->CharData(s, strlen(s));
}

static void XMLCALL
endCdata(void *userData) {
  const char* s = "]]>";
  ((ExpatHandler*)userData)->CharData(s, strlen(s));
}

static void XMLCALL
charData(void *userData, const XML_Char *s, int len) {
  ((ExpatHandler*)userData)->CharData(s, len);
}

static bool ExpatParser(const std::string& xml, ExpatHandler* expat_handler,
                        std::string* errors) {
  XML_Parser parser = XML_ParserCreate(NULL);
  expat_handler->set_parser(parser);
  XML_SetUserData(parser, expat_handler);
  XML_SetElementHandler(parser, startElement, endElement);
  XML_SetCdataSectionHandler(parser, startCdata, endCdata);
  XML_SetCharacterDataHandler(parser, charData);
  XML_Status status = XML_Parse(parser, xml.c_str(), xml.size(), xml.size());
  if (status != XML_STATUS_OK && errors) {
    // This is the other half of XML_StopParser() which is our way of
    // stopping expat if the root element is not KML.
    if (status == XML_STATUS_SUSPENDED) {
      *errors = "Invalid root element";
    } else {
      std::stringstream strstream;
      strstream << XML_ErrorString(XML_GetErrorCode(parser));
      strstream << " on line ";
      strstream << XML_GetCurrentLineNumber(parser);
      strstream << " at offset ";
      strstream << XML_GetCurrentColumnNumber(parser);
      *errors = strstream.str();
    }
  }
  XML_ParserFree(parser);
  return status == XML_STATUS_OK;
}

// This is the implementation of the public API to parse KML from a memory
// buffer.
Element* Parse(const std::string& kml, std::string* errors) {
  KmlHandler kml_handler;
  bool status = ExpatParser(kml, &kml_handler, errors);
  if (status) {
    return kml_handler.PopRoot();
  }
  return NULL;
}

// Parse the KML in the given string.  NULL is returned on any parse errors,
// but the error string is unavailable with this function.
Element* ParseKml(const std::string& kml) {
  std::string ignored;
  return Parse(kml, &ignored);
}

} // end namespace kmldom
