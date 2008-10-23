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
#include "kml/base/unit_test.h"
#include "kml/engine/engine_types.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

class GetLinkParentsTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(GetLinkParentsTest);
  CPPUNIT_TEST(TestNull);
  CPPUNIT_TEST(TestAll);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNull();
  void TestAll();
};

CPPUNIT_TEST_SUITE_REGISTRATION(GetLinkParentsTest);

// Verify that GetLinks() returns false if given no output vector or bad kml.
void GetLinkParentsTest::TestNull() {
  const std::string nothing;
  CPPUNIT_ASSERT(!GetLinkParents(nothing, NULL));
  ElementVector will_remain_empty;
  CPPUNIT_ASSERT(!GetLinkParents("parse will fail", &will_remain_empty));
  CPPUNIT_ASSERT(will_remain_empty.empty());
}

// Verify that GetParentLinks finds all kinds of parents of links in a KML file.
void GetLinkParentsTest::TestAll() {
  const std::string kAllLinks = std::string(DATADIR) + "/links/alllinks.kml";
  std::string kml;
  CPPUNIT_ASSERT(kmlbase::File::ReadFileToString(kAllLinks, &kml));
  ElementVector link_parents;
  CPPUNIT_ASSERT(GetLinkParents(kml, &link_parents));
  // This is obviously exactly matched to the content of alllinks.kml.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), link_parents.size());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_IconStyle, link_parents[0]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_ItemIcon, link_parents[1]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_NetworkLink, link_parents[2]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_GroundOverlay, link_parents[3]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_ScreenOverlay, link_parents[4]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_PhotoOverlay, link_parents[5]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Model, link_parents[6]->Type());
#if 0
  // TODO: handle styleUrl(?) and SchemaData
  CPPUNIT_ASSERT_EQUAL(std::string("style.kml#style"), href_vector[6]->Type());
  CPPUNIT_ASSERT_EQUAL(std::string("#myschema"), href_vector[7]);
#endif
}

}  // end namespace kmlengine

TEST_MAIN
