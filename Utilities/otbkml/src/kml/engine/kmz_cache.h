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

// This file contains the KmzCache class declaration.

#ifndef KML_ENGINE_KMZ_CACHE_H__
#define KML_ENGINE_KMZ_CACHE_H__

#include <map>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/memory_file.h"
#include "kml/base/net_cache.h"
#include "kml/engine/kml_uri.h"
#include "kml/engine/kmz_file.h"

namespace kmlengine {

// This class is a cache of network-fetched KmzFile's.  The NetFetcher
// is supplied by the caller to implement application-specific networking.
// See kmlbase::NetCache for more information.  The intended usage is for
// a given application to create one KmzCache through which all KML-related
// fetches occur.
class KmzCache : public kmlbase::NetCache<KmzFile> {
  typedef kmlbase::NetCache<kmlbase::MemoryFile> MemoryFileCache;
 public:
  // This creates a KmzCache to hold up to the given number of KmzFiles.
  // This same size is used for an internal cache of MemoryFile's of fetched
  // files which are not KMZ.
  KmzCache(kmlbase::NetFetcher* net_fetcher_, size_t max_size)
    : kmlbase::NetCache<KmzFile>(net_fetcher_, max_size) {
    memory_file_cache_.reset(new MemoryFileCache(net_fetcher_, max_size));
  }

  // This is the main public method to fetch a "KML URL" through the cache.
  // A "KML URL" is anything that may appear in a <href> especially including
  // file references into a KMZ such as: "http://foo.com/goo.kmz/images/z.jpg".
  // The engine::KmzSplit method is used to detect and save the two parts
  // of a "KML URL" if it is this variety of KMZ file access.  If the
  // KmzCache has no entry for the given KMZ file it is fetched using the
  // NetFetcher provided in the constructuro.  If the KmzCache does have
  // this KMZ cached then this simply accesses the internal KmzFile for this
  // KMZ and reads the content.  This returns true if the KMZ was fetched
  // properly and/or the file within the KMZ was found and read properly.
  // Finally, if the URL is not to or through a KMZ the file is fetched
  // and cached in the internal MemoryFileCache.
  bool FetchUrl(const std::string& kml_url, std::string* content);

  // If the given "KML URL" is to a KMZ file within the cache then the content
  // for this file within the KMZ is saved to the supplied buffer if such is
  // supplied.  The return status indicates whether or not a KMZ file for
  // this URL is the cache and if that KMZ has the given file.  See the
  // the discussion in KmzSplit above.  In most circumstances application
  // code should use FetchUrl(), but this method is well behaved as described.
  bool FetchFromCache(const std::string& fetchable_url,
                      const std::string& path_in_kmz,
                      std::string* content) const;

 private:
  boost::scoped_ptr<MemoryFileCache> memory_file_cache_;
};

}  // end namespace kmlengine

#endif  // KML_ENGINE_KMZ_CACHE_H__
