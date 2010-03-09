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

#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/networklinkcontrol.h"
#include "gtest/gtest.h"

namespace kmldom {

// <Create> tests.
class CreateTest : public testing::Test {
 protected:
  virtual void SetUp() {
    create_ = KmlFactory::GetFactory()->CreateCreate();
  }

  CreatePtr create_;
};

TEST_F(CreateTest, TestType) {
  ASSERT_EQ(Type_Create, create_->Type());
  ASSERT_TRUE(create_->IsA(Type_Create));
}

TEST_F(CreateTest, TestLists) {
  ASSERT_EQ(static_cast<size_t>(0), create_->get_container_array_size());
  create_->add_container(KmlFactory::GetFactory()->CreateDocument());
  create_->add_container(KmlFactory::GetFactory()->CreateFolder());
  ASSERT_EQ(static_cast<size_t>(2), create_->get_container_array_size());
  ASSERT_EQ(Type_Document, create_->get_container_array_at(0)->Type());
  ASSERT_EQ(Type_Folder, create_->get_container_array_at(1)->Type());
}

// <Delete> tests.
class DeleteTest : public testing::Test {
 protected:
  virtual void SetUp() {
    delete_ = KmlFactory::GetFactory()->CreateDelete();
  }

  DeletePtr delete_;
};

TEST_F(DeleteTest, TestType) {
  ASSERT_EQ(Type_Delete, delete_->Type());
  ASSERT_TRUE(delete_->IsA(Type_Delete));
}

TEST_F(DeleteTest, TestLists) {
  ASSERT_EQ(static_cast<size_t>(0), delete_->get_feature_array_size());
  delete_->add_feature(KmlFactory::GetFactory()->CreatePlacemark());
  delete_->add_feature(KmlFactory::GetFactory()->CreateNetworkLink());
  ASSERT_EQ(static_cast<size_t>(2), delete_->get_feature_array_size());
  ASSERT_EQ(Type_Placemark, delete_->get_feature_array_at(0)->Type());
  ASSERT_EQ(Type_NetworkLink, delete_->get_feature_array_at(1)->Type());
}

// <Change> tests.
class ChangeTest : public testing::Test {
 protected:
  virtual void SetUp() {
    change_ = KmlFactory::GetFactory()->CreateChange();
  }

  ChangePtr change_;
};

TEST_F(ChangeTest, TestType) {
  ASSERT_EQ(Type_Change, change_->Type());
  ASSERT_TRUE(change_->IsA(Type_Change));
}

TEST_F(ChangeTest, TestLists) {
  ASSERT_EQ(static_cast<size_t>(0), change_->get_object_array_size());
  change_->add_object(KmlFactory::GetFactory()->CreateIconStyle());
  change_->add_object(KmlFactory::GetFactory()->CreateLocation());
  ASSERT_EQ(static_cast<size_t>(2), change_->get_object_array_size());
  ASSERT_EQ(Type_IconStyle, change_->get_object_array_at(0)->Type());
  ASSERT_EQ(Type_Location, change_->get_object_array_at(1)->Type());
}

// <Update> tests.
class UpdateTest : public testing::Test {
 protected:
  virtual void SetUp() {
    update_ = KmlFactory::GetFactory()->CreateUpdate();
  }

  UpdatePtr update_;
};

TEST_F(UpdateTest, TestType) {
  ASSERT_EQ(Type_Update, update_->Type());
  ASSERT_TRUE(update_->IsA(Type_Update));
}

TEST_F(UpdateTest, TestTargetHref) {
  ASSERT_FALSE(update_->has_targethref());
  ASSERT_EQ(string(""), update_->get_targethref());
  string targethref("#checkoutthiscrosssection");
  update_->set_targethref(targethref);
  ASSERT_TRUE(update_->has_targethref());
  ASSERT_TRUE(targethref == update_->get_targethref());
  update_->clear_targethref();
  ASSERT_FALSE(update_->has_targethref());
  ASSERT_EQ(string(""), update_->get_targethref());
}

TEST_F(UpdateTest, TestLists) {
  // Vector is empty.
  ASSERT_EQ(static_cast<size_t>(0), update_->get_updateoperation_array_size());
  // Add create:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateCreate());
  // And delete:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateDelete());
  // We have two items in the array:
  ASSERT_EQ(static_cast<size_t>(2), update_->get_updateoperation_array_size());
  // Add another create:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateCreate());
  // And change:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateChange());
  // We have four items in the array:
  ASSERT_EQ(static_cast<size_t>(4), update_->get_updateoperation_array_size());
  // And another delete:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateDelete());
  // And another change:
  update_->add_updateoperation(KmlFactory::GetFactory()->CreateChange());
  // We have six items in the array:
  ASSERT_EQ(static_cast<size_t>(6), update_->get_updateoperation_array_size());
  // Assert ordering is preserved in array.
  ASSERT_EQ(Type_Create,
                 update_->get_updateoperation_array_at(0)->Type());
  ASSERT_EQ(Type_Delete,
                 update_->get_updateoperation_array_at(1)->Type());
  ASSERT_EQ(Type_Create,
                 update_->get_updateoperation_array_at(2)->Type());
  ASSERT_EQ(Type_Change,
                 update_->get_updateoperation_array_at(3)->Type());
  ASSERT_EQ(Type_Delete,
                 update_->get_updateoperation_array_at(4)->Type());
  ASSERT_EQ(Type_Change,
                 update_->get_updateoperation_array_at(5)->Type());
}

// <NetworkLinkControl> tests.
class NetworkLinkControlTest : public testing::Test {
 protected:
  virtual void SetUp() {
    nlc_ = KmlFactory::GetFactory()->CreateNetworkLinkControl();
  }

  NetworkLinkControlPtr nlc_;
};

TEST_F(NetworkLinkControlTest, TestType) {
  ASSERT_TRUE(nlc_->IsA(Type_NetworkLinkControl));
}

// Verify proper defaults:
TEST_F(NetworkLinkControlTest, TestDefaults) {
  ASSERT_FALSE(nlc_->has_minrefreshperiod());
  ASSERT_DOUBLE_EQ(0.0, nlc_->get_minrefreshperiod());
  ASSERT_FALSE(nlc_->has_maxsessionlength());
  ASSERT_DOUBLE_EQ(0.0, nlc_->get_maxsessionlength());
  ASSERT_FALSE(nlc_->has_cookie());
  ASSERT_EQ(string(""), nlc_->get_cookie());
  ASSERT_FALSE(nlc_->has_message());
  ASSERT_EQ(string(""), nlc_->get_message());
  ASSERT_FALSE(nlc_->has_linkname());
  ASSERT_EQ(string(""), nlc_->get_linkname());
  ASSERT_FALSE(nlc_->has_linkdescription());
  ASSERT_EQ(string(""), nlc_->get_linkdescription());
  ASSERT_FALSE(nlc_->has_linksnippet());
  ASSERT_TRUE(NULL == nlc_->get_linksnippet());
  ASSERT_FALSE(nlc_->has_expires());
  ASSERT_EQ(string(""), nlc_->get_expires());
  ASSERT_FALSE(nlc_->has_update());
  ASSERT_TRUE(NULL == nlc_->get_update());
  ASSERT_FALSE(nlc_->has_abstractview());
  ASSERT_TRUE(NULL == nlc_->get_abstractview());
}

// Verify setting default makes has_xxx() true:
TEST_F(NetworkLinkControlTest, TestSetToDefaultValues) {
  nlc_->set_minrefreshperiod(nlc_->get_minrefreshperiod());
  ASSERT_TRUE(nlc_->has_minrefreshperiod());
  nlc_->set_maxsessionlength(nlc_->get_maxsessionlength());
  ASSERT_TRUE(nlc_->has_maxsessionlength());
  nlc_->set_cookie(nlc_->get_cookie());
  ASSERT_TRUE(nlc_->has_cookie());
  nlc_->set_message(nlc_->get_message());
  ASSERT_TRUE(nlc_->has_message());
  nlc_->set_linkname(nlc_->get_linkname());
  ASSERT_TRUE(nlc_->has_linkname());
  nlc_->set_linkdescription(nlc_->get_linkdescription());
  ASSERT_TRUE(nlc_->has_linkdescription());
  nlc_->set_linksnippet(NULL);
  ASSERT_FALSE(nlc_->has_linksnippet());  // ptr is still null
  nlc_->set_expires(nlc_->get_expires());
  ASSERT_TRUE(nlc_->has_expires());
  nlc_->set_update(NULL);
  ASSERT_FALSE(nlc_->has_update());  // ptr is still null
  nlc_->set_abstractview(NULL);
  ASSERT_FALSE(nlc_->has_abstractview());  // ptr is still null
}

// Verify set, get, has, clear:
TEST_F(NetworkLinkControlTest, TestSetGetHasClear) {
  // Non-default values.
  const double minrefreshperiod = 1.0;
  const double maxsessionlength = 2.0;
  const string cookie("cookie");
  const string message("message");
  const string linkname("linkname");
  const string linkdescription("linkdescription");
  LinkSnippetPtr linksnippet = KmlFactory::GetFactory()->CreateLinkSnippet();
  const string expires("expires");
  UpdatePtr update = KmlFactory::GetFactory()->CreateUpdate();
  CameraPtr camera = KmlFactory::GetFactory()->CreateCamera();

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
  ASSERT_TRUE(nlc_->has_minrefreshperiod());
  ASSERT_DOUBLE_EQ(minrefreshperiod,nlc_->get_minrefreshperiod());
  ASSERT_TRUE(nlc_->has_maxsessionlength());
  ASSERT_DOUBLE_EQ(maxsessionlength, nlc_->get_maxsessionlength());
  ASSERT_TRUE(nlc_->has_cookie());
  ASSERT_EQ(cookie,nlc_->get_cookie());
  ASSERT_TRUE(nlc_->has_message());
  ASSERT_EQ(message, nlc_->get_message());
  ASSERT_TRUE(nlc_->has_linkname());
  ASSERT_EQ(linkname, nlc_->get_linkname());
  ASSERT_TRUE(nlc_->has_linkdescription());
  ASSERT_EQ(linkdescription, nlc_->get_linkdescription());
  ASSERT_TRUE(nlc_->has_linksnippet());
  ASSERT_EQ(linksnippet, nlc_->get_linksnippet());
  ASSERT_TRUE(nlc_->has_expires());
  ASSERT_EQ(expires, nlc_->get_expires());
  ASSERT_TRUE(nlc_->has_update());
  ASSERT_EQ(update, nlc_->get_update());
  ASSERT_TRUE(nlc_->has_abstractview());
  ASSERT_EQ(camera, nlc_->get_abstractview());

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
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
