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

// This file contains the implementation of the KML URI resolution functions.

#include "kml/engine/kml_uri.h"
#include "kml/engine/kml_uri_internal.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/uri_parser.h"

using kmlbase::UriParser;

namespace kmlengine {

KmlUri::KmlUri(const string& base, const string& target)
  : is_kmz_(false),
    base_(base),
    target_(target),
    target_uri_(kmlbase::UriParser::CreateFromParse(target.c_str())) {
}

// This is required to keep the point of instatiation of the scoped_ptr
// template arg where the full class definition (of UriParser) is known.
KmlUri::~KmlUri() {
}

// static
KmlUri* KmlUri::CreateRelative(const string& base,
                               const string& target) {
  KmlUri* kml_uri = new KmlUri(base, target);
  // To create a valid KmlUri the base must be absolute, the target must be
  // valid and the resolution must succeed.  If any of these are false then
  // NULL is returned.  The returned KmlUri object must be managed by the
  // caller; boost::scoped_ptr is recommended.
  // TODO: streamline UriParser::CreateFromParse, ResolveUri, GetFetchableUri,
  // and KmzSplit, possibly push all of KmlUri into kmlbase::UriParser.
  string fetchable_url;
  if (kml_uri->target_uri_.get() &&
      ResolveUri(base, target, &kml_uri->url_) &&
      GetFetchableUri(kml_uri->url_, &fetchable_url)) {
    kml_uri->is_kmz_ = KmzSplit(fetchable_url,
                                &kml_uri->kmz_url_,
                                &kml_uri->path_in_kmz_);
    return kml_uri;
  }
  // KmlCache NULL or base or target invalid.
  delete kml_uri;
  return NULL;
}


// Note that this is implemented in terms of the 3rd party uriparser library
// which is fully encapsulated here.
bool ResolveUri(const string& base, const string& relative,
                string* result) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateResolvedUri(base.c_str(), relative.c_str()));
  return uri_parser.get() && uri_parser->ToString(result);
}

bool NormalizeUri(const string& uri, string* result) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  return uri_parser.get() && uri_parser->Normalize() &&
         uri_parser->ToString(result);
}

bool NormalizeHref(const string& href, string* result) {
  if (!result) {
    return false;
  }
  // Convert to URI.
  string uri;
  if (!FilenameToUri(href, &uri)) {
    return false;
  }
  // Normalize.
  string normalized_uri;
  if (!NormalizeUri(uri, &normalized_uri)) {
    return false;
  }
  // Then convert back to href.
  string normalized_href;
  if (!UriToFilename(normalized_uri, &normalized_href)) {
    return false;
  }
  *result = normalized_href;
  return true;
}

bool UriToFilename(const string& uri, string* output) {
  return UriParser::UriToFilename(uri, output);
}

bool FilenameToUri(const string& filename, string* output) {
  return UriParser::FilenameToUri(filename, output);
}

// TODO: provide a query splitter.
// Note that RFC 3986 does not define the structure of a query.  However,
// the uriparser library does implement a name-value pair splitter and
// assembler which can be front-ended in a future libkml function.
bool SplitUri(const string& uri, string* scheme, string* host,
              string* port, string* path, string* query,
              string* fragment) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }
  // These all ignore a NULL string arg.
  uri_parser->GetScheme(scheme);
  uri_parser->GetHost(host);
  uri_parser->GetPort(port);
  uri_parser->GetPath(path);
  uri_parser->GetQuery(query);
  uri_parser->GetFragment(fragment);
  return true;
}

bool SplitUriPath(const string& uri, string* path) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }
  return uri_parser->GetPath(path);
}

bool SplitUriFragment(const string& uri, string* fragment) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }
  return uri_parser->GetFragment(fragment);
}

bool GetFetchableUri(const string& uri, string* fetchable_uri) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }

  if (!fetchable_uri) {
    return true;  // uri parsed fine, just not interested in output.
  }
  string scheme;
  uri_parser->GetScheme(&scheme);
  string host;
  uri_parser->GetHost(&host);

  if (!scheme.empty() && !host.empty()) {
    fetchable_uri->append(scheme).append("://",3).append(host);
    string port;
    uri_parser->GetPort(&port);
    if (!port.empty()) {
      fetchable_uri->append(":",1).append(port);
    }
    fetchable_uri->append("/",1);
  }

  string path;
  uri_parser->GetPath(&path);
  if (!path.empty()) {
    fetchable_uri->append(path);
  }
  return true;
}

bool KmzSplit(const string& kml_url, string* kmz_url,
              string* kmz_path) {
  size_t kmz = kml_url.find(".kmz");
  if (kmz == string::npos) {
    return false;
  }
  if (kmz_url) {
    *kmz_url = kml_url.substr(0, kmz + 4);
  }
  if (kmz_path && kml_url.size() > kmz + 4) {
    *kmz_path = kml_url.substr(kmz + 4 + 1);  // one past / after ".kmz/"
  }
  return true;
}

bool ResolveModelTargetHref(const string& base_url,
                            const string& geometry_href,
                            const string& target_href,
                            string* result) {
  if (!result) {
    return false;
  }

  // First resolve the geometry href against the base URI.
  string geometry_url;
  if (!ResolveUri(base_url, geometry_href, &geometry_url)) {
    return false;  // Failed to resolve geometry URL.
  }

  // Resolve the targetHref against the resolved geometry URI.
  if (!ResolveUri(geometry_url, target_href, result)) {
    return false;
  }

  return true;
}

}  // end namespace kmlengine
