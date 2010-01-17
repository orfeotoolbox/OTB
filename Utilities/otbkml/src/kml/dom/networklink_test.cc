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

// This file contains the unit test for the NetworkLink element.

#include "kml/dom/networklink.h"
#include <string>
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class NetworkLinkTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(NetworkLinkTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestParseUrl);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    networklink_ = KmlFactory::GetFactory()->CreateNetworkLink();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestParseUrl();
  void TestSerialize();

 private:
  NetworkLinkPtr networklink_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(NetworkLinkTest);

void NetworkLinkTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_NetworkLink, networklink_->Type());
  CPPUNIT_ASSERT(networklink_->IsA(Type_NetworkLink));
  CPPUNIT_ASSERT(networklink_->IsA(Type_Feature));
  CPPUNIT_ASSERT(networklink_->IsA(Type_Object));
}

// Verify proper defaults:
void NetworkLinkTest::TestDefaults() {
  CPPUNIT_ASSERT(false == networklink_->has_refreshvisibility());
  CPPUNIT_ASSERT(false == networklink_->get_refreshvisibility());
  CPPUNIT_ASSERT(false == networklink_->has_flytoview());
  CPPUNIT_ASSERT(false == networklink_->get_flytoview());
  CPPUNIT_ASSERT(false == networklink_->has_link());
  CPPUNIT_ASSERT(NULL == networklink_->get_link());
}

// Verify setting default makes has_xxx() true:
void NetworkLinkTest::TestSetToDefaultValues() {
  TestDefaults();
  networklink_->set_refreshvisibility(networklink_->get_refreshvisibility());
  CPPUNIT_ASSERT(networklink_->has_refreshvisibility());
  networklink_->set_flytoview(networklink_->get_flytoview());
  CPPUNIT_ASSERT(networklink_->has_flytoview());
  networklink_->set_link(NULL);
  CPPUNIT_ASSERT(false == networklink_->has_link());  // ptr still null
}

// Verify set, get, has, clear:
void NetworkLinkTest::TestSetGetHasClear() {
  // Non-default values:
  bool refreshvisibility = true;
  bool flytoview = true;
  LinkPtr link = KmlFactory::GetFactory()->CreateLink();

  // Set all fields:
  networklink_->set_refreshvisibility(refreshvisibility);
  networklink_->set_flytoview(flytoview);
  networklink_->set_link(link);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(networklink_->has_refreshvisibility());
  CPPUNIT_ASSERT_EQUAL(refreshvisibility,
                       networklink_->get_refreshvisibility());
  CPPUNIT_ASSERT(networklink_->has_flytoview());
  CPPUNIT_ASSERT_EQUAL(flytoview, networklink_->get_flytoview());
  CPPUNIT_ASSERT(networklink_->has_link());
  CPPUNIT_ASSERT(link == networklink_->get_link());

  // Clear all fields:
  networklink_->clear_refreshvisibility();
  networklink_->clear_flytoview();
  networklink_->clear_link();

  // Verify now in default state:
  TestDefaults();
}

// NetworkLink accepts Url as Link.
void NetworkLinkTest::TestParseUrl() {
  const std::string kHref("foo.kml");
  const std::string kNetworkLinkUrl("<NetworkLink><Url><href>");
  const std::string kUrlNetworkLink("</href></Url></NetworkLink>");
  std::string errors;
  ElementPtr root = Parse(kNetworkLinkUrl + kHref + kUrlNetworkLink, &errors);
  CPPUNIT_ASSERT(errors.empty());
  const NetworkLinkPtr networklink = AsNetworkLink(root);
  CPPUNIT_ASSERT(networklink);
  // Verify that the Url was set as the Link.
  CPPUNIT_ASSERT(networklink->has_link());
  CPPUNIT_ASSERT_EQUAL(Type_Url, networklink->get_link()->Type());
  CPPUNIT_ASSERT(networklink->get_link()->has_href());
  CPPUNIT_ASSERT_EQUAL(kHref, networklink->get_link()->get_href());
}

void NetworkLinkTest::TestSerialize() {
  const std::string expect_empty("<NetworkLink/>");
  CPPUNIT_ASSERT_EQUAL(expect_empty, SerializeRaw(networklink_));
  KmlFactory* factory = KmlFactory::GetFactory();
  networklink_->set_link(factory->CreateLink());
  const std::string expect_link("<NetworkLink><Link/></NetworkLink>");
  CPPUNIT_ASSERT_EQUAL(expect_link, SerializeRaw(networklink_));
  networklink_->set_refreshvisibility(false);
  networklink_->set_flytoview(false);
  const std::string expect_all_false(
    "<NetworkLink>"
    "<refreshVisibility>0</refreshVisibility>"
    "<flyToView>0</flyToView>"
    "<Link/>"
    "</NetworkLink>");
  CPPUNIT_ASSERT_EQUAL(expect_all_false, SerializeRaw(networklink_));
  networklink_->set_refreshvisibility(true);
  networklink_->set_flytoview(true);
  const std::string expect_all_true(
    "<NetworkLink>"
    "<refreshVisibility>1</refreshVisibility>"
    "<flyToView>1</flyToView>"
    "<Link/>"
    "</NetworkLink>");
  CPPUNIT_ASSERT_EQUAL(expect_all_true, SerializeRaw(networklink_));
  networklink_->set_name("networklink");
  const std::string expect_feature(
    "<NetworkLink>"
    "<name>networklink</name>"
    "<refreshVisibility>1</refreshVisibility>"
    "<flyToView>1</flyToView>"
    "<Link/>"
    "</NetworkLink>");
}

}  // end namespace kmldom

TEST_MAIN
