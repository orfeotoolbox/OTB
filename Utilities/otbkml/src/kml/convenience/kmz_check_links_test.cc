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

// This file contains the unit tests for the KmzCheckLinks() function.

#include "kml/convenience/kmz_check_links.h"
#include <string>
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/engine/kmz_file.h"
#include "kml/base/unit_test.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlengine::KmzFile;
using std::vector;

namespace kmlconvenience {

class KmzCheckLinksTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmzCheckLinksTest);
  CPPUNIT_TEST(TestNoLinks);
  CPPUNIT_TEST(TestNull);
  CPPUNIT_TEST(TestOverlay);
  CPPUNIT_TEST(TestBadOverlay);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNoLinks();
  void TestNull();
  void TestOverlay();
  void TestBadOverlay();

 public:
  void setUp() {
  }

  void tearDown() {
  }

 private:
  boost::scoped_ptr<KmzFile> kmz_file_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmzCheckLinksTest);

// Verify that KmzCheckLinks() is not confused by no links in the KML.
void KmzCheckLinksTest::TestNoLinks() {
  // doc.kmz has no links.
  const std::string kNoLinks = std::string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNoLinks.c_str()));
  CPPUNIT_ASSERT(kmz_file_);
  CPPUNIT_ASSERT(KmzCheckLinks(*kmz_file_, NULL));
}

// Verify that KmzCheckLinks() properly verifies an overlay KMZ made in GE.
void KmzCheckLinksTest::TestOverlay() {
  // This is a known good KMZ with two links.
  const std::string kPhotoLink = std::string(DATADIR) +
                                 "/kmz/zermatt-photo.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kPhotoLink.c_str()));
  // The KML parses fine.
  CPPUNIT_ASSERT(kmz_file_);
  // And all expected files are within the KMZ.
  vector<std::string> missing_links;
  CPPUNIT_ASSERT(KmzCheckLinks(*kmz_file_, &missing_links));
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), missing_links.size());
}

// Verify that KmzCheckLinks() properly flags a sabotaged KMZ.
void KmzCheckLinksTest::TestBadOverlay() {
  // This is the same as zermatt-photo.kmz but w/o the overlay image.
  const std::string kNoPhotoLink = std::string(DATADIR) +
                                   "/kmz/zermatt-photo-bad.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNoPhotoLink.c_str()));
  // The KML parses file.
  CPPUNIT_ASSERT(kmz_file_);
  // But there's a dangling link.
  vector<std::string> missing_links;
  CPPUNIT_ASSERT(!KmzCheckLinks(*kmz_file_, &missing_links));
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), missing_links.size());
  CPPUNIT_ASSERT_EQUAL(std::string("files/zermatt.jpg"), missing_links[0]);
}

// Verify that KmzCheckLinks() returns false on a KMZ w/o a KML file.
void KmzCheckLinksTest::TestNull() {
  // There is no KML file in this KMZ file.
  const std::string kNoKml = std::string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNoKml.c_str()));
  CPPUNIT_ASSERT(kmz_file_);
  CPPUNIT_ASSERT(!KmzCheckLinks(*kmz_file_, NULL));
}

}  // end namespace kmlconvenience

TEST_MAIN
