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

// This file contains the implementation of the GooglePicasaWeb class.

#include "kml/convenience/google_picasa_web.h"

#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"

namespace kmlconvenience {

static const char* kServiceName = "lh2";

static const char* kScope = "http://picasaweb.google.com";

static const char* kPicasaWebMetaFeedUri = "/data/feed/api/user/default";

static const char* kGdataVersion = "2";

// static
GooglePicasaWeb* GooglePicasaWeb::Create(
    HttpClient* http_client) {
  // The HttpClient must exist.
  if (!http_client) {
    return NULL;
  }
  GooglePicasaWeb* gs = new GooglePicasaWeb;
  http_client->AddHeader("GData-Version", kGdataVersion);
  gs->http_client_.reset(http_client);
  return gs;
}

// static
const char* GooglePicasaWeb::get_service_name() {
  return kServiceName;
}

// static
const char* GooglePicasaWeb::get_metafeed_uri() {
  return kPicasaWebMetaFeedUri;
}

static string GetScope() {
  if (const char* scope = getenv("GOOGLE_PICASA_WEB_SCOPE")) {
    return scope;
  }
  return kScope;
}

GooglePicasaWeb::GooglePicasaWeb()
  : scope_(GetScope()) {
}

// Keep POI of scoped_ptr<GoogleHttpClient>'s dtor out of .h
GooglePicasaWeb::~GooglePicasaWeb() {
}

bool GooglePicasaWeb::GetMetaFeedXml(string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, scope_ + kPicasaWebMetaFeedUri,
                                   NULL, NULL, atom_feed);
}

kmldom::AtomFeedPtr GooglePicasaWeb::GetMetaFeed() const {
  string meta_feed;
  if (GetMetaFeedXml(&meta_feed)) {
    return kmldom::AsAtomFeed(kmldom::ParseAtom(meta_feed, NULL));
  }
  return NULL;
}

}  // end namespace kmlconvenience
