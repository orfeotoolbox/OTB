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

#include <string>
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/networklinkcontrol.h"
#include "kml/base/unit_test.h"

namespace kmldom {

// <Create> tests.
class CreateTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CreateTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    create_ = KmlFactory::GetFactory()->CreateCreate();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestLists();

 private:
  CreatePtr create_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CreateTest);

void CreateTest::TestType() {
  CPPUNIT_ASSERT(Type_Create == create_->Type());
  CPPUNIT_ASSERT(true == create_->IsA(Type_Create));
}

void CreateTest::TestLists() {
  CPPUNIT_ASSERT(0 == create_->get_container_array_size());
  create_->add_container(KmlFactory::GetFactory()->CreateDocument());
  create_->add_container(KmlFactory::GetFactory()->CreateFolder());
  CPPUNIT_ASSERT(2 == create_->get_container_array_size());
  CPPUNIT_ASSERT(Type_Document == create_->get_container_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_Folder == create_->get_container_array_at(1)->Type());
}

// <Delete> tests.
class DeleteTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(DeleteTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    delete_ = KmlFactory::GetFactory()->CreateDelete();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestLists();

 private:
  DeletePtr delete_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DeleteTest);

void DeleteTest::TestType() {
  CPPUNIT_ASSERT(Type_Delete == delete_->Type());
  CPPUNIT_ASSERT(true == delete_->IsA(Type_Delete));
}

void DeleteTest::TestLists() {
  CPPUNIT_ASSERT(0 == delete_->get_feature_array_size());
  delete_->add_feature(KmlFactory::GetFactory()->CreatePlacemark());
  delete_->add_feature(KmlFactory::GetFactory()->CreateNetworkLink());
  CPPUNIT_ASSERT(2 == delete_->get_feature_array_size());
  CPPUNIT_ASSERT(Type_Placemark == delete_->get_feature_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_NetworkLink == delete_->get_feature_array_at(1)->Type());
}

// <Change> tests.
class ChangeTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ChangeTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    change_ = KmlFactory::GetFactory()->CreateChange();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestLists();

 private:
  ChangePtr change_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ChangeTest);

void ChangeTest::TestType() {
  CPPUNIT_ASSERT(Type_Change == change_->Type());
  CPPUNIT_ASSERT(true == change_->IsA(Type_Change));
}

void ChangeTest::TestLists() {
  CPPUNIT_ASSERT(0 == change_->get_object_array_size());
  change_->add_object(KmlFactory::GetFactory()->CreateIconStyle());
  change_->add_object(KmlFactory::GetFactory()->CreateLocation());
  CPPUNIT_ASSERT(2 == change_->get_object_array_size());
  CPPUNIT_ASSERT(Type_IconStyle == change_->get_object_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_Location == change_->get_object_array_at(1)->Type());
}

// <Update> tests.
class UpdateTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(UpdateTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestTargetHref);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    update_ = KmlFactory::GetFactory()->CreateUpdate();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestTargetHref();
  void TestLists();

 private:
  UpdatePtr update_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(UpdateTest);

void UpdateTest::TestType() {
  CPPUNIT_ASSERT(Type_Update == update_->Type());
  CPPUNIT_ASSERT(true == update_->IsA(Type_Update));
}

void UpdateTest::TestTargetHref() {
  CPPUNIT_ASSERT(false == update_->has_targethref());
  CPPUNIT_ASSERT("" == update_->get_targethref());
  std::string targethref("#checkoutthiscrosssection");
  update_->set_targethref(targethref);
  CPPUNIT_ASSERT(true == update_->has_targethref());
  CPPUNIT_ASSERT(targethref == update_->get_targethref());
  update_->clear_targethref();
  CPPUNIT_ASSERT(false == update_->has_targethref());
  CPPUNIT_ASSERT("" == update_->get_targethref());
}

void UpdateTest::TestLists() {
  // Vector is empty.
  CPPUNIT_ASSERT(0 == update_->get_updateoperation_array_size());
  // Add create:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateCreate());
  // And delete:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateDelete());
  // We have two items in the array:
  CPPUNIT_ASSERT(2 == update_->get_updateoperation_array_size());
  // Add another create:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateCreate());
  // And change:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateChange());
  // We have four items in the array:
  CPPUNIT_ASSERT(4 == update_->get_updateoperation_array_size());
  // And another delete:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateDelete());
  // And another change:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateChange());
  // We have six items in the array:
  CPPUNIT_ASSERT(6 == update_->get_updateoperation_array_size());
  // Assert ordering is preserved in array.
  CPPUNIT_ASSERT(Type_Create ==
                 update_->get_updateoperation_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_Delete ==
                 update_->get_updateoperation_array_at(1)->Type());
  CPPUNIT_ASSERT(Type_Create ==
                 update_->get_updateoperation_array_at(2)->Type());
  CPPUNIT_ASSERT(Type_Change ==
                 update_->get_updateoperation_array_at(3)->Type());
  CPPUNIT_ASSERT(Type_Delete ==
                 update_->get_updateoperation_array_at(4)->Type());
  CPPUNIT_ASSERT(Type_Change ==
                 update_->get_updateoperation_array_at(5)->Type());
}

// <NetworkLinkControl> tests.
class NetworkLinkControlTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(NetworkLinkControlTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    nlc_ = KmlFactory::GetFactory()->CreateNetworkLinkControl();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  NetworkLinkControlPtr nlc_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(NetworkLinkControlTest);

void NetworkLinkControlTest::TestType() {
  CPPUNIT_ASSERT(true == nlc_->IsA(Type_NetworkLinkControl));
}

// Verify proper defaults:
void NetworkLinkControlTest::TestDefaults() {
  CPPUNIT_ASSERT(false == nlc_->has_minrefreshperiod());
  CPPUNIT_ASSERT(0.0 == nlc_->get_minrefreshperiod());
  CPPUNIT_ASSERT(false == nlc_->has_maxsessionlength());
  CPPUNIT_ASSERT(0.0 == nlc_->get_maxsessionlength());
  CPPUNIT_ASSERT(false == nlc_->has_cookie());
  CPPUNIT_ASSERT("" == nlc_->get_cookie());
  CPPUNIT_ASSERT(false == nlc_->has_message());
  CPPUNIT_ASSERT("" == nlc_->get_message());
  CPPUNIT_ASSERT(false == nlc_->has_linkname());
  CPPUNIT_ASSERT("" == nlc_->get_linkname());
  CPPUNIT_ASSERT(false == nlc_->has_linkdescription());
  CPPUNIT_ASSERT("" == nlc_->get_linkdescription());
  CPPUNIT_ASSERT(false == nlc_->has_linksnippet());
  CPPUNIT_ASSERT(NULL == nlc_->get_linksnippet());
  CPPUNIT_ASSERT(false == nlc_->has_expires());
  CPPUNIT_ASSERT("" == nlc_->get_expires());
  CPPUNIT_ASSERT(false == nlc_->has_update());
  CPPUNIT_ASSERT(NULL == nlc_->get_update());
  CPPUNIT_ASSERT(false == nlc_->has_abstractview());
  CPPUNIT_ASSERT(NULL == nlc_->get_abstractview());
}

// Verify setting default makes has_xxx() true:
void NetworkLinkControlTest::TestSetToDefaultValues() {
  TestDefaults();
  nlc_->set_minrefreshperiod(nlc_->get_minrefreshperiod());
  CPPUNIT_ASSERT(true == nlc_->has_minrefreshperiod());
  nlc_->set_maxsessionlength(nlc_->get_maxsessionlength());
  CPPUNIT_ASSERT(true == nlc_->has_maxsessionlength());
  nlc_->set_cookie(nlc_->get_cookie());
  CPPUNIT_ASSERT(true == nlc_->has_cookie());
  nlc_->set_message(nlc_->get_message());
  CPPUNIT_ASSERT(true == nlc_->has_message());
  nlc_->set_linkname(nlc_->get_linkname());
  CPPUNIT_ASSERT(true == nlc_->has_linkname());
  nlc_->set_linkdescription(nlc_->get_linkdescription());
  CPPUNIT_ASSERT(true == nlc_->has_linkdescription());
  nlc_->set_linksnippet(NULL);
  CPPUNIT_ASSERT(false == nlc_->has_linksnippet());  // ptr is still null
  nlc_->set_expires(nlc_->get_expires());
  CPPUNIT_ASSERT(true == nlc_->has_expires());
  nlc_->set_update(NULL);
  CPPUNIT_ASSERT(false == nlc_->has_update());  // ptr is still null
  nlc_->set_abstractview(NULL);
  CPPUNIT_ASSERT(false == nlc_->has_abstractview());  // ptr is still null
}

// Verify set, get, has, clear:
void NetworkLinkControlTest::TestSetGetHasClear() {
  // Non-default values.
  const double minrefreshperiod = 1.0;
  const double maxsessionlength = 2.0;
  const std::string cookie("cookie");
  const std::string message("message");
  const std::string linkname("linkname");
  const std::string linkdescription("linkdescription");
  LinkSnippet* linksnippet = KmlFactory::GetFactory()->CreateLinkSnippet();
  const std::string expires("expires");
  Update* update = KmlFactory::GetFactory()->CreateUpdate();
  Camera* camera = KmlFactory::GetFactory()->CreateCamera();

  // Set all fields:
  nlc_->set_minrefreshperiod(minrefreshperiod);
  nlc_->set_maxsessionlength(maxsessionlength);
  nlc_->set_cookie(cookie);
  nlc_->set_message(message);
  nlc_->set_linkname(linkname);
  nlc_->set_linkdescription(linkdescription);
  nlc_->set_linksnippet(linksnippet);
  nlc_->set_expires(expires);
  nlc_->set_update(update);
  nlc_->set_abstractview(camera);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == nlc_->has_minrefreshperiod());
  CPPUNIT_ASSERT(minrefreshperiod == nlc_->get_minrefreshperiod());
  CPPUNIT_ASSERT(true == nlc_->has_maxsessionlength());
  CPPUNIT_ASSERT(maxsessionlength == nlc_->get_maxsessionlength());
  CPPUNIT_ASSERT(true == nlc_->has_cookie());
  CPPUNIT_ASSERT(cookie == nlc_->get_cookie());
  CPPUNIT_ASSERT(true == nlc_->has_message());
  CPPUNIT_ASSERT(message == nlc_->get_message());
  CPPUNIT_ASSERT(true == nlc_->has_linkname());
  CPPUNIT_ASSERT(linkname == nlc_->get_linkname());
  CPPUNIT_ASSERT(true == nlc_->has_linkdescription());
  CPPUNIT_ASSERT(linkdescription == nlc_->get_linkdescription());
  CPPUNIT_ASSERT(true == nlc_->has_linksnippet());
  CPPUNIT_ASSERT(linksnippet == nlc_->get_linksnippet());
  CPPUNIT_ASSERT(true == nlc_->has_expires());
  CPPUNIT_ASSERT(expires == nlc_->get_expires());
  CPPUNIT_ASSERT(true == nlc_->has_update());
  CPPUNIT_ASSERT(update == nlc_->get_update());
  CPPUNIT_ASSERT(true == nlc_->has_abstractview());
  CPPUNIT_ASSERT(camera == nlc_->get_abstractview());

  // Clear all fields:
  nlc_->clear_minrefreshperiod();
  nlc_->clear_maxsessionlength();
  nlc_->clear_cookie();
  nlc_->clear_message();
  nlc_->clear_linkname();
  nlc_->clear_linkdescription();
  nlc_->clear_linksnippet();
  nlc_->clear_expires();
  nlc_->clear_update();
  nlc_->clear_abstractview();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
