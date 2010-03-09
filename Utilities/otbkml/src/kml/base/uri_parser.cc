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

// This file contains the implementation of the internal UriParser class.

#include "kml/base/uri_parser.h"
#include <string.h>  // memset()
#include "uriparser/Uri.h"

namespace kmlbase {

class UriParserPrivate {
 public:
  UriParserPrivate() {
   // Initialize the UriUriA struct this class wraps to a sane state.
   memset((void *)&uri_, 0, sizeof(UriUriA));
  }

  ~UriParserPrivate() {
    uriFreeUriMembersA(&uri_);
  }

  UriUriA* get_mutable_uri() {
    return &uri_;
  }

  UriUriA* get_uri() const {
    return const_cast<UriUriA*>(&uri_);
  }

  // This helper function detects the existence of the given component and
  // converts it to a string if one is supplied.  If this component does not
  // exist false is returned.  If the component does exist true is returned.
  bool GetUriComponent(const UriTextRangeA& text_range,
                       string* output) const {
    if (!text_range.first || !text_range.afterLast) {
      return false;
    }
    if (output) {
      output->assign(text_range.first, text_range.afterLast - text_range.first);
    }
    return true;
  }

 private:
  UriUriA uri_;
};

UriParser* UriParser::CreateFromParse(const char* str) {
  UriParser* uri_parser = new UriParser;
  if (uri_parser->Parse(str)) {
    return uri_parser;
  }
  delete uri_parser;
  return NULL;
}

UriParser* UriParser::CreateResolvedUri(const char* base,
                                        const char* relative) {
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

UriParser::UriParser() {
  uri_parser_private_.reset(new UriParserPrivate);
}

UriParser::~UriParser() {
}

bool UriParser::Parse(const char* str) {
  UriParserStateA state;
  state.uri = uri_parser_private_->get_mutable_uri();
  if (uriParseUriA(&state, str) != URI_SUCCESS) {
    uriFreeUriMembersA(uri_parser_private_->get_mutable_uri());
    return false;
  }
  return true;
}

bool UriParser::Normalize() {
  return uriNormalizeSyntaxA(uri_parser_private_->get_mutable_uri()) ==
                             URI_SUCCESS;
}

bool UriParser::Resolve(const UriParser& base, const UriParser& relative) {
  return uriAddBaseUriA(uri_parser_private_->get_mutable_uri(),
                        relative.uri_parser_private_->get_uri(),
                        base.uri_parser_private_->get_uri()) == URI_SUCCESS;
}

bool UriParser::ToString(string* output) const {
  if (!output) {
    return false;
  }
  int chars_required;
  if (uriToStringCharsRequiredA(uri_parser_private_->get_mutable_uri(),
                                &chars_required) != URI_SUCCESS) {
    return false;
  }
  char* dest_str = (char*)malloc(chars_required+1);
  if (!dest_str) {
    return false;
  }
  int chars_written;
  if (uriToStringA(dest_str, uri_parser_private_->get_mutable_uri(),
                   chars_required+1, &chars_written) != URI_SUCCESS) {
    free(dest_str);
    return false;
  }
  *output = dest_str;
  free(dest_str);
  return true;
}

bool UriParser::UriToFilename(const string& uri,
                              string* output) {
#ifdef WIN32
  return UriToWindowsFilename(uri, output);
#else
  return UriToUnixFilename(uri, output);
#endif
}

bool UriParser::UriToUnixFilename(const string& uri,
                                  string* output) {
  if (!output) {
    return false;
  }
  const int chars_required = static_cast<int>(uri.size()) + 1;
  char* filename = (char*)malloc(chars_required * sizeof(char));
  if (uriUriStringToUnixFilenameA(uri.c_str(), filename) != URI_SUCCESS) {
    free(filename);
    return false;
  }
  *output = filename;
  free(filename);
  return true;
}

bool UriParser::UriToWindowsFilename(const string& uri,
                                     string* output) {
  if (!output) {
    return false;
  }
  const int chars_required = static_cast<int>(uri.size()) + 1;
  char* filename = (char*)malloc(chars_required * sizeof(char));
  if (uriUriStringToWindowsFilenameA(uri.c_str(), filename) != URI_SUCCESS) {
    free(filename);
    return false;
  }
  *output = filename;
  free(filename);
  return true;
}

bool UriParser::FilenameToUri(const string& filename,
                              string* output) {
#ifdef WIN32
  return WindowsFilenameToUri(filename, output);
#else
  return UnixFilenameToUri(filename, output);
#endif
}

bool UriParser::UnixFilenameToUri(const string& filename,
                                  string* output) {
  if (!output) {
    return false;
  }
  const int chars_required = 7 + 3 * static_cast<int>(filename.size()) + 1;
  char* uri = (char*)malloc(chars_required * sizeof(char));
  if (uriUnixFilenameToUriStringA(filename.c_str(), uri) != URI_SUCCESS) {
    free(uri);
    return false;
  }
  *output = uri;
  free(uri);
  return true;
}

bool UriParser::WindowsFilenameToUri(const string& filename,
                                     string* output) {
  if (!output) {
    return false;
  }
  const int chars_required = 8 + 3 * static_cast<int>(filename.size()) + 1;
  char* uri = (char*)malloc(chars_required * sizeof(char));
  if (uriWindowsFilenameToUriStringA(filename.c_str(), uri) != URI_SUCCESS) {
    free(uri);
    return false;
  }
  *output = uri;
  free(uri);
  return true;
}

bool UriParser::GetScheme(string* scheme) const {
  return uri_parser_private_->GetUriComponent(
      uri_parser_private_->get_uri()->scheme, scheme);
}

bool UriParser::GetHost(string* host) const {
  return uri_parser_private_->GetUriComponent(
      uri_parser_private_->get_uri()->hostText, host);
}

bool UriParser::GetPort(string* port) const {
  return uri_parser_private_->GetUriComponent(
      uri_parser_private_->get_uri()->portText, port);
}

bool UriParser::GetQuery(string* query) const {
  return uri_parser_private_->GetUriComponent(
      uri_parser_private_->get_uri()->query, query);
}

bool UriParser::GetFragment(string* fragment) const {
  return uri_parser_private_->GetUriComponent(
      uri_parser_private_->get_uri()->fragment, fragment);
}

bool UriParser::GetPath(string* path) const {
  if (!uri_parser_private_->get_uri()->pathHead ||
      !uri_parser_private_->get_uri()->pathTail) {
    return false;
  }
  if (path) {
    path->clear();
    UriPathSegmentA* segment = uri_parser_private_->get_uri()->pathHead;
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

}  // end namespace kmlbase
