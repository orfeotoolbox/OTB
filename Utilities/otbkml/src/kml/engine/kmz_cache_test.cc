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

// This file contains the unit tests for the KmzCache class.

#include "kml/engine/kmz_cache.h"
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/net_cache_test_util.h"
#include "gtest/gtest.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_uri.h"
#include "kml/engine/kml_uri_internal.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

// This is effectively a mock network of KMZ files.
const static struct {
  const char* url;
  const char* kmz_test_file;
} kMockKmzNet[] = {
  {
  "http://somehost.com/kmz/doc.kmz",
  "/kmz/doc.kmz",
  },
  {
  "http://otherhost.com/kmz/model-macky.kmz",
  "/kmz/model-macky.kmz",
  },
  {
  "http://localhost/kmz/zermatt-photo.kmz",
  "/kmz/zermatt-photo.kmz",
  },
  {
  "http://foo.com/kmz/screenoverlay-continents.kmz",
  "/kmz/screenoverlay-continents.kmz",
  }
};

const size_t kMaxTestCacheSize = 3;

class KmzCacheTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kmz_cache_.reset(new KmzCache(&testdata_net_fetcher_, kMaxTestCacheSize));
    kml_cache_.reset(new KmlCache(&testdata_net_fetcher_, kMaxTestCacheSize));
  }

  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<KmlUri> kml_uri_;
  boost::scoped_ptr<KmzCache> kmz_cache_;
  boost::scoped_ptr<KmlCache> kml_cache_;
  void VerifyContentInCache(const string& kml_url,
                            const string& want_data);
};

// Verify the state of a freshly created empty KmzCache.
TEST_F(KmzCacheTest, TestDefaultState) {
  // Use a valid looking base url for the sake of creating a proper KmlUri.
  const string kBase("http://hi.com/");
  const string kNoSuchUrl("no-such-url-in-mock-net");
  ASSERT_EQ(static_cast<size_t>(0), kmz_cache_->Size());
  kml_uri_.reset(KmlUri::CreateRelative(kBase, kNoSuchUrl));
  ASSERT_TRUE(kml_uri_.get());
  ASSERT_FALSE(kmz_cache_->DoFetch(kml_uri_.get(), NULL));
  kml_uri_->set_path_in_kmz("no-such-path");
  ASSERT_FALSE(kmz_cache_->FetchFromCache(kml_uri_.get(), NULL));
  ASSERT_FALSE(kmz_cache_->LookUp(kNoSuchUrl));
  ASSERT_FALSE(kmz_cache_->Delete(kNoSuchUrl));
  ASSERT_FALSE(kmz_cache_->RemoveOldest());
}

// This test verifies basic usage of the Save(), LookUp(), and Delete() methods.
// Save() and Delete() are intended to be internal, but are still well behaved
// as per assertions in this test.
TEST_F(KmzCacheTest, TestBasicSaveLookUpDelete) {
  const string kUrl("http://host.com/dir/doc.kmz");
  const string kGoodKmz = string(DATADIR) + "/kmz/doc.kmz";
  string want_kml_data;
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(kGoodKmz.c_str());
  ASSERT_TRUE(kmz_file);
  kmz_file->ReadKml(&want_kml_data);

  // Save this KmzFile into the cache under a given URL.
  ASSERT_TRUE(kmz_cache_->Save(kUrl, kmz_file));

  // Lookup the KmzFile with that same URL.
  KmzFilePtr lookup_kmz_file = kmz_cache_->LookUp(kUrl);
  ASSERT_TRUE(lookup_kmz_file);

  // Make sure the content of the KmzFile is as expected.
  // This KMZ test file is known to have one KML file.
  string got_kml_data;
  lookup_kmz_file->ReadKml(&got_kml_data);
  ASSERT_EQ(want_kml_data, got_kml_data);

  // Verify this is the only entry in the cache.
  ASSERT_EQ(static_cast<size_t>(1), kmz_cache_->Size());

  // Delete this entry from the cache and assert that this entry was found.
  ASSERT_TRUE(kmz_cache_->Delete(kUrl));
}

// Verify basic use of FetchUrl() for a URL mapping to a valid KMZ.
// NOTE: This is the main public API method of KmzCache.
TEST_F(KmzCacheTest, TestBasicFetchUrl) {
  // Read the "network" via the given URL.
  const string& url = kMockKmzNet[0].url;
  kml_uri_.reset(KmlUri::CreateRelative(url, url));
  ASSERT_TRUE(kml_uri_.get());
  string got_kml_data;
  ASSERT_TRUE(kmz_cache_->DoFetch(kml_uri_.get(), &got_kml_data));

  // Read the data for that URL directly.
  string want_kml_data;
  const string kKmzTestFile(string(DATADIR) +
                                 kMockKmzNet[0].kmz_test_file);
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(kKmzTestFile.c_str());
  ASSERT_TRUE(kmz_file);
  ASSERT_TRUE(kmz_file->ReadKml(&want_kml_data));

  ASSERT_EQ(want_kml_data, got_kml_data);
  // Delete this entry from the cache and assert that this entry was found.
  ASSERT_TRUE(kmz_cache_->Delete(url));
}

// Verify basic use of FetchFromCache().
TEST_F(KmzCacheTest, TestBasicFetchFromCache) {
  const char* kUrl = kMockKmzNet[0].url;
  string net_url;
  string kmz_path;
  KmzSplit(kUrl, &net_url, &kmz_path);

  string data;
  // First verify that FetchFromCache() does not have the data.
  kml_uri_.reset(KmlUri::CreateRelative(kUrl, kUrl));
  ASSERT_TRUE(kml_uri_.get());
  ASSERT_FALSE(kmz_cache_->FetchFromCache(kml_uri_.get(), &data));
  // Also verify that a NULL data arg behaves properly.
  ASSERT_FALSE(kmz_cache_->FetchFromCache(kml_uri_.get(), NULL));

  // Use FetchUrl() to bring this into cache.
  string got_kml_data;
  ASSERT_TRUE(kmz_cache_->DoFetch(kml_uri_.get(), &got_kml_data));

  // Verify that a NULL data arg behaves properly.
  // TODO: KmzFile::ReadKml() returns false on NULL arg
  //       Be hand if it could behave as a "HasKml()" in this instance.
  //ASSERT_TRUE(kmz_cache_->FetchFromCache(kUrl, NULL));

  string got_data;
  // First verify that FetchFromCache() has the right data.
  ASSERT_TRUE(kmz_cache_->FetchFromCache(kml_uri_.get(), &data));

  // Read the data for that URL directly from the test data dir.
  string want_kml_data;
  const string kKmzTestFile(string(DATADIR) +
                                 kMockKmzNet[0].kmz_test_file);
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(kKmzTestFile.c_str());
  ASSERT_TRUE(kmz_file);
  ASSERT_TRUE(kmz_file->ReadKml(&want_kml_data));

  ASSERT_EQ(want_kml_data, got_kml_data);
}

// This is a helper function which uses the internal FetchFromCache()
// to fetch the data for the given file within the given KMZ.
void KmzCacheTest::VerifyContentInCache(const string& kml_url,
                                        const string& want_data) {
  string net_url;
  string kmz_path;
  // An internal assertion to verify that we're only ever passing KMZ urls.
  ASSERT_TRUE(KmzSplit(kml_url, &net_url, &kmz_path));
  kml_uri_.reset(KmlUri::CreateRelative(kml_url, kml_url));
  ASSERT_TRUE(kml_uri_.get());
  kml_uri_->set_path_in_kmz(kmz_path);
  string got_data;
  ASSERT_TRUE(kmz_cache_->FetchFromCache(kml_uri_.get(), &got_data));
  ASSERT_EQ(want_data, got_data);
}

// This test verifies that the oldest entry is removed from cache after
// fetching the 1st URL after maximum capacity is reached.
TEST_F(KmzCacheTest, TestOverflowCacheWithFetchUrl) {
  const size_t kMockKmzNetSize = sizeof(kMockKmzNet)/sizeof(kMockKmzNet[0]);
  // An internal verification that the "network" is bigger than the cache.
  ASSERT_TRUE(kMockKmzNetSize > kMaxTestCacheSize);

  std::vector<string> mock_net_data;
  // Fetch the whole "network".
  for (size_t i = 0; i < kMockKmzNetSize; ++i) {
    // Use FetchUrl() to bring this into cache.
    const string& url = kMockKmzNet[i].url;
    kml_uri_.reset(KmlUri::CreateRelative(url, url));
    ASSERT_TRUE(kml_uri_.get());
    string data;
    ASSERT_TRUE(kmz_cache_->DoFetch(kml_uri_.get(), &data));
    ASSERT_FALSE(data.empty());
    mock_net_data.push_back(data);

    // Verify that each new FetchUrl always got its data into cache.
    VerifyContentInCache(kMockKmzNet[i].url, data);

    // Verify that the cache never exceeds maximum size.
    ASSERT_TRUE(kmz_cache_->Size() <= kMaxTestCacheSize);
  }

  // Verify that the 0'th entry is gone (it's oldest).
  string net_url;
  string kmz_path;
  const string& url = kMockKmzNet[0].url;
  KmzSplit(url, &net_url, &kmz_path);
  kml_uri_.reset(KmlUri::CreateRelative(url, url));
  ASSERT_TRUE(kml_uri_.get());
  kml_uri_->set_path_in_kmz(kmz_path);
  string data;
  ASSERT_FALSE(kmz_cache_->FetchFromCache(kml_uri_.get(), &data));

  // Verify the other entries are all in cache.
  for (size_t i = 1; i < kMockKmzNetSize; ++i) {
    VerifyContentInCache(kMockKmzNet[i].url, mock_net_data[i]);
  }
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
