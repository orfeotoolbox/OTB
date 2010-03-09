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

// This file contains the unit tests for the GetLinkParents function.

#include "kml/engine/get_link_parents.h"
#include "kml/base/file.h"
#include "gtest/gtest.h"
#include "kml/engine/engine_types.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

class GetLinkParentsTest : public testing::Test {
};

// Verify that GetLinks() returns false if given no output vector or bad kml.
TEST_F(GetLinkParentsTest, TestNull) {
  const string nothing;
  ASSERT_FALSE(GetLinkParents(nothing, NULL));
  ElementVector will_remain_empty;
  ASSERT_FALSE(GetLinkParents("parse will fail", &will_remain_empty));
  ASSERT_TRUE(will_remain_empty.empty());
}

// Verify that GetParentLinks finds all kinds of parents of links in a KML file.
TEST_F(GetLinkParentsTest, TestAll) {
  const string kAllLinks = string(DATADIR) + "/links/alllinks.kml";
  string kml;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kAllLinks, &kml));
  ElementVector link_parents;
  ASSERT_TRUE(GetLinkParents(kml, &link_parents));
  // This is obviously exactly matched to the content of alllinks.kml.
  ASSERT_EQ(static_cast<size_t>(7), link_parents.size());
  ASSERT_EQ(kmldom::Type_IconStyle, link_parents[0]->Type());
  ASSERT_EQ(kmldom::Type_ItemIcon, link_parents[1]->Type());
  ASSERT_EQ(kmldom::Type_NetworkLink, link_parents[2]->Type());
  ASSERT_EQ(kmldom::Type_GroundOverlay, link_parents[3]->Type());
  ASSERT_EQ(kmldom::Type_ScreenOverlay, link_parents[4]->Type());
  ASSERT_EQ(kmldom::Type_PhotoOverlay, link_parents[5]->Type());
  ASSERT_EQ(kmldom::Type_Model, link_parents[6]->Type());
#if 0
  // TODO: handle styleUrl(?) and SchemaData
  ASSERT_EQ(string("style.kml#style"), href_vector[6]->Type());
  ASSERT_EQ(string("#myschema"), href_vector[7]);
#endif
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
