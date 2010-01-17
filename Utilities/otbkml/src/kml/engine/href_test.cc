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

// This file contains the unit tests for the Href class.

#include "kml/engine/href.h"
#include "kml/base/unit_test.h"

namespace kmlengine {

class HrefTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(HrefTest);
  CPPUNIT_TEST(TestBasicAbsolute);
  CPPUNIT_TEST(TestSchemeSetGetHasClear);
  CPPUNIT_TEST(TestNetLocSetGetHasClear);
  CPPUNIT_TEST(TestPathSetGetHasClear);
  CPPUNIT_TEST(TestFragmentSetGetHasClear);
  CPPUNIT_TEST(TestFragmentSimple);
  CPPUNIT_TEST(TestRelativeWithFragment);
  CPPUNIT_TEST(TestTestHrefs);
  CPPUNIT_TEST(TestIsFragmentOnly);
  CPPUNIT_TEST(TestIsRelativePath);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestBasicAbsolute();
  void TestSchemeSetGetHasClear();
  void TestNetLocSetGetHasClear();
  void TestPathSetGetHasClear();
  void TestFragmentSetGetHasClear();
  void TestFragmentSimple();
  void TestRelativeWithFragment();
  void TestTestHrefs();
  void TestIsFragmentOnly();
  void TestIsRelativePath();

 private:
  void VerifyString(const char* want, const std::string& got);
};

CPPUNIT_TEST_SUITE_REGISTRATION(HrefTest);

// Verify usage for a basic absolute href.
void HrefTest::TestBasicAbsolute() {
  const std::string kHttp("http");
  const std::string kFooCom("foo.com");
  const std::string kIndexKml("index.kml");
  Href href(kHttp + "://" + kFooCom + "/" + kIndexKml);
  CPPUNIT_ASSERT(!href.IsRelative());
  CPPUNIT_ASSERT(!href.has_fragment());
  CPPUNIT_ASSERT_EQUAL(kHttp, href.get_scheme());
  CPPUNIT_ASSERT_EQUAL(kFooCom, href.get_net_loc());
  CPPUNIT_ASSERT_EQUAL(kIndexKml, href.get_path());
}

// Verify basic usage of the get,set,has,clear_scheme methods.
void HrefTest::TestSchemeSetGetHasClear() {
  const std::string kScheme("http");
  Href href;
  href.set_scheme(kScheme);
  CPPUNIT_ASSERT(href.has_scheme());
  CPPUNIT_ASSERT_EQUAL(kScheme, href.get_scheme());
  href.clear_scheme();
  CPPUNIT_ASSERT(!href.has_scheme());
}

// Verify basic usage of the get,set,has,clear_net_loc methods.
void HrefTest::TestNetLocSetGetHasClear() {
  const std::string kNetLoc("www.google.com");
  Href href;
  href.set_net_loc(kNetLoc);
  CPPUNIT_ASSERT(href.has_net_loc());
  CPPUNIT_ASSERT_EQUAL(kNetLoc, href.get_net_loc());
  href.clear_net_loc();
  CPPUNIT_ASSERT(!href.has_net_loc());
}

// Verify basic usage of the get,set,has,clear_path methods.
void HrefTest::TestPathSetGetHasClear() {
  const std::string kPath("a/b/c/d/cool.kml");
  Href href;
  href.set_path(kPath);
  CPPUNIT_ASSERT(href.has_path());
  CPPUNIT_ASSERT_EQUAL(kPath, href.get_path());
  href.clear_path();
  CPPUNIT_ASSERT(!href.has_path());
}

// Verify basic usage of the get,set,has,clear_fragment methods.
void HrefTest::TestFragmentSetGetHasClear() {
  const std::string kFragment("the-fragment");
  Href href;
  href.set_fragment(kFragment);
  CPPUNIT_ASSERT(href.has_fragment());
  CPPUNIT_ASSERT_EQUAL(kFragment, href.get_fragment());
  href.clear_fragment();
  CPPUNIT_ASSERT(!href.has_fragment());
}

void HrefTest::TestFragmentSimple() {
  const std::string kId("hi");
  Href href("#" + kId);
  CPPUNIT_ASSERT(!href.has_scheme());
  CPPUNIT_ASSERT(!href.has_path());
  CPPUNIT_ASSERT(href.has_fragment());
  CPPUNIT_ASSERT_EQUAL(kId, href.get_fragment());
}

void HrefTest::TestRelativeWithFragment() {
  const std::string kPath("style.kml");
  const std::string kId("shared-style");
  Href href(kPath + "#" + kId);
  CPPUNIT_ASSERT(href.IsRelative());
  CPPUNIT_ASSERT(!href.has_scheme());
  CPPUNIT_ASSERT(href.has_path());
  CPPUNIT_ASSERT(href.has_fragment());
  CPPUNIT_ASSERT_EQUAL(kId, href.get_fragment());
  CPPUNIT_ASSERT_EQUAL(kPath, href.get_path());
}

static struct {
  const char* href;
  const bool is_relative;
  const char* scheme;
  const char* net_loc;
  const char* path;
  const char* fragment;
} kTestHrefs[] = {
  {
  "http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png",
  false,
  "http",
  "maps.google.com",
  "mapfiles/kml/pushpin/ylw-pushpin.png",
  NULL
  },
  {
  "#msn_ylw-pushpin",
  true,
  NULL,
  NULL,
  NULL,
  "msn_ylw-pushpin"
  },
  {
  "files/zermatt.jpg",
  true,
  NULL,
  NULL,
  "files/zermatt.jpg",
  NULL
  }
};

// This is a helper function to convert the want string to a std::string
// and compare to got.  If want is NULL then got is expected to be empty.
void HrefTest::VerifyString(const char* want, const std::string& got) {
  if (want) {
    CPPUNIT_ASSERT_EQUAL(std::string(want), got);
  } else {
    CPPUNIT_ASSERT(got.empty());
  }
}

// Verify all hrefs in the kTestHrefs table.
void HrefTest::TestTestHrefs() {
  size_t count = sizeof(kTestHrefs)/sizeof(kTestHrefs[0]);
  for (size_t i = 0; i < count; ++i) {
    Href href(kTestHrefs[i].href);
    CPPUNIT_ASSERT_EQUAL(kTestHrefs[i].is_relative, href.IsRelative());
    VerifyString(kTestHrefs[i].scheme, href.get_scheme());
    VerifyString(kTestHrefs[i].net_loc, href.get_net_loc());
    VerifyString(kTestHrefs[i].path, href.get_path());
    VerifyString(kTestHrefs[i].fragment, href.get_fragment());
  }
}

void HrefTest::TestIsFragmentOnly() {
  const std::string kJustAFragment("#hi-there");
  Href a(kJustAFragment);
  CPPUNIT_ASSERT(a.IsFragmentOnly());

  const std::string kPathAndFragment("style.kml#shared-style-id");
  Href b(kPathAndFragment);
  CPPUNIT_ASSERT(!b.IsFragmentOnly());
}

void HrefTest::TestIsRelativePath() {
  const std::string kJustAFragment("#hi-there");
  Href a(kJustAFragment);
  CPPUNIT_ASSERT(!a.IsRelativePath());

  const std::string kPathAndFragment("style.kml#shared-style-id");
  Href b(kPathAndFragment);
  CPPUNIT_ASSERT(b.IsRelativePath());
}

}  // end namespace kmlengine

TEST_MAIN
