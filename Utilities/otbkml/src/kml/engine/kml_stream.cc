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

// This file contains the implementation of the KmlStream class.

#include "kml/engine/kml_stream.h"
#include "kml/base/expat_parser.h"
#include "kml/dom/kml_handler.h"
#include "kml/dom/parser_observer.h"

using kmldom::ElementPtr;
using kmldom::ParserObserver;

namespace kmlengine {

KmlStream* KmlStream::ParseFromIstream(
    std::istream* input, string* errors, ParserObserver* observer) {
  if (!input) {
    return NULL;
  }

  // Initialize Kml parser.
  kmldom::parser_observer_vector_t observers;
  if (observer) {
    observers.push_back(observer);
  }
  kmldom::KmlHandler kml_handler(observers);

  // Perform buffered parse
  kmlbase::ExpatParser parser(&kml_handler, false);
  const int kBufSize = 4096;
  while (input->good()) {
    if (void* buf = parser.GetInternalBuffer(kBufSize)) {
      std::streamsize read_size =
          input->read(static_cast<char*>(buf), kBufSize).gcount();
      if (!parser.ParseInternalBuffer(read_size, errors, input->eof())) {
        return NULL;  // Parse error
      }
    } else {
      if (errors) {
        *errors = "memory error";
      }
      return NULL;
    }
  }

  // Parse succeeded.  Construct KmlStream.
  KmlStream* kml_stream = new KmlStream;
  kml_stream->set_root(kml_handler.PopRoot());
  return kml_stream;
}

}  // end namespace kmlengine
