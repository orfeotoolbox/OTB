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

#include "kml/base/net_cache.h"
#include "kml/base/memory_file.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/referent.h"
#include "kml/base/unit_test.h"

namespace kmlbase {

static const size_t kSize0 = 0;
static const size_t kSize1 = 1;

// This NetCache uses TestDataNetFetcher which maps each URL path to a file
// under testdata.  The CacheItem is a MemoryFile which holds the file content.
const size_t kTestDataNetCacheSize = 10;
typedef NetCache<MemoryFile> TestDataNetCache;

// This NetCache CacheItem is an empty which saves no data, however an
// empty string indicates that no NullCacheItem is to be created.
class NullCacheItem : public Referent {
 public:
  static NullCacheItem* CreateFromString(const std::string& data) {
    return data.empty() ? NULL : new NullCacheItem;
  }
};

typedef boost::intrusive_ptr<NullCacheItem> NullCacheItemPtr;

// This NetCache CacheItem has instrumentation to track creation/deletion.
static size_t instrumented_cache_item_count;
class InstrumentedCacheItem : public Referent {
 public:
   static InstrumentedCacheItem* CreateFromString(const std::string& data) {
     return new InstrumentedCacheItem(data);
   }

   const std::string& get_content() const {
     return content_;
   }

 private:
  InstrumentedCacheItem(const std::string& content) : content_(content) {
    ++instrumented_cache_item_count;
  }
  ~InstrumentedCacheItem() {
    --instrumented_cache_item_count;
  }

  std::string content_;
};

typedef boost::intrusive_ptr<InstrumentedCacheItem> InstrumentedCacheItemPtr;

// Since the default NetFetcher always returns false this cache will never
// accept content.  The size is set to non-zero to verify that cache internal
// limits are not the limiter for this behavior.
const size_t kNullNetCacheSize = 1;
typedef NetCache<NullCacheItem> NullNetCache;

// This NetFetcher simply sets the output data to the url itself.
class UrlDataNetFetcher : public NetFetcher {
 public:
  bool FetchUrl(const std::string& url, std::string* data) const {
    if (data) {
      *data = url;
      return true;
    }
    return false;
  }
};

// This NetCache essentially maps each URL to a MemoryFile
// whose content is that URL.
const size_t kUrlDataNetCacheSize = 1234;
typedef NetCache<MemoryFile> UrlDataNetCache;

class NetCacheTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(NetCacheTest);
  CPPUNIT_TEST(TestBasicSize);
  CPPUNIT_TEST(TestBasicFetch);
  CPPUNIT_TEST(TestBasicLookUp);
  CPPUNIT_TEST(TestBasicSave);
  CPPUNIT_TEST(TestBasicDelete);
  CPPUNIT_TEST(TestBasicRemoveOldest);
  CPPUNIT_TEST(TestOverflow);
  CPPUNIT_TEST(TestDeleteCache);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    null_net_cache_.reset(new NullNetCache(&null_net_fetcher_,
                                           kNullNetCacheSize));
    testdata_net_cache_.reset(new TestDataNetCache(&testdata_net_fetcher_,
                                                   kTestDataNetCacheSize));
    url_data_net_cache_.reset(new UrlDataNetCache(&url_data_net_fetcher_,
                                                  kUrlDataNetCacheSize));
  }

 protected:
  void TestBasicSize();
  void TestBasicFetch();
  void TestBasicLookUp();
  void TestBasicSave();
  void TestBasicDelete();
  void TestBasicRemoveOldest();
  void TestOverflow();
  void TestDeleteCache();

 private:
  NetFetcher null_net_fetcher_;
  boost::scoped_ptr<NullNetCache> null_net_cache_;
  TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<TestDataNetCache> testdata_net_cache_;
  UrlDataNetFetcher url_data_net_fetcher_;
  boost::scoped_ptr<UrlDataNetCache> url_data_net_cache_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(NetCacheTest);

// Verif very basic usage of the Size() method.
void NetCacheTest::TestBasicSize() {
  CPPUNIT_ASSERT(kNullNetCacheSize >= null_net_cache_->Size());
}

// Verify basic usage of the Fetch() method.
void NetCacheTest::TestBasicFetch() {
  const std::string kUrl("http://host.com/style/simple.kml");
  // Fetch always fails on NullNetCache.
  CPPUNIT_ASSERT(!null_net_cache_->Fetch(kUrl));
  CPPUNIT_ASSERT_EQUAL(kSize0, null_net_cache_->Size());
  // Fetch of a valid testdata path succeeds.
  CPPUNIT_ASSERT(testdata_net_cache_->Fetch(kUrl));
  // TODO read the test file directly and compare content
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), testdata_net_cache_->Size());
  // Fetch on UrlDataNetCache returns URL.
  CPPUNIT_ASSERT_EQUAL(kUrl, url_data_net_cache_->Fetch(kUrl)->get_content());
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), url_data_net_cache_->Size());
}

// Verify basic usage of the LookUp() method.
void NetCacheTest::TestBasicLookUp() {
  const std::string kUrl("http://host.com/style/simple.kml");
  // Verify that initially all caches return false for a LookUp of this URL.
  CPPUNIT_ASSERT(!null_net_cache_->LookUp(kUrl));
  CPPUNIT_ASSERT(!testdata_net_cache_->LookUp(kUrl));
  CPPUNIT_ASSERT(!url_data_net_cache_->LookUp(kUrl));
  // Fetch this URL into the cache in those caches that save content.
  CPPUNIT_ASSERT(testdata_net_cache_->Fetch(kUrl));
  CPPUNIT_ASSERT(url_data_net_cache_->Fetch(kUrl));
  // Verify that these caches return true now on LookUp.
  CPPUNIT_ASSERT(testdata_net_cache_->LookUp(kUrl));
  CPPUNIT_ASSERT(url_data_net_cache_->LookUp(kUrl));
}

// Verify basic usage of the Save() method.
void NetCacheTest::TestBasicSave() {
  const std::string kUrl("http://host.com/style/simple.kml");
  const std::string kContent("some random blob of data");
  MemoryFilePtr test_data_item = MemoryFile::CreateFromString(kContent);
  CPPUNIT_ASSERT(url_data_net_cache_->Save(kUrl, test_data_item));
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), url_data_net_cache_->Size());
  CPPUNIT_ASSERT(url_data_net_cache_->LookUp(kUrl));
  CPPUNIT_ASSERT_EQUAL(kContent,
                       url_data_net_cache_->Fetch(kUrl)->get_content());
}

// Verify basic usage of the Delete() method.
void NetCacheTest::TestBasicDelete() {
  const std::string kUrl("http://host.com/style/simple.kml");
  CPPUNIT_ASSERT(url_data_net_cache_->Fetch(kUrl));
  CPPUNIT_ASSERT(url_data_net_cache_->Delete(kUrl));
  CPPUNIT_ASSERT_EQUAL(kSize0, url_data_net_cache_->Size());
  CPPUNIT_ASSERT(!url_data_net_cache_->LookUp(kUrl));
}

// Verify basic usage of the RemoveOldest method.
void NetCacheTest::TestBasicRemoveOldest() {
  const std::string kUrl("http://host.com/style/simple.kml");
  CPPUNIT_ASSERT(url_data_net_cache_->Fetch(kUrl));
  CPPUNIT_ASSERT(url_data_net_cache_->RemoveOldest());
  CPPUNIT_ASSERT_EQUAL(kSize0, url_data_net_cache_->Size());
  CPPUNIT_ASSERT(!url_data_net_cache_->LookUp(kUrl));
}

// TODO move to base/string_util.h
template<typename T>
inline std::string ToString(T value) {
  std::stringstream ss;
  ss.precision(15);
  ss << value;
  return ss.str();
}

// Verify that the NetCache never exceeds the maximum configured size and
// that it drains fully.
void NetCacheTest::TestOverflow() {
  for (size_t i = 0; i < kUrlDataNetCacheSize*2; ++i) {
    const std::string kUrl("http://host.com/" + ToString(i));
    MemoryFilePtr url_data = url_data_net_cache_->Fetch(kUrl);
    CPPUNIT_ASSERT(url_data);  // UrlDataNetFetcher never fails.
    // UrlDataNetFetcher simply uses the url as the content.
    CPPUNIT_ASSERT_EQUAL(kUrl, url_data->get_content());
    // The most recently Fetch()'ed url is guaranteed to bein the cache.
    CPPUNIT_ASSERT(url_data_net_cache_->LookUp(kUrl));
    const size_t want_size =
        i < kUrlDataNetCacheSize ? i + 1 : kUrlDataNetCacheSize;
    CPPUNIT_ASSERT_EQUAL(want_size, url_data_net_cache_->Size());
  }
  // Cache is full so LookUp will succeed on all URLs in the 2nd half of the
  // the test range.
  for (size_t i = kUrlDataNetCacheSize; i < kUrlDataNetCacheSize*2; ++i) {
    const std::string kUrl("http://host.com/" + ToString(i));
    CPPUNIT_ASSERT(url_data_net_cache_->LookUp(kUrl));
  }
  // RemoveOldest() removes items one at a time.
  for (size_t i = 0; i < kUrlDataNetCacheSize; ++i) {
    CPPUNIT_ASSERT(url_data_net_cache_->RemoveOldest());
    CPPUNIT_ASSERT_EQUAL(kUrlDataNetCacheSize - i - 1,
                         url_data_net_cache_->Size());
  }

  // Cache is empty so LookUp will fail on all URLs.
  for (size_t i = 0; i < kUrlDataNetCacheSize*2; ++i) {
    const std::string kUrl("http://host.com/" + ToString(i));
    CPPUNIT_ASSERT(!url_data_net_cache_->LookUp(kUrl));
  }
  
  // At size 0 RemoveOldest returns false.
  CPPUNIT_ASSERT_EQUAL(kSize0, url_data_net_cache_->Size());
  CPPUNIT_ASSERT(!url_data_net_cache_->RemoveOldest());
}

// Verify that destruction of the cache destroys all items in the cache.
void NetCacheTest::TestDeleteCache() {
  // Verify proper operation of the internal InstrumentedCacheItem class.
  CPPUNIT_ASSERT_EQUAL(kSize0, instrumented_cache_item_count);
  const std::string kContent("some random stuf");
  InstrumentedCacheItemPtr item =
      InstrumentedCacheItem::CreateFromString(kContent);
  CPPUNIT_ASSERT_EQUAL(kContent, item->get_content());
  CPPUNIT_ASSERT_EQUAL(kSize1, instrumented_cache_item_count);
  item = NULL;  // Forces delete of object managed by intrusive_ptr.
  CPPUNIT_ASSERT_EQUAL(kSize0, instrumented_cache_item_count);

  {
    NetCache<InstrumentedCacheItem> net_cache(&url_data_net_fetcher_,
                                               kUrlDataNetCacheSize);
    CPPUNIT_ASSERT_EQUAL(kSize0, instrumented_cache_item_count);
    for (size_t i = 0; i < kUrlDataNetCacheSize*2; ++i) {
      const std::string kUrl("http://host.com/" + ToString(i));
      InstrumentedCacheItemPtr url_data = net_cache.Fetch(kUrl);
      const size_t want_size =
          i < kUrlDataNetCacheSize ? i + 1 : kUrlDataNetCacheSize;
      CPPUNIT_ASSERT_EQUAL(want_size, instrumented_cache_item_count);
      CPPUNIT_ASSERT_EQUAL(want_size, net_cache.Size());
    }
    CPPUNIT_ASSERT(net_cache.RemoveOldest());
    CPPUNIT_ASSERT_EQUAL(kUrlDataNetCacheSize - 1,
                         instrumented_cache_item_count);
    CPPUNIT_ASSERT_EQUAL(kUrlDataNetCacheSize - 1,
                         net_cache.Size());
  }
  // End of scope deletes net_cache_ and all CacheItems
  CPPUNIT_ASSERT_EQUAL(kSize0, instrumented_cache_item_count);
}

}  // end namespace kmlengine

TEST_MAIN
