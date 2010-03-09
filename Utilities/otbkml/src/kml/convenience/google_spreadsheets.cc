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

// This file contains the implementation of the GoogleSpreadsheets class.

#include "kml/convenience/google_spreadsheets.h"

#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"

namespace kmlconvenience {

static const char* kServiceName = "wise";

static const char* kScope = "http://spreadsheets.google.com";

static const char* kMetaFeedUri = "/feeds/spreadsheets/private/full";

// static
GoogleSpreadsheets* GoogleSpreadsheets::Create(
    HttpClient* http_client) {
  // The HttpClient must exist.
  if (!http_client) {
    return NULL;
  }
  GoogleSpreadsheets* gs = new GoogleSpreadsheets;
  http_client->AddHeader("GData-Version", "3.0");
  gs->http_client_.reset(http_client);
  return gs;
}

static string GetScope() {
  if (const char* scope = getenv("GOOGLE_SPREADSHEETS_DATA_SCOPE")) {
    return scope;
  }
  return kScope;
}

GoogleSpreadsheets::GoogleSpreadsheets()
  : scope_(GetScope()) {
}

// Keep POI of scoped_ptr<GoogleHttpClient>'s dtor out of .h
GoogleSpreadsheets::~GoogleSpreadsheets() {
}

// static
const char* GoogleSpreadsheets::get_service_name() {
  return kServiceName;
}

// static
const char* GoogleSpreadsheets::get_metafeed_uri() {
  return kMetaFeedUri;
}

const string& GoogleSpreadsheets::get_scope() const {
  return scope_;
}

bool GoogleSpreadsheets::GetMetaFeedXml(string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, scope_ + kMetaFeedUri,
                                   NULL, NULL, atom_feed);
}

kmldom::AtomFeedPtr GoogleSpreadsheets::GetMetaFeed() const {
  string meta_feed;
  if (GetMetaFeedXml(&meta_feed)) {
    return kmldom::AsAtomFeed(kmldom::ParseAtom(meta_feed, NULL));
  }
  return NULL;
}

// http://spreadsheets.google.com/feeds/download/spreadsheets/Export?key={resource_id}&exportFormat={format}
bool GoogleSpreadsheets::DownloadSpreadsheet(const kmldom::AtomEntryPtr& entry,
                                             const string& format,
                                             string* spreadsheet_data) {
  string resource_id;
  if (!AtomUtil::GetGdResourceId(entry, &resource_id)) {
    return false;
  }
  const string uri = scope_ + "/feeds/download/spreadsheets/Export?key=" +
      resource_id + "&exportFormat=" + format;
  return http_client_->SendRequest(HTTP_GET, uri, NULL, NULL, spreadsheet_data);
}


}  // end namespace kmlconvenience
