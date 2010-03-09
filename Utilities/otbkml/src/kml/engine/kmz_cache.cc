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
#include "kml/base/time_util.h"
#include "kml/engine/kml_uri_internal.h"

using kmlbase::MemoryFilePtr;

namespace kmlengine {

// This encodes "a bit of history" regarding the two-level fetch semantics of
// KMZ relative references.  While there exists no formal KMZ specification
// it is possible that such would specify a simpler deterministic scheme
// such as supporting only the more obvious meaning of relative.  But, at this
// time (pre KMZ spec) this encodes the behavior of Google Earth for KMZ
// files in widespread use today.
//
// If the base KML file is in a KMZ and the target is relative there are two
// possible ways to resolve the absolute URL of the target.  The first is
// relative to the path _within_ the KMZ and the second is _relative_ to the
// KMZ.  This code first computes the KMZ-internal path and attempts a lookup
// within the KMZ and if that fails computes a KMZ-relative path and attempts
// a fetch of that.
//
// This scheme is used for anything referenced in any Link/href-like element
// including the <a href="..."> and <img src="..."> elements within the HTML
// of a balloon.  No specific knowledge of any KML or HTML element exists
// in this code.
//
// Here are some examples.  In all cases the base is the fully resolved
// URL of the KML file containing the Link/href-like element.
//
// Here the base URI is not within a KMZ:
// base:     http://example.com/path/file.kml
// target:   more.kml
// resolved: http://example.com/path/more.kml
//
// Here the base IS within a KMZ.  Note that this code assumes that the
// base is the full URL into the KMZ (as opposed to the "short hand" URL
// of the KMZ itself whose behavior is encoded in KmzFile::ReadFile()).
// base:     http://example.com/path/file.kmz/doc.kml
// target:   pretty.jpg [for example as <Icon><href>pretty.jpg</href></Icon>]
// resolved: http://example.com/path/file.kmz/pretty.jpg
// OR
// resolved: http://example.com/path/pretty.jpg
//
// base:     http://example.com/path/file.kmz/a/b/c.kml
// target:   ../images/0.jpg
// resolved: http://example.com/path/file.kmz/a/images/0.jpg
// OR
// resolved: http://example.com/images/0.jpg
bool KmzCache::DoFetch(KmlUri* kml_uri, string* content) {
  if (!kml_uri || !content) {
    return false;
  }
  // There are three possibilities:
  // 1) If this is not relative to a KMZ just fetch the raw file.
  if (!kml_uri->is_kmz()) {
    if (MemoryFilePtr file = memory_file_cache_->Fetch(kml_uri->get_url())) {
      *content = file->get_content();
      return true;
    }
    return false;  // Network fetch for this URL failed.
  }

  // 2) This is a KMZ reference _within_ the KMZ.  First get the KMZ file.
  // This _may_ entail a network fetch.  But, in typical usage this KMZ is
  // already in cache given that this is the KMZ that holds the KML file now
  // doing a relative fetch.
  const KmzFilePtr kmz_file = Fetch(kml_uri->get_kmz_url());
  if (!kmz_file) {
    return false;  // No such KMZ file was found.
  }
  // The KMZ file is now in cache, proceed to try to read the file within the
  // KMZ.  This is expected to be a very lightweight operation especially if
  // the target does not exist in the KMZ file.
  if (FetchFromCache(kml_uri, content)) {
    return true;
  }
  // Fall through if the target in the KmlUri was not within the KMZ.

  // 3) This is a KMZ reference _relative_ to the KMZ file.
  // PATH/foo.kmz + bar.jpg can mean either PATH/foo.kmz/bar.jpg
  // OR PATH/bar.jpg.  The following attempts the latter.
  boost::scoped_ptr<KmlUri> kmz_relative(
      KmlUri::CreateRelative(kml_uri->get_kmz_url(),
      kml_uri->get_target()));
  return DoFetch(kmz_relative.get(), content);
}

bool KmzCache::FetchFromCache(KmlUri* kml_uri, string* content) const {
  if (!kml_uri || !content) {
    return false;
  }
  // The caller expects the KmlUri references a file in a KMZ.
  // First see if the KMZ is already cached.
  if (const KmzFilePtr kmz_file = LookUp(kml_uri->get_kmz_url())) {
    // Yes, the KMZ is in the cache.  Now see if the desired file is in the KMZ.
    if (!kml_uri->get_path_in_kmz().empty()) {
      // An explicit path within the KMZ was specified.  Try to read the
      // content.
      return kmz_file->ReadFile(kml_uri->get_path_in_kmz().c_str(), content);
    }
    // No explicit path within the KMZ means "the KML file".
    // NOTE: It is considered a best practice to always use "doc.kml" as the
    // name of "the KML file" within a KMZ, but this is not guaranteed.
    // See ReadKml() in kmz_file.h for a discussion on this subject.
    string kml_path;
    if (kmz_file->ReadKmlAndGetPath(content, &kml_path)) {
      // A default KML file was found and its name was saved to kml_path.
      kml_uri->set_path_in_kmz(kml_path);
      return true;
    }
  }
  // Fall through to here if this KMZ was not in the cache or if the desired
  // file was not in the KMZ.
  return false;
}

}  // end namespace kmlengine
