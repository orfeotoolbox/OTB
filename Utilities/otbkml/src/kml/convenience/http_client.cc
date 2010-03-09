// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the default implementation of the HttpClient interface.
// This implementation is fairly specific to the Google Data API authentication
// system as described here:

#include "kml/convenience/http_client.h"

namespace kmlconvenience {

class HttpMethod {
 public:
  static const char* GetMethodString(HttpMethodEnum method_enum) {
    if (method_enum >= HTTP_OPTIONS && method_enum <= HTTP_CONNECT) {
      return kHttpMethodString[method_enum];
    }
    return NULL;
  }

 private:
  // This must exactly match the order in HttpMethodEnum.
  static const char* kHttpMethodString[];
};

// private static
const char* HttpMethod::kHttpMethodString[] = {
    NULL,  // HTTP_NONE = 0,
    "OPTIONS",
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "TRACE",
    "CONNECT"
};

static const char* kVersion = "1.0";
static const char* kGoogleClientLoginUrl =
    "https://www.google.com/accounts/ClientLogin";
static const char* kAccountType = "HOSTED_OR_GOOGLE";

HttpClient::HttpClient(const string& application_name)
    : application_name_(application_name) {
  // Standard headers for every request
  PushHeader("User-Agent", application_name_ + " GData-C++/" + kVersion,
             &headers_);
}

void HttpClient::AddHeader(const string& field_name,
                           const string& field_value) {
  PushHeader(field_name, field_value, &headers_);
}

// This is an implementation of this:
// http://code.google.com/apis/accounts/docs/AuthForInstalledApps.html
bool HttpClient::Login(const string& service_name,
                       const string& email,
                       const string& password) {
  const string data = string("Email=") + email +
                                       "&Passwd=" + password +
                                       "&accountType=" + kAccountType +
                                       "&source=" + application_name_ +
                                       "&service=" + service_name;
  string response;
  if (!SendRequest(HTTP_POST, kGoogleClientLoginUrl, NULL, &data, &response)) {
    return false;
  }

  const string kAuth = "Auth=";  // prefix of the ClientLogin token
  const size_t auth_offset = response.find(kAuth);
  if (auth_offset == string::npos) {
    return false;
  }
  const string token = response.substr(auth_offset + kAuth.size());

  auth_token_ = token.substr(0, token.size() - 1);  // remove trailing "\n"

  // Attach Authorization header to every subsequent request
  PushHeader("Authorization", "GoogleLogin auth=" + auth_token_, &headers_);

  return true;
}

// This default implemention is really only for debugging and unit testing.
bool HttpClient::SendRequest(const HttpMethodEnum method,
                             const string& uri,
                             const StringPairVector* headers,
                             const string* data,
                             string* response) const {
  if (response) {
    response->append(HttpMethod::GetMethodString(method));
    response->append(" ");
    response->append(uri);
    response->append("\n");
    for (size_t i = 0; i < headers_.size(); ++i) {
      response->append(HttpClient::FormatHeader(headers_[i]));
      response->append("\n");
    }
    if (headers) {
      for (size_t i = 0; i < headers->size(); ++i) {
        response->append(HttpClient::FormatHeader((*headers)[i]));
        response->append("\n");
      }
    }
  }
  return true;
}

bool HttpClient::FetchUrl(const string& url, string* data) const {
  return SendRequest(HTTP_GET, url, NULL, NULL, data);
}

// static
void HttpClient::PushHeader(const string& field_name,
                            const string& field_value,
                            StringPairVector* headers) {
  if (headers) {
    headers->push_back(std::make_pair(field_name, field_value));
  }
}

// static
bool HttpClient::FindHeader(const string& field_name,
                            const StringPairVector& headers,
                            string* field_value) {
  for (size_t i = 0; i < headers.size(); ++i) {
    if (field_name == headers[i].first) {
      if (field_value) {
        *field_value = headers[i].second;
      }
      return true;
    }
  }
  return false;
}

// static
string HttpClient::FormatHeader(const StringPair& header) {
  return header.first + ": " + header.second;
}

// static
void HttpClient::AppendHeaders(const StringPairVector& src,
                               StringPairVector* dest) {
  if (dest) {
    for (size_t i = 0; i < src.size(); ++i) {
      dest->push_back(src[i]);
    }
  }
}

}  // end namespace kmlconvenience
