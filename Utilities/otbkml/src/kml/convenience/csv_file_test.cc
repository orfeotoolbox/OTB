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

// This file contains the unit tests for the CsvFile class.

#include "kml/convenience/csv_file.h"
#include <vector>
#include "gtest/gtest.h"
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/feature_list.h"
#include "kml/engine/bbox.h"
#include "kml/engine/location_util.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlconvenience {

using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmlconvenience::FeatureList;
using std::vector;

class CsvFileTest : public testing::Test {
 protected:
  void ComparePlacemark(const PlacemarkPtr& placemark, size_t csv_offset);
};

TEST_F(CsvFileTest, TestEmpty) {
  FeatureList feature_list;
  CsvFile cvs_file(&feature_list);
}

static const struct CsvTestLine {
  const char* csv_line;
  const int score;
  const double lat;
  const double lon;
  const char* name;
  const char* description;
  const char* styleurl;
} kCsvTestLines[] = {
  // 0th line of testdata/csv/data.csv
  {
    "1733|34.7339215|-109.731499|Potter Mesa|1733 meters Apache AZ",
    1733,
    34.7339215,
    -109.731499,
    "Potter Mesa",
    "1733 meters Apache AZ",
    NULL
  },
  // last line of testdata/csv/data.csv
  {
    "2143|34.0631006|-110.0189953|Big Mountain|2143 meters Navajo AZ",
    2143,
    34.0631006,
    -110.0189953,
    "Big Mountain",
    "2143 meters Navajo AZ",
    NULL
  }
};

void CsvFileTest::ComparePlacemark(const PlacemarkPtr& placemark,
                                   size_t csv_offset) {
  const CsvTestLine& csv_test_line = kCsvTestLines[csv_offset];
  int score = kmlconvenience::GetFeatureScore(placemark);
  double lat, lon;
  ASSERT_TRUE(kmlengine::GetFeatureLatLon(placemark, &lat, &lon));
  ASSERT_EQ(csv_test_line.score, score);
  ASSERT_EQ(csv_test_line.lat, lat);
  ASSERT_EQ(csv_test_line.lon, lon);
  if (csv_test_line.name) {
    ASSERT_EQ(string(csv_test_line.name),
                         placemark->get_name());
  }
  if (csv_test_line.description) {
    ASSERT_EQ(string(csv_test_line.description),
                         placemark->get_description());
  }
  if (csv_test_line.styleurl) {
    ASSERT_EQ(string(csv_test_line.styleurl),
                         placemark->get_styleurl());
  }
}

TEST_F(CsvFileTest, TestParseCsvLine) {
  FeatureList feature_list;
  CsvFile csv_file(&feature_list);
  const size_t kLineCount = sizeof(kCsvTestLines)/sizeof(kCsvTestLines[0]);
  for (size_t i = 0; i < kLineCount; ++i) {
    csv_file.ParseCsvLine(kCsvTestLines[i].csv_line);
  }
  FolderPtr folder = KmlFactory::GetFactory()->CreateFolder();
  feature_list.Save(folder);
  ASSERT_EQ(kLineCount, folder->get_feature_array_size());
  for (size_t i = 0; i < kLineCount; ++i) {
    PlacemarkPtr placemark = kmldom::AsPlacemark(
        folder->get_feature_array_at(i));
    ASSERT_TRUE(placemark);
    ComparePlacemark(placemark, i);
  }
}

TEST_F(CsvFileTest, TestParseCsvFile) {
  FeatureList feature_list;
  CsvFile csv_file(&feature_list);
  const size_t kTestCsvLines(101);
  const string kTestCsv(string(DATADIR) +
                             string("/csv/test.csv"));
  csv_file.ParseCsvFile(kTestCsv.c_str());
  FolderPtr folder = KmlFactory::GetFactory()->CreateFolder();
  feature_list.Save(folder);

  ASSERT_EQ(kTestCsvLines, folder->get_feature_array_size());
  PlacemarkPtr placemark;
  ComparePlacemark(kmldom::AsPlacemark(folder->get_feature_array_at(0)),
                   0);
  const size_t kTestLineCount = sizeof(kCsvTestLines)/sizeof(kCsvTestLines[0]);
  ComparePlacemark(kmldom::AsPlacemark(
                       folder->get_feature_array_at(kTestCsvLines-1)),
                   kTestLineCount-1);
}

}  // namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
