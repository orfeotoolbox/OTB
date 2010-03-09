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
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "gtest/gtest.h"

namespace kmldom {

class NetworkLinkTest : public testing::Test {
 protected:
  virtual void SetUp() {
    networklink_ = KmlFactory::GetFactory()->CreateNetworkLink();
  }

  NetworkLinkPtr networklink_;
};

TEST_F(NetworkLinkTest, TestType) {
  ASSERT_EQ(Type_NetworkLink, networklink_->Type());
  ASSERT_TRUE(networklink_->IsA(Type_NetworkLink));
  ASSERT_TRUE(networklink_->IsA(Type_Feature));
  ASSERT_TRUE(networklink_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(NetworkLinkTest, TestDefaults) {
  ASSERT_FALSE(networklink_->has_refreshvisibility());
  ASSERT_FALSE(networklink_->get_refreshvisibility());
  ASSERT_FALSE(networklink_->has_flytoview());
  ASSERT_FALSE(networklink_->get_flytoview());
  ASSERT_FALSE(networklink_->has_link());
  ASSERT_TRUE(NULL == networklink_->get_link());
}

// Verify setting default makes has_xxx() true:
TEST_F(NetworkLinkTest, TestSetToDefaultValues) {
  networklink_->set_refreshvisibility(networklink_->get_refreshvisibility());
  ASSERT_TRUE(networklink_->has_refreshvisibility());
  networklink_->set_flytoview(networklink_->get_flytoview());
  ASSERT_TRUE(networklink_->has_flytoview());
  networklink_->set_link(NULL);
  ASSERT_FALSE(networklink_->has_link());  // ptr still null
}

// Verify set, get, has, clear:
TEST_F(NetworkLinkTest, TestSetGetHasClear) {
  // Non-default values:
  bool refreshvisibility = true;
  bool flytoview = true;
  LinkPtr link = KmlFactory::GetFactory()->CreateLink();

  // Set all fields:
  networklink_->set_refreshvisibility(refreshvisibility);
  networklink_->set_flytoview(flytoview);
  networklink_->set_link(link);

  // Verify getter and has_xxx():
  ASSERT_TRUE(networklink_->has_refreshvisibility());
  ASSERT_EQ(refreshvisibility, networklink_->get_refreshvisibility());
  ASSERT_TRUE(networklink_->has_flytoview());
  ASSERT_EQ(flytoview, networklink_->get_flytoview());
  ASSERT_TRUE(networklink_->has_link());
  ASSERT_TRUE(link == networklink_->get_link());

  // Clear all fields:
  networklink_->clear_refreshvisibility();
  networklink_->clear_flytoview();
  networklink_->clear_link();
}

// NetworkLink accepts Url as Link.
TEST_F(NetworkLinkTest, TestParseUrl) {
  const string kHref("foo.kml");
  const string kNetworkLinkUrl("<NetworkLink><Url><href>");
  const string kUrlNetworkLink("</href></Url></NetworkLink>");
  string errors;
  ElementPtr root = Parse(kNetworkLinkUrl + kHref + kUrlNetworkLink, &errors);
  ASSERT_TRUE(errors.empty());
  const NetworkLinkPtr networklink = AsNetworkLink(root);
  ASSERT_TRUE(networklink);
  // Verify that the Url was set as the Link.
  ASSERT_TRUE(networklink->has_link());
  ASSERT_EQ(Type_Url, networklink->get_link()->Type());
  ASSERT_TRUE(networklink->get_link()->has_href());
  ASSERT_EQ(kHref, networklink->get_link()->get_href());
}

TEST_F(NetworkLinkTest, TestSerialize) {
  const string expect_empty("<NetworkLink/>");
  ASSERT_EQ(expect_empty, SerializeRaw(networklink_));
  KmlFactory* factory = KmlFactory::GetFactory();
  networklink_->set_link(factory->CreateLink());
  const string expect_link("<NetworkLink><Link/></NetworkLink>");
  ASSERT_EQ(expect_link, SerializeRaw(networklink_));
  networklink_->set_refreshvisibility(false);
  networklink_->set_flytoview(false);
  const string expect_all_false(
    "<NetworkLink>"
    "<refreshVisibility>0</refreshVisibility>"
    "<flyToView>0</flyToView>"
    "<Link/>"
    "</NetworkLink>");
  ASSERT_EQ(expect_all_false, SerializeRaw(networklink_));
  networklink_->set_refreshvisibility(true);
  networklink_->set_flytoview(true);
  const string expect_all_true(
    "<NetworkLink>"
    "<refreshVisibility>1</refreshVisibility>"
    "<flyToView>1</flyToView>"
    "<Link/>"
    "</NetworkLink>");
  ASSERT_EQ(expect_all_true, SerializeRaw(networklink_));
  networklink_->set_name("networklink");
  const string expect_feature(
    "<NetworkLink>"
    "<name>networklink</name>"
    "<refreshVisibility>1</refreshVisibility>"
    "<flyToView>1</flyToView>"
    "<Link/>"
    "</NetworkLink>");
}

TEST_F(NetworkLinkTest, TestSerializeWithId) {
  const string kId("networklink-id");
  networklink_->set_id(kId);
  ASSERT_EQ(string("<NetworkLink id=\"" + kId + "\"/>"),
            SerializeRaw(networklink_));
  networklink_->clear_id();
  networklink_->set_targetid(kId);
  ASSERT_EQ(string("<NetworkLink targetId=\"" + kId + "\"/>"),
            SerializeRaw(networklink_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
