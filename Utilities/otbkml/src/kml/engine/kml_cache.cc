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

// This file contains the implementation of the KmlClass class.

#include "kml/engine/kml_cache.h"
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/engine/kml_file.h"
#include "kml/engine/kmz_cache.h"

namespace kmlengine {

KmlCache::KmlCache(kmlbase::NetFetcher* net_fetcher, size_t max_size) {
  kml_file_cache_.reset(new KmlFileNetCache(net_fetcher, max_size));
  kmz_file_cache_.reset(new KmzCache(net_fetcher, max_size));
}

KmlFilePtr KmlCache::FetchKml(const std::string& kml_url) {
  // If the KmlFile for this URL is already cached just return it.
  // Note that we do _not_ use kml_file_cache_->Fetch() as we wish to let
  // KmzCache detect the KMZ-ness of the data.
  KmlFilePtr kml_file = kml_file_cache_->LookUp(kml_url);
  if (kml_file) {
    return kml_file;
  }
  // No such KmlFile for this URL in the cache: go fetch.
  // The KmzCache returns KML data for either KMZ or KML urls.
  std::string kml_content;
  if (kmz_file_cache_->FetchUrl(kml_url, &kml_content)) {
    // Make a KmlFile out of the fetched KML.
    KmlFilePtr kml_file = KmlFile::CreateFromStringWithUrl(kml_content,
                                                           kml_url, this);
    if (kml_file) {
      // Parsed fine so save in cache and return.
      kml_file_cache_->Save(kml_url, kml_file);
      return kml_file;
    }
  }
  // Fall through to here if this URL did not exist or if its content did
  // not parse as KML.
  return NULL;
}

bool KmlCache::FetchData(const std::string& data_url, std::string* content) {
  return kmz_file_cache_->FetchUrl(data_url, content);
}

}  // end namespace kmlengine
