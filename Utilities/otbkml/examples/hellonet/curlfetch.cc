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

#include "curlfetch.h"
#include <string>
#include <curl/curl.h>

using std::string;

// CURLOPT_WRITEFUNCTION:
// size*nmemb bytes of data are at ptr, stream is user data
// return must be size*nmemb or curl itself will fail.
static size_t FetchToString(void* ptr, size_t size, size_t nmemb, void* user) {
  // static function, only user is DoCurlToString which uses CURLOPT_WRITEDATA
  // to set the "user" arg which is the C++ string (buffer) to write to.
  size_t nbytes = size * nmemb;
  string *output_buffer = reinterpret_cast<string*>(user);
  output_buffer->append(reinterpret_cast<char*>(ptr), nbytes);
  return nbytes;
}

// Separate worker function to simplify bool return logic.
static bool DoCurlToString(CURL* curl, const char* url, string* data) {
#define CURLOK(f) (f == CURLE_OK)
  if (CURLOK(curl_easy_setopt(curl, CURLOPT_URL, url)) &&
      CURLOK(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FetchToString)) &&
      CURLOK(curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                              reinterpret_cast<void*>(data))) &&
      CURLOK(curl_easy_perform(curl))) {
    return true;
  }
  return false;
}

// Wrapper to manage curl handle.  Very simple stateless implementation.  Less
// simplistic would be to reuse the CURL* handle between invocations.
bool CurlToString(const char* url, string* data) {
  CURL* curl = curl_easy_init();
  bool ret = DoCurlToString(curl, url, data);
  curl_easy_cleanup(curl);
  return ret;
}
