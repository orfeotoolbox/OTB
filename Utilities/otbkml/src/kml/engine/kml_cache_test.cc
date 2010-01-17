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

// This file contains the unit tests for the KmlCache class.

#include "kml/engine/kml_cache.h"
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/unit_test.h"
#include "kml/dom.h"
#include "kml/engine/location_util.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

static const size_t kCacheSize = 137;

class KmlCacheTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlCacheTest);
  CPPUNIT_TEST(TestNullBadFetch);
  CPPUNIT_TEST(TestBasicFetchKml);
  CPPUNIT_TEST(TestBasicFetchData);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    kml_cache_.reset(new KmlCache(&testdata_net_fetcher_, kCacheSize));
  }

  // Called after each test.
  void tearDown() {
    // intrusive_ptr deletes kml_cache_
  }

 protected:
  void TestNullBadFetch();
  void TestBasicFetchKml();
  void TestBasicFetchData();

 private:
  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<KmlCache> kml_cache_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlCacheTest);

// Verify the FetchKml() and FetchData() with null/bad arguments.
void KmlCacheTest::TestNullBadFetch() {
  const std::string kEmpty;
  std::string data;
  CPPUNIT_ASSERT(!kml_cache_->FetchKml(kEmpty));
  CPPUNIT_ASSERT(!kml_cache_->FetchData(kEmpty, &data));
  CPPUNIT_ASSERT(data.empty());
  const std::string kGarbage("this is not a url");
  CPPUNIT_ASSERT(!kml_cache_->FetchKml(kGarbage));
  CPPUNIT_ASSERT(!kml_cache_->FetchData(kGarbage, &data));
  CPPUNIT_ASSERT(data.empty());
}

// Verify basic usage of the FetchKml() method on known valid data.
void KmlCacheTest::TestBasicFetchKml() {
  const std::string kPath("style/weather/point-sarnen.kml");
  const std::string kUrl(std::string("http://host.com/" + kPath));
  KmlFilePtr kml_file = kml_cache_->FetchKml(kUrl);
  // Verify that this file parses, has a Placemark with the given id and
  // is at the given lat,lon.
  CPPUNIT_ASSERT(kml_file);
  kmldom::PlacemarkPtr placemark =
      AsPlacemark(kml_file->GetObjectById("SZXX0026"));
  CPPUNIT_ASSERT(placemark);
  double lat, lon;
  CPPUNIT_ASSERT(kmlengine::GetFeatureLatLon(placemark, &lat, &lon));
  CPPUNIT_ASSERT_EQUAL(46.9, lat);
  CPPUNIT_ASSERT_EQUAL(8.23, lon);

  // Verify that fetching it again works fine.
  kml_file = NULL;  // Releases our reference to this KmlFile.
  placemark = NULL;  // Releases our reference to this Placemark
  kml_file = kml_cache_->FetchKml(kUrl);
  CPPUNIT_ASSERT(kml_file);
  placemark = AsPlacemark(kml_file->GetObjectById("SZXX0026"));
  CPPUNIT_ASSERT(placemark);
}

// Verify basic usage of the FetchData() method.
void KmlCacheTest::TestBasicFetchData() {
  // Fetch the KML from the previous test, but just as raw data.
  const std::string kPath("style/weather/point-sarnen.kml");
  const std::string kUrl(std::string("http://host.com/" + kPath));
  std::string got_content;
  CPPUNIT_ASSERT(kml_cache_->FetchData(kUrl, &got_content));

  // Read this out of the file system to compare.
  std::string want_content;
  CPPUNIT_ASSERT(kmlbase::File::ReadFileToString(
      kmlbase::File::JoinPaths(DATADIR, kPath), &want_content));

  CPPUNIT_ASSERT_EQUAL(want_content, got_content);

  // Fetch again to verify all is well.
  got_content.clear();
  CPPUNIT_ASSERT(kml_cache_->FetchData(kUrl, &got_content));
  CPPUNIT_ASSERT_EQUAL(want_content, got_content);
}

}  // end namespace kmlengine

TEST_MAIN
