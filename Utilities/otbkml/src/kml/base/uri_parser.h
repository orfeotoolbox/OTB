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

// This file contains the internal UriParser class which front-ends the
// third_party/uriparser in a C++ API.  See kml/engine/kml_uri.h for the
// public API to URI handling.

#ifndef KML_BASE_URI_PARSER_H__
#define KML_BASE_URI_PARSER_H__

#include <string.h>  // memset()
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/util.h"
#include "uriparser/Uri.h"

namespace kmlbase {

// This class is a memory-safe wrapper to uriparser's UriUriA.
class UriParser {
 public:
  // UriParser is always constructed from one of the following static methods.
  // The main intentended usage of UriParser is within libkml and is restricted
  // to these static methods.

  // This creates a UriParser from a URI in string form.
  static UriParser* CreateFromParse(const char* str) {
    UriParser* uri_parser = new UriParser;
    if (uri_parser->Parse(str)) {
      return uri_parser;
    }
    delete uri_parser;
    return NULL;
  }

  // This creates a UriParser representing the resolution of the given
  // relative URI against the given base URI.
  static UriParser* CreateResolvedUri(const char* base, const char* relative) {
    boost::scoped_ptr<UriParser> base_uri(CreateFromParse(base));
    boost::scoped_ptr<UriParser> relative_uri(CreateFromParse(relative));
    if (!base_uri.get() || !relative_uri.get()) {
      return NULL;
    }
    UriParser* resolved_uri = new UriParser;
    if (resolved_uri->Resolve(*base_uri.get(), *relative_uri.get())) {
      return resolved_uri;
    }
    delete resolved_uri;
    return NULL;
  }

  // The intended usage is to create a UriParser from a static method.
  UriParser() {
     // Initialize the UriUriA struct this class wraps to a sane state.
     memset((void *)&uri_, 0, sizeof(UriUriA));
  }

  // The destructor must perform uriparser-specific operations to release
  // resources.  It is highly recommdended that a UriParser* be managed
  // with boost::scoped_ptr or equivalent (as is done in CreateResolveUri).
  ~UriParser() {
    uriFreeUriMembersA(&uri_);
  }

  // This parses the given URI string into the UriParser object and obliterates
  // any previous URI parsed into this object.  If the parse succeeds true is
  // returned, else false is returned.  This method is intended for use mainly
  // with the CreateFromParse() static method.
  bool Parse(const char* str) {
    UriParserStateA state;
    state.uri = &uri_;
    if (uriParseUriA(&state, str) != URI_SUCCESS) {
      uriFreeUriMembersA(&uri_);
      return false;
    }
    return true;
  }

  // UriParser (and the underlying uriparser library) does not automatically
  // normalize any URI.  (Normalize resolves the ..'s in a path, for example).
  // This method may be used at any time to normalize the URI.  RFC 3986
  // requires a fetching client to normalize a URI before fetching it.
  bool Normalize() {
    return uriNormalizeSyntaxA(&uri_) == URI_SUCCESS;
  }

  // This resolves the URI represented by the UriParser relative against the
  // URI represented by the UriParser base.  This method is intended for use
  // mainly with the CreateResolvedUri() static method.
  bool Resolve(const UriParser& base, const UriParser& relative) {
    return uriAddBaseUriA(&uri_, relative.get_uri(), base.get_uri())
      == URI_SUCCESS;
  }

  // This method saves the URI in string form into the given string.  This
  // returns false if a NULL string argument is supplied or on any internal
  // errors in saving to this string.  True is returned on success.
  bool ToString(std::string* output) const {
    if (!output) {
      return false;
    }
    int chars_required;
    if (uriToStringCharsRequiredA(&uri_, &chars_required) != URI_SUCCESS) {
      return false;
    }
    char* dest_str = (char*)malloc(chars_required+1);
    if (!dest_str) {
      return false;
    }
    int chars_written;
    if (uriToStringA(dest_str, &uri_, chars_required+1, &chars_written)
        != URI_SUCCESS) {
      free(dest_str);
      return false;
    }
    *output = dest_str;
    free(dest_str);
    return true;
  }

  // This returns the scheme of the URI if one exists.
  bool GetScheme(std::string* scheme) const {
    return GetUriComponent(uri_.scheme, scheme);
  }

  // This returns the host of the URI if one exists.
  bool GetHost(std::string* host) const {
    return GetUriComponent(uri_.hostText, host);
  }

  // This returns the port of the URI if one exists.
  bool GetPort(std::string* port) const {
    return GetUriComponent(uri_.portText, port);
  }

  // This returns the query of the URI if one exists.
  bool GetQuery(std::string* query) const {
    return GetUriComponent(uri_.query, query);
  }

  // This method returns the fragment portion of the URI into the given
  // string if such is supplied.  If no string is supplied or if there is no
  // fragment in this URI false is returned.  The fragment returned does not
  // include the '#' found in the corresponding URI.
  bool GetFragment(std::string* fragment) const {
    return GetUriComponent(uri_.fragment, fragment);
  }

  // This method returns true if the uri has a path.  If an output string
  // pointer is supplied the path is saved there.
  bool GetPath(std::string* path) const {
    if (!uri_.pathHead || !uri_.pathTail) {
      return false;
    }
    if (path) {
      path->clear();
      UriPathSegmentA* segment = uri_.pathHead;
      while (segment) {
        UriTextRangeA* text_range = &segment->text;
        if (!text_range || !text_range->first || !text_range->afterLast) {
          return false;  // Something is corrupt.
        }
        path->append(text_range->first,
                     text_range->afterLast - text_range->first);
        segment = segment->next;
        if (segment) {  // If there's a next segment append a separator.
          path->append("/");
        }
      }
    }
    return true;
  }

 private:
  // Only this class can reach into the uriparser-specific internals of a
  // UriParser.
  UriUriA* get_uri() const {
    return const_cast<UriUriA*>(&uri_);
  }

  // This helper function detects the existence of the given component and
  // converts it to a string if one is supplied.  If this component does not
  // exist false is returned.  If the component does exist true is returned.
  bool GetUriComponent(const UriTextRangeA& text_range, std::string* output)
      const {
    if (!text_range.first || !text_range.afterLast) {
      return false;
    }
    if (output) {
      output->assign(text_range.first, text_range.afterLast - text_range.first);
    }
    return true;
  }

  // The main role of this class is to front-end this C type in uriparser.
  UriUriA uri_;

  // No copy construction or assignment please.
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(UriParser);
};

}  // end namespace kmlbase

#endif  // KML_BASE_URI_PARSER_H__
