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

#include "kml/engine/get_links.h"
#include "kml/dom/parser.h"
#include "kml/dom/parser_observer.h"
// TODO: deprecate use of kmlengine::Href. kml_url.h and/or kmlbase::UriParser
// should be used instead.
#include "kml/engine/href.h"

using kmldom::Parser;

namespace kmlengine {

bool GetLinks(const string& kml, href_vector_t* href_vector) {
  if (!href_vector) {
    return false;
  }
  GetLinksParserObserver get_links(href_vector);
  Parser parser;
  parser.AddObserver(&get_links);
  return parser.Parse(kml, NULL) != NULL;
}

bool GetRelativeLinks(const string& kml, href_vector_t* href_vector) {
  if (!href_vector) {
    return false;
  }
  href_vector_t all_hrefs;
  if (!GetLinks(kml, &all_hrefs)) {
    return false;
  }

  href_vector_t::const_iterator itr;
  for (itr = all_hrefs.begin(); itr != all_hrefs.end(); ++ itr) {
    Href href(*itr);
    if (href.IsRelativePath()) {
      href_vector->push_back(*itr);
    }
  }
  return true;
}

}  // end namespace kmlengine
