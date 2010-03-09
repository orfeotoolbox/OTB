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
#include "kml/base/attributes.h"
#include "kml/base/expat_parser.h"
#include "kml/base/expat_handler_ns.h"
#include "kml/base/xmlns.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_handler.h"
#include "kml/dom/kml_handler_ns.h"
#include "kml/dom/parser.h"
#include "kml/dom/parser_observer.h"

namespace kmldom {

// This is an internal API to add Element-level SAX-style handlers to
// a given parser instance.  TODO: determine how/if to make public and SWIG.
void Parser::AddObserver(ParserObserver* parser_observer) {
  observers_.push_back(parser_observer);
}

// This is the internal API to the parser.  TODO: determine how/if to make
// public and SWIG.
ElementPtr Parser::Parse(const string& kml, string* errors) {
  KmlHandler kml_handler(observers_);
  kmlbase::ExpatParser parser(&kml_handler, false);
  if (kmlbase::ExpatParser::ParseString(kml, &kml_handler, errors, false)) {
    return kml_handler.PopRoot();
  }
  return NULL;
}


// As Parser::Parse(), but invokes the underlying XML parser's namespace-aware
// mode.
ElementPtr Parser::ParseNS(const string& kml, string* errors) {
  KmlHandlerNS kml_handler(observers_);
  if (kmlbase::ExpatParser::ParseString(kml, &kml_handler, errors, true)) {
    return kml_handler.PopRoot();
  }
  return NULL;
}

// This is obviously a bit of a special case.  If libkml always used full
// namespace-aware parsing we'd not need this.
ElementPtr Parser::ParseAtom(const string& atom, string* errors) {
  // Create a garden variety KML parser with "short-hand" namespace prefixes
  // for Atom.
  KmlHandler kml_handler(observers_);
  kmlbase::Attributes attributes;
  // Create a namespace aware expat handler which converts the Atom namespace
  // elements to the "short-hand" namespace prefixing used in KmlHandler.
  // Here's the overall flow:
  // 1) instance file has <feed xmlns="http://www.w3.org/2005/Atom">...
  // 2) namespace-enabled expat turns this into:
  //    <http://www.w3.org/2005/Atom|feed>
  // 3) ExpatHandlerNns turns this to <atom:feed>
  // 4) KmlHandler knows that <atom:feed> is kmldom::AtomFeed
  attributes.SetValue("xmlns", "http://www.opengis.net/kml/2.2");
  attributes.SetValue("xmlns:atom", "http://www.w3.org/2005/Atom");
  boost::scoped_ptr<kmlbase::Xmlns> xmlns(kmlbase::Xmlns::Create(attributes));
  kmlbase::ExpatHandlerNs expat_handler_ns(&kml_handler, xmlns.get());
  if (kmlbase::ExpatParser::ParseString(atom, &expat_handler_ns, errors,
                                        true)) {
    return kml_handler.PopRoot();
  }
  return NULL;
}

// This is the implementation of the public API to parse KML from a memory
// buffer.
ElementPtr Parse(const string& kml, string* errors) {
  Parser parser;
  return parser.Parse(kml, errors);
}

// As Parse(), but invokes the underlying XML parser's namespace-aware mode.
ElementPtr ParseNS(const string& kml, string* errors) {
  Parser parser;
  return parser.ParseNS(kml, errors);
}

// Parse the KML in the given string.  NULL is returned on any parse errors,
// but the error string is unavailable with this function.
ElementPtr ParseKml(const string& kml) {
  return Parse(kml, NULL);
}

ElementPtr ParseAtom(const string& atom, string* errors) {
  Parser parser;
  return parser.ParseAtom(atom, errors);
}

} // end namespace kmldom
