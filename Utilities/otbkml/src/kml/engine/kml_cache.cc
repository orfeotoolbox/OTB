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

// This file contains the implementation of the KmlCache class.

#include "kml/engine/kml_cache.h"
#include "boost/scoped_ptr.hpp"
#include "kml/engine/kml_file.h"
#include "kml/engine/kml_uri_internal.h"
#include "kml/engine/kmz_cache.h"

namespace kmlengine {

KmlCache::KmlCache(kmlbase::NetFetcher* net_fetcher, size_t max_size) {
  kml_file_cache_.reset(new KmlFileNetCache(net_fetcher, max_size));
  kmz_file_cache_.reset(new KmzCache(net_fetcher, max_size));
}

KmlFilePtr KmlCache::FetchKmlRelative(const string& base,
                                      const string& target) {
  boost::scoped_ptr<KmlUri> kml_uri(KmlUri::CreateRelative(base, target));
  if (!kml_uri.get()) {
    // Failed to create KmlUri likely due to bad url or href.
    return NULL;
  }
  const string& url = kml_uri->get_url();
  // If there's a KmlFile cached for this URL just return it and we're done.
  if (KmlFilePtr kml_file = kml_file_cache_->LookUp(url)) {
    return kml_file;
  }
  // No KmlFile cached for this URL.  Fetch the KML through the KMZ cache.
  string content;
  if (kmz_file_cache_->DoFetch(kml_uri.get(), &content)) {
    // The KML content was found within in a fetched and/or cached KMZ.
    // Parse it into a KmlFile for it and cache it.
    KmlFilePtr kml_file = KmlFile::CreateFromStringWithUrl(content, url, this);
    if (kml_file) {
      // Parsed fine so save in KmlFile cache and return.
      kml_file_cache_->Save(url, kml_file);
      return kml_file;
    }
  }
  return NULL;
}

// TODO teach KmlUri about the concept of absolute...
KmlFilePtr KmlCache::FetchKmlAbsolute(const string& kml_uri) {
  // The base url must be a valid absolute URL even if the target is
  // absolute.  See the above TODO w.r.t KmlUri and absolute.
  // FetchXxxRelative is the most common use case.
  return FetchKmlRelative(kml_uri, kml_uri);
}

bool KmlCache::FetchDataRelative(const string& base,
                                 const string& target,
                                 string* data) {
  boost::scoped_ptr<KmlUri> kml_uri(KmlUri::CreateRelative(base, target));
  // KmzCache::Fetch has NULL pointer check.
  if (kmz_file_cache_->DoFetch(kml_uri.get(), data)) {
    return true;
  }
  return false;
}

// TODO is a FetchDataAbsolute necessary?

}  // end namespace kmlengine
