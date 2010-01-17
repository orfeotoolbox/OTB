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

// This file contains the implementation of the KmzCache class.

#include "kml/engine/kmz_cache.h"
#include "kml/engine/kml_uri.h"
#include "kml/base/time_util.h"

namespace kmlengine {

bool KmzCache::FetchUrl(const std::string& kml_url, std::string* content) {
  std::string fetchable_url;
  std::string path_in_kmz;
  bool is_kmz = KmzSplit(kml_url, &fetchable_url, &path_in_kmz);
  if (!is_kmz) {
    // This not a KMZ file: fetch it through the internal memory file cache.
    if (kmlbase::MemoryFilePtr file = memory_file_cache_->Fetch(kml_url)) {
      if (content) {
        *content = file->get_content();
      }
      return true;
    }
    return false;  // No such URL.
  }
  // See if this in in the cache.
  if (FetchFromCache(fetchable_url, path_in_kmz, content)) {
    return true;
  }
  // If we get here the URL isn't into a KMZ in the cache, so fetch the KMZ.
  const KmzFilePtr kmz_file = Fetch(fetchable_url);
  if (!kmz_file) {
    return false;  // Network fetch failed, return the bad news.
  }
  // Network fetch succeeded.  Now proceed to try to dig out the content of the
  // given file within the KMZ.
  return FetchFromCache(fetchable_url, path_in_kmz, content);
}

bool KmzCache::FetchFromCache(const std::string& fetchable_url,
                              const std::string& path_in_kmz,
                              std::string* content) const {
  // Yes, this is a URL to/into a KMZ file.  See if it's in the cache.
  if (const KmzFilePtr kmz_file = LookUp(fetchable_url)) {
    // Yes, it's in the cache.
    // No explicit path within the KMZ means "the KML file".
    if (path_in_kmz.empty()) {
      return kmz_file->ReadKml(content);
    }
    // Otherwise we look up the named file.
    return kmz_file->ReadFile(path_in_kmz.c_str(), content);
  }
  return false;
}

}  // end namespace kmlengine
