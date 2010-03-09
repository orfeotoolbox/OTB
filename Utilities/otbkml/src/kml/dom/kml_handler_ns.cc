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

// This file implements the KML ParseNS() function.  The parser uses expat in
// its namespace-aware mode.  See kml_handler.cc for more details.

#include "kml/dom/kml_handler_ns.h"
#include "kml/dom/parser.h"  // for kXmlnsSeparator.
#include <string.h>  // For strchr().

namespace kmldom {

// The uri:local_name separator used in expat's ParserCreateNS.
// TODO: pull this from a common header with the separator in parse.h.
static const char kXmlnsSeparator = '|';

KmlHandlerNS::KmlHandlerNS(parser_observer_vector_t& observers)
  : KmlHandler(observers) {
}

KmlHandlerNS::~KmlHandlerNS() {
}

void KmlHandlerNS::StartElement(const string& name,
                                const kmlbase::StringVector& atts) {
  // Expat guarantees that there will be delimited uri-name string here.

  size_t token = name.find_last_of(kXmlnsSeparator) + 1;
  KmlHandler::StartElement(name.substr(token), atts);
}

void KmlHandlerNS::EndElement(const string& name) {
  // Expat guarantees that there will be delimited uri-name string here.

  size_t token = name.find_last_of(kXmlnsSeparator) + 1;
  KmlHandler::EndElement(name.substr(token));
}

void KmlHandlerNS::CharData(const string& s) {
  KmlHandler::CharData(s);
}

void KmlHandlerNS::StartNamespace(const string& prefix,
                                  const string& uri) {
  // TODO: save the uri:prefix mappings.
  // TODO: enforce the rule that the ONLY default namespace is KML's?
  // TODO: enforce that there must be a 1:1 uri:prefix mapping per file?
}

void KmlHandlerNS::EndNamespace(const string& prefix) {
}

}  // end namespace kmldom
