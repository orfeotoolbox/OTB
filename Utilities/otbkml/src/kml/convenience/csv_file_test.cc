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
#include "kml/base/unit_test.h"
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/feature_list.h"
#include "kml/engine/bbox.h"
#include "kml/engine/location_util.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlconvenience {

using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmlconvenience::FeatureList;
using std::vector;

class CsvFileTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CsvFileTest);
  CPPUNIT_TEST(TestEmpty);
  CPPUNIT_TEST(TestParseCsvLine);
  CPPUNIT_TEST(TestParseCsvFile);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
  }

  void tearDown() {
  }

 protected:
  void TestEmpty();
  void TestParseCsvLine();
  void TestParseCsvFile();

 private:
  void ComparePlacemark(const PlacemarkPtr& placemark, size_t csv_offset);
};

CPPUNIT_TEST_SUITE_REGISTRATION(CsvFileTest);

void CsvFileTest::TestEmpty() {
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
  {  // 0th line of testdata/kmz/ch-stations.csv
  "41|46.6245|9.57563|Salouf, posta|41 connections",
  41,46.6245,9.57563,"Salouf, posta","41 connections",NULL,
  },
  {
  "74|47.4938|8.44866|Steinmaur, Gemeindehaus|74 connections",
  74,47.4938,8.44866,"Steinmaur, Gemeindehaus","74 connections",NULL,
  },
  {
  "50|46.934|8.61257|Tellsplatte|50 connections|foo.kml#style-me",
  50,46.934,8.61257,"Tellsplatte","50 connections","foo.kml#style-me",
  },
  {
  "2|46.0237|8.85624|Miglieglia SMM|2 connections",
  2,46.0237,8.85624,"Miglieglia SMM","2 connections",NULL,
  },
  {
  "294|46.9913|8.66783|Schlattli, Talstation|294 connections",
  294,46.9913,8.66783,"Schlattli, Talstation","294 connections",NULL,
  },
  { // last line of testdata/kmz/ch-stations.csv
  "60|46.0526|7.14872|La Douay|60 connections",
  60,46.0526,7.14872,"La Douay","60 connections",NULL,
  }
};

void CsvFileTest::ComparePlacemark(const PlacemarkPtr& placemark,
                                   size_t csv_offset) {
  const CsvTestLine& csv_test_line = kCsvTestLines[csv_offset];
  int score = kmlconvenience::GetFeatureScore(placemark);
  double lat, lon;
  CPPUNIT_ASSERT(kmlengine::GetFeatureLatLon(placemark, &lat, &lon));
  CPPUNIT_ASSERT_EQUAL(csv_test_line.score, score);
  CPPUNIT_ASSERT_EQUAL(csv_test_line.lat, lat);
  CPPUNIT_ASSERT_EQUAL(csv_test_line.lon, lon);
  if (csv_test_line.name) {
    CPPUNIT_ASSERT_EQUAL(std::string(csv_test_line.name),
                         placemark->get_name());
  }
  if (csv_test_line.description) {
    CPPUNIT_ASSERT_EQUAL(std::string(csv_test_line.description),
                         placemark->get_description());
  }
  if (csv_test_line.styleurl) {
    CPPUNIT_ASSERT_EQUAL(std::string(csv_test_line.styleurl),
                         placemark->get_styleurl());
  }
}

void CsvFileTest::TestParseCsvLine() {
  FeatureList feature_list;
  CsvFile csv_file(&feature_list);
  const size_t kLineCount = sizeof(kCsvTestLines)/sizeof(kCsvTestLines[0]);
  for (size_t i = 0; i < kLineCount; ++i) {
    csv_file.ParseCsvLine(kCsvTestLines[i].csv_line);
  }
  FolderPtr folder = KmlFactory::GetFactory()->CreateFolder();
  feature_list.Save(folder);
  CPPUNIT_ASSERT_EQUAL(kLineCount, folder->get_feature_array_size());
  for (size_t i = 0; i < kLineCount; ++i) {
    PlacemarkPtr placemark = kmldom::AsPlacemark(
        folder->get_feature_array_at(i));
    CPPUNIT_ASSERT(placemark);
    ComparePlacemark(placemark, i);
  }
}

void CsvFileTest::TestParseCsvFile() {
  FeatureList feature_list;
  CsvFile csv_file(&feature_list);
  const size_t kChStationsLines(15140);
  const std::string kChStationsCsv(std::string(DATADIR) +
                                   std::string("/csv/ch-stations.csv"));
  csv_file.ParseCsvFile(kChStationsCsv.c_str());
  FolderPtr folder = KmlFactory::GetFactory()->CreateFolder();
  feature_list.Save(folder);

  CPPUNIT_ASSERT_EQUAL(kChStationsLines, folder->get_feature_array_size());
  PlacemarkPtr placemark;
  ComparePlacemark(kmldom::AsPlacemark(folder->get_feature_array_at(0)),
                   0);
  const size_t kTestLineCount = sizeof(kCsvTestLines)/sizeof(kCsvTestLines[0]);
  ComparePlacemark(kmldom::AsPlacemark(
                       folder->get_feature_array_at(kChStationsLines-1)),
                   kTestLineCount-1);
}

}  // namespace kmlconvenience

TEST_MAIN
