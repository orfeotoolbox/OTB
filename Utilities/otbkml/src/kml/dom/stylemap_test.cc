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

#include "kml/dom/stylemap.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class PairTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PairTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    pair_ = KmlFactory::GetFactory()->CreatePair();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  PairPtr pair_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(PairTest);

void PairTest::TestType() {
  CPPUNIT_ASSERT(Type_Pair == pair_->Type());
  CPPUNIT_ASSERT(true == pair_->IsA(Type_Pair));
  CPPUNIT_ASSERT(true == pair_->IsA(Type_Object));
}

// Verify proper defaults:
void PairTest::TestDefaults() {
  CPPUNIT_ASSERT(false == pair_->has_key());
  CPPUNIT_ASSERT(STYLESTATE_NORMAL == pair_->get_key());
  CPPUNIT_ASSERT(false == pair_->has_styleurl());
  CPPUNIT_ASSERT("" == pair_->get_styleurl());
  CPPUNIT_ASSERT(false == pair_->has_styleselector());
  CPPUNIT_ASSERT(NULL == pair_->get_styleselector());
}

// Verify setting default makes has_xxx() true:
void PairTest::TestSetToDefaultValues() {
  TestDefaults();
  pair_->set_key(pair_->get_key());
  CPPUNIT_ASSERT(true == pair_->has_key());
  pair_->set_styleurl(pair_->get_styleurl());
  CPPUNIT_ASSERT(true == pair_->has_styleurl());
  pair_->set_styleselector(NULL);
  CPPUNIT_ASSERT(false == pair_->has_styleselector()); // ptr is null
}

// Verify set, get, has, clear:
void PairTest::TestSetGetHasClear() {
  // Non-default values:
  StyleStateEnum key = STYLESTATE_HIGHLIGHT;
  std::string styleurl("#url");
  StylePtr styleselector(KmlFactory::GetFactory()->CreateStyle());

  // Set all fields:
  pair_->set_key(key);
  pair_->set_styleurl(styleurl);
  pair_->set_styleselector(styleselector);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == pair_->has_key());
  CPPUNIT_ASSERT(key == pair_->get_key());
  CPPUNIT_ASSERT(true == pair_->has_styleurl());
  CPPUNIT_ASSERT(styleurl == pair_->get_styleurl());
  CPPUNIT_ASSERT(true == pair_->has_styleselector());
  CPPUNIT_ASSERT(styleselector== pair_->get_styleselector());

  // Clear all fields:
  pair_->clear_key();
  pair_->clear_styleurl();
  pair_->clear_styleselector();

  // Verify now in default state:
  TestDefaults();
}

class StyleMapTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(StyleMapTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    stylemap_ = KmlFactory::GetFactory()->CreateStyleMap();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestLists();

 private:
  StyleMapPtr stylemap_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(StyleMapTest);

void StyleMapTest::TestType() {
  CPPUNIT_ASSERT(true == stylemap_->IsA(Type_StyleMap));
  CPPUNIT_ASSERT(Type_StyleMap == stylemap_->Type());
  CPPUNIT_ASSERT(true == stylemap_->IsA(Type_StyleSelector));
  CPPUNIT_ASSERT(true == stylemap_->IsA(Type_Object));
}

void StyleMapTest::TestLists() {
  CPPUNIT_ASSERT(0 == stylemap_->get_pair_array_size());
  stylemap_->add_pair(KmlFactory::GetFactory()->CreatePair());
  stylemap_->add_pair(KmlFactory::GetFactory()->CreatePair());
  CPPUNIT_ASSERT(2 == stylemap_->get_pair_array_size());
  CPPUNIT_ASSERT(Type_Pair == stylemap_->get_pair_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_Pair == stylemap_->get_pair_array_at(1)->Type());
}

}  // end namespace kmldom

TEST_MAIN
