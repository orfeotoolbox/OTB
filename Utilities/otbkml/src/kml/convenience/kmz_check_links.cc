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

// This file contains the implementation of the KmzCheckLinks() function.

#include "kml/convenience/kmz_check_links.h"
#include <vector>
#include "kml/engine/get_links.h"
#include "kml/engine/href.h"
#include "kml/engine/kmz_file.h"

using kmlengine::Href;
using kmlengine::KmzFile;
using std::vector;

namespace kmlconvenience {

bool KmzCheckLinks(const KmzFile& kmzfile, vector<string>* missing_links) {
  string kml;
  if (!kmzfile.ReadKml(&kml)) {
    return false;
  }

  kmlengine::href_vector_t href_vector;
  if (!kmlengine::GetLinks(kml, &href_vector)) {
    return false;  // Parse error.
  }

  bool ret = true;  // Innocent until proven guilty.
  for (size_t i = 0; i < href_vector.size(); ++i) {
    Href href(href_vector[i]);
    if (href.IsRelative()) {
      string content;
      if (!kmzfile.ReadFile(href.get_path().c_str(), &content)) {
        if (missing_links) {
          missing_links->push_back(href_vector[i]);
        }
        ret = false;
      }
    }
  }
  return ret;
}

}  // end namespace kmlconvenience
