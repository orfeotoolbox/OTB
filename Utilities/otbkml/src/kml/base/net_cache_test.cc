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
// Uncomment this #define to enable output of timing results.
// #define PRINT_TIME_RESULTS
#ifdef PRINT_TIME_RESULTS
#include <iostream>
#endif
#include "kml/base/memory_file.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/referent.h"
#include "gtest/gtest.h"

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
  static NullCacheItem* CreateFromString(const string& data) {
    return data.empty() ? NULL : new NullCacheItem;
  }
};

typedef boost::intrusive_ptr<NullCacheItem> NullCacheItemPtr;

// This NetCache CacheItem has instrumentation to track creation/deletion.
static size_t instrumented_cache_item_count;
class InstrumentedCacheItem : public Referent {
 public:
   static InstrumentedCacheItem* CreateFromString(const string& data) {
     return new InstrumentedCacheItem(data);
   }

   const string& get_content() const {
     return content_;
   }

 private:
  InstrumentedCacheItem(const string& content) : content_(content) {
    ++instrumented_cache_item_count;
  }
  ~InstrumentedCacheItem() {
    --instrumented_cache_item_count;
  }

  string content_;
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
  bool FetchUrl(const string& url, string* data) const {
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

class NetCacheTest : public testing::Test {
 protected:
  virtual void SetUp() {
    null_net_cache_.reset(new NullNetCache(&null_net_fetcher_,
                                           kNullNetCacheSize));
    testdata_net_cache_.reset(new TestDataNetCache(&testdata_net_fetcher_,
                                                   kTestDataNetCacheSize));
    url_data_net_cache_.reset(new UrlDataNetCache(&url_data_net_fetcher_,
                                                  kUrlDataNetCacheSize));
  }

  NetFetcher null_net_fetcher_;
  boost::scoped_ptr<NullNetCache> null_net_cache_;
  TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<TestDataNetCache> testdata_net_cache_;
  UrlDataNetFetcher url_data_net_fetcher_;
  boost::scoped_ptr<UrlDataNetCache> url_data_net_cache_;
};

// Verify very basic usage of the Size() method.
TEST_F(NetCacheTest, TestBasicSize) {
  ASSERT_TRUE(kNullNetCacheSize >= null_net_cache_->Size());
}

// Verify basic usage of the Fetch() method.
TEST_F(NetCacheTest, TestBasicFetch) {
  const string kUrl("http://host.com/style/simple.kml");
  // Fetch always fails on NullNetCache.
  ASSERT_FALSE(null_net_cache_->Fetch(kUrl));
  ASSERT_EQ(kSize0, null_net_cache_->Size());
  // Fetch of a valid testdata path succeeds.
  ASSERT_TRUE(testdata_net_cache_->Fetch(kUrl));
  // TODO read the test file directly and compare content
  ASSERT_EQ(static_cast<size_t>(1), testdata_net_cache_->Size());
  // Fetch on UrlDataNetCache returns URL.
  ASSERT_EQ(kUrl, url_data_net_cache_->Fetch(kUrl)->get_content());
  ASSERT_EQ(static_cast<size_t>(1), url_data_net_cache_->Size());
}

// Verify basic usage of the LookUp() method.
TEST_F(NetCacheTest, TestBasicLookUp) {
  const string kUrl("http://host.com/style/simple.kml");
  // Verify that initially all caches return false for a LookUp of this URL.
  ASSERT_FALSE(null_net_cache_->LookUp(kUrl));
  ASSERT_FALSE(testdata_net_cache_->LookUp(kUrl));
  ASSERT_FALSE(url_data_net_cache_->LookUp(kUrl));
  // Fetch this URL into the cache in those caches that save content.
  ASSERT_TRUE(testdata_net_cache_->Fetch(kUrl));
  ASSERT_TRUE(url_data_net_cache_->Fetch(kUrl));
  // Verify that these caches return true now on LookUp.
  ASSERT_TRUE(testdata_net_cache_->LookUp(kUrl));
  ASSERT_TRUE(url_data_net_cache_->LookUp(kUrl));
}

// Verify basic usage of the Save() method.
TEST_F(NetCacheTest, TestBasicSave) {
  const string kUrl("http://host.com/style/simple.kml");
  const string kContent("some random blob of data");
  MemoryFilePtr test_data_item = MemoryFile::CreateFromString(kContent);
  ASSERT_TRUE(url_data_net_cache_->Save(kUrl, test_data_item));
  ASSERT_EQ(static_cast<size_t>(1), url_data_net_cache_->Size());
  ASSERT_TRUE(url_data_net_cache_->LookUp(kUrl));
  ASSERT_EQ(kContent,
                       url_data_net_cache_->Fetch(kUrl)->get_content());
}

// Verify basic usage of the Delete() method.
TEST_F(NetCacheTest, TestBasicDelete) {
  const string kUrl("http://host.com/style/simple.kml");
  ASSERT_TRUE(url_data_net_cache_->Fetch(kUrl));
  ASSERT_TRUE(url_data_net_cache_->Delete(kUrl));
  ASSERT_EQ(kSize0, url_data_net_cache_->Size());
  ASSERT_FALSE(url_data_net_cache_->LookUp(kUrl));
}

// Verify basic usage of the RemoveOldest method.
TEST_F(NetCacheTest, TestBasicRemoveOldest) {
  const string kUrl("http://host.com/style/simple.kml");
  ASSERT_TRUE(url_data_net_cache_->Fetch(kUrl));
  ASSERT_TRUE(url_data_net_cache_->RemoveOldest());
  ASSERT_EQ(kSize0, url_data_net_cache_->Size());
  ASSERT_FALSE(url_data_net_cache_->LookUp(kUrl));
}

// TODO move to base/string_util.h
template<typename T>
inline string ToString(T value) {
  std::stringstream ss;
  ss.precision(15);
  ss << value;
  return ss.str();
}

// Verify that the NetCache never exceeds the maximum configured size and
// that it drains fully.
TEST_F(NetCacheTest, TestOverflow) {
  for (size_t i = 0; i < kUrlDataNetCacheSize*2; ++i) {
    const string kUrl("http://host.com/" + ToString(i));
    MemoryFilePtr url_data = url_data_net_cache_->Fetch(kUrl);
    ASSERT_TRUE(url_data);  // UrlDataNetFetcher never fails.
    // UrlDataNetFetcher simply uses the url as the content.
    ASSERT_EQ(kUrl, url_data->get_content());
    // The most recently Fetch()'ed url is guaranteed to bein the cache.
    ASSERT_TRUE(url_data_net_cache_->LookUp(kUrl));
    const size_t want_size =
        i < kUrlDataNetCacheSize ? i + 1 : kUrlDataNetCacheSize;
    ASSERT_EQ(want_size, url_data_net_cache_->Size());
  }
  // Cache is full so LookUp will succeed on all URLs in the 2nd half of the
  // the test range.
  for (size_t i = kUrlDataNetCacheSize; i < kUrlDataNetCacheSize*2; ++i) {
    const string kUrl("http://host.com/" + ToString(i));
    ASSERT_TRUE(url_data_net_cache_->LookUp(kUrl));
  }
  // RemoveOldest() removes items one at a time.
  for (size_t i = 0; i < kUrlDataNetCacheSize; ++i) {
    ASSERT_TRUE(url_data_net_cache_->RemoveOldest());
    ASSERT_EQ(kUrlDataNetCacheSize - i - 1,
                         url_data_net_cache_->Size());
  }

  // Cache is empty so LookUp will fail on all URLs.
  for (size_t i = 0; i < kUrlDataNetCacheSize*2; ++i) {
    const string kUrl("http://host.com/" + ToString(i));
    ASSERT_FALSE(url_data_net_cache_->LookUp(kUrl));
  }
  
  // At size 0 RemoveOldest returns false.
  ASSERT_EQ(kSize0, url_data_net_cache_->Size());
  ASSERT_FALSE(url_data_net_cache_->RemoveOldest());
}

// Verify that destruction of the cache destroys all items in the cache.
TEST_F(NetCacheTest, TestDeleteCache) {
  // Verify proper operation of the internal InstrumentedCacheItem class.
  ASSERT_EQ(kSize0, instrumented_cache_item_count);
  const string kContent("some random stuf");
  InstrumentedCacheItemPtr item =
      InstrumentedCacheItem::CreateFromString(kContent);
  ASSERT_EQ(kContent, item->get_content());
  ASSERT_EQ(kSize1, instrumented_cache_item_count);
  item = NULL;  // Forces delete of object managed by intrusive_ptr.
  ASSERT_EQ(kSize0, instrumented_cache_item_count);

  {
    NetCache<InstrumentedCacheItem> net_cache(&url_data_net_fetcher_,
                                               kUrlDataNetCacheSize);
    ASSERT_EQ(kSize0, instrumented_cache_item_count);
    for (size_t i = 0; i < kUrlDataNetCacheSize*2; ++i) {
      const string kUrl("http://host.com/" + ToString(i));
      InstrumentedCacheItemPtr url_data = net_cache.Fetch(kUrl);
      const size_t want_size =
          i < kUrlDataNetCacheSize ? i + 1 : kUrlDataNetCacheSize;
      ASSERT_EQ(want_size, instrumented_cache_item_count);
      ASSERT_EQ(want_size, net_cache.Size());
    }
    ASSERT_TRUE(net_cache.RemoveOldest());
    ASSERT_EQ(kUrlDataNetCacheSize - 1,
                         instrumented_cache_item_count);
    ASSERT_EQ(kUrlDataNetCacheSize - 1,
                         net_cache.Size());
  }
  // End of scope deletes net_cache_ and all CacheItems
  ASSERT_EQ(kSize0, instrumented_cache_item_count);
}

#ifdef PRINT_TIME_RESULTS
// This is a simple timing test to estimate when the cache_count_ rolls over.
// On a near-zero-latency network such as the one faked in UrlDataNetFetcher's
// FetchUrl (which does no I/O at all) the elapsed time below is 22 seconds on
// 2.33 Ghz Intel Core 2 Duo on a MacBook Pro.
TEST_F(NetCacheTest, TimingTest) {
  time_t start = time(NULL);
  const int count = 1000000;
  for (int i = 0; i < count; ++i) {
    ASSERT_TRUE(url_data_net_cache_->Fetch(ToString(i)));
  }
  time_t elapsed = time(NULL) - start;
  std::cerr << count << " Fetch's in " << elapsed << " seconds" << std::endl;
  // ISO/IEC 988:1999 7.18.2.1
#define UINT64_MAX        18446744073709551615ULL
  std::cerr << "Seconds to roll over " << (UINT64_MAX/count)*elapsed
            << std::endl;
}
#endif

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
