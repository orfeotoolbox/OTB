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

// This file contains the unit tests for the GoogleSpreadsheets class.

#include "kml/convenience/google_spreadsheets.h"

#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/file.h"
#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"
#include "kml/convenience/http_client_test_util.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlconvenience {

class GoogleSpreadsheetsTest : public testing::Test {
 protected:
  void SetUp() {
  }
  boost::scoped_ptr<GoogleSpreadsheets> google_spreadsheets_;
};

// This tests NULL use of the Create method.
TEST_F(GoogleSpreadsheetsTest, TestNullCreate) {
  // NULL/empty args?  Null return (no crash, no hang, no exception, etc).
  ASSERT_FALSE(GoogleSpreadsheets::Create(NULL));
}

TEST_F(GoogleSpreadsheetsTest, TestGetConstants) {
  // Assert merely that the constant getters exist and return something.
  const char* service_name = GoogleSpreadsheets::get_service_name();
  ASSERT_TRUE(service_name);
  ASSERT_TRUE(strlen(service_name));  // It's a null terminated C string.
  const char* metafeed_uri = GoogleSpreadsheets::get_metafeed_uri();
  ASSERT_TRUE(metafeed_uri);
  ASSERT_TRUE(strlen(metafeed_uri));  // It's a null terminated C string.
}

TEST_F(GoogleSpreadsheetsTest, TestGetScope) {
  // No user of the GoogleSpreadsheets class should
  // assume anything of the scope.  But, we
  // can expect that it exists.
  google_spreadsheets_.reset(
      GoogleSpreadsheets::Create(new HttpClient("dummy")));
  ASSERT_FALSE(google_spreadsheets_->get_scope().empty());
}

// This tests basic normal use of the GetMetaFeedXml() method.
TEST_F(GoogleSpreadsheetsTest, TestGetMetaFeedXml) {
  HttpClient* http_client = new HttpClient("TestGetMetaFeedXml");
  http_client->Login("local", "user@gmail.com", "user-password");
  google_spreadsheets_.reset(GoogleSpreadsheets::Create(http_client));
  ASSERT_TRUE(google_spreadsheets_.get());
  string meta_feed;
  ASSERT_TRUE(google_spreadsheets_->GetMetaFeedXml(&meta_feed));
  size_t end_of_first_line = meta_feed.find('\n');
  ASSERT_NE(string::npos, end_of_first_line);
  ASSERT_EQ(string("GET ") + google_spreadsheets_->get_scope() +
                google_spreadsheets_->get_metafeed_uri(),
            meta_feed.substr(0, end_of_first_line));
}

TEST_F(GoogleSpreadsheetsTest, TestGetMetaFeed) {
  google_spreadsheets_.reset(
      GoogleSpreadsheets::Create(new OneFileHttpClient(
          string(DATADIR) + "/gmaps/gsheet-metafeed.xml")));
  ASSERT_TRUE(google_spreadsheets_.get());
  // Call the method under test.
  kmldom::AtomFeedPtr atom_feed = google_spreadsheets_->GetMetaFeed();
  ASSERT_TRUE(atom_feed.get());
  // Check a few things known to be in metafeed.xml:
  ASSERT_EQ(string("Available Spreadsheets - kml.bent@gmail.com"),
            atom_feed->get_title());
  ASSERT_EQ(static_cast<size_t>(3), atom_feed->get_link_array_size());
  ASSERT_EQ(static_cast<size_t>(2), atom_feed->get_entry_array_size());
  ASSERT_EQ(string("oz"), atom_feed->get_entry_array_at(0)->get_title());
  ASSERT_EQ(string("gplex"),
      atom_feed->get_entry_array_at(1)->get_title());
}

TEST_F(GoogleSpreadsheetsTest, TestDownloadSpreadsheet) {
  // Fetch a feed which has an entry which has a gd:resourceId.
  TestDataHttpClient test_data_http_client;
  kmldom::AtomFeedPtr feed = AtomUtil::GetAndParseFeed(
      "http://example.com/gdata/doclist-metafeed.xml",
      test_data_http_client);
  ASSERT_TRUE(feed);

  HttpRequestVector request_log;
  google_spreadsheets_.reset(
      GoogleSpreadsheets::Create(new LoggingHttpClient(&request_log)));
  string spreadsheet_data;
  // This asserts that 1) the DownloadSpreadsheet method exists with the
  // advertised signature and 2) that true can be returned if the entry
  // specified holds a gd:resourceId.  Given that nothing is actually fetched
  // there's little else to check.
  ASSERT_TRUE(google_spreadsheets_->DownloadSpreadsheet(
      feed->get_entry_array_at(0), "csv", &spreadsheet_data));
  // That DownloadSpreadsheet is implemented as one HTTP GET is arguably and
  // implementation detail...
  ASSERT_EQ(static_cast<size_t>(1), request_log.size());
  ASSERT_EQ(HTTP_GET, request_log[0].http_method_);

  // Verify that false is returned on an entry with no gd:resourceId.
  kmldom::AtomEntryPtr entry =
      kmldom::KmlFactory::GetFactory()->CreateAtomEntry();
  ASSERT_FALSE(google_spreadsheets_->DownloadSpreadsheet(entry, "csv",
                                                         &spreadsheet_data));
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
