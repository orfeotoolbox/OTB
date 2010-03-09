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
#include "gtest/gtest.h"

namespace kmlengine {

class HrefTest : public testing::Test {
 protected:
  void VerifyString(const char* want, const string& got);
};

// Verify usage for a basic absolute href.
TEST_F(HrefTest, TestBasicAbsolute) {
  const string kHttp("http");
  const string kFooCom("foo.com");
  const string kIndexKml("index.kml");
  Href href(kHttp + "://" + kFooCom + "/" + kIndexKml);
  ASSERT_FALSE(href.IsRelative());
  ASSERT_FALSE(href.has_fragment());
  ASSERT_EQ(kHttp, href.get_scheme());
  ASSERT_EQ(kFooCom, href.get_net_loc());
  ASSERT_EQ(kIndexKml, href.get_path());
}

// Verify basic usage of the get,set,has,clear_scheme methods.
TEST_F(HrefTest, TestSchemeSetGetHasClear) {
  const string kScheme("http");
  Href href;
  href.set_scheme(kScheme);
  ASSERT_TRUE(href.has_scheme());
  ASSERT_EQ(kScheme, href.get_scheme());
  href.clear_scheme();
  ASSERT_FALSE(href.has_scheme());
}

// Verify basic usage of the get,set,has,clear_net_loc methods.
TEST_F(HrefTest, TestNetLocSetGetHasClear) {
  const string kNetLoc("www.google.com");
  Href href;
  href.set_net_loc(kNetLoc);
  ASSERT_TRUE(href.has_net_loc());
  ASSERT_EQ(kNetLoc, href.get_net_loc());
  href.clear_net_loc();
  ASSERT_FALSE(href.has_net_loc());
}

// Verify basic usage of the get,set,has,clear_path methods.
TEST_F(HrefTest, TestPathSetGetHasClear) {
  const string kPath("a/b/c/d/cool.kml");
  Href href;
  href.set_path(kPath);
  ASSERT_TRUE(href.has_path());
  ASSERT_EQ(kPath, href.get_path());
  href.clear_path();
  ASSERT_FALSE(href.has_path());
}

// Verify basic usage of the get,set,has,clear_fragment methods.
TEST_F(HrefTest, TestFragmentSetGetHasClear) {
  const string kFragment("the-fragment");
  Href href;
  href.set_fragment(kFragment);
  ASSERT_TRUE(href.has_fragment());
  ASSERT_EQ(kFragment, href.get_fragment());
  href.clear_fragment();
  ASSERT_FALSE(href.has_fragment());
}

TEST_F(HrefTest, TestFragmentSimple) {
  const string kId("hi");
  Href href("#" + kId);
  ASSERT_FALSE(href.has_scheme());
  ASSERT_FALSE(href.has_path());
  ASSERT_TRUE(href.has_fragment());
  ASSERT_EQ(kId, href.get_fragment());
}

TEST_F(HrefTest, TestRelativeWithFragment) {
  const string kPath("style.kml");
  const string kId("shared-style");
  Href href(kPath + "#" + kId);
  ASSERT_TRUE(href.IsRelative());
  ASSERT_FALSE(href.has_scheme());
  ASSERT_TRUE(href.has_path());
  ASSERT_TRUE(href.has_fragment());
  ASSERT_EQ(kId, href.get_fragment());
  ASSERT_EQ(kPath, href.get_path());
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

// This is a helper function to convert the want string to a string
// and compare to got.  If want is NULL then got is expected to be empty.
void HrefTest::VerifyString(const char* want, const string& got) {
  if (want) {
    ASSERT_EQ(string(want), got);
  } else {
    ASSERT_TRUE(got.empty());
  }
}

// Verify all hrefs in the kTestHrefs table.
TEST_F(HrefTest, TestTestHrefs) {
  size_t count = sizeof(kTestHrefs)/sizeof(kTestHrefs[0]);
  for (size_t i = 0; i < count; ++i) {
    Href href(kTestHrefs[i].href);
    ASSERT_EQ(kTestHrefs[i].is_relative, href.IsRelative());
    VerifyString(kTestHrefs[i].scheme, href.get_scheme());
    VerifyString(kTestHrefs[i].net_loc, href.get_net_loc());
    VerifyString(kTestHrefs[i].path, href.get_path());
    VerifyString(kTestHrefs[i].fragment, href.get_fragment());
  }
}

TEST_F(HrefTest, TestIsFragmentOnly) {
  const string kJustAFragment("#hi-there");
  Href a(kJustAFragment);
  ASSERT_TRUE(a.IsFragmentOnly());

  const string kPathAndFragment("style.kml#shared-style-id");
  Href b(kPathAndFragment);
  ASSERT_FALSE(b.IsFragmentOnly());
}

TEST_F(HrefTest, TestIsRelativePath) {
  const string kJustAFragment("#hi-there");
  Href a(kJustAFragment);
  ASSERT_FALSE(a.IsRelativePath());

  const string kPathAndFragment("style.kml#shared-style-id");
  Href b(kPathAndFragment);
  ASSERT_TRUE(b.IsRelativePath());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
