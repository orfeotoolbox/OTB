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

// This file contains the implementation of the internal ExpatParser class.

#include "kml/base/expat_parser.h"
#include <cstring>  // For memcpy.
#include <sstream>
#include "kml/base/expat_handler.h"

namespace kmlbase {

static void XMLCALL
startElement(void *userData, const XML_Char *name, const XML_Char **atts) {
  string flatname = xml_char_to_string(name);
  // TODO: kmlbase::Attributes would be a more appropriate type here.
  StringVector flatatts;
  xml_char_to_string_vec(atts, &flatatts);
  static_cast<ExpatHandler*>(userData)->StartElement(flatname, flatatts);
}

static void XMLCALL
endElement(void *userData, const XML_Char *name) {
  string flatname = xml_char_to_string(name);
  static_cast<ExpatHandler*>(userData)->EndElement(flatname);
}

static void XMLCALL
charData(void *userData, const XML_Char *s, int length) {
  string flatdata = xml_char_to_string_n(s, length);;
  static_cast<ExpatHandler*>(userData)->CharData(flatdata);
}

static void XMLCALL
startNamespace(void *userData, const XML_Char *prefix, const XML_Char *uri) {
  string flatprefix = xml_char_to_string(prefix);
  string flaturi = xml_char_to_string(uri);
  static_cast<ExpatHandler*>(userData)->StartNamespace(flatprefix, flaturi);
}

static void XMLCALL
endNamespace(void *userData, const XML_Char *prefix) {
  string flatprefix = xml_char_to_string(prefix);
  static_cast<ExpatHandler*>(userData)->EndNamespace(flatprefix);
}

static void XMLCALL
entityDeclHandler(void *userData, const XML_Char *entityName,
                  int is_parameter_entity, const XML_Char *value,
                  int value_length, const XML_Char *base,
                  const XML_Char *systemId, const XML_Char *publicId,
                  const XML_Char *notationName) {
  XML_Parser parser = static_cast<ExpatHandler*>(userData)->get_parser();
  XML_StopParser(parser, XML_FALSE);
}

ExpatParser::ExpatParser(ExpatHandler* handler, bool namespace_aware)
  : expat_handler_(handler) {
  XML_Parser parser =
    namespace_aware ? XML_ParserCreateNS(NULL, kExpatNsSeparator)
                    : XML_ParserCreate(NULL);
  expat_handler_->set_parser(parser);
  XML_SetUserData(parser, expat_handler_);
  XML_SetElementHandler(parser, startElement, endElement);
  XML_SetCharacterDataHandler(parser, charData);
  XML_SetEntityDeclHandler(parser, entityDeclHandler);
  if (namespace_aware) {
    XML_SetNamespaceDeclHandler(parser, startNamespace, endNamespace);
  }
  parser_ = parser;
}

ExpatParser::~ExpatParser() {
  XML_ParserFree(parser_);
}

// Static.
bool ExpatParser::ParseString(const string& xml, ExpatHandler* handler,
                              string* errors, bool namespace_aware) {
  ExpatParser parser(handler, namespace_aware);
  return parser._ParseString(xml, errors);
}

void* ExpatParser::GetInternalBuffer(size_t len) {
  return static_cast<void*>(XML_GetBuffer(parser_, static_cast<int>(len)));
}

bool ExpatParser::ParseBuffer(const string& input, string* errors,
                              bool is_final) {
  void* buf = GetInternalBuffer(input.size());
  if (!buf) {
    if (errors) {
      *errors = "could not allocate memory";
    }
    return false;
  }
  memcpy(buf, input.data(), input.size());
  return ParseInternalBuffer(input.size(), errors, is_final);
}

bool ExpatParser::ParseInternalBuffer(size_t len, string* errors,
                                      bool is_final) {
  XML_Status status = XML_ParseBuffer(parser_, static_cast<int>(len), is_final);
  // If we have just parsed the final buffer, we need to check if Expat
  // has stopped parsing. Failure here indicates invalid (badly formed)
  // XML content.
  if (is_final) {
    XML_ParsingStatus parsing_status;
    XML_GetParsingStatus(parser_, &parsing_status);
    if (parsing_status.parsing != XML_FINISHED) {
      ReportError(parser_, errors);
      return false;
    }
  }
  if (status != XML_STATUS_OK) {
    ReportError(parser_, errors);
  }
  return status == XML_STATUS_OK;
}

// Private.
bool ExpatParser::_ParseString(const string& xml, string* errors) {
  int xml_size = static_cast<int>(xml.size());
  XML_Status status = XML_Parse(parser_, xml.c_str(), xml_size, xml_size);
  if (status != XML_STATUS_OK && errors) {
    // This is the other half of XML_StopParser() which is our way of
    // stopping expat if the root element is not KML.
    if (status == XML_STATUS_SUSPENDED) {
      *errors = "Invalid root element";
    } else {
      ReportError(parser_, errors);
    }
  }
  return status == XML_STATUS_OK;
}

// Private.
void ExpatParser::ReportError(XML_Parser parser, string* errors) {
  if (!errors) {
    return;
  }
  std::stringstream strstream;
  strstream << XML_ErrorString(XML_GetErrorCode(parser));
  strstream << " on line ";
  strstream << XML_GetCurrentLineNumber(parser);
  strstream << " at offset ";
  strstream << XML_GetCurrentColumnNumber(parser);
  *errors = strstream.str();
}

}  // end namespace kmlbase
