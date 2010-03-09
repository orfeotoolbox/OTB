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

#include "kml/engine/link_util.h"
#include "kml/dom.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_uri.h"
#include "kml/engine/kml_file.h"

namespace kmlengine {

KmlFilePtr FetchLink(const KmlFilePtr& kml_file,
                     const kmldom::NetworkLinkPtr& networklink) {
  if (KmlCache* kml_cache = kml_file->get_kml_cache()) {
    string href;
    if (kmlengine::GetLinkParentHref(networklink, &href)) {
      return kml_cache->FetchKmlRelative(kml_file->get_url(), href);
    }
  }
  return NULL;
}

bool FetchIcon(const KmlFilePtr& kml_file,
               const kmldom::OverlayPtr& overlay,
               string* data) {
  if (KmlCache* kml_cache = kml_file->get_kml_cache()) {
    string href;
    if (kmlengine::GetIconParentHref(overlay, &href)) {
      return kml_cache->FetchDataRelative(kml_file->get_url(), href, data);
    }
  }
  return false;
}

}  // end namespace kmlengine
