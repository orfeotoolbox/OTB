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

#include "kml/dom/liststyle.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kmldom.h"
#include "gtest/gtest.h"

using kmlbase::Color32;

namespace kmldom {

class ItemIconTest : public testing::Test {
 protected:
  virtual void SetUp() {
    itemicon_ = KmlFactory::GetFactory()->CreateItemIcon();
  }

  ItemIconPtr itemicon_;
};

TEST_F(ItemIconTest, TestType) {
  ASSERT_TRUE(itemicon_->IsA(Type_ItemIcon));
  ASSERT_TRUE(itemicon_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(ItemIconTest, TestDefaults) {
  ASSERT_FALSE(itemicon_->has_state());
  ASSERT_EQ(static_cast<size_t>(1), itemicon_->get_state_array_size());
  ASSERT_EQ(ITEMICONSTATE_OPEN, itemicon_->get_state_array_at(0));
  ASSERT_FALSE(itemicon_->has_href());
  ASSERT_EQ(string(""), itemicon_->get_href());
}

// Verify setting default makes has_xxx() true:
TEST_F(ItemIconTest, TestSetToDefaultValues) {
  itemicon_->add_state(itemicon_->get_state_array_at(0));
  ASSERT_TRUE(itemicon_->has_state());
  itemicon_->set_href(itemicon_->get_href());
  ASSERT_TRUE(itemicon_->has_href());
}

// Verify set, get, has, clear:
TEST_F(ItemIconTest, TestSetGetHasClear) {
  // Non-default values:
  ItemIconStateEnum state = ITEMICONSTATE_ERROR;
  string href("http://example.com/foo.jpg");

  // Set all fields:
  itemicon_->clear_state();
  itemicon_->add_state(state);
  itemicon_->set_href(href);

  // Verify getter and has_xxx():
  ASSERT_TRUE(itemicon_->has_state());
  ASSERT_EQ(state, itemicon_->get_state_array_at(0));
  ASSERT_TRUE(itemicon_->has_href());
  ASSERT_EQ(href, itemicon_->get_href());

  // Clear all fields:
  itemicon_->clear_state();
  itemicon_->clear_href();
}

TEST_F(ItemIconTest, TestStateArrays) {
  ItemIconStateEnum iis_open= ITEMICONSTATE_OPEN;
  ItemIconStateEnum iis_error = ITEMICONSTATE_ERROR;
  ItemIconStateEnum iis_fetching0 = ITEMICONSTATE_FETCHING0;

  // Verify the default constructed state.
  ASSERT_FALSE(itemicon_->has_state());
  ASSERT_EQ(static_cast<size_t>(1), itemicon_->get_state_array_size());
  ASSERT_EQ(iis_open, itemicon_->get_state_array_at(0));

  // Clear <state> array.
  itemicon_->clear_state();
  ASSERT_FALSE(itemicon_->has_state());
  ASSERT_EQ(static_cast<size_t>(0), itemicon_->get_state_array_size());

  // Add an explict error enum.
  itemicon_->add_state(iis_error);
  ASSERT_TRUE(itemicon_->has_state());
  ASSERT_EQ(static_cast<size_t>(1), itemicon_->get_state_array_size());
  ASSERT_EQ(iis_error, itemicon_->get_state_array_at(0));

  // Add a fetching0 to the open.
  itemicon_->add_state(iis_fetching0);
  ASSERT_TRUE(itemicon_->has_state());
  ASSERT_EQ(static_cast<size_t>(2), itemicon_->get_state_array_size());
  ASSERT_EQ(iis_error, itemicon_->get_state_array_at(0));
  ASSERT_EQ(iis_fetching0, itemicon_->get_state_array_at(1));
}

class ListStyleTest : public testing::Test {
 protected:
  virtual void SetUp() {
    liststyle_ = KmlFactory::GetFactory()->CreateListStyle();
  }

  ListStylePtr liststyle_;
};

TEST_F(ListStyleTest, TestType) {
  ASSERT_TRUE(liststyle_->IsA(Type_ListStyle));
  ASSERT_EQ(Type_ListStyle, liststyle_->Type());
  ASSERT_TRUE(liststyle_->IsA(Type_SubStyle));
}

TEST_F(ListStyleTest, TestLists) {
  ASSERT_EQ(static_cast<size_t>(0), liststyle_->get_itemicon_array_size());
  liststyle_->add_itemicon(KmlFactory::GetFactory()->CreateItemIcon());
  liststyle_->add_itemicon(KmlFactory::GetFactory()->CreateItemIcon());
  ASSERT_EQ(static_cast<size_t>(2), liststyle_->get_itemicon_array_size());
  ASSERT_EQ(Type_ItemIcon, liststyle_->get_itemicon_array_at(0)->Type());
  ASSERT_EQ(Type_ItemIcon, liststyle_->get_itemicon_array_at(1)->Type());
}

// Verify proper defaults:
TEST_F(ListStyleTest, TestDefaults) {
  ASSERT_FALSE(liststyle_->has_listitemtype());
  ASSERT_EQ(LISTITEMTYPE_CHECK, liststyle_->get_listitemtype());
  ASSERT_FALSE(liststyle_->has_bgcolor());
  ASSERT_TRUE(Color32(0xffffffff) == liststyle_->get_bgcolor());
  ASSERT_EQ(2, liststyle_->get_maxsnippetlines());
}

// Verify setting default makes has_xxx() true:
TEST_F(ListStyleTest, TestSetToDefaultValues) {
  liststyle_->set_listitemtype(liststyle_->get_listitemtype());
  ASSERT_TRUE(liststyle_->has_listitemtype());
  liststyle_->set_bgcolor(liststyle_->get_bgcolor());
  ASSERT_TRUE(liststyle_->has_bgcolor());
  liststyle_->set_maxsnippetlines(liststyle_->get_maxsnippetlines());
  ASSERT_TRUE(liststyle_->has_maxsnippetlines());
}

// Verify set, get, has, clear:
TEST_F(ListStyleTest, TestSetGetHasClear) {
  // Non-default values:
  ListItemTypeEnum listitemtype = LISTITEMTYPE_CHECKHIDECHILDREN;
  Color32 bgcolor(Color32(0x00112233));
  int maxsnippetlines(3);

  // Set all fields:
  liststyle_->set_listitemtype(listitemtype);
  liststyle_->set_bgcolor(bgcolor);
  liststyle_->set_maxsnippetlines(maxsnippetlines);

  // Verify getter and has_xxx():
  ASSERT_TRUE(liststyle_->has_listitemtype());
  ASSERT_EQ(listitemtype, liststyle_->get_listitemtype());
  ASSERT_TRUE(liststyle_->has_bgcolor());
  ASSERT_TRUE(bgcolor == liststyle_->get_bgcolor());
  ASSERT_TRUE(liststyle_->has_maxsnippetlines());
  ASSERT_EQ(maxsnippetlines, liststyle_->get_maxsnippetlines());

  // Clear all fields:
  liststyle_->clear_listitemtype();
  liststyle_->clear_bgcolor();
  liststyle_->clear_maxsnippetlines();
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
