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
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/engine/kmz_file.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlengine::KmzFile;
using std::vector;

namespace kmlconvenience {

class KmzCheckLinksTest : public testing::Test {
 protected:
  boost::scoped_ptr<KmzFile> kmz_file_;
};

// Verify that KmzCheckLinks() is not confused by no links in the KML.
TEST_F(KmzCheckLinksTest, TestNoLinks) {
  // doc.kmz has no links.
  const string kNoLinks = string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNoLinks.c_str()));
  ASSERT_TRUE(kmz_file_);
  ASSERT_TRUE(KmzCheckLinks(*kmz_file_, NULL));
}

// Verify that KmzCheckLinks() properly verifies an overlay KMZ made in GE.
TEST_F(KmzCheckLinksTest, TestOverlay) {
  // This is a known good KMZ with two links.
  const string kPhotoLink = string(DATADIR) +
                                 "/kmz/zermatt-photo.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kPhotoLink.c_str()));
  // The KML parses fine.
  ASSERT_TRUE(kmz_file_);
  // And all expected files are within the KMZ.
  vector<string> missing_links;
  ASSERT_TRUE(KmzCheckLinks(*kmz_file_, &missing_links));
  ASSERT_EQ(static_cast<size_t>(0), missing_links.size());
}

// Verify that KmzCheckLinks() properly flags a sabotaged KMZ.
TEST_F(KmzCheckLinksTest, TestBadOverlay) {
  // This is the same as zermatt-photo.kmz but w/o the overlay image.
  const string kNoPhotoLink = string(DATADIR) +
                                   "/kmz/zermatt-photo-bad.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNoPhotoLink.c_str()));
  // The KML parses file.
  ASSERT_TRUE(kmz_file_);
  // But there's a dangling link.
  vector<string> missing_links;
  ASSERT_FALSE(KmzCheckLinks(*kmz_file_, &missing_links));
  ASSERT_EQ(static_cast<size_t>(1), missing_links.size());
  ASSERT_EQ(string("files/zermatt.jpg"), missing_links[0]);
}

// Verify that KmzCheckLinks() returns false on a KMZ w/o a KML file.
TEST_F(KmzCheckLinksTest, TestNull) {
  // There is no KML file in this KMZ file.
  const string kNoKml = string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNoKml.c_str()));
  ASSERT_TRUE(kmz_file_);
  ASSERT_FALSE(KmzCheckLinks(*kmz_file_, NULL));
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
