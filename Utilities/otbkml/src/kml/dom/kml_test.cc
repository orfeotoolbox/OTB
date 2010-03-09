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

#include "kml/dom/kml.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

namespace kmldom {

class KmlTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_ = KmlFactory::GetFactory()->CreateKml();
  }

  KmlPtr kml_;
};

TEST_F(KmlTest, TestType) {
  ASSERT_EQ(Type_kml, kml_->Type());
  ASSERT_TRUE(kml_->IsA(Type_kml));
}

// Verify proper defaults:
TEST_F(KmlTest, TestDefaults) {
  ASSERT_TRUE(NULL == kml_->get_networklinkcontrol());
  ASSERT_FALSE(kml_->has_networklinkcontrol());
  ASSERT_TRUE(NULL == kml_->get_feature());
  ASSERT_FALSE(kml_->has_feature());
}

// Verify set, get, has, clear:
TEST_F(KmlTest, TestSetGetHasClear) {
  NetworkLinkControl* nlc =
      KmlFactory::GetFactory()->CreateNetworkLinkControl();
  kml_->set_networklinkcontrol(nlc);
  ASSERT_EQ(nlc, kml_->get_networklinkcontrol());
  ASSERT_TRUE(kml_->has_networklinkcontrol());
  kml_->clear_networklinkcontrol();

  Placemark* placemark = KmlFactory::GetFactory()->CreatePlacemark();
  kml_->set_feature(placemark);
  ASSERT_EQ(placemark, kml_->get_feature());
  ASSERT_TRUE(kml_->has_feature());
  kml_->clear_feature();
}

// Verify hint= attr:
TEST_F(KmlTest, TestHint) {
  ASSERT_FALSE(kml_->has_hint());
  ASSERT_EQ(string(""), kml_->get_hint());
  string hint("target=sky");
  kml_->set_hint(hint);
  ASSERT_TRUE(kml_->has_hint());
  ASSERT_EQ(hint, kml_->get_hint());
  kml_->clear_hint();
  ASSERT_FALSE(kml_->has_hint());
  ASSERT_EQ(string(""), kml_->get_hint());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
