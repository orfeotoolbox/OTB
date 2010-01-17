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
#include "kml/base/unit_test.h"

namespace kmldom {

class KmlTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestHint);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    kml_ = KmlFactory::GetFactory()->CreateKml();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetGetHasClear();
  void TestHint();

 private:
  KmlPtr kml_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlTest);

void KmlTest::TestType() {
  CPPUNIT_ASSERT(Type_kml == kml_->Type());
  CPPUNIT_ASSERT(true == kml_->IsA(Type_kml));
}

// Verify proper defaults:
void KmlTest::TestDefaults() {
  CPPUNIT_ASSERT(NULL == kml_->get_networklinkcontrol());
  CPPUNIT_ASSERT(false == kml_->has_networklinkcontrol());
  CPPUNIT_ASSERT(NULL == kml_->get_feature());
  CPPUNIT_ASSERT(false == kml_->has_feature());
}

// Verify set, get, has, clear:
void KmlTest::TestSetGetHasClear() {
  NetworkLinkControl* nlc =
      KmlFactory::GetFactory()->CreateNetworkLinkControl();
  kml_->set_networklinkcontrol(nlc);
  CPPUNIT_ASSERT(nlc == kml_->get_networklinkcontrol());
  CPPUNIT_ASSERT(true == kml_->has_networklinkcontrol());
  kml_->clear_networklinkcontrol();
  TestDefaults();

  Placemark* placemark = KmlFactory::GetFactory()->CreatePlacemark();
  kml_->set_feature(placemark);
  CPPUNIT_ASSERT(placemark == kml_->get_feature());
  CPPUNIT_ASSERT(true == kml_->has_feature());
  kml_->clear_feature();
  TestDefaults();
}

// Verify hint= attr:
void KmlTest::TestHint() {
  CPPUNIT_ASSERT(false == kml_->has_hint());
  CPPUNIT_ASSERT("" == kml_->get_hint());
  std::string hint("target=sky");
  kml_->set_hint(hint);
  CPPUNIT_ASSERT(true == kml_->has_hint());
  CPPUNIT_ASSERT(hint == kml_->get_hint());
  kml_->clear_hint();
  CPPUNIT_ASSERT(false == kml_->has_hint());
  CPPUNIT_ASSERT("" == kml_->get_hint());
}

}  // end namespace kmldom

TEST_MAIN
