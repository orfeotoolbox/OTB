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
#include "gtest/gtest.h"

namespace kmldom {

class PairTest : public testing::Test {
 protected:
  virtual void SetUp() {
    pair_ = KmlFactory::GetFactory()->CreatePair();
  }

  PairPtr pair_;
};

TEST_F(PairTest, TestType) {
  ASSERT_EQ(Type_Pair, pair_->Type());
  ASSERT_TRUE(pair_->IsA(Type_Pair));
  ASSERT_TRUE(pair_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(PairTest, TestDefaults) {
  ASSERT_FALSE(pair_->has_key());
  ASSERT_EQ(STYLESTATE_NORMAL, pair_->get_key());
  ASSERT_FALSE(pair_->has_styleurl());
  ASSERT_EQ(string(""), pair_->get_styleurl());
  ASSERT_FALSE(pair_->has_styleselector());
  ASSERT_TRUE(NULL == pair_->get_styleselector());
}

// Verify setting default makes has_xxx() true:
TEST_F(PairTest, TestSetToDefaultValues) {
  pair_->set_key(pair_->get_key());
  ASSERT_TRUE(pair_->has_key());
  pair_->set_styleurl(pair_->get_styleurl());
  ASSERT_TRUE(pair_->has_styleurl());
  pair_->set_styleselector(NULL);
  ASSERT_FALSE(pair_->has_styleselector()); // ptr is null
}

// Verify set, get, has, clear:
TEST_F(PairTest, TestSetGetHasClear) {
  // Non-default values:
  StyleStateEnum key = STYLESTATE_HIGHLIGHT;
  string styleurl("#url");
  StylePtr styleselector(KmlFactory::GetFactory()->CreateStyle());

  // Set all fields:
  pair_->set_key(key);
  pair_->set_styleurl(styleurl);
  pair_->set_styleselector(styleselector);

  // Verify getter and has_xxx():
  ASSERT_TRUE(pair_->has_key());
  ASSERT_EQ(key, pair_->get_key());
  ASSERT_TRUE(pair_->has_styleurl());
  ASSERT_EQ(styleurl, pair_->get_styleurl());
  ASSERT_TRUE(pair_->has_styleselector());
  ASSERT_EQ(styleselector, pair_->get_styleselector());

  // Clear all fields:
  pair_->clear_key();
  pair_->clear_styleurl();
  pair_->clear_styleselector();
}

class StyleMapTest : public testing::Test {
 protected:
  virtual void SetUp() {
    stylemap_ = KmlFactory::GetFactory()->CreateStyleMap();
  }

  StyleMapPtr stylemap_;
};

TEST_F(StyleMapTest, TestType) {
  ASSERT_EQ(Type_StyleMap, stylemap_->Type());
  ASSERT_TRUE(stylemap_->IsA(Type_StyleMap));
  ASSERT_TRUE(stylemap_->IsA(Type_StyleSelector));
  ASSERT_TRUE(stylemap_->IsA(Type_Object));
}

TEST_F(StyleMapTest, TestLists) {
  ASSERT_EQ(static_cast<size_t>(0), stylemap_->get_pair_array_size());
  stylemap_->add_pair(KmlFactory::GetFactory()->CreatePair());
  stylemap_->add_pair(KmlFactory::GetFactory()->CreatePair());
  ASSERT_EQ(static_cast<size_t>(2), stylemap_->get_pair_array_size());
  ASSERT_EQ(Type_Pair, stylemap_->get_pair_array_at(0)->Type());
  ASSERT_EQ(Type_Pair, stylemap_->get_pair_array_at(1)->Type());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
