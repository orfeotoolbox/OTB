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

#include "kml/base/expat_handler_ns.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/xmlns.h"
#include "kml/base/expat_parser.h"

namespace kmlbase {

ExpatHandlerNs::ExpatHandlerNs(ExpatHandler* expat_handler, const Xmlns* xmlns)
  : expat_handler_(expat_handler), xmlns_(xmlns) {
}

const string ExpatHandlerNs::TranslatePrefixedName(
    const string prefixed_name) const {
  string::size_type sep = prefixed_name.find(kExpatNsSeparator);
  // Name has no separator?  Just return it.
  if (sep == string::npos) {  // Not expected to happen
    return prefixed_name;
  }
  // Name is in the default namespace?  Return without the prefix.
  if (xmlns_->get_default() == prefixed_name.substr(0, sep)) {
    return prefixed_name.substr(sep+1);
  }
  // Name is in some other namespace?  Map to the prefix known to the
  // expat_handler as indicated in the Xmlns we were constructed with.
  string prefix = xmlns_->GetKey(prefixed_name.substr(0, sep));
  if (!prefix.empty()) {
    return prefix + ":" + prefixed_name.substr(sep+1);
  }
  // This namepace is unknown.  Return the whole name.
  // TODO: grab prefixes down in StartNamespace and use that assuming no
  // collision with Xmlns.
  return prefixed_name;
}

void ExpatHandlerNs::StartElement(const string& prefixed_name,
                                  const kmlbase::StringVector& atts) {
  expat_handler_->StartElement(TranslatePrefixedName(prefixed_name), atts);
}

void ExpatHandlerNs::EndElement(const string& prefixed_name) {
  expat_handler_->EndElement(TranslatePrefixedName(prefixed_name));
}

void ExpatHandlerNs::CharData(const string& s) {
  expat_handler_->CharData(s);
}

void ExpatHandlerNs::StartNamespace(const string& prefix,
                                    const string& uri) {
}

void ExpatHandlerNs::EndNamespace(const string& prefix) {
}

}  // end namespace kmlbase
