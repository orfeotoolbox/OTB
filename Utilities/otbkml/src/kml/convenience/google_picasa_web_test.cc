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

// This file contains the unit tests for the GooglePicasaWeb class.

#include "kml/convenience/google_picasa_web.h"

#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/file.h"
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

class GooglePicasaWebTest : public testing::Test {
 protected:
  void SetUp() {
  }
  boost::scoped_ptr<GooglePicasaWeb> google_picasa_web_;
};

// This tests NULL use of the Create method.
TEST_F(GooglePicasaWebTest, TestNullCreate) {
  // NULL/empty args?  Null return (no crash, no hang, no exception, etc).
  ASSERT_FALSE(GooglePicasaWeb::Create(NULL));
}

TEST_F(GooglePicasaWebTest, TestGetMetafeedUri) {
  // No user of the GooglePicasaWeb class should
  // assume anything of the metafeed URI itself.  But, we
  // can expect that it exists.
  ASSERT_TRUE(GooglePicasaWeb::get_metafeed_uri());
}

TEST_F(GooglePicasaWebTest, TestGetScope) {
  // No user of the GooglePicasaWeb class should
  // assume anything of the scope.  But, we
  // can expect that it exists.
  google_picasa_web_.reset(
      GooglePicasaWeb::Create(new HttpClient("dummy")));
  ASSERT_FALSE(google_picasa_web_->get_scope().empty());
}

// This tests basic normal use of the GetMetaFeedXml() method.
TEST_F(GooglePicasaWebTest, TestGetMetaFeedXml) {
  HttpClient* http_client = new HttpClient("TestGetMetaFeedXml");
  http_client->Login("local", "user@gmail.com", "user-password");
  google_picasa_web_.reset(GooglePicasaWeb::Create(http_client));
  ASSERT_TRUE(google_picasa_web_.get());
  string meta_feed;
  ASSERT_TRUE(google_picasa_web_->GetMetaFeedXml(&meta_feed));
  size_t end_of_first_line = meta_feed.find('\n');
  ASSERT_NE(string::npos, end_of_first_line);
  ASSERT_EQ(string("GET ") + google_picasa_web_->get_scope() +
                google_picasa_web_->get_metafeed_uri(),
            meta_feed.substr(0, end_of_first_line));
}

TEST_F(GooglePicasaWebTest, TestGetMetaFeed) {
  google_picasa_web_.reset(
      GooglePicasaWeb::Create(new OneFileHttpClient(
          string(DATADIR) + "/gdata/picasaweb-metafeed.xml")));
  ASSERT_TRUE(google_picasa_web_.get());
  // Call the method under test.
  kmldom::AtomFeedPtr atom_feed = google_picasa_web_->GetMetaFeed();
  ASSERT_TRUE(atom_feed.get());
  // Check a few things known to be in metafeed.xml:
  ASSERT_EQ(string("ben.gardenfield"),
            atom_feed->get_title());
  ASSERT_EQ(static_cast<size_t>(5), atom_feed->get_link_array_size());
  ASSERT_EQ(static_cast<size_t>(17), atom_feed->get_entry_array_size());
  ASSERT_EQ(string("sydney-manly-nov-09"),
            atom_feed->get_entry_array_at(0)->get_title());
  ASSERT_EQ(string("green-thumbcafe"),
            atom_feed->get_entry_array_at(16)->get_title());
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
