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
#include "boost/scoped_ptr.hpp"
#include "kml/base/uri_parser.h"

using kmlbase::UriParser;

namespace kmlengine {

// Note that this is implemented in terms of the 3rd party uriparser library
// which is fully encapsulated here.
// TODO: provide KML-specific (KMZ mostly) URI resolution using this.
bool ResolveUri(const std::string& base, const std::string& relative,
                std::string* result) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateResolvedUri(base.c_str(), relative.c_str()));
  return uri_parser.get() && uri_parser->ToString(result);
}

// TODO: provide a query splitter.
// Note that RFC 3986 does not define the structure of a query.  However,
// the uriparser library does implement a name-value pair splitter and
// assembler which can be front-ended in a future libkml function.
bool SplitUri(const std::string& uri, std::string* scheme, std::string* host,
              std::string* port, std::string* path, std::string* query,
              std::string* fragment) {
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

bool SplitUriPath(const std::string& uri, std::string* path) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }
  return uri_parser->GetPath(path);
}

bool SplitUriFragment(const std::string& uri, std::string* fragment) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }
  return uri_parser->GetFragment(fragment);
}

bool GetFetchableUri(const std::string& uri, std::string* fetchable_uri) {
  boost::scoped_ptr<UriParser> uri_parser(
      UriParser::CreateFromParse(uri.c_str()));
  if (!uri_parser.get()) {
    return false;
  }

  if (!fetchable_uri) {
    return true;  // uri parsed fine, just not interested in output.
  }
  std::string scheme;
  uri_parser->GetScheme(&scheme);
  std::string host;
  uri_parser->GetHost(&host);

  if (!scheme.empty() && !host.empty()) {
    fetchable_uri->append(scheme).append("://",3).append(host);
    std::string port;
    uri_parser->GetPort(&port);
    if (!port.empty()) {
      fetchable_uri->append(":",1).append(port);
    }
    fetchable_uri->append("/",1);
  }

  std::string path;
  uri_parser->GetPath(&path);
  if (!path.empty()) {
    fetchable_uri->append(path);
  }
  return true;
}

bool KmzSplit(const std::string& kml_url, std::string* kmz_url,
              std::string* kmz_path) {
  size_t kmz = kml_url.find(".kmz");
  if (kmz == std::string::npos) {
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

bool ResolveModelTargetHref(const std::string& base_url,
                            const std::string& geometry_href,
                            const std::string& target_href,
                            std::string* result) {
  if (!result) {
    return false;
  }

  // First resolve the geometry href against the base URI.
  std::string geometry_url;
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
