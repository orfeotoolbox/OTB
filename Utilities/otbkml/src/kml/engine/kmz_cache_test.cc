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
#include <string>
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/unit_test.h"
#include "kml/engine/kml_uri.h"

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

class KmzCacheTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmzCacheTest);
  CPPUNIT_TEST(TestDefaultState);
  CPPUNIT_TEST(TestBasicSaveLookUpDelete);
  CPPUNIT_TEST(TestBasicFetchUrl);
  CPPUNIT_TEST(TestBasicFetchFromCache);
  CPPUNIT_TEST(TestOverflowCacheWithFetchUrl);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    kmz_cache_.reset(new KmzCache(&testdata_net_fetcher_, kMaxTestCacheSize));
  }

  void tearDown() {
    // kmz_cache_ managed by scoped_ptr
  }

 protected:
  void TestDefaultState();
  void TestBasicSaveLookUpDelete();
  void TestBasicFetchUrl();
  void TestBasicFetchFromCache();
  void TestOverflowCacheWithFetchUrl();

 private:
  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<KmzCache> kmz_cache_;
  void VerifyContentInCache(const std::string& kml_url,
                            const std::string& want_data);
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmzCacheTest);

// Verify the state of a freshly created empty KmzCache.
void KmzCacheTest::TestDefaultState() {
  const std::string kNoSuchUrl("no-such-url-in-mock-net");
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), kmz_cache_->Size());
  CPPUNIT_ASSERT(!kmz_cache_->FetchUrl(kNoSuchUrl, NULL));
  CPPUNIT_ASSERT(!kmz_cache_->FetchFromCache(kNoSuchUrl, "no-such-path", NULL));
  CPPUNIT_ASSERT(!kmz_cache_->LookUp(kNoSuchUrl));
  CPPUNIT_ASSERT(!kmz_cache_->Delete(kNoSuchUrl));
  CPPUNIT_ASSERT(!kmz_cache_->RemoveOldest());
}

// This test verifies basic usage of the Save(), LookUp(), and Delete() methods.
// Save() and Delete() are intended to be internal, but are still well behaved
// as per assertions in this test.
void KmzCacheTest::TestBasicSaveLookUpDelete() {
  const std::string kUrl("http://host.com/dir/doc.kmz");
  const std::string kGoodKmz = std::string(DATADIR) + "/kmz/doc.kmz";
  std::string want_kml_data;
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(kGoodKmz.c_str());
  CPPUNIT_ASSERT(kmz_file);
  kmz_file->ReadKml(&want_kml_data);

  // Save this KmzFile into the cache under a given URL.
  CPPUNIT_ASSERT(kmz_cache_->Save(kUrl, kmz_file));

  // Lookup the KmzFile with that same URL.
  KmzFilePtr lookup_kmz_file = kmz_cache_->LookUp(kUrl);
  CPPUNIT_ASSERT(lookup_kmz_file);

  // Make sure the content of the KmzFile is as expected.
  // This KMZ test file is known to have one KML file.
  std::string got_kml_data;
  lookup_kmz_file->ReadKml(&got_kml_data);
  CPPUNIT_ASSERT_EQUAL(want_kml_data, got_kml_data);

  // Verify this is the only entry in the cache.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), kmz_cache_->Size());

  // Delete this entry from the cache and assert that this entry was found.
  CPPUNIT_ASSERT(kmz_cache_->Delete(kUrl));

  TestDefaultState();  // Verify that kmz_cache_ is back to default state.
}

// Verify basic use of FetchUrl() for a URL mapping to a valid KMZ.
// NOTE: This is the main public API method of KmzCache.
void KmzCacheTest::TestBasicFetchUrl() {
  // Read the "network" via the given URL.
  std::string got_kml_data;
  CPPUNIT_ASSERT(kmz_cache_->FetchUrl(kMockKmzNet[0].url, &got_kml_data));

  // Read the data for that URL directly.
  std::string want_kml_data;
  const std::string kKmzTestFile(std::string(DATADIR) +
                                 kMockKmzNet[0].kmz_test_file);
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(kKmzTestFile.c_str());
  CPPUNIT_ASSERT(kmz_file);
  CPPUNIT_ASSERT(kmz_file->ReadKml(&want_kml_data));

  CPPUNIT_ASSERT_EQUAL(want_kml_data, got_kml_data);

  // Delete this entry from the cache and assert that this entry was found.
  CPPUNIT_ASSERT(kmz_cache_->Delete(kMockKmzNet[0].url));

  TestDefaultState();  // Verify that kmz_cache_ is back to default state.
}

// Verify basic use of FetchFromCache().
void KmzCacheTest::TestBasicFetchFromCache() {
  const char* kUrl = kMockKmzNet[0].url;
  std::string net_url;
  std::string kmz_path;
  KmzSplit(kUrl, &net_url, &kmz_path);

  std::string data;
  // First verify that FetchFromCache() does not have the data.
  CPPUNIT_ASSERT(!kmz_cache_->FetchFromCache(net_url, kmz_path, &data));
  // Also verify that a NULL data arg behaves properly.
  CPPUNIT_ASSERT(!kmz_cache_->FetchFromCache(net_url, kmz_path, NULL));

  // Use FetchUrl() to bring this into cache.
  std::string got_kml_data;
  CPPUNIT_ASSERT(kmz_cache_->FetchUrl(kUrl, &got_kml_data));

  // Verify that a NULL data arg behaves properly.
  // TODO: KmzFile::ReadKml() returns false on NULL arg
  //       Be hand if it could behave as a "HasKml()" in this instance.
  //CPPUNIT_ASSERT(kmz_cache_->FetchFromCache(kUrl, NULL));

  std::string got_data;
  // First verify that FetchFromCache() has the right data.
  CPPUNIT_ASSERT(kmz_cache_->FetchFromCache(net_url, kmz_path, &data));

  // Read the data for that URL directly from the test data dir.
  std::string want_kml_data;
  const std::string kKmzTestFile(std::string(DATADIR) +
                                 kMockKmzNet[0].kmz_test_file);
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(kKmzTestFile.c_str());
  CPPUNIT_ASSERT(kmz_file);
  CPPUNIT_ASSERT(kmz_file->ReadKml(&want_kml_data));

  CPPUNIT_ASSERT_EQUAL(want_kml_data, got_kml_data);
}

// This is a helper function which uses the internal FetchFromCache()
// to fetch the data for the given file within the given KMZ.
void KmzCacheTest::VerifyContentInCache(const std::string& kml_url,
                                        const std::string& want_data) {
  std::string net_url;
  std::string kmz_path;
  // An internal assertion to verify that we're only ever passing KMZ urls.
  CPPUNIT_ASSERT(KmzSplit(kml_url, &net_url, &kmz_path));
  std::string got_data;
  CPPUNIT_ASSERT(kmz_cache_->FetchFromCache(net_url, kmz_path, &got_data));
  CPPUNIT_ASSERT_EQUAL(want_data, got_data);
}

// This test verifies that the oldest entry is removed from cache after
// fetching the 1st URL after maximum capacity is reached.
void KmzCacheTest::TestOverflowCacheWithFetchUrl() {
  const size_t kMockKmzNetSize = sizeof(kMockKmzNet)/sizeof(kMockKmzNet[0]);
  // An internal verification that the "network" is bigger than the cache.
  CPPUNIT_ASSERT(kMockKmzNetSize > kMaxTestCacheSize);

  std::vector<std::string> mock_net_data;
  // Fetch the whole "network".
  for (size_t i = 0; i < kMockKmzNetSize; ++i) {
    // Use FetchUrl() to bring this into cache.
    std::string data;
    CPPUNIT_ASSERT(kmz_cache_->FetchUrl(kMockKmzNet[i].url, &data));
    CPPUNIT_ASSERT(!data.empty());
    mock_net_data.push_back(data);

    // Verify that each new FetchUrl always got its data into cache.
    VerifyContentInCache(kMockKmzNet[i].url, data);

    // Verify that the cache never exceeds maximum size.
    CPPUNIT_ASSERT(kmz_cache_->Size() <= kMaxTestCacheSize);
  }

  // Verify that the 0'th entry is gone (it's oldest).
  std::string net_url;
  std::string kmz_path;
  KmzSplit(kMockKmzNet[0].url, &net_url, &kmz_path);
  std::string data;
  CPPUNIT_ASSERT(!kmz_cache_->FetchFromCache(net_url, kmz_path, &data));

  // Verify the other entries are all in cache.
  for (size_t i = 1; i < kMockKmzNetSize; ++i) {
    VerifyContentInCache(kMockKmzNet[i].url, mock_net_data[i]);
  }
}

}  // end namespace kmlengine

TEST_MAIN
