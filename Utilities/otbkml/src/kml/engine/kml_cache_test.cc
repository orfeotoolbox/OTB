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
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/net_cache_test_util.h"
#include "gtest/gtest.h"
#include "kml/dom.h"
#include "kml/engine/location_util.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

static const size_t kCacheSize = 137;

class KmlCacheTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_cache_.reset(new KmlCache(&testdata_net_fetcher_, kCacheSize));
  }

  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<KmlCache> kml_cache_;
};

// Verify the FetchKml() and FetchData() with null/bad arguments.
TEST_F(KmlCacheTest, TestNullBadFetch) {
  const string kEmpty;
  string data;
  ASSERT_FALSE(kml_cache_->FetchKmlAbsolute(kEmpty));
  ASSERT_FALSE(kml_cache_->FetchKmlRelative(kEmpty, kEmpty));
  ASSERT_FALSE(kml_cache_->FetchDataRelative(kEmpty, kEmpty, NULL));
  ASSERT_FALSE(kml_cache_->FetchDataRelative(kEmpty, kEmpty, &data));
  ASSERT_TRUE(data.empty());
  const string kGarbage("this is not a url");
  ASSERT_FALSE(kml_cache_->FetchKmlAbsolute(kGarbage));
  ASSERT_FALSE(kml_cache_->FetchKmlRelative(kGarbage, kGarbage));
  ASSERT_FALSE(kml_cache_->FetchDataRelative(kGarbage, kGarbage, &data));
  ASSERT_TRUE(data.empty());
}

// Verify basic usage of the FetchKml() method on known valid data.
TEST_F(KmlCacheTest, TestBasicFetchKml) {
  // Fetch point-sarnen.kml as it if is a relative NetworkLink from
  // some mythic.kml in the same directory on the same server.
  const string kHost("http://www.example.com/");
  const string kDir("style/weather");
  const string kBaseKml("mythic.kml");
  const string kTargetHref("point-sarnen.kml");
  const string kBaseUrl(kHost + kDir + "/" + kBaseKml);
  const string kTargetUrl(kHost + kDir + "/" + kTargetHref);
  KmlFilePtr kml_file = kml_cache_->FetchKmlRelative(kBaseUrl, kTargetHref);
  // Verify that this file fetches, parses, has the right url, has a Placemark
  // with the given id and is at the given lat,lon.
  ASSERT_TRUE(kml_file);
  ASSERT_EQ(kTargetUrl, kml_file->get_url());
     
  kmldom::PlacemarkPtr placemark =
      AsPlacemark(kml_file->GetObjectById("SZXX0026"));
  ASSERT_TRUE(placemark);
  double lat, lon;
  ASSERT_TRUE(kmlengine::GetFeatureLatLon(placemark, &lat, &lon));
  ASSERT_EQ(46.9, lat);
  ASSERT_EQ(8.23, lon);

  // The preceding should have caused exactly one fetch.
  //ASSERT_EQ(1, testdata_net_fetcher_.get_fetch_count());

  // Verify that fetching it again works fine.
  kml_file = NULL;  // Releases our reference to this KmlFile.
  placemark = NULL;  // Releases our reference to this Placemark
  kml_file = kml_cache_->FetchKmlRelative(kBaseUrl, kTargetHref);
  ASSERT_TRUE(kml_file);
  placemark = AsPlacemark(kml_file->GetObjectById("SZXX0026"));
  ASSERT_TRUE(placemark);

  // The immediately preceding should not have caused a fetch.
  //ASSERT_EQ(1, testdata_net_fetcher_.get_fetch_count());
}

// Verify basic usage of the FetchData() method.
TEST_F(KmlCacheTest, TestBasicFetchData) {
  // Fetch the KML from the previous test, but just as raw data.
  const string kPath("/style/weather/point-sarnen.kml");
  const string kUrl(string("http://host.com/" + kPath));
  string got_content;
  ASSERT_TRUE(kml_cache_->FetchDataRelative(kUrl, kUrl, &got_content));

  // Read this out of the file system to compare.
  string want_content;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(
      kmlbase::File::JoinPaths(DATADIR, kPath), &want_content));

  ASSERT_EQ(want_content, got_content);

  // Fetch again to verify all is well.
  got_content.clear();
  ASSERT_TRUE(kml_cache_->FetchDataRelative(kUrl, kUrl, &got_content));
  ASSERT_EQ(want_content, got_content);
}

// Verify that the URL of the KmlFile created for a fetch of a KMZ for
// the default KML file is as expected.
TEST_F(KmlCacheTest, TestBasicFetchKmz) {
  const string kKmzPath("kmz/radar-animation.kmz");
  const string kKmzUrl(string("http://host.com/" + kKmzPath));
  KmlFilePtr kml_file = kml_cache_->FetchKmlAbsolute(kKmzUrl);
  // The KML file in this KMZ archive is known to be doc.kml
  ASSERT_EQ(string(kKmzUrl + "/doc.kml"), kml_file->get_url());

  // Verify that the URL set for the KmlFile for an explicit KML reference
  // into a KMZ is exactly the same as the fetch URL.
  const string kLevel03131Path("level03/0131.kml");
  const string kLevel03131Url(kKmzUrl + "/" + kLevel03131Path);
  kml_file = kml_cache_->FetchKmlAbsolute(kLevel03131Url);
  ASSERT_EQ(kLevel03131Url, kml_file->get_url());
}

static struct {
  const char* base_url;  // The URL of a KML file.
  const char* target_href;  // The content of an <href>, <styleUrl> in the KML.
  size_t bytes;  // 0 means expected to not exist.
  bool in_kmz;  // verify against TestDataNetFetcher counter
} kTestCases[] = {
  {  // This KMZ has the following 4 images within.
    "http://host.com/kmz/screenoverlay-continents.kmz/doc.kml",
    "pngs/africa.png",
    1863,
    true
  },
  {
    "http://host.com/kmz/screenoverlay-continents.kmz/doc.kml",
    "pngs/europe.png",
    1849,
    true
  },
  {
    "http://host.com/kmz/screenoverlay-continents.kmz/doc.kml",
    "pngs/north-america.png",
    2167,
    true
  },
  {
    "http://host.com/kmz/screenoverlay-continents.kmz/doc.kml",
    "pngs/south-america.png",
    2250,
    true
  },
  {
    "http://host.com/kmz/screenoverlay-continents.kmz/doc.kml",
    "pngs/no-such-file-anywhere.png",
    0,
    false
  },
  {  // This KMZ has nothing but the KML.
    "http://host.com/kmz/rumsey/kml/lc01.kmz/"
        "khvol/temp/Rumsey/North_America/Lewis_and_Clark_1814/kml/01.kml",
    "010.kmz",
    1552,  // TODO 690 something is being to smart an finding the KML in 010.kmz
    false
  },
  {
    "http://host.com/kmz/rumsey/kml/lc01.kmz/"
        "khvol/temp/Rumsey/North_America/Lewis_and_Clark_1814/kml/01.kml",
    "012.kmz",
    1551,  // TODO 692 size of KML insize 012.kmz
    false
  },
  {
    "http://host.com/kmz/rumsey/kml/lc01.kmz/"
        "khvol/temp/Rumsey/North_America/Lewis_and_Clark_1814/kml/01.kml",
    "../imagery/01_4.png",
    6742,
    false
  },
  {
    "http://host.com/kmz/rumsey/kml/lc01.kmz/"
        "khvol/temp/Rumsey/North_America/Lewis_and_Clark_1814/kml/01.kml",
    "../imagery/01_8.png",
    7364,
    false
  }
};

TEST_F(KmlCacheTest, TestFetchDataRelativeTestCases) {
  size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    string data;
    bool status = kml_cache_->FetchDataRelative(kTestCases[i].base_url,
                                                kTestCases[i].target_href,
                                                &data);
    if (kTestCases[i].bytes) {
      ASSERT_TRUE(status);
      ASSERT_EQ(kTestCases[i].bytes, data.size());
    } else {
      ASSERT_FALSE(status);
    }
  }
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
