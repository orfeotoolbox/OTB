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
#include "kml/base/unit_test.h"

namespace kmldom {

class ItemIconTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ItemIconTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    itemicon_ = KmlFactory::GetFactory()->CreateItemIcon();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  ItemIconPtr itemicon_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ItemIconTest);

void ItemIconTest::TestType() {
  CPPUNIT_ASSERT(true == itemicon_->IsA(Type_ItemIcon));
  CPPUNIT_ASSERT(true == itemicon_->IsA(Type_Object));
}

// Verify proper defaults:
void ItemIconTest::TestDefaults() {
  CPPUNIT_ASSERT(false == itemicon_->has_state());
  CPPUNIT_ASSERT(ITEMICONSTATE_OPEN == itemicon_->get_state());
  CPPUNIT_ASSERT(false == itemicon_->has_href());
  CPPUNIT_ASSERT("" == itemicon_->get_href());
}

// Verify setting default makes has_xxx() true:
void ItemIconTest::TestSetToDefaultValues() {
  TestDefaults();
  itemicon_->set_state(itemicon_->get_state());
  CPPUNIT_ASSERT(true == itemicon_->has_state());
  itemicon_->set_href(itemicon_->get_href());
  CPPUNIT_ASSERT(true == itemicon_->has_href());
}

// Verify set, get, has, clear:
void ItemIconTest::TestSetGetHasClear() {
  // Non-default values:
  ItemIconStateEnum state = ITEMICONSTATE_ERROR;
  std::string href("http://example.com/foo.jpg");

  // Set all fields:
  itemicon_->set_state(state);
  itemicon_->set_href(href);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == itemicon_->has_state());
  CPPUNIT_ASSERT(state == itemicon_->get_state());
  CPPUNIT_ASSERT(true == itemicon_->has_href());
  CPPUNIT_ASSERT(href == itemicon_->get_href());

  // Clear all fields:
  itemicon_->clear_state();
  itemicon_->clear_href();

  // Verify now in default state:
  TestDefaults();
}


class ListStyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ListStyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    liststyle_ = KmlFactory::GetFactory()->CreateListStyle();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestLists();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  ListStylePtr liststyle_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ListStyleTest);

void ListStyleTest::TestType() {
  CPPUNIT_ASSERT(true == liststyle_->IsA(Type_ListStyle));
  CPPUNIT_ASSERT(Type_ListStyle == liststyle_->Type());
  CPPUNIT_ASSERT(true == liststyle_->IsA(Type_SubStyle));
}

void ListStyleTest::TestLists() {
  CPPUNIT_ASSERT(0 == liststyle_->get_itemicon_array_size());
  liststyle_->add_itemicon(KmlFactory::GetFactory()->CreateItemIcon());
  liststyle_->add_itemicon(KmlFactory::GetFactory()->CreateItemIcon());
  CPPUNIT_ASSERT(2 == liststyle_->get_itemicon_array_size());
  CPPUNIT_ASSERT(Type_ItemIcon == liststyle_->get_itemicon_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_ItemIcon == liststyle_->get_itemicon_array_at(1)->Type());
}

// Verify proper defaults:
void ListStyleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == liststyle_->has_listitemtype());
  CPPUNIT_ASSERT(LISTITEMTYPE_CHECK == liststyle_->get_listitemtype());
  CPPUNIT_ASSERT(false == liststyle_->has_bgcolor());
  CPPUNIT_ASSERT("ffffffff" == liststyle_->get_bgcolor());
}

// Verify setting default makes has_xxx() true:
void ListStyleTest::TestSetToDefaultValues() {
  TestDefaults();
  liststyle_->set_listitemtype(liststyle_->get_listitemtype());
  CPPUNIT_ASSERT(true == liststyle_->has_listitemtype());
  liststyle_->set_bgcolor(liststyle_->get_bgcolor());
  CPPUNIT_ASSERT(true == liststyle_->has_bgcolor());
}

// Verify set, get, has, clear:
void ListStyleTest::TestSetGetHasClear() {
  // Non-default values:
  ListItemTypeEnum listitemtype = LISTITEMTYPE_CHECKHIDECHILDREN;
  std::string bgcolor("00112233");

  // Set all fields:
  liststyle_->set_listitemtype(listitemtype);
  liststyle_->set_bgcolor(bgcolor);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == liststyle_->has_listitemtype());
  CPPUNIT_ASSERT(listitemtype == liststyle_->get_listitemtype());
  CPPUNIT_ASSERT(true == liststyle_->has_bgcolor());
  CPPUNIT_ASSERT(bgcolor == liststyle_->get_bgcolor());

  // Clear all fields:
  liststyle_->clear_listitemtype();
  liststyle_->clear_bgcolor();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
